#include "usart.h"

void usart_init(uint32_t baudRate)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	USART_InitTypeDef usart_init_struct;
	
	usart_init_struct.USART_Mode					= USART_Mode_Rx | USART_Mode_Tx;
	usart_init_struct.USART_BaudRate				= baudRate;
	usart_init_struct.USART_WordLength				= USART_WordLength_8b;
	usart_init_struct.USART_Parity					= USART_Parity_No;
	usart_init_struct.USART_StopBits				= USART_StopBits_1;
	usart_init_struct.USART_HardwareFlowControl 	= USART_HardwareFlowControl_None;
	
	USART_Init(USART1, &usart_init_struct);
	USART_Cmd(USART1, ENABLE);
	
	/* Trick to solve the MCU problem: Reset the Transmission Completion flag. */
	//USART_ClearFlag(USART1, USART_FLAG_TC);
}

void usart_transmit(uint16_t byte)
{
	USART_SendData(USART1, byte);
	
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)	/* Transmit Data Register Empty*/
		;
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
		;
}

uint16_t usart_receive(void)
{ 
    while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)	/* Receive Data Register Not Empty*/
		;
	
    uint16_t bibyte = USART_ReceiveData(USART1); 
     
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
		;
	
    USART_SendData(USART1, bibyte);
	
    return bibyte;
}