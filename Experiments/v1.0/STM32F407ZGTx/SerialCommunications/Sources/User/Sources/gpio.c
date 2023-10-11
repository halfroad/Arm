#include <string.h>
#include "stm32f4xx_gpio.h"
#include "gpio.h"

/* Initialize the GPIOA 9 and 10 for USART. */
void init_uart_gpio_ports(void)
{
	/* Enable the RCC clock for GPIOA */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	/* Configure the Alternate functionality for GPIOA pin 9. */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	/* Configure the Alternate functionality for GPIOA pin 10. */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
	
	// Initialize the settings for GPIOA pin 9.
	GPIO_InitTypeDef gpio_init_struct;
	
	gpio_init_struct.GPIO_Pin	= GPIO_Pin_9 | GPIO_Pin_10;
	gpio_init_struct.GPIO_Mode 	= GPIO_Mode_AF;
	gpio_init_struct.GPIO_OType	= GPIO_OType_PP;
	gpio_init_struct.GPIO_Speed	= GPIO_Speed_50MHz;
	gpio_init_struct.GPIO_PuPd 	= GPIO_PuPd_UP;
	
	GPIO_Init(GPIOA, &gpio_init_struct);
}