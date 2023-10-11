#include "stm32f4xx_rcc.h"

#include "GPIO.h"

void init_gpio_ports()
{
	/* Enable the RCC Clock of GPIOA. */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	/* Initialize the settings for GPIOA pin 9 and 10. */
	GPIO_InitTypeDef gpio_init_struct;
	
	gpio_init_struct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	gpio_init_struct.GPIO_Mode = GPIO_Mode_AF;
	gpio_init_struct.GPIO_OType = GPIO_OType_PP;
	gpio_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
	gpio_init_struct.GPIO_PuPd = GPIO_PuPd_UP;
	
	GPIO_Init(GPIOA, &gpio_init_struct);
	
	/* Enable the Alternate Function for GPIOA port 9 (USART1_TXD1) and 10 (USART1_RXD1).*/
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
}