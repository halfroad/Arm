#include <stdio.h>
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "misc.h"

#include "BasicTimer.h"
#include "LED.h"

/* Basic timer with soft delay. */
void init_basic_timer_delay(uint16_t prescaler, uint16_t period, void (*onTimeArrival)())
{
	/* Enable the clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
	
	/* Intialize the timer TIM7 with default settings. */
	TIM_DeInit(TIM7);
	
	TIM_TimeBaseInitTypeDef tim_time_base_init_struct;
	
	/* Prescaler */
	tim_time_base_init_struct.TIM_Prescaler = prescaler;
	tim_time_base_init_struct.TIM_Period = period;
	tim_time_base_init_struct.TIM_CounterMode = TIM_CounterMode_Up;
	tim_time_base_init_struct.TIM_ClockDivision = TIM_CKD_DIV1;
	
	TIM_TimeBaseInit(TIM7, &tim_time_base_init_struct);
	
	/* Post the initialization, there would be an update flag, should be cleared.  */
	TIM_ClearFlag(TIM7, TIM_FLAG_Update);
	
	/* Enable the timer */
	TIM_Cmd(TIM7, ENABLE);
	
	/* Await until period reaches. */
	while(TIM_GetFlagStatus(TIM7, TIM_FLAG_Update) == RESET)
		onTimeArrival();
	
	TIM_ClearFlag(TIM7, TIM_FLAG_Update);
	/* Disable the timer. */
	TIM_Cmd(TIM7, DISABLE);
}

void init_basic_timer_irq(uint16_t prescaler, uint16_t period)
{
	/* Enable the clock. */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
	
	/* Reset to default status. */
	// TIM_DeInit(TIM7);
	
	TIM_TimeBaseInitTypeDef tim_time_base_init_struct;
	
	tim_time_base_init_struct.TIM_Prescaler = prescaler;
	tim_time_base_init_struct.TIM_Period = period;
	tim_time_base_init_struct.TIM_CounterMode = TIM_CounterMode_Up;
	tim_time_base_init_struct.TIM_ClockDivision = TIM_CKD_DIV1;
	
	TIM_TimeBaseInit(TIM7, &tim_time_base_init_struct);
	TIM_UpdateRequestConfig(TIM7, TIM_UpdateSource_Regular);
	
	/* Enable the timer. */
	TIM_Cmd(TIM7, ENABLE);
	TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);

	// TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
}

void config_nvic(IRQn_Type irqn, uint32_t preemptionPriority, uint32_t subpriority)
{
	// NVIC_SetPriorityGrouping(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef nvic_init_struct;
	
	nvic_init_struct.NVIC_IRQChannel = irqn;
	nvic_init_struct.NVIC_IRQChannelPreemptionPriority = preemptionPriority;
	nvic_init_struct.NVIC_IRQChannelSubPriority = subpriority;
	nvic_init_struct.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&nvic_init_struct);
}

void TIM7_IRQHandler(void)
{
	/* Check the flag. */
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) == SET)
	{
		/* Clear the flag */
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
		
		/* User custom logics. */
		GPIO_ToggleBits(GPIOF, GPIO_Pin_9 | GPIO_Pin_10);
	}
}