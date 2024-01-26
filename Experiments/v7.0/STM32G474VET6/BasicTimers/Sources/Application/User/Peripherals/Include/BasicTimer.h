#ifndef __ADVANCED_TIMER_H
#define __ADVANCED_TIMER_H

#include <stm32g4xx.h>

HAL_StatusTypeDef InitBasicTimer(uint32_t prescaler, uint32_t autoReload);

#endif /* __ADVANCED_TIMER_H */