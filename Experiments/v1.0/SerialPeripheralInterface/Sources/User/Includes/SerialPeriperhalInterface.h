#ifndef SERIAL_PERIPERHAL_INTERFACE_H
#define SERIAL_PERIPERHAL_INTERFACE_H

#include "stm32f4xx_rcc.h"
#include "stm32f4xx_spi.h"

#define SPI1_CS_HIGH		GPIO_SetBits(GPIOA, GPIO_Pin_4)
#define SPI1_CS_LOW			GPIO_ResetBits(GPIOA, GPIO_Pin_4)

#define SPI1_SCK_HIGH		GPIO_SetBits(GPIOA, GPIO_Pin_5)
#define SPI1_SCK_LOW		GPIO_ResetBits(GPIOA, GPIO_Pin_5)

#define SPI1_MISO			GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6)

#define SPI1_MOSI_HIGH		GPIO_SetBits(GPIOA, GPIO_Pin_7)
#define SPI1_MOSI_LOW		GPIO_ResetBits(GPIOA, GPIO_Pin_7)

void init_serial_peripheral_interface(void);

uint16_t spi_read_write(uint16_t byte);
uint8_t spi_transmit(uint8_t byte);

#endif

