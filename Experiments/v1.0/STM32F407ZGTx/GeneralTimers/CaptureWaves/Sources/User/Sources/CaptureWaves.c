#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "misc.h"

#include "CaptureWaves.h"
#include "Serial.h"

#define PERIOD		7
#define PRESCALER	8

void init_pwm_wave_input_gpio(void)
{
	/* Enable Clock. */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	/* Initialize the GPIOA. */
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

void init_pwm_wave_input_timer(void)
{
	/* Enable cloc.k */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	
	/* Time Base initialization */
	TIM_TimeBaseInitTypeDef tim_time_base_init_struct;
	
	tim_time_base_init_struct.TIM_Period = PERIOD;
	tim_time_base_init_struct.TIM_Prescaler = PRESCALER;
	tim_time_base_init_struct.TIM_ClockDivision = TIM_CKD_DIV1;
	tim_time_base_init_struct.TIM_CounterMode = TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(TIM5, &tim_time_base_init_struct);
	
	/* PWM Waves capture initialization. */
	TIM_ICInitTypeDef tim_ic_init_struct;
	
	/* Channel 2 as the source of capture. */
	tim_ic_init_struct.TIM_Channel = TIM_Channel_2;
	/* Capture the rising edge. */
	tim_ic_init_struct.TIM_ICPolarity = TIM_ICPolarity_Rising;
	/* IC2 as the input source. */
	tim_ic_init_struct.TIM_ICSelection = TIM_ICSelection_DirectTI;
	/*No prescaler. */
	tim_ic_init_struct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	/* No filter */
	tim_ic_init_struct.TIM_ICFilter = 0x00;
	
	TIM_PWMIConfig(TIM5, &tim_ic_init_struct);
	
	/* The inout source of slave mode */
	TIM_SelectInputTrigger(TIM5, TIM_TS_TI2FP2);
	/* The slave mode is reset. */
	TIM_SelectSlaveMode(TIM5, TIM_SlaveMode_Reset);
	
	/* Enable the slave mode.*/
	TIM_SelectMasterSlaveMode(TIM5, TIM_MasterSlaveMode_Enable);
	
	TIM_ITConfig(TIM5, TIM_IT_CC2, ENABLE);
	
	config_nvic();
	
	TIM_Cmd(TIM5, ENABLE);
}

void TIM5_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM5, TIM_IT_CC2) == SET)
	{
		/* Clear the flag. */
		TIM_ClearITPendingBit(TIM5, TIM_IT_CC1);
		
		printf("Low power: %d\n", TIM_GetCapture1(TIM5) + 1);
		printf("Sum power: %d\n", TIM_GetCapture2(TIM5) + 1);
	}
}
	