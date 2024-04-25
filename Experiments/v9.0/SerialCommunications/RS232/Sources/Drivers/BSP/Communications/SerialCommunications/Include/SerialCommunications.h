#ifndef __SERIAL_COMMUNICATIONS_H
#define __SERIAL_COMMUNICATIONS_H

#include <stm32g4xx.h>

void InitSerialCommunications(uint32_t baudRate, void *protocol, void (* newBytesReceivedHandler)(void *protocol, uint8_t *bytes, uint16_t length));

HAL_StatusTypeDef TransmitDMA(uint8_t *bytes, uint16_t length);
HAL_StatusTypeDef ReceivedDMA(uint8_t *bytes,uint8_t length);

#endif