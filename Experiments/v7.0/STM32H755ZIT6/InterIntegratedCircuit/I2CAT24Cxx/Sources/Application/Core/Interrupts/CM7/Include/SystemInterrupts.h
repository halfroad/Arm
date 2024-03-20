#ifndef SYSTEM_INTERRUPTS_H
#define SYSTEM_INTERRUPTS_H

#include <stm32h7xx.h>

#define NVIC_PRIORTY_GROUP  2

void ConfigNestedVectoredInterruptsPriortyGroup(uint8_t groupNumber);
void SetNestedVectoredInterruptPriorty(uint8_t preemptionPriority, uint8_t subPriority, uint8_t irqn);

#endif