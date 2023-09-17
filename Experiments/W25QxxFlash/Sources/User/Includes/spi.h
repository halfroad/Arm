#include "stm32f4xx_spi.h"

#define SPI_CHIP_SELECT_LOW(port, pin)		GPIO_ResetBits(port, pin)	/* Lower the Chip Select pin to commence the transmission. */
#define SPI_CHIP_SELECT_HIGH(port, pin)		GPIO_SetBits(port, pin)		/* Up the Chip Select pin to stop the transmission. */

void spi_init();

/* Write and Read byte (In parallel) via SPI1. */
uint8_t spi_write_read(uint8_t byte);