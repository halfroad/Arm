#include <stdio.h>
#include <string.h>

#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_syscfg.h"
#include "stm32f4xx_exti.h"
#include "misc.h"

#include "Button.h"
#include "LED.h"
#include "ExternalInterrupt.h"

void config_nvic(IRQn_Type irqn, uint32_t preemptPriority, uint32_t subpriority)
{
	NVIC_InitTypeDef nvic_init_struct;
	
	nvic_init_struct.NVIC_IRQChannel = irqn;
	nvic_init_struct.NVIC_IRQChannelPreemptionPriority = preemptPriority;
	nvic_init_struct.NVIC_IRQChannelSubPriority = subpriority;
	nvic_init_struct.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&nvic_init_struct);
}

void init_external_interrupt_line(void)
{
	init_external_interrupt_ports();
	init_led_gpios();
	
	/* Enable SYSCFG peripheral clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
	
	EXTI_InitTypeDef exti_init_struct;
	
	exti_init_struct.EXTI_Line = EXTI_Line0;
	exti_init_struct.EXTI_Mode = EXTI_Mode_Interrupt;
	exti_init_struct.EXTI_Trigger = EXTI_Trigger_Rising;
	exti_init_struct.EXTI_LineCmd = ENABLE;
	
	EXTI_Init(&exti_init_struct);
	
	/* Set the source of external interrupt line to PB9: k0 */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource8);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource9);
	
	memset(&exti_init_struct, 0, sizeof exti_init_struct);
	
	exti_init_struct.EXTI_Line = EXTI_Line8 | EXTI_Line9;
	exti_init_struct.EXTI_Mode = EXTI_Mode_Interrupt;
	exti_init_struct.EXTI_Trigger = EXTI_Trigger_Falling;
	exti_init_struct.EXTI_LineCmd = ENABLE;
	
	EXTI_Init(&exti_init_struct);
	
	/* Set the interrupt priorities and group */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	config_nvic(EXTI0_IRQn, 0x00, 0x02);
	config_nvic(EXTI9_5_IRQn, 0x03, 0x02);
}

void EXTI0_IRQHandler(void)
{
	EXTI_ClearITPendingBit(EXTI_Line0);
}
	
void EXTI9_5_IRQHandler(void)
{
	sprintf("%s", "aaaaaaaaaaa");
	
	/* External Interrupt occurs */
	if (EXTI_GetITStatus(EXTI_Line8) == SET)
	{
		/* Reset the flag of external interrupt */
		EXTI_ClearITPendingBit(EXTI_Line8);
		
		/* Custom logics, i.e. turn on/off the LED */
		GPIO_ToggleBits(GPIOF, GPIO_Pin_8);
	}
	
	if (EXTI_GetITStatus(EXTI_Line9) == SET)
	{
		/* Reset the flag of external interrupt */
		EXTI_ClearITPendingBit(EXTI_Line9);
		
		/* Custom logics, i.e. turn on/off the LED */
		GPIO_ToggleBits(GPIOF, GPIO_Pin_9);
	}
}