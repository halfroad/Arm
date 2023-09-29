#include "stm32f4xx_exti.h"
#include "stm32f4xx_gpio.h"
#include "misc.h"

#include "SoftwareInterrupts.h"

void config_nvic(IRQn_Type irqn, uint32_t preemptPriority, uint32_t subpriority);

void init_software_interrupt(void)
{
	EXTI_InitTypeDef exti_init_struct;
	
	exti_init_struct.EXTI_Line = EXTI_Line1;
	exti_init_struct.EXTI_Mode = EXTI_Mode_Interrupt;
	exti_init_struct.EXTI_LineCmd = ENABLE;
	
	EXTI_Init(&exti_init_struct);
	
	config_nvic(EXTI1_IRQn, 1, 2);
}

void config_nvic(IRQn_Type irqn, uint32_t preemptPriority, uint32_t subpriority)
{
	NVIC_SetPriorityGrouping(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef nvic_init_struct;
	
	nvic_init_struct.NVIC_IRQChannel = irqn;
	nvic_init_struct.NVIC_IRQChannelPreemptionPriority = preemptPriority;
	nvic_init_struct.NVIC_IRQChannelSubPriority = subpriority;
	nvic_init_struct.NVIC_IRQChannelCmd = ENABLE;
}

void EXTI1_IRQHandler(void)
{
	/* If software interrupt occurs */
	if (EXTI_GetITStatus(EXTI_Line1) == SET)
	{
		/* Clear the flag of sofrware interrupt */
		EXTI_ClearITPendingBit(EXTI_Line1);
		
		/* User custom logics */
		/* Turn the LED off */
		GPIO_SetBits(GPIOF, GPIO_Pin_9);
		/* Turn the LED on */
		GPIO_ResetBits(GPIOF, GPIO_Pin_10);
	}
}