#ifndef _TIMER_H
#define _TIMER_H

#include <stdint.h>
#include "stm32f4xx.h"
 
void init_delay(uint8_t SYSCLK);

void delay_us(uint32_t ticks);
void delay_ms(uint16_t ms);

#endif
