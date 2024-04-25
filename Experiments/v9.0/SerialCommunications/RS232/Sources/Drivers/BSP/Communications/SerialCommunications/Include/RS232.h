#ifndef __RS485_H
#define __RS485_H

#include <stm32g4xx.h>

void InitRS485(void onBytesReceivedHandler(void *protocol, uint8_t *bytes, uint16_t length));

void SendRS485Message(char *message);

#endif  /*  #define __RS485_H   */