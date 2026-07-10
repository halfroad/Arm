#ifndef __USART_H
#define __USART_H

#include <stdint.h>

void InitUSART (uint32_t baudRate);
void TransmitReceive (void);

#endif