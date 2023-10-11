#include "NestedVectoredInterruptController.h"

void configure_nested_vectored_interrupt_controller(IRQn_Type irqn, uint32_t preemptionPriority, uint32_t subpriority)
{
	NVIC_InitTypeDef nvic_init_type;
	
	/* Set the priority grou. */
	NVIC_SetPriorityGrouping(NVIC_PriorityGroup_2);
	
	nvic_init_type.NVIC_IRQChannel = irqn;
	nvic_init_type.NVIC_IRQChannelPreemptionPriority = preemptionPriority;
	nvic_init_type.NVIC_IRQChannelSubPriority = subpriority;
	nvic_init_type.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&nvic_init_type);
}