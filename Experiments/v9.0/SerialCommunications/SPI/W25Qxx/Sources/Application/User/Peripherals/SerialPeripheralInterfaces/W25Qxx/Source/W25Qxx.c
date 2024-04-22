#include "../Include/SerialPeripheralInterfaces.h"

#include "../Include/W25Qxx.h"

#define WRITE_DUMMAY_BYTE                                                               0x00
#define INSTRUCTION_WRITE_STATUS_REGISTER_1                                             0x01
#define INSTRUCTION_PAGE_PROGRAM                                                        0x02
#define INSTRUCTION_READ_DATA                                                           0x03
#define INSTRUCTION_READ_STATUS_REGISTER_1                                              0x05
#define INSTRUCTION_WRITE_ENABLE                                                        0x06
#define INSTRUCTION_WRITE_STATUS_REGISTER_3                                             0x11
#define INSTRUCTION_READ_STATUS_REGISTER_3                                              0x15
#define INSTRUCTION_SECTOR_ERASE                                                        0x20
#define INSTRUCTION_WRITE_STATUS_REGISTER_2                                             0x31
#define INSTRUCTION_READ_STATUS_REGISTER_2                                              0x35
#define INSTRUCTION_MANUFACTURER_DEVICE_IDENTIFICATION                                  0x90
#define INSTRUCTION_ENABLE_FOUR_BYTES_ADDRESS                                           0xB7
#define INSTRUCTION_CHIP_ERASE                                                          0xC7

#define READ_DUMMAY_BYTE                                                                0xFF
#define W25QXX_EMPTY_BYTE                                                               0xFF

#define W25Q80_DEVICE_ID                                                                0xEF13          /* W25Q80 */
#define W25Q16_DEVICE_ID                                                                0xEF14          /* W25Q16 */
#define W25Q32_DEVICE_ID                                                                0xEF15          /* W25Q32 */
#define W25Q64_DEVICE_ID                                                                0xEF16          /* W25Q64 */
#define W25Q128_DEVICE_ID                                                               0xEF17          /* W25Q128 */
#define W25Q256_DEVICE_ID                                                               0xEF18          /* W25Q256 */
#define BY25Q64_DEVICE_ID                                                               0x6816          /* BY25Q64 */
#define BY25Q128_DEVICE_ID                                                              0x6817          /* BY25Q128 */
#define NM25Q64_DEVICE_ID                                                               0x5216          /* NM25Q64 */
#define NM25Q128_DEVICE_ID                                                              0x5217          /* NM25Q128 */

#define SECTOR_BUFFER_SIZE                                                              (4 * 1024)
#define BYTES_SIZE_PER_PAGE                                                             256

#define STATUS_REGISTER_1_BUSY                                                          0x01

typedef enum
{
    StatusRegister1,
    StatusRegister2,
    StatusRegister3,
    
}   StatusRegisterNumbers;

uint8_t sectorBackupBuffer[SECTOR_BUFFER_SIZE];
uint16_t deviceIdentification;

static uint8_t ReadStatusRegister(StatusRegisterNumbers registerNumber)
{
    /*
    
    W25Q128.pdf
    7.2.2 Instruction Set Table 1 (Erase, Program Instructions)

    INSTRUCTION NAME            BYTE 1 (CODE)       BYTE 2          BYTE 3      BYTE 4      BYTE 5      BYTE 6

    Read Status Register-1      05h                 (S7¨CS0)
    Read Status Register-2      35h                 (S15¨CS8)
    
    */
    
    uint8_t instruction;
    
    switch (registerNumber)
    {
        case StatusRegister1:
            instruction = INSTRUCTION_READ_STATUS_REGISTER_1;
            break;
        
        case StatusRegister2:
            instruction = INSTRUCTION_READ_STATUS_REGISTER_2;
            break;
        
        case StatusRegister3:
            instruction = INSTRUCTION_READ_STATUS_REGISTER_3;
            break;
    }
    
    SelectSlave(SlaveSelected);
    
    Transceive(instruction);
    uint8_t byte = Transceive(READ_DUMMAY_BYTE);
    
    SelectSlave(SlaveNotSelected);
    
    return byte;
}

