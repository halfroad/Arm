#include <string.h>
#include "stm32f4xx_exti.h"

#include "Button.h"

void init_external_interrupt_ports(void)
{
	/* Enable GPIO Clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef gpio_init_struct;
	
	gpio_init_struct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	gpio_init_struct.GPIO_Mode = GPIO_Mode_IN;
	gpio_init_struct.GPIO_Speed = GPIO_Speed_100MHz;
	gpio_init_struct.GPIO_PuPd = GPIO_PuPd_UP;
	
	GPIO_Init(GPIOB, &gpio_init_struct);
	
	// memset(&gpio_init_struct, 0, sizeof gpio_init_struct);
	
	// WAKE-UP
	gpio_init_struct.GPIO_Pin = GPIO_Pin_0;
	gpio_init_struct.GPIO_PuPd = GPIO_PuPd_DOWN;
	
	GPIO_Init(GPIOA, &gpio_init_struct);
}