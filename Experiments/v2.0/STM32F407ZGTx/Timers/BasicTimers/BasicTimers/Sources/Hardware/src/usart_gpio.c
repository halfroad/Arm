#include "stm32f4xx_gpio.h"
#include "usart_gpio.h"

void init_usart_gpio_ports(void)
{
	GPIO_InitTypeDef gpio_init_struct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
	
	gpio_init_struct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	gpio_init_struct.GPIO_Mode = GPIO_Mode_AF;
	gpio_init_struct.GPIO_OType = GPIO_OType_PP;
	gpio_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
	gpio_init_struct.GPIO_PuPd = GPIO_PuPd_UP;
	
	GPIO_Init(GPIOA, &gpio_init_struct);
}
