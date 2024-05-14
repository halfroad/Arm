#ifndef __ZS_040_H
#define __ZS_040_H

#include <stm32h5xx.h>

#include "./Peripherals/Communications/SerialCommunications/Include/CircularBuffer.h"

void InitSerialBlueTooth(uint32_t baudRate, void (* OnBytesReceived)(void));

HAL_StatusTypeDef Tranmist(uint8_t *byte, uint16_t length);

#endif  /*  #ifndef __ZS_040_H  */