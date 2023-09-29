#include "stm32f4xx_gpio.h"

#include "delay.h"
#include "usart.h"
#include "gpio.h"

void init_led_gpios(void)
{
	/* Enable Clock for GPIOA on AHB1. */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	
	GPIO_InitTypeDef gpio_init_struct;
	
	gpio_init_struct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	gpio_init_struct.GPIO_Mode = GPIO_Mode_OUT;
	gpio_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
	gpio_init_struct.GPIO_OType = GPIO_OType_PP;
	gpio_init_struct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	GPIO_Init(GPIOF, &gpio_init_struct);
	
}

void toggle_leds(void)
{
	printf("Toggle LEDs.\n");
	
	GPIO_ToggleBits(GPIOF, GPIO_Pin_9 | GPIO_Pin_10);
	delay_ms(1000);
	printf("LEDs toggled.\n");
}