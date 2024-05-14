#include "./Peripherals/Communications/SPI/Include/SerialPeripheralInterfaces.h"

#include "../Include/W25Qxx.h"

#define READ_MANUFACTURE_IDENTIFIER_ADDRESS_BYTES               0x000000

#define INSTRUCTION_PAGE_PROGRAM                                0x02
#define INSTRUCTION_READ                                        0x03
#define INSTRUCTION_READ_SR1                                    0x05
#define INSTRUCTION_WRITE_ENABLE                                0x06
#define INSTRUCTION_SECTOR_ERASE                                0x20
#define INSTRUCTION_READ_MANUFACTURE_IDENTIFIER                 0x90
#define INSTRUCTION_CHIP_ERASE                                  0xC7

#define READ_DUMMY_BYTE                                         0xFF

#define NUMBER_OF_BYTES_PER_PAGE                                256
#define NUMBER_OF_BYTES_PER_SECTOR                              (16 * NUMBER_OF_BYTES_PER_PAGE)

uint8_t sectorBackupBuffer[NUMBER_OF_BYTES_PER_SECTOR];

void InitW25Qxx(void)
{
    InitSerialPeripheralInterfaces();
    
    SelectSlave(ChipUnselectedSignal);
}

static HAL_StatusTypeDef TransmitByte(uint8_t byte)
{
    return SpiTransmit(&byte, 1);
}

static HAL_StatusTypeDef TransceiveByte(uint8_t byte, uint8_t *receivedByte)
{
    return SpiTransceive(&byte, receivedByte, 1);
}

static uint8_t ReadStatusRegister1(void)
{
    SelectSlave(ChipSelectedSignal);
    
    uint8_t receivedByte;
    uint8_t command = INSTRUCTION_READ_SR1;
    
    HAL_StatusTypeDef statusType = TransceiveByte(command, &receivedByte);
    
    if (HAL_OK != statusType)
        receivedByte = READ_DUMMY_BYTE;
    
    SelectSlave(ChipUnselectedSignal);
    
    return receivedByte;
}

static void WaitUntilNotBusy(void)
{    
    while(ReadStatusRegister1() & StatusRegister1FlagSet << W25QxxStatusRegister1WriteInProgress)
        ;
}

static void EnableWrite(void)
{
    SelectSlave(ChipSelectedSignal);
    
    uint8_t receivedByte;
    
    TransceiveByte(INSTRUCTION_WRITE_ENABLE, &receivedByte);
    
    SelectSlave(ChipUnselectedSignal);
}

uint16_t W25QxxReadManufactureIdentifier(void)
{
    SelectSlave(ChipSelectedSignal);
    
    uint8_t commands[] =
    {
        INSTRUCTION_READ_MANUFACTURE_IDENTIFIER,
        
        (uint8_t)(READ_MANUFACTURE_IDENTIFIER_ADDRESS_BYTES >> 16),
        (uint8_t)(READ_MANUFACTURE_IDENTIFIER_ADDRESS_BYTES >> 8),
        (uint8_t)READ_MANUFACTURE_IDENTIFIER_ADDRESS_BYTES,
        
    };
    
    HAL_StatusTypeDef statusType = SpiTransmit(commands, sizeof(commands));
    
    uint8_t receivedByte    = 0;
    uint16_t identifier     = 0;
    
    if (HAL_OK == TransceiveByte(READ_DUMMY_BYTE, &receivedByte))
        identifier = receivedByte << 8;
    if (HAL_OK == TransceiveByte(READ_DUMMY_BYTE, &receivedByte))
        identifier |= receivedByte;
    
    SelectSlave(ChipUnselectedSignal);
    
    return identifier;
}

HAL_StatusTypeDef W25QxxEraseChip(void)
{
    EnableWrite();
    WaitUntilNotBusy();
    
    SelectSlave(ChipSelectedSignal);
    
    HAL_StatusTypeDef statusType = TransmitByte(INSTRUCTION_CHIP_ERASE);
    
    SelectSlave(ChipUnselectedSignal);
    
    WaitUntilNotBusy();
    
    return statusType;
}

HAL_StatusTypeDef EraseSector(uint32_t sectorNumber)
{
    sectorNumber *= NUMBER_OF_BYTES_PER_SECTOR;
    
    EnableWrite();
    WaitUntilNotBusy();
    
    SelectSlave(ChipSelectedSignal);
    
    uint8_t commands[] =
    {
        INSTRUCTION_SECTOR_ERASE,
        
        (uint8_t)(sectorNumber >> 16),
        (uint8_t)(sectorNumber >> 8),
        (uint8_t)sectorNumber,
    };
    
    HAL_StatusTypeDef statusType = SpiTransmit(commands, sizeof(commands));
    
    SelectSlave(ChipUnselectedSignal);
    
    WaitUntilNotBusy();
    
    return statusType;
}

