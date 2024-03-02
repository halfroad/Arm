#ifndef __TICK_TIMER_H
#define __TICK_TIMER_H

#include <stm32g4xx.h>

void InitTickTimer(uint32_t prescaler, uint32_t autoReload);

void OnTickTimerPeriodElapsed(TIM_HandleTypeDef *htim);

#endif  /*  #ifndef __TICK_TIMER_H  */