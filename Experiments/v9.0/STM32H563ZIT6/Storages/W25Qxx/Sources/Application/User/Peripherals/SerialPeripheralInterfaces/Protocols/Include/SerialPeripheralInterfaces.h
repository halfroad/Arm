#ifndef __SERIAL_PERIPHERAL_INTERFACES_H
#define __SERIAL_PERIPHERAL_INTERFACES_H

#include <stm32g4xx.h>

typedef enum
{
    SlaveNotSelected,
    SlaveSelected,
    
} SlaveSelection;

void InitSerialPeripheralInterface(void);

void ConfigureBaudRate(uint8_t prescaler);
void SelectSlave(SlaveSelection selection);

uint8_t Transceive(uint8_t transmitByte);
void Transmit(uint8_t *bytes, uint16_t size);


#endif  /*  #ifndef __SERIAL_PERIPHERAL_INTERFACES_H    */