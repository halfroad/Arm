#ifndef PULSE_WIDTH_MODULATION_H
#define PULSE_WIDTH_MODULATION_H

#include <stm32h7xx.h>

void InitPulseWdithModulation(uint32_t prescaler, uint32_t autoReload);
void SetCaptureCompare(uint16_t newCompare);

#endif