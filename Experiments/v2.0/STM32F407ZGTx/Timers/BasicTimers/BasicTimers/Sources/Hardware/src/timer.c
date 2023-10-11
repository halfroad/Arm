#include "misc.h"

#include "usart.h"
#include "timer.h"


void init_basic_timer(uint16_t prescaler, uint16_t period)
{
	TIM_TimeBaseInitTypeDef tim_time_base_init_struct = {0};
	
	init_usart(115200);
	
	/* Enable the RCC clock on */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	
	
	/* 84MHz / 84 00 == 84 000 000 Hz / 84 00 = 10 000Hz == 10khz, 1 / 10KHz == 1 / 10 0000Hz == 0.0001s =  0.1ms == 100us. */
	tim_time_base_init_struct.TIM_Prescaler = prescaler;
	tim_time_base_init_struct.TIM_CounterMode = TIM_CounterMode_Up;
	/* 10 000 * 0.0001s == 1s.*/
	tim_time_base_init_struct.TIM_Period = period;
	tim_time_base_init_struct.TIM_ClockDivision = TIM_CKD_DIV1;
	
	TIM_TimeBaseInit(TIM6, &tim_time_base_init_struct);
	
	TIM_ClearFlag(TIM6, TIM_FLAG_Update);
	
	TIM_Cmd(TIM6, ENABLE);
	
	while (TIM_GetFlagStatus(TIM6, TIM_FLAG_Update) == RESET)
		;
	
	printf("Delay reached.\n");
	
	TIM_Cmd(TIM6, DISABLE);
}

void configure_nvic(IRQn_Type irqn, uint32_t preemptionPriority, uint32_t subpriority)
{
	NVIC_InitTypeDef nvic_init_struct;
	
	NVIC_SetPriorityGrouping(NVIC_PriorityGroup_2);
	
	nvic_init_struct.NVIC_IRQChannel = irqn;
	nvic_init_struct.NVIC_IRQChannelPreemptionPriority = preemptionPriority;
	nvic_init_struct.NVIC_IRQChannelSubPriority = subpriority;
	nvic_init_struct.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&nvic_init_struct);
	
}

void init_basic_timer_interrupt(uint16_t prescaler, uint16_t period)
{
	TIM_TimeBaseInitTypeDef tim_time_base_init_struct;

	init_usart(115200);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
		
	tim_time_base_init_struct.TIM_Prescaler = prescaler;
	tim_time_base_init_struct.TIM_CounterMode = TIM_CounterMode_Up;
	tim_time_base_init_struct.TIM_Period = period;
	tim_time_base_init_struct.TIM_ClockDivision = TIM_CKD_DIV1;
	
	TIM_TimeBaseInit(TIM6, &tim_time_base_init_struct);
	
	TIM_ClearFlag(TIM6, TIM_IT_Update);
	/* Enable the peripheral Update interrupt for the given timer TIM6. */
	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
	
	/* Enable the timer. */
	TIM_Cmd(TIM6, ENABLE);
	
	configure_nvic(TIM6_DAC_IRQn, 2, 2);
}

void TIM6_DAC_IRQHandler(void)
{
	/* Check whether the update flag is set. */
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) == SET)
	{
		/* Do what you want. */
		printf("Delay reached.\n");
	}
	
	/* Clear the Update flag to avoid be triggered repeatedly. */
	TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
}