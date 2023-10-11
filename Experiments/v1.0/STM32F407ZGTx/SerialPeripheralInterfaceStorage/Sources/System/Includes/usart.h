#include <stdio.h>

#include "stm32f4xx_usart.h"

void usart_init(uint32_t baudRate);

void usart_transmit(uint16_t byte);
uint16_t usart_receive(void);