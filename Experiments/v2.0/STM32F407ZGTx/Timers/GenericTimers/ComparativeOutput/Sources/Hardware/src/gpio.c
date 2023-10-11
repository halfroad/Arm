#include "stm32f4xx_gpio.h"

#include "gpio.h"

void init_gpio_port(void)
{
	/* Enable RCC clock on GPIOA. */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef gpio_init_type;
	
	gpio_init_type.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	gpio_init_type.GPIO_Mode = GPIO_Mode_AF;
	gpio_init_type.GPIO_Speed = GPIO_Speed_50MHz;
	gpio_init_type.GPIO_OType = GPIO_OType_PP;
	gpio_init_type.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	GPIO_Init(GPIOA, &gpio_init_type);
	
	/* Configure the Alternate Functions. */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_TIM3);
}