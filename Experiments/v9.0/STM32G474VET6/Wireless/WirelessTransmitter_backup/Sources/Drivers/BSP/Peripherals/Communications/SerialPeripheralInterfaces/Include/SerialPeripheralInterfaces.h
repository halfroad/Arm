#ifndef __SERIAL_PERIPHERAL_INTERFACES_H
#define __SERIAL_PERIPHERAL_INTERFACES_H

#include <stm32g4xx.h>

typedef enum
{
    SlaveSelectedSignal  ,
    SlaveDeselectedSignal,
    
} SlaveSelectionSignals;

void InitSerialPeripheralInterfaces(void);

void SelectSlave(SlaveSelectionSignals signal);

HAL_StatusTypeDef SpiTransmit(uint8_t *bytes, uint8_t size);
HAL_StatusTypeDef SpiReceive(uint8_t *bytes,  uint8_t size);
HAL_StatusTypeDef SpiTransceive(uint8_t *transmitBytes, uint8_t *receivedBytes,  uint8_t size);

#endif  /*  #ifndef __SERIAL_PERIPHERAL_INTERFACES_H    */