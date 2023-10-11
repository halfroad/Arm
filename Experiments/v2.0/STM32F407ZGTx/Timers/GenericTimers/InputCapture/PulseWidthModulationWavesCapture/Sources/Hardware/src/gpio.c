#include <string.h>

#include "stm32f4xx_gpio.h"

#include "gpio.h"

void init_gpio(void)
{
	/* Enable the RCC clock for GPIOA on AHB1. */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef gpio_init_type;
	
	/* GPIO 0 and 1 on GPIOA as the Waves Generator. */
	gpio_init_type.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	gpio_init_type.GPIO_Mode = GPIO_Mode_AF;
	gpio_init_type.GPIO_Speed = GPIO_Speed_25MHz;
	gpio_init_type.GPIO_OType = GPIO_OType_PP;
	gpio_init_type.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	GPIO_Init(GPIOA, &gpio_init_type);
	
	/* Opt the Alternate Functions for Pin 0 and 1 of GPIOA. */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM5);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM5);
	
	memset(&gpio_init_type, 0, sizeof gpio_init_type);
	
	gpio_init_type.GPIO_Pin = GPIO_Pin_6;
	gpio_init_type.GPIO_Mode = GPIO_Mode_IN;
	gpio_init_type.GPIO_Speed = GPIO_Speed_25MHz;
	gpio_init_type.GPIO_PuPd = GPIO_PuPd_UP;
	
	GPIO_Init(GPIOA, &gpio_init_type);
}