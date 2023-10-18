#include "stm32f4xx_gpio.h"

#include "gpio.h"

void init_gpio(void)
{
	GPIO_InitTypeDef gpio_init_type;
	
	/* Enable Clock for GPIOA on AHB1. */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	gpio_init_type.GPIO_Pin = GPIO_Pin_1;
	gpio_init_type.GPIO_Mode = GPIO_Mode_AN;
	
	GPIO_Init(GPIOA, &gpio_init_type);	
}
