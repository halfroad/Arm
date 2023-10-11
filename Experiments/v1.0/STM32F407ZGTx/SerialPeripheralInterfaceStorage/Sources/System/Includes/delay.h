#ifndef _TIMER_H
#define _TIMER_H

#include <stdint.h>
#include "stm32f4xx.h"
 
void delay_init(uint8_t SYSCLK);

void delay_ms(uint16_t ms);
void delay_us(uint32_t us);

#endif