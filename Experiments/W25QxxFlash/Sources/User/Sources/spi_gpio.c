#include <string.h>
#include "stm32f4xx_gpio.h"

#include "spi_gpio.h"

void spi_gpios_init(void)
{
	/*
	
	PA4/SPI1_NSS	(PA15/JTDI/SPI1_NSS/SPI3_NSS)
	PA5/SPI1_SCK	(PB3/JTDO/TRACESWO/TIM2_CH2/SPI1_SCK)
	PA6/SPI1_MISO	(PB4/JNTRST/TIM3_CH1/SPI1_MISO)
	PA7/SPI1_MOSI	(PB5/TIM3_CH2/SPI1_MOSI)
	
	*/
	
	/* Enable the RCC clock for GPIOA. */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef gpio_init_struct;
	
	gpio_init_struct.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	gpio_init_struct.GPIO_Mode = GPIO_Mode_AF;
	gpio_init_struct.GPIO_OType = GPIO_OType_PP;
	gpio_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &gpio_init_struct);
	
	/* GPIOA Pin 4 is connected to Chip Select pinout of W25Qxx, thus the mode of CS is Output. */
	gpio_init_struct.GPIO_Pin = GPIO_Pin_4;
	gpio_init_struct.GPIO_Mode = GPIO_Mode_OUT;
	gpio_init_struct.GPIO_OType = GPIO_OType_PP;
	gpio_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &gpio_init_struct);
	
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);
	
	/* Configure the pins for Alternate Functions.
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);
	*/
}