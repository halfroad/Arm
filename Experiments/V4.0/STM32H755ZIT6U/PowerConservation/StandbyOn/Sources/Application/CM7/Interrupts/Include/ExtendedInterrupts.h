#ifndef EXTERNAL_INTERRUPTS_H
#define EXTERNAL_INTERRUPTS_H

#include <stm32h7xx.h>

void ConfigureExternalInterrupt(uint8_t GPIOx, uint8_t bitNumber, uint8_t triggerEdge);

#endif