HAL_StatusTypeDef W25QxxRead(uint8_t *bytes, uint32_t address, uint16_t length)
{
    SelectSlave(ChipSelectedSignal);
    
    uint8_t commands[] =
    {
        INSTRUCTION_READ,
        
        (uint8_t)(address >> 16),
        (uint8_t)(address >> 8),
        (uint8_t)address,
    };
    
    HAL_StatusTypeDef statusType = SpiTransmit(commands, sizeof(commands));
    
    if (HAL_OK == statusType)
    {
        uint8_t command = INSTRUCTION_READ;
        
        statusType = SpiTransceive(&command, bytes, length);
    }
    
    SelectSlave(ChipUnselectedSignal);
    
    return statusType;
}

HAL_StatusTypeDef W25QxxPageProgram(uint8_t *bytes, uint32_t address, uint16_t length)
{
    EnableWrite();
    WaitUntilNotBusy();
    
    SelectSlave(ChipSelectedSignal);
    
    uint8_t receivedByte;
    
    
    uint8_t commands[] =
    {
        INSTRUCTION_PAGE_PROGRAM,
        
        (uint8_t)(address >> 16),
        (uint8_t)(address >> 8),
        (uint8_t)address,
    };
    
    HAL_StatusTypeDef statusType = SpiTransmit(commands, sizeof(commands));
    
    if (HAL_OK == statusType)
    {
        for (uint16_t i = 0; i < length; i ++)
        {
            statusType = TransmitByte(bytes[i]);
            
            if (HAL_OK != statusType)
                break;
        }
    }
    
    SelectSlave(ChipUnselectedSignal);
    
    WaitUntilNotBusy();
    
    return statusType;
}

static void DirectWrite(uint8_t *bytes, uint32_t address, uint16_t length)
{
    uint16_t residualBytes = NUMBER_OF_BYTES_PER_PAGE - address % NUMBER_OF_BYTES_PER_PAGE;
    
    if (length <= residualBytes)
        residualBytes = length;
    
    while (1)
    {
        if (HAL_OK == W25QxxPageProgram(bytes, address, residualBytes))
        {
            if (length == residualBytes)
                break;
            else
            {
                bytes += residualBytes;
                address += residualBytes;
                length -= residualBytes;
                
                if (length > NUMBER_OF_BYTES_PER_PAGE)
                    residualBytes = NUMBER_OF_BYTES_PER_PAGE;
                else
                    residualBytes = length;
            }
        }
        else
            break;
    }
}

void W25QxxWrite(uint8_t *bytes, uint32_t address, uint16_t length)
{
    uint32_t currentSector          = address / NUMBER_OF_BYTES_PER_SECTOR;
    uint16_t sectorOffset           = address % NUMBER_OF_BYTES_PER_SECTOR;
    uint16_t residualBytes          = NUMBER_OF_BYTES_PER_SECTOR - sectorOffset;
    uint8_t *sectorBackupPointer    = sectorBackupBuffer;

    if (length <= residualBytes)
        residualBytes = length;
    
    uint16_t i = 0;
    
    while (1)
    {
        if (HAL_OK == W25QxxRead(sectorBackupPointer, currentSector * NUMBER_OF_BYTES_PER_SECTOR, NUMBER_OF_BYTES_PER_SECTOR))
        {
            for (i = 0; i < residualBytes; i ++)
            {
                if (sectorBackupPointer[sectorOffset + i] != READ_DUMMY_BYTE)
                    break;
            }
            
            if (i < residualBytes)
            {
                if (HAL_OK == EraseSector(currentSector))
                {
                    for (i = 0; i < residualBytes; i ++)
                    sectorBackupPointer[sectorOffset + i] = bytes[i]; 
                
                    DirectWrite(sectorBackupPointer, currentSector * NUMBER_OF_BYTES_PER_SECTOR, NUMBER_OF_BYTES_PER_SECTOR);
                }
                else
                    break;
            }
            else
                DirectWrite(bytes, address, residualBytes);
            
            if (residualBytes == length)
                break;
            else
            {
                currentSector ++;
                sectorOffset = 0;
                
                bytes += residualBytes;
                address += residualBytes;
                length -= residualBytes;
                
                if (length > NUMBER_OF_BYTES_PER_SECTOR)
                    residualBytes = NUMBER_OF_BYTES_PER_SECTOR;
                else
                    residualBytes = length;
            }
        }
        else
            break;
    }
}