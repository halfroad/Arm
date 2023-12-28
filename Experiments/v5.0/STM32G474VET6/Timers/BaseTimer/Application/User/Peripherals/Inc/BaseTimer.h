#ifndef __BASE_TIMER_H
#define __BASE_TIMER_H

#include <stm32g4xx.h>

void InitTimer(uint16_t prescaler, uint16_t autoReload, void (* peroidArrived)(TIM_HandleTypeDef *htim));

#endif