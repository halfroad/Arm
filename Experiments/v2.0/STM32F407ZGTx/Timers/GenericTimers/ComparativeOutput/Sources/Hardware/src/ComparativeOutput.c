#include "gpio.h"
#include "timer.h"
#include "ComparativeOutput.h"


void init_output_comparation(uint32_t pulse)
{
	TIM_OCInitTypeDef tim_oc_init_type;
	
	TIM_OCStructInit(&tim_oc_init_type);
	
	tim_oc_init_type.TIM_OCMode = TIM_OCMode_PWM1;
	tim_oc_init_type.TIM_OutputState = TIM_OutputState_Enable;
	/* TIM_Pulse: Specifies the pulse value to be loaded into the Capture Compare Register (CCR) */
	tim_oc_init_type.TIM_Pulse = pulse;
	/* Polarity: High or Low. */
	tim_oc_init_type.TIM_OCPolarity = TIM_OCPolarity_High;
	
	/* Utilize the channel 1 on TIM3 to output the PWM waves on Alternated Functions PA6 to control the lightness of LED. */
	TIM_OC1Init(TIM3, &tim_oc_init_type);
	
	/* Channel 1 - 4.
	
	TIM_OC2Init(TIM3, &tim_oc_init_type);
	TIM_OC3Init(TIM3, &tim_oc_init_type);
	TIM_OC4Init(TIM3, &tim_oc_init_type);
	*/
	
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
	TIM_Cmd(TIM3, ENABLE);
	
}

void assign_pwm_comparation(uint16_t comparation)
{
	TIM_SetCompare1(TIM3, comparation);
}


void generate_pwm_waves(void)
{
	init_gpio_port();
	init_pwm_timer(84 - 1, 16800 - 1);
	init_output_comparation(1277 - 1);
}
