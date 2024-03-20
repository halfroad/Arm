#ifndef __SERIAL_PERIPHERAL_INTERFACE_H
#define __SERIAL_PERIPHERAL_INTERFACE_H

#include <stm32h7xx.h>

void InitSerialPeripheralInterface(void);

void Transceive(uint8_t byte);

#endif  /*  #ifndef __SERIAL_PERIPHERAL_INTERFACE_H */