static void WriteStatusRegister(StatusRegisterNumbers registerNumber, uint8_t statusRegister)
{
     uint8_t instruction;
    
    switch (registerNumber)
    {
        case StatusRegister1:
            instruction = INSTRUCTION_WRITE_STATUS_REGISTER_1;
            break;
        
        case StatusRegister2:
            instruction = INSTRUCTION_WRITE_STATUS_REGISTER_2;
            break;
        
        case StatusRegister3:
            instruction = INSTRUCTION_WRITE_STATUS_REGISTER_3;
            break;
    }
    
    SelectSlave(SlaveSelected);
    
    Transceive(instruction);
    Transceive(statusRegister);
    
    SelectSlave(SlaveNotSelected);
}

static void EnableWrite(void)
{
    /*

    W25Q128.pdf
    7.2.4 Instruction Set Table 3 (ID, Security Instructions)

    INSTRUCTION NAME            BYTE 1 (CODE)       BYTE 2          BYTE 3      BYTE 4      BYTE 5      BYTE 6

    Write Enable                06h

    */
    
    SelectSlave(SlaveSelected);
    
    Transceive(INSTRUCTION_WRITE_ENABLE);
    
    SelectSlave(SlaveNotSelected);
}

static void SendAddress(uint32_t address)
{
    if (W25Q256_DEVICE_ID == deviceIdentification)
        /*  Send the bits 31 - 24   */
        Transceive((uint8_t)((address) >> 24));
    
    /*  Send the bits 23 - 16   */
    Transceive((uint8_t)((address) >> 16));
    /*  Send the bits 15 - 8   */
    Transceive((uint8_t)((address) >> 8));
    /*  Send the bits 7 - 0   */
    Transceive((uint8_t)address);
}

static void WaitUntilNotBusy(void)
{
    while ((ReadStatusRegister(StatusRegister1) & STATUS_REGISTER_1_BUSY) == STATUS_REGISTER_1_BUSY)
        ;
}

static void EradicateSector(uint32_t sectorAddress)
{
    sectorAddress *= 4096;
    
    EnableWrite();
    WaitUntilNotBusy();
    
    SelectSlave(SlaveSelected);
    
    Transceive(INSTRUCTION_SECTOR_ERASE);
    SendAddress(sectorAddress);
    
    SelectSlave(SlaveNotSelected);
    
    /*  Wait until the completion of eradication.   */
    WaitUntilNotBusy();
}

static void WritePage(uint8_t *bytes, uint32_t address, uint16_t size)
{
    EnableWrite();
    
    SelectSlave(SlaveSelected);
    
    Transceive(INSTRUCTION_PAGE_PROGRAM);
    SendAddress(address);
    
    for (uint16_t i = 0; i < size; i ++)
        Transceive(bytes[i]);
    
    SelectSlave(SlaveNotSelected);
    WaitUntilNotBusy();
}

static void DirectWrite(uint8_t *bytes, uint32_t address, uint16_t size)
{
    uint16_t bytesResidual = BYTES_SIZE_PER_PAGE - address % BYTES_SIZE_PER_PAGE;
    
    if (size <= bytesResidual)
        bytesResidual = size;
    
    while (1)
    {
        WritePage(bytes, address, bytesResidual);
        
        /*  Write is completed. */
        if (bytesResidual == size)
            break;
        else
        {
            bytes += bytesResidual;
            address += bytesResidual;
            size -= bytesResidual;
            
            if (size > BYTES_SIZE_PER_PAGE)
                bytesResidual = BYTES_SIZE_PER_PAGE;
            else
                bytesResidual = size;
        }
    }
}

void InitW25Qxx(void)
{
    SelectSlave(SlaveNotSelected);
    
    InitSerialPeripheralInterface();
    
    deviceIdentification = ReadManufacturerIdentification();
    
    if (W25Q256_DEVICE_ID == deviceIdentification)
    {
        uint8_t statusRegister3 = ReadStatusRegister(StatusRegister3);
        
        /*  If not 4-byte address mode, then switch the mode of 4-byte address.  */
        if ((statusRegister3 & 0x01) == 0)
        {
            /*  Enable Write */
            EnableWrite();
            
            statusRegister3 |= 0x01 << 1;
            
            WriteStatusRegister(StatusRegister3, statusRegister3);
            
            SelectSlave(SlaveSelected);
            
            Transceive(INSTRUCTION_ENABLE_FOUR_BYTES_ADDRESS);
            
            SelectSlave(SlaveSelected);
        }
    }
}

