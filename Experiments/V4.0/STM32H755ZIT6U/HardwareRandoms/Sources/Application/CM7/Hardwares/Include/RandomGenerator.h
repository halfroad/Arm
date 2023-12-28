#ifndef RANDOM_GENERATOR_H
#define RANDOM_GENERATOR_H

#include <stm32h7xx.h>

uint8_t InitRandomGenerator(void);

uint32_t GenerateRandomNumber(void);
uint32_t GenerateRangedRandomNumber(uint32_t minimum, uint32_t maximum);

#endif