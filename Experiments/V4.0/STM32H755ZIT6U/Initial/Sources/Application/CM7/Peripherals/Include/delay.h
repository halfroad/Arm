#ifndef DELAY_H

#define DELAY_H


#include "stm32h7xx.h"

void InitDelay(uint16_t ticks);

void DelayMicroseconds(uint32_t numberOfMicroseconds);
void DelayMilliseconds(uint16_t numberOfMilliseconds);

#endif