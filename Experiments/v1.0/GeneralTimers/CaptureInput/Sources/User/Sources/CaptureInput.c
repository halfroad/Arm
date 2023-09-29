#include <stdio.h>
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "misc.h"

#include "CaptureInput.h"

#define CAPTURE_PULSE_PERIOD		7
#define CAPTURE_PULSE_PRESCALER		8

void init_capture_pulse_port(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef gpio_init_struct;
	
	gpio_init_struct.GPIO_Pin = GPIO_Pin_1;
	gpio_init_struct.GPIO_Mode = GPIO_Mode_AF;
	gpio_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &gpio_init_struct);
}

void config_nvic(void)
{
	NVIC_InitTypeDef nvic_init_struct;
	
	nvic_init_struct.NVIC_IRQChannel = TIM5_IRQn;
	nvic_init_struct.NVIC_IRQChannelPreemptionPriority = 0x01;
	nvic_init_struct.NVIC_IRQChannelSubPriority = 0x02;
	nvic_init_struct.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&nvic_init_struct);
}

void init_capture_pulse_timer(void)
{
	/* Enable Clock. */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	
	/* Time Base initialization */
	TIM_TimeBaseInitTypeDef tim_time_base_init_struct;
	
	tim_time_base_init_struct.TIM_Period = CAPTURE_PULSE_PERIOD;
	tim_time_base_init_struct.TIM_Prescaler = CAPTURE_PULSE_PRESCALER;
	tim_time_base_init_struct.TIM_ClockDivision = TIM_CKD_DIV1;
	tim_time_base_init_struct.TIM_CounterMode = TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(TIM5, &tim_time_base_init_struct);
	
	/* Input Capture */
	TIM_ICInitTypeDef tim_ic_init_struct;
	
	/* Select Channe 2 as the input source. */
	tim_ic_init_struct.TIM_Channel = TIM_Channel_2;
	/* Capture by rising edge */
	tim_ic_init_struct.TIM_ICPolarity = TIM_ICPolarity_Rising;
	/* Map to TI2 */
	tim_ic_init_struct.TIM_ICSelection = TIM_ICSelection_DirectTI;
	/* No prescaler. */
	tim_ic_init_struct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	/* Input filter. */
	tim_ic_init_struct.TIM_ICFilter = 0x00;
	
	TIM_ICInit(TIM5,&tim_ic_init_struct);
	
	TIM_ITConfig(TIM5, TIM_IT_CC2, ENABLE);
	
	config_nvic();
	
	TIM_Cmd(TIM5, ENABLE);
}

void TIM5_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM5, TIM_IT_CC2) == SET)
	{
		/* Clear the flag. */
		TIM_ClearITPendingBit(TIM5, TIM_IT_CC2);
		
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == Bit_SET)
		{
			/* Input captured is rising edge. */
			TIM_Cmd(TIM5, ENABLE);
			
			printf("File: %s\tline: %d\n", __FILE__, __LINE__);
		}
		else
		{
			/* Falling edge */
			TIM_Cmd(TIM5, DISABLE);
			/* Counter number by 1us, the value printed here is the current us of the pulse wave. */
			printf("Captured pulse: %d\n", TIM_GetCapture2(TIM5));
			
		}
	}
	
}
