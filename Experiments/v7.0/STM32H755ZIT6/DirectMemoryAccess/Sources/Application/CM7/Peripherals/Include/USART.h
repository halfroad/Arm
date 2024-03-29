#ifndef USART_H
#define USART_H

#include <stm32h7xx.h>

#define USART                                   USART3

void InitUSART(uint32_t APB1Clock, uint32_t baudRate, void(*onReceived)(uint8_t *buffer, uint8_t status));
void SendByte(uint8_t byte);

#endif