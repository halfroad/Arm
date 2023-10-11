#include <string.h>

#include "timer.h"

void init_timer(TIM_TypeDef *tim, uint32_t RCC_APB1Periph, uint32_t prescaler, uint32_t period)
{
	/* Enable the RCC clock on APB1. */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph, ENABLE);
	
	TIM_TimeBaseInitTypeDef tim_time_base_init_type;
	
	/* Timer settings. */
	tim_time_base_init_type.TIM_Prescaler = prescaler;
	tim_time_base_init_type.TIM_CounterMode = TIM_CounterMode_Up;
	tim_time_base_init_type.TIM_Period = period;
	tim_time_base_init_type.TIM_ClockDivision = TIM_CKD_DIV1;
	/* Only availbale on TIM1 and TIM8, the Advanced Timers.
	tim_time_base_init_type.TIM_RepetitionCounter = 0;
	*/
	
	TIM_TimeBaseInit(tim, &tim_time_base_init_type);
}