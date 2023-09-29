#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"

#include "PulseWidthModulation.h"

#define AUTO_RELOAD_REGISTER		7
#define PRESCALER					8
#define CAPTURE_COMPARE_REGISTER	4

void init_pwm_out_gpios(void)
{
	/* Enable the clock. https://zhuanlan.zhihu.com/p/120997067. */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef gpio_init_struct;
	
	gpio_init_struct.GPIO_Pin = GPIO_Pin_8;
	gpio_init_struct.GPIO_Mode = GPIO_Mode_AF;
	gpio_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &gpio_init_struct);
}

void init_pwm_out_timer(void)
{
	/* Enable the clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	/* Time base initialization. */
	TIM_TimeBaseInitTypeDef tim_time_base_init_struct;
	
	tim_time_base_init_struct.TIM_Period = AUTO_RELOAD_REGISTER;
	tim_time_base_init_struct.TIM_Prescaler = PRESCALER;
	tim_time_base_init_struct.TIM_ClockDivision = TIM_CKD_DIV1;
	tim_time_base_init_struct.TIM_CounterMode = TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(TIM1, &tim_time_base_init_struct);
	
	/*Output Comparation initialization. */
	TIM_OCInitTypeDef tim_oc_init_struct;
	
	/* The Pulse Width Modulation mode. */
	tim_oc_init_struct.TIM_OCMode = TIM_OCMode_PWM2;
	/* Enable the comparation output. */
	tim_oc_init_struct.TIM_OutputState = TIM_OutputState_Enable;
	/* Duty Ratio */
	tim_oc_init_struct.TIM_Pulse = CAPTURE_COMPARE_REGISTER;
	tim_oc_init_struct.TIM_OCPolarity = TIM_OCPolarity_High;
	
	TIM_OC1Init(TIM1, &tim_oc_init_struct);
	
	/* Enbale the Preload Register on CCR1 of TIM1. */
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	
	/* Enable ARPE */
	TIM_ARRPreloadConfig(TIM1, ENABLE);
	/* Enable the timer */
	TIM_Cmd(TIM1, ENABLE);
	
	/* MOE main output enbale. */
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
}