#include <stdio.h>

#include "stm32f4xx_usart.h"

void usart_init(uint32_t baudRate);

void transmit(uint16_t bibyte);
uint16_t receive(void);