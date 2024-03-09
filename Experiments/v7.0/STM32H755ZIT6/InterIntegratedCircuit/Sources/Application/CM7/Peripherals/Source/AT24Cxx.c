#include "../Include/InterIntegratedCircuit.h"

#include "../Include/AT24Cxx.h"

#define AT24C01                                             127
#define AT24C02                                             225
#define AT24C04                                             511
#define AT24C08                                             1023
#define AT24C16                                             2047
#define AT24C32                                             4095
#define AT24C64                                             8191
#define AT24C128                                            16383
#define AT24C256                                            32767

#define AT24CXX_DEVICE                                      AT24C02

void InitAT24Cxx(void)
{
    InitInterIntegratedCircuit();
}

uint8_t AT24CxxReadByte(uint16_t address)
{
    IssueStartSignal();
    
    if (AT24CXX_DEVICE > AT24C16)
    {
        /*  Send WRITE command to write the address of AT24Cxx device, the Device Address 0x1010 000 + the bit #0 = 0xA0.   */
        TransmitByte(0xA0);
        PollAcknowledgement();
        
        /*  Send the higher 8 bits address. */
        TransmitByte(address >> 8);
    }
    else
        TransmitByte(0xA0 + ((address / 256) << 1));
    
    PollAcknowledgement();
    
    /*  Send the lower 8 bits address. */
    TransmitByte(address % 256);
    PollAcknowledgement();
    
    IssueStartSignal();
     
    TransmitByte(0xA1);
    PollAcknowledgement();
    
    uint8_t byte = ReadByte(0);
    
    IssueStopSignal();
    
    return byte;
}

void AT24CxxReadBytes(uint16_t address, uint8_t *bytes, uint16_t length)
{
    while (length --)
        *bytes ++ = AT24CxxReadByte(address ++);
}

void AT24CxxWriteByte(uint16_t address, uint8_t byte)
{
    IssueStartSignal();
    
    if (AT24CXX_DEVICE > AT24C16)
    {
        /*  Send WRITE command to write the address of AT24Cxx device, the Device Address 0x1010 000 + the bit #0 = 0xA0.   */
        TransmitByte(0xA0);
        PollAcknowledgement();
        
        /*  Send the 8 bits higher address. */
        TransmitByte(address >> 8);
    }
    else
        TransmitByte(0xA0 + ((address / 256) << 1));
    
    PollAcknowledgement();
    
    /*  Send the 8 bits lower address.  */
    TransmitByte(address % 256);
    PollAcknowledgement();
    
    /*  Send the byte to be written.    */
    TransmitByte(byte);
    PollAcknowledgement();
    
    IssueStopSignal();
    
    /*  It could be slow while writing byte to EEPROM. */
    DelayMs(1);
}

void AT24CxxWriteBytes(uint16_t address, uint8_t *bytes, uint16_t length)
{
    while (length --)
        AT24CxxWriteByte(address ++, *bytes ++);
}

uint8_t AT24CxxValidate(uint8_t address, uint8_t notation)
{
    uint8_t byte = AT24CxxReadByte(address);
    
    if ((byte & notation) == byte)
        return 0;
    else
    {
        AT24CxxWriteByte(address, notation);
        
        byte = AT24CxxReadByte(address);
        
        if ((byte & notation) == byte)
            return 0;
        else
            return 1;
    }
}

