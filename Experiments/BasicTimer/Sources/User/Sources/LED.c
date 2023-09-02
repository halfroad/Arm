#include "stm32f4xx_rcc.h"
#include "LED.h"

void init_led_gpios(uint32_t LED_pin)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	
	GPIO_InitTypeDef gpio_init_struct;
	
	gpio_init_struct.GPIO_Pin = LED_pin;
	gpio_init_struct.GPIO_Mode = GPIO_Mode_OUT;
	gpio_init_struct.GPIO_OType = GPIO_OType_PP;
	gpio_init_struct.GPIO_PuPd = GPIO_PuPd_DOWN;
	gpio_init_struct.GPIO_Speed = GPIO_Speed_100MHz;
	
	GPIO_Init(GPIOF, &gpio_init_struct);
	
	GPIO_SetBits(GPIOF, LED_pin);
}