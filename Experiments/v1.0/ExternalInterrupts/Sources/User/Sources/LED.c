#include "stm32f4xx_exti.h"
#include "LED.h"

void init_led_gpios(void)
{
	/* Enable GPIO Clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	
	GPIO_InitTypeDef gpio_init_struct;
		
	gpio_init_struct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	gpio_init_struct.GPIO_Mode = GPIO_Mode_OUT;
	gpio_init_struct.GPIO_OType = GPIO_OType_PP;
	gpio_init_struct.GPIO_Speed = GPIO_Speed_100MHz;
	gpio_init_struct.GPIO_PuPd = GPIO_PuPd_UP;
	
	GPIO_Init(GPIOF, &gpio_init_struct);
	
	// High level to turn the LED off
	GPIO_SetBits(GPIOF, GPIO_Pin_9 | GPIO_Pin_10);
}