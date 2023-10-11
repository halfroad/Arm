#include <stdio.h>
#include "stm32f4xx_tim.h"

#include "gpio.h"
#include "timer.h"
#include "usart.h"
#include "NestedVectoredInterruptController.h"
#include "InputCapture.h"

void init_pulse_waves_capture(void)
{
	TIM_ICInitTypeDef tim_ic_init_type;
	
	init_usart(115200);
	init_gpio_port();
	/*
		https://blog.csdn.net/qq_42348833/article/details/105853239
	*/
	/* Milliseconds. */
	init_timer(8400 - 1, 100 - 1);
	/* Microseconds.
		init_timer(84 - 1, 0xffffffff);
	*/
	
	tim_ic_init_type.TIM_Channel = TIM_Channel_2;
	tim_ic_init_type.TIM_ICPolarity = TIM_ICPolarity_BothEdge;
	tim_ic_init_type.TIM_ICSelection = TIM_ICSelection_DirectTI;
	tim_ic_init_type.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	tim_ic_init_type.TIM_ICFilter = 0x0000;
	
	TIM_ICInit(TIM5, &tim_ic_init_type);
	
	TIM_ITConfig(TIM5, TIM_IT_CC2, ENABLE);
	
	/* Disable the tiemr for this moment. */
	TIM_Cmd(TIM5, DISABLE);
	
	configure_nested_vectored_interrupt_controller(TIM5_IRQn, 0x01, 0x03);
}

void TIM5_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM5, TIM_IT_CC2) == SET)
	{
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == Bit_SET)
		{
			/* Bit SET means the Rising edge is captured, then the timer is triggered to start recording. */
			TIM_Cmd(TIM5, ENABLE);
			
			printf("file: %s\tline; %d\n", __FILE__, __LINE__);
		}
		else
		{
			/* Otherwise, the Bit RESET means the falling edge is captured, then the timer is triggered to stop the recording. */
			TIM_Cmd(TIM5, DISABLE);
			
			/* Capture2 reflects to Capture/Comparator 2. */
			uint32_t microseconds = TIM_GetCapture2(TIM5);
			
			printf("Captured pulse; %d\n", microseconds);
		}
	}
	
	/* Clear the interrupt pending flag. */
	TIM_ClearITPendingBit(TIM5, TIM_IT_CC1);
}