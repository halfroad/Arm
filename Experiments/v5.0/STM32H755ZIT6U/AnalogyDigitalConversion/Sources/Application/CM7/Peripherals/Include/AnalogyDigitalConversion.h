#ifndef __ANALOGY_DIGITAL_CONVERSION_H

#define __ANALOGY_DIGITAL_CONVERSION_H

#include <stm32h7xx.h>

void InitAnalogyDigitalConversion(void);

uint16_t GetAverageAnalogyDigitalConversion(uint8_t channel, uint8_t times);

#endif