#ifndef DELAY_H
#define DELAY_H


#include <stm32h7xx.h>

void InitDelay();

void DelayUs(uint32_t us);
void DelayMs(uint32_t ms);

#endif