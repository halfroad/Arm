#include "../include/InterIntegratedCircuit.h"
#include "../Include/Delay.h"

#include "../Include/SHT20.h"

#define SHT20_DEVICE_ADDRESS                        0x40    /*  7 bits base address.    */
#define CRC_MODEL                                   0x131

typedef enum
{
    CommandHolderMasterTriggerTemperatureMeasurement            = 0b11100011,
    CommandHolderMasterTriggerRelativeHumidityMeasurement       = 0b11100101,
    
    CommandNoHolderMasterTriggerTemperatureMeasurement          = 0b11110011,
    CommandNoHolderMasterTriggerRelativeHumidityMeasurement     = 0b11110101,
    
    CommandWriteUserRegister                                    = 0b11100110,
    CommandReadUserRegister                                     = 0b11100111,
    
    CommandSoftwareReset                                        = 0b11111110,
    
} SensorCommands;

void InitHumidityTemperatureSensor(void)
{
    InitInterIntegratedCircuit();
}

uint8_t CRC8Validate(uint8_t *byte, uint8_t length, uint8_t checksum)
{
    uint8_t i, crc = 0x00;
    
    while (length --)
    {
        crc ^= *byte ++;
        
        for (i = 8; i > 0; --i)
        {
            if (crc & 0x80)
                crc = (crc << 1) ^ CRC_MODEL;
            else
                crc = crc << 1;
        }
    }
    
    if (checksum == crc)
        return 0;
    else
        return 1;
}

uint16_t SendCommand(SensorCommands command)
{
    IssueStartSignal();
    
    /*  Write address 0x40, Read address 0x41. */
    uint8_t address = SHT20_DEVICE_ADDRESS << 1;
    
    /*  Write.  */
    TransmitByte(address);
    PollAcknowledgement();
    
    TransmitByte(command);
    PollAcknowledgement();
    
    /*  Start signal again. */
    IssueStartSignal();
    
    /*  Read.   */
    TransmitByte(address | 0x01);
    PollAcknowledgement();
    
    DelayMs(70);
    
    uint8_t buffer[2] = { 0 };
    
    buffer[0] = ReadByte(1);
    buffer[1] = ReadByte(1);
    
    uint8_t checksum = ReadByte(0);
    
    IssueStopSignal();
    
    uint16_t ambiences = (buffer[0] << 8) + buffer[1];
    
    if (CRC8Validate(buffer, 2, checksum) == 0)
    {
        if (command == CommandHolderMasterTriggerTemperatureMeasurement ||
            command == CommandNoHolderMasterTriggerTemperatureMeasurement)
        {
            ambiences = (175.72 * ambiences / 65536 - 46.85) * 10;
        }
        else if (command == CommandHolderMasterTriggerRelativeHumidityMeasurement ||
            command == CommandNoHolderMasterTriggerRelativeHumidityMeasurement)
        {
            ambiences = (125.0 * ambiences / 65536 - 6.0) * 10;
        }
        
        return ambiences;
    }
    else
        return 0xFFFF;
}

uint16_t AcquireTemperature(void)
{
    return SendCommand(CommandHolderMasterTriggerTemperatureMeasurement);
}

uint16_t AcquireRelativeHumidity(void)
{
    return SendCommand(CommandHolderMasterTriggerRelativeHumidityMeasurement);
}