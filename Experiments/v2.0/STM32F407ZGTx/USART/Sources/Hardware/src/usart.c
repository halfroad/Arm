#include "usart_gpio.h"
#include "usart.h"

void usart_ports_init(uint32_t baudRate)
{
	init_usart_gpio_ports();
	
	/* Enable RCC clock for USART1. */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	USART_InitTypeDef usart_init_struct;
	
	usart_init_struct.USART_BaudRate = baudRate;
	usart_init_struct.USART_WordLength = USART_WordLength_8b;
	usart_init_struct.USART_StopBits = USART_StopBits_1;
	usart_init_struct.USART_Parity = USART_Parity_No;
	usart_init_struct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	usart_init_struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	
	USART_Init(USART1, &usart_init_struct);
	
	/* Enable the USART1. */
	USART_Cmd(USART1, ENABLE);
	
	USART_ClearFlag(USART1, USART_FLAG_TC);
}

void send_byte(uint16_t byte)
{
	/* Wait until the send register is empty. */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
		;
		
	USART_SendData(USART1, byte);
}

void send_bytes(uint8_t *bytes, uint16_t length)
{
	for (uint16_t i = 0; i < length; i ++)
		send_byte(bytes[i]);
}

uint16_t receive_byte(void)
{
	while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
		;
	
	return USART_ReceiveData(USART1);
}

void receive_bytes(uint8_t *bytes, uint16_t length)
{
	for (uint16_t i = 0; i < length; i ++)
		bytes[i] = receive_byte();
}