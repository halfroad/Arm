#include "timer.h"


void init_timer(uint32_t prescaler, uint32_t period)
{
	/* Enable the RCC clock for TIM5 on APB1. */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	
	TIM_TimeBaseInitTypeDef tim_time_base_init_type;
	
	tim_time_base_init_type.TIM_Prescaler = prescaler;
	tim_time_base_init_type.TIM_CounterMode = TIM_CounterMode_Up;
	tim_time_base_init_type.TIM_Period = period;
	tim_time_base_init_type.TIM_ClockDivision = TIM_CKD_DIV1;
	/*TIM_RepetitionCounter only available on Advanced Timer 1 and 8
	tim_time_base_init_type.TIM_RepetitionCounter = 0;
	 */
	
	TIM_TimeBaseInit(TIM5, &tim_time_base_init_type);
}