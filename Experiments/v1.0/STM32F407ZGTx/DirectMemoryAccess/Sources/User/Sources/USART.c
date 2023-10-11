#include "USART.h"

void init_usart_ports(uint32_t baudRate)
{
	/* Enable the RCC clock for USART1. */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	/* Initialize USART1 settings. */
	USART_InitTypeDef usart_init_struct;
	
	usart_init_struct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	usart_init_struct.USART_BaudRate = baudRate;
	usart_init_struct.USART_WordLength = USART_WordLength_8b;
	usart_init_struct.USART_Parity = USART_Parity_No;
	usart_init_struct.USART_StopBits = USART_StopBits_1;
	usart_init_struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	
	USART_Init(USART1, &usart_init_struct);
	USART_Cmd(USART1, ENABLE);
}