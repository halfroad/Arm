#include "SerialPeriperhalInterface.h"

void init_serial_peripheral_interface(void)
{
	/* Enable the RCC clock for SPI1. */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
	/* SPI initlization struct. */
	SPI_InitTypeDef spi_init_struct;
	
	spi_init_struct.SPI_Mode 					= SPI_Mode_Master;							/* Work mode: Master mode.*/
	spi_init_struct.SPI_Direction 				= SPI_Direction_2Lines_FullDuplex;			/* Set the direction, 2 lines, 2 directions, Full duplex.
	Also there is one direction.
	*/
	spi_init_struct.SPI_DataSize 				= SPI_DataSize_8b;							/* Data frame with 8 bits, not 16 bits. */
	spi_init_struct.SPI_CPOL					= SPI_CPOL_High;							/* Clock Polatiry, the default level when clock is idle. (1, 1) Mode 3? */
	spi_init_struct.SPI_CPHA					= SPI_CPHA_2Edge;							/* Clock Phase. The bits will be sampled when 2nd rising or falling edge. */
	spi_init_struct.SPI_NSS						= SPI_NSS_Soft;								/* Specifies whether the NSS signal is managed by
                                         hardware (NSS pin) or by software using the SSI bit. This parameter can be a value of @ref SPI_Slave_Select_management. */
	spi_init_struct.SPI_BaudRatePrescaler		= SPI_BaudRatePrescaler_256;				/* Prescaler of Baud Rate, 256. */
	spi_init_struct.SPI_FirstBit				= SPI_FirstBit_MSB;							/* Big or little endian. First bit is Most or Least Significant Bit. */
	spi_init_struct.SPI_CRCPolynomial			= 7;										/* This field (and CRC register) defines the CRC polynomial in a binary form. See the link http://en.wikipedia.org/wiki/Cyclic_redundancy_check for more. */
	
	/* Initialize the SPI1. */
	SPI_Init(SPI1, &spi_init_struct);
	/* Enable SPI. */
	SPI_Cmd(SPI1, ENABLE);
}

uint16_t spi_read_write(uint16_t byte)
{
	// Wait until the Tranmission buffer is empty.
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
		;
	
	SPI_I2S_SendData(SPI1, byte);
	
	// Wait until the Receive buffer is not empty.
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
		;
	
	/* Return the latest received byte. */
	return SPI_I2S_ReceiveData(SPI1);
}

uint8_t spi_transmit(uint8_t byte)
{
	uint8_t buffer = 0;
	
	for (uint8_t i = 0; i < 8; i ++)
	{
		SPI1_SCK_LOW;			/* Lower the Chip Select register to commence communication. */
		
		if (byte & (0x80 >> i))
			SPI1_MOSI_HIGH;		/* Master sends, Slave receives the high level. */
		else
			SPI1_MOSI_LOW;		/* Master sends, Slave receives the low level. */
		
		SPI1_SCK_HIGH;			/* Generate rising edge, both Master and Slave receives data. */
		
		buffer <<= 1;			/* Empty the least bit. */
		
		if (SPI1_MISO)			/* Master reads data from W25Qxx. */
			buffer |= 1;
	}
	
	return buffer;
}