#include <string.h>
#include "usart.h"

#define USART_RECV_MAX_BUF_LEN 100

typedef struct
{
	uint32_t usart_recv_buf_len;
	uint8_t usart_recv_buf[USART_RECV_MAX_BUF_LEN];
	uint8_t usart_recv_flag;
	
} USARTData;

void configure_nvic(IRQn_Type irqn, uint32_t preemptPriority, uint32_t subpriority)
{
	NVIC_InitTypeDef nvic_init_struct;
	
	/* Set the grouping priority of NVIC */
	NVIC_SetPriorityGrouping(NVIC_PriorityGroup_1);
	
	/* Enable the NVIC interrupt */
	nvic_init_struct.NVIC_IRQChannel = irqn;
	nvic_init_struct.NVIC_IRQChannelPreemptionPriority = preemptPriority;
	nvic_init_struct.NVIC_IRQChannelSubPriority = subpriority;
	nvic_init_struct.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&nvic_init_struct); 
}

void init_usart_gpio_ports(void)
{
	/* Enabe GPIOA clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	/* Initialize GPIOA9 interface */
	GPIO_InitTypeDef gpio_init_struct;
	
	gpio_init_struct.GPIO_Pin = GPIO_Pin_9;
	gpio_init_struct.GPIO_Mode = GPIO_Mode_AF;
	gpio_init_struct.GPIO_OType = GPIO_OType_PP;
	gpio_init_struct.GPIO_Speed = GPIO_Speed_100MHz;
	gpio_init_struct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	GPIO_Init(GPIOA, &gpio_init_struct);
	
	/* Set the type of AF for Pin 9 */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	
	memset(&gpio_init_struct, 0, sizeof gpio_init_struct);
	
	gpio_init_struct.GPIO_Pin = GPIO_Pin_10;
	gpio_init_struct.GPIO_Mode = GPIO_Mode_AF;
	gpio_init_struct.GPIO_Speed = GPIO_Speed_100MHz;
	gpio_init_struct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	GPIO_Init(GPIOA, &gpio_init_struct);
	
	/* Set the type of AF for pin 10 */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
}

void init_usart_ports(uint32_t baudRate)
{
	init_usart_gpio_ports();
	
	/* Enable the USART1 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	/* Initialize the settings of USART1 */
	USART_InitTypeDef usart_init_struct;
	
	usart_init_struct.USART_BaudRate = baudRate;
	usart_init_struct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	usart_init_struct.USART_Parity = USART_Parity_No;
	usart_init_struct.USART_WordLength = USART_WordLength_8b;
	usart_init_struct.USART_StopBits = USART_StopBits_1;
	usart_init_struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	
	USART_Init(USART1, &usart_init_struct);
	
	/* Settings of Interrupt */
	configure_nvic(USART1_IRQn, 2, 2);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
	
	USART_Cmd(USART1, ENABLE);
	
	/* Clear the Completion Flag */
	USART_ClearFlag(USART1, USART_FLAG_TC);
}

void send(uint8_t byte)
{
	/* Wait until the send register is empty */
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
		;
	
	USART_SendData(USART1, byte);
}

void USART1_IRQHandler(void)
{
	USARTData usart_data;
	
	/* Check the flag of RX interrupt */
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		/* Clear the RX interrupt flag to avoid repetition */
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		
		/* Read the byte in the case the received data length is less than the storage space */
		if (usart_data.usart_recv_buf_len < USART_RECV_MAX_BUF_LEN)
			usart_data.usart_recv_buf[usart_data.usart_recv_buf_len++] = USART_ReceiveData(USART1);
	}
	
	/* Idle */
	if (USART_GetITStatus(USART1, USART_IT_IDLE) == SET)
	{
		/* Clear the interrupt flag */
		USART_ReceiveData(USART1);
		
		/* Completion of RX */
		usart_data.usart_recv_flag = 1;
	}
}