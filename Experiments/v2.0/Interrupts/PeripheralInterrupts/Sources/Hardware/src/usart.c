#include <stdio.h>
#include "misc.h"

#include "usart_gpio.h"
#include "usart.h"
#include "printable.h"

#define USART_RECEIVE_MAXIMUM_BUFFER_LENGTH		100

static void config_nvic(IRQn_Type irqn, uint32_t preemptionPriority, uint32_t subpriority);

struct USART_Data
{
	uint16_t bibyte;
	uint16_t usart_receive_buffer_length;
	uint8_t usart_receive_buffer[USART_RECEIVE_MAXIMUM_BUFFER_LENGTH];
	uint8_t usart_receive_flag;
	
} usart_data;

void init_usart(uint32_t baudRate)
{
	init_usart_gpio_ports();
	
	/* Enbale RCC clock for USART1. */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	USART_InitTypeDef usart_init_struct;
	
	usart_init_struct.USART_BaudRate = baudRate;
	usart_init_struct.USART_WordLength = USART_WordLength_8b;
	usart_init_struct.USART_StopBits = USART_StopBits_1;
	usart_init_struct.USART_Parity = USART_Parity_No;
	usart_init_struct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	usart_init_struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	
	USART_Init(USART1, &usart_init_struct);
	
	/* 0b1010 0000. */
	config_nvic(USART1_IRQn, 2, 2);
	
	USART_Cmd(USART1, ENABLE);
}

static void config_nvic(IRQn_Type irqn, uint32_t preemptionPriority, uint32_t subpriority)
{
	NVIC_InitTypeDef nvic_init_struct;
	
	/* Set the group of priorities. */
	NVIC_SetPriorityGrouping(NVIC_PriorityGroup_4);
	
	/* Initialize the struct of NVIC settings. */
	nvic_init_struct.NVIC_IRQChannel = irqn;
	nvic_init_struct.NVIC_IRQChannelPreemptionPriority = preemptionPriority;
	nvic_init_struct.NVIC_IRQChannelSubPriority = subpriority;
	nvic_init_struct.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&nvic_init_struct);
	
	/* Enable the Receive Not empty interrupt on USART1. */
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	/* Enable the Idle interrupt on USART1. */
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
}

void send_byte(uint16_t byte)
{
	/* Wait until the transmission register is empty. */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
		;
	
	USART_SendData(USART1, byte);
}

void USART1_IRQHandler(void)
{
	/* Check if the Receive Interrupt Flag is triggered. */
	if (USART_GetFlagStatus(USART1, USART_IT_RXNE) != RESET)
	{		
		/* Reset the Flag to avoid being triggered repeatedly. */
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
				
		if (usart_data.usart_receive_buffer_length < USART_RECEIVE_MAXIMUM_BUFFER_LENGTH)
		{
			usart_data.bibyte = USART_ReceiveData(USART1);
			
			//printf("%#04x\n", usart_data.bibyte);
			printf("Received character: %c\n", usart_data.bibyte);
			
			usart_data.usart_receive_buffer[usart_data.usart_receive_buffer_length ++] = USART_ReceiveData(USART1);
			
			printf("Received string: %s\n", usart_data.usart_receive_buffer);
		}
		
	}
	
	/* Check if the Idle Interrupt Flag is triggered.*/
	if (USART_GetITStatus(USART1, USART_IT_IDLE) == SET)
	{
		printf("%s\n", "USART_IT_IDLE");
		
		USART_ClearITPendingBit(USART1, USART_IT_IDLE);
		usart_data.usart_receive_flag = 1;
		
		printf("%s\n", usart_data.usart_receive_buffer);
	}
}