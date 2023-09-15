#include "stm32f4xx_spi.h"

#define SPI_CHIP_SELECT_LOW(port, pin)		GPIO_ResetBits(port, pin)	/* Lower the Chip Select pin to commence the transmission. */
#define SPI_CHIP_SELECT_HIGH(port, pin)		GPIO_SetBits(port, pin)		/* Up the Chip Select pin to stop the transmission. */

void spi_init();

/* Read and Write byte (In parallel) via SPI1. */
uint16_t spi_read_write(uint16_t byte);