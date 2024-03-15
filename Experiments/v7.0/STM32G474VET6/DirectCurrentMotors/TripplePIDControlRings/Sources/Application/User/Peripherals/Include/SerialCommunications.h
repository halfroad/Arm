#ifndef __SERIAL_COMMUNICATIONS_H
#define __SERIAL_COMMUNICATIONS_H

#include <stm32g4xx.h>

void InitSerialCommunications(uint32_t baudRate, void *protocol, void onByteReceivedHandler(void *protocol, uint8_t *byte));

HAL_StatusTypeDef tranmist(uint8_t *byte, uint16_t length);

#endif