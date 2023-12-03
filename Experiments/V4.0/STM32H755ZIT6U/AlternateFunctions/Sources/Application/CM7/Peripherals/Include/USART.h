#ifndef USART_H
#define USART_H

#include <stm32h7xx.h>

void InitUSART(uint32_t APB1Clock, uint32_t baudRate, void(*onReceived)(uint8_t *buffer, uint8_t status));

#endif