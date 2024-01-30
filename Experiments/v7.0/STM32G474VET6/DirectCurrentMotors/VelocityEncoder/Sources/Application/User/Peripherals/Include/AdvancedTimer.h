#ifndef __ADVANCED_TIMER_H
#define __ADVANCED_TIMER_H

#include <stm32g4xx.h>

void InitAdvancedTimer(uint16_t prescaler, uint16_t autoReload,uint16_t defaultCompare);

#endif /* __ADVANCED_TIMER_H */