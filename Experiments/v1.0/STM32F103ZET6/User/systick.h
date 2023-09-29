#ifndef _SYSTICK_H_
#define _SYSTICK_H_

#include "stm32f10x.h"

void Delay_us(uint32_t us);
void Delay_ms(uint16_t ms);
void Delay_count(void);

#endif
