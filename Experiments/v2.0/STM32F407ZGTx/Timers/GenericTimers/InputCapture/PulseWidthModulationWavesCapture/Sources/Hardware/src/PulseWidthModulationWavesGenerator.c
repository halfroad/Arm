#include "gpio.h"
#include "timer.h"

#include "PulseWidthModulationWavesGenerator.h"


/*

https://blog.csdn.net/zhuhongqiana/article/details/129581954

*/

void init_pulse_width_modulation_waves_generator(uint32_t prescaler, uint32_t period)
{
	init_gpio();
	
	/* TIM3 for Pulse Width Modulation Waves Generator. */
	init_timer(TIM3, RCC_APB1Periph_TIM3, prescaler, period);
	
	TIM_OCInitTypeDef tim_oc_init_type;
	
	TIM_OCStructInit(&tim_oc_init_type);
	
	tim_oc_init_type.TIM_OCMode = TIM_OCMode_PWM1;
	tim_oc_init_type.TIM_OutputState = TIM_OutputState_Enable;
	 /*!< Specifies the pulse value to be loaded into the Capture Compare Register. 
                                   This parameter can be a number between 0x0000 and 0xFFFF
	TIM_SetCompare1(New Capture Compare Register)
	*/
	tim_oc_init_type.TIM_Pulse = 0x00000000;
	
	tim_oc_init_type.TIM_OCPolarity = TIM_OCPolarity_High;
	
	/* Channel 1 on TIM3. */
	TIM_OC1Init(TIM3, &tim_oc_init_type);
	
	/* Enable Preload on cCR1.
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	Enable ARPE.
	TIM_ARRPreloadConfig(TIM3, ENABLE);
	*/
	
	TIM_Cmd(TIM3, ENABLE);	
}

void set_pulse_width_modulation_waves_generator_comparation(uint32_t comparation)
{
	TIM_SetCompare1(TIM3, comparation);
}