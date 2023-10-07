#include "stm32f4xx_tim.h"

#include "timer.h"


void init_basic_timer(uint16_t delay)
{
	/* Enable the RCC clock on */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	
	TIM_TimeBaseInitTypeDef tim_time_base_init_struct;
	
	/* 84MHz / 84 00 == 84 000 000 Hz / 84 00 = 10 000Hz == 10khz, 1 / 10KHz == 1 / 10 0000Hz == 0.0001s =  0.1ms == 100us. */
	tim_time_base_init_struct.TIM_Prescaler = 8400 - 1;
	/* 10 000 * 0.0001s == 1s.*/
	tim_time_base_init_struct.TIM_Period = 100000;
	
	TIM_TimeBaseInit(TIM6, &tim_time_base_init_struct);
	
	TIM_Cmd(TIM6, ENABLE);
	
	while (TIM_GetFlagStatus(TIM6, TIM_FLAG_Update) == RESET)
		;
	
	//
	
	TIM_ClearFlag(TIM6, TIM_FLAG_Update);
	
	TIM_Cmd(TIM6, DISABLE);
}