#ifndef __PULSE_WIDTH_MODULATION_H

#define __PULSE_WIDTH_MODULATION_H

#include <stm32g4xx.h>

void InitPulseWidthModulation(uint16_t prescaler, uint16_t autoReload);

void SetCompare(uint16_t newCompare);

#endif