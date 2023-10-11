#include <stdio.h>

#include "timer.h"

#include "PulseWidthModulationWavesGenerator.h"
#include "NestedVectoredInterruptController.h"
#include "PulseWidthModulationWavesCapture.h"

void init_pulse_width_modulation_waves_capture(uint32_t prescaler, uint32_t period)
{
	/*
	No need to initialize the GPIO anymore.
	init_gpio();
	*/
	
	/* TIM5 as the Pulse Width Modulation Waves Capture. */
	init_timer(TIM5, RCC_APB1Periph_TIM5, prescaler, period);
	
	TIM_ICInitTypeDef tim_ic_init_type;
	
	tim_ic_init_type.TIM_Channel = TIM_Channel_1;
	tim_ic_init_type.TIM_ICPolarity = TIM_ICPolarity_Rising;
	tim_ic_init_type.TIM_ICSelection = TIM_ICSelection_DirectTI;
	tim_ic_init_type.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	tim_ic_init_type.TIM_ICFilter = 0x0000;
	
	TIM_ICInit(TIM5, &tim_ic_init_type);
	
	/* Select the source of trigger. */
	TIM_SelectInputTrigger(TIM5, TIM_TS_TI1FP1);
	/* Select the Slave mode. */
	TIM_SelectSlaveMode(TIM5, TIM_SlaveMode_Reset);
	
	TIM_ITConfig(TIM5, TIM_IT_CC1, ENABLE);
	
	TIM_Cmd(TIM5, ENABLE);
	
	configure_nested_vectored_interrupt_controller(TIM5_IRQn, 0x00000001, 0x00000001);
}

uint32_t get_frequency(void)
{
	return 1000000 / (TIM_GetCapture1(TIM5) + 1);
}

uint32_t get_duty_ratio(void)
{
	return (TIM_GetCapture2(TIM5) + 1) * 100 / (TIM_GetCapture1(TIM5) + 1);
}

void TIM5_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM5, TIM_IT_CC1) == SET)
	{
		
		printf("Frequency: %d, Dudy Ratio: %d.\n", get_frequency(), get_duty_ratio());
		
		TIM_ClearITPendingBit(TIM5, TIM_IT_CC1);
	}
}