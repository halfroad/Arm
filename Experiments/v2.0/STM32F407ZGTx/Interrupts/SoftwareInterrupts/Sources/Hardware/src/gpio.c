#include "stm32f4xx_gpio.h"

#include "gpio.h"

void init_led_gpios(void)
{
	/* Enable the RCC clock for GPIOF on AHB1. */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	
	GPIO_InitTypeDef gpio_init_struct;
	
	gpio_init_struct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	gpio_init_struct.GPIO_Mode = GPIO_Mode_OUT;
	gpio_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
	gpio_init_struct.GPIO_OType = GPIO_OType_PP;
	gpio_init_struct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	GPIO_Init(GPIOF, &gpio_init_struct);
}