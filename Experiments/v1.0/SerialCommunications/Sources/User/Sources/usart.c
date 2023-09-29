#include "stm32f4xx_gpio.h"
#include "usart.h"

void init_usart_ports(uint32_t bautRate)
{
	/* Enable USART1 clock. */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	/* Initialize USART1 Settings */
	USART_InitTypeDef usart_init_struct;
	
	usart_init_struct.USART_BaudRate 						= bautRate;
	/* Receive and Transmit */
	usart_init_struct.USART_Mode 								= USART_Mode_Tx | USART_Mode_Rx;
	usart_init_struct.USART_WordLength					= USART_WordLength_8b;
	usart_init_struct.USART_Parity							= USART_Parity_No;
	usart_init_struct.USART_StopBits						= USART_StopBits_1;
	usart_init_struct.USART_HardwareFlowControl	= USART_HardwareFlowControl_None;
	
	USART_Init(USART1, &usart_init_struct);
	
	USART_Cmd(USART1, ENABLE);
	
	/* Trick to solve the MCU problem: Reset the completion flag. */
	USART_ClearFlag(USART1, USART_FLAG_TC);
}

void transmit(uint8_t byte)
{
	/* Wait for the the TX register to be empty. */
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
		;
	
	USART_SendData(USART1, byte);
}

uint8_t receive(void)
{
	/* Wait until a byte arrives */
	while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
		;
	
	/* Byte received */
	return USART_ReceiveData(USART1);
}
