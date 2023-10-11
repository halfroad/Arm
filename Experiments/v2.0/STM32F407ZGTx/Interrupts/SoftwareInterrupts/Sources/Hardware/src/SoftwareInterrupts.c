#include "stm32f4xx_exti.h"
#include "misc.h"

#include "gpio.h"
#include "SoftwareInterrupts.h"

static void configure_nvic(IRQn_Type irqn, uint32_t preemptPriority, uint32_t subpriority);

void init_software_interrupt_line_0(void)
{
	EXTI_InitTypeDef exti_init_struct;
	
	init_led_gpios();
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	
	exti_init_struct.EXTI_Line = EXTI_Line0;
	exti_init_struct.EXTI_Mode = EXTI_Mode_Interrupt;
	exti_init_struct.EXTI_Trigger = EXTI_Trigger_Rising;
	exti_init_struct.EXTI_LineCmd = ENABLE;
	
	EXTI_Init(&exti_init_struct);
	
	configure_nvic(EXTI0_IRQn, 2, 2);
}

void configure_nvic(IRQn_Type irqn, uint32_t preemptPriority, uint32_t subpriority)
{
	NVIC_InitTypeDef nvic_init_struct;
	
	/* Priority grouping. */
	NVIC_SetPriorityGrouping(NVIC_PriorityGroup_2);
	
	nvic_init_struct.NVIC_IRQChannel = irqn;
	nvic_init_struct.NVIC_IRQChannelPreemptionPriority = preemptPriority;
	nvic_init_struct.NVIC_IRQChannelSubPriority = subpriority;
	nvic_init_struct.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&nvic_init_struct);
}

void EXTI0_IRQHandler(void)
{
	/* Check if the External Interrupt Line 1 is asserted. */
	if (EXTI_GetFlagStatus(EXTI_Line0) == SET)
	{
		/* Clear the flag of External Interrupt Line 1 to avoid being asserted repeatedly. */
		EXTI_ClearITPendingBit(EXTI_Line0);
		
		GPIO_ToggleBits(GPIOF, GPIO_Pin_9 | GPIO_Pin_10);
	}
}

void trigger_software_interrupt(void)
{
	EXTI_GenerateSWInterrupt(EXTI_Line0);
}