uint16_t ReadManufacturerIdentification(void)
{
    SelectSlave(SlaveSelected);
    
    /*

    W25Q128.pdf
    7.2.4 Instruction Set Table 3 (ID, Security Instructions)

    INSTRUCTION NAME            BYTE 1 (CODE)       BYTE 2          BYTE 3      BYTE 4      BYTE 5      BYTE 6

    Manufacturer / Device ID    90h                 dummy           dummy       dummy       (MF7-MF0)   (ID7-ID0)

    */
    /*
        Byte 0: Send the instruction of Identification Read.
    */
    Transceive(INSTRUCTION_MANUFACTURER_DEVICE_IDENTIFICATION);
    /*
        Byte 1: Dummy Byte.
    */
    Transceive(WRITE_DUMMAY_BYTE);
    /*
        Byte 2: Dummy Byte.
    */
    Transceive(WRITE_DUMMAY_BYTE);
    /*
        Byte 3: Dummy Byte.
    */
    Transceive(WRITE_DUMMAY_BYTE);
    
    uint16_t identification = Transceive(READ_DUMMAY_BYTE) << 8;
    identification |= Transceive(READ_DUMMAY_BYTE);
    
    SelectSlave(SlaveNotSelected);
    
    return identification;
}

void W25QxxRead(uint8_t *bytes, uint32_t address, uint16_t size)
{
    SelectSlave(SlaveSelected);
    
    Transceive(INSTRUCTION_READ_DATA);
    SendAddress(address);
    
    for (int16_t i = 0; i < size; i ++)
        bytes[i] = Transceive(READ_DUMMAY_BYTE);
    
    SelectSlave(SlaveNotSelected);
}
    
void W25QxxWrite(uint8_t *bytes, uint32_t address, uint16_t size)
{
    uint32_t sectorAddressCursor        = address / SECTOR_BUFFER_SIZE;
    uint16_t sectorAddressOffset        = address % SECTOR_BUFFER_SIZE;
    uint16_t sectorAddressResidual      = 4096 - sectorAddressOffset;
    
    if (size <= sectorAddressResidual)
        sectorAddressResidual = size;
    
    uint16_t i;
    
    while (1)
    {
        W25QxxRead(sectorBackupBuffer, sectorAddressCursor * SECTOR_BUFFER_SIZE, SECTOR_BUFFER_SIZE);
        
        for (i = 0; i < sectorAddressResidual; i ++)
        {
            /*  Not empty, eradication is needed.   */
            if (sectorBackupBuffer[sectorAddressOffset + i] != W25QXX_EMPTY_BYTE)
                break;
        }
        
        if (i < sectorAddressResidual)
        {
            EradicateSector(sectorAddressCursor);
            
            for (i = 0; i < sectorAddressResidual; i ++)
                /*  Copy the bytes. */
                sectorBackupBuffer[i + sectorAddressOffset] = bytes[i];
            
            DirectWrite(sectorBackupBuffer, sectorAddressCursor * SECTOR_BUFFER_SIZE, SECTOR_BUFFER_SIZE);
        }
        else
            DirectWrite(bytes, address, sectorAddressResidual);
        
        if (size == sectorAddressResidual)
            break;
        
        else
        {
            sectorAddressCursor ++;
            sectorAddressOffset = 0;
            
            bytes += sectorAddressResidual;
            address += sectorAddressResidual;
            size -= sectorAddressResidual;
            
            if (size > SECTOR_BUFFER_SIZE)
                sectorAddressResidual =  SECTOR_BUFFER_SIZE;
            else
                sectorAddressResidual = size;
        }
    }
}

void EradicateChip(void)
{
    EnableWrite();
    WaitUntilNotBusy();
    
    SelectSlave(SlaveSelected);
    
    Transceive(INSTRUCTION_CHIP_ERASE);
    
    SelectSlave(SlaveNotSelected);
    
    WaitUntilNotBusy();
}