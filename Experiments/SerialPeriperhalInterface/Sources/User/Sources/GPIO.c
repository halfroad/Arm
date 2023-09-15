#include <string.h>
#include "stm32f4xx_rcc.h"

#include "GPIO.h"

void init_gpio_ports(void)
{
	/*
	
	PA4 - SPI1_NSS (PA15 - SPI1_NSS)
	PA5 - SPI1_SCK (PB3 - SPI1_SCK)
	PA6 - SPI1_MISO (PB4 - SPI1_MISO)
	PA7 - SPI1_MOSI (PB5 - SPI1_MOSI)
	
	*/
	RCC_APB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	/* Initialization struct. */
	GPIO_InitTypeDef gpio_init_struct;
	
	gpio_init_struct.GPIO_Pin = GPIO_Pin_4;
	gpio_init_struct.GPIO_Mode = GPIO_Mode_OUT;
	gpio_init_struct.GPIO_OType = GPIO_OType_PP;
	gpio_init_struct.GPIO_PuPd = GPIO_PuPd_UP;
	gpio_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
	
	/* Initialize the GPIO Pin 4 for SPI1_NSS (CS). */
	GPIO_Init(GPIOA, &gpio_init_struct);
	
	memset(&gpio_init_struct, 0, sizeof gpio_init_struct);
	
	gpio_init_struct.GPIO_Pin =  GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	gpio_init_struct.GPIO_Mode = GPIO_Mode_AF;
	gpio_init_struct.GPIO_OType = GPIO_OType_OD;
	gpio_init_struct.GPIO_PuPd = GPIO_PuPd_UP;
	gpio_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
	
	/* Initialize the GPIO Pin 5, 6 and 7 for SPI1_SCK, SPI1_MISO, SPI1_MOSI respectively. */
	GPIO_Init(GPIOA, &gpio_init_struct);
	
	/* Associate the Alternate Function Pins to Serial Peripheral Interface 1. */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);
}