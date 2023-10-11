#include "stm32f4xx_tim.h"

#include "gpio.h"

void init_gpio_port()
{
	/* Enable the RCC clock for GPIOA. */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	/* Initialize the GPIOA port with Alternate Functions. */
	GPIO_InitTypeDef gpio_init_type;
	
	gpio_init_type.GPIO_Pin = GPIO_Pin_1;
	gpio_init_type.GPIO_Mode = GPIO_Mode_AF;
	gpio_init_type.GPIO_Speed = GPIO_Speed_50MHz;
	gpio_init_type.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	GPIO_Init(GPIOA, &gpio_init_type);
	
	/* Select the TIM5 as the Alternate Functions. */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM5);
}