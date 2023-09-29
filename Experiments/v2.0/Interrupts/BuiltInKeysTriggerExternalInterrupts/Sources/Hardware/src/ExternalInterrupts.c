#include "stm32f4xx_exti.h"
#include "stm32f4xx_syscfg.h"
#include "misc.h"

#include "gpio.h"
#include "ExternalInterrupts.h"

void configure_nvic(IRQn_Type irqn, uint32_t preemptPriority, uint32_t subpriority);

void init_external_interrupt_line_8_9(void)
{
	init_led_gpios();
	init_external_interrupt_gpio();
	
	/* Enable the SYSCFG Peripheral clock. */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	/**
	  * @brief  Selects the GPIO pin used as EXTI Line.
	  * @param  EXTI_PortSourceGPIOx : selects the GPIO port to be used as source for
	  *          EXTI lines
	  *            
	  * @param  EXTI_PinSourcex: specifies the EXTI line to be configured. 
	  *             
	  * @retval None
	*/
	/*
	Be noted that External Interrupt Line 0 can be mapped to PA0 - PI0,
	External Interrupt Line 1 can be mapped to PA1 - PI1,
	External Interrupt Line 2 can be mapped to PA2 - PI2,
	.
	.
	.
	External Interrupt Line 15 can be mapped to PA15 - PI15,
	External Interrupt Line 16 can be mapped to PVD
	.
	.
	.
	External Interrupt Line 22 can be mapped to RTC Awake event.
	*/
	
	/* External Interrupt Line 8 and 9 link to Pin 8 and 9 on GPIOB. */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource8);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource9);
	
	EXTI_InitTypeDef exti_init_struct;
	
	exti_init_struct.EXTI_Line = EXTI_Line8 | EXTI_Line9;
	exti_init_struct.EXTI_Mode = EXTI_Mode_Interrupt;
	exti_init_struct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	exti_init_struct.EXTI_LineCmd = ENABLE;
	
	EXTI_Init(&exti_init_struct);
	
	configure_nvic(EXTI9_5_IRQn, 2, 2);
	
	/*
	configure_nvic(EXTI0_IRQn, 2, 2);
	configure_nvic(EXTI9_5_IRQn, 2, 2);
	configure_nvic(EXTI15_10_IRQn, 2, 2);
	*/
}

void configure_nvic(IRQn_Type irqn, uint32_t preemptPriority, uint32_t subpriority)
{
	NVIC_InitTypeDef nvic_intit_struct;
	
	/* Enable the priority group. */
	NVIC_SetPriorityGrouping(NVIC_PriorityGroup_2);
	
	nvic_intit_struct.NVIC_IRQChannel = irqn;
	nvic_intit_struct.NVIC_IRQChannelPreemptionPriority = preemptPriority;
	nvic_intit_struct.NVIC_IRQChannelSubPriority = subpriority;
	nvic_intit_struct.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&nvic_intit_struct);
}

void EXTI9_5_IRQHandler(void)
{
	/* Check if the External Interrupt Line 8 is triggered. */
	if (EXTI_GetITStatus(EXTI_Line8) == SET)
	{
		/* Clear the pending Interrupt flag to avoid being triggered repeadtedly. */
		EXTI_ClearITPendingBit(EXTI_Line8);
		
		GPIO_ToggleBits(GPIOF, GPIO_Pin_9 | GPIO_Pin_10);
	}
	else if (EXTI_GetITStatus(EXTI_Line9) == SET)
	{
		/* Clear the pending Interrupt flag to avoid being triggered repeadtedly. */
		EXTI_ClearITPendingBit(EXTI_Line9);
		
		GPIO_ToggleBits(GPIOF, GPIO_Pin_9 | GPIO_Pin_10);
	}
}