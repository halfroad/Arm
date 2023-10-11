#include "spi_gpio.h"
#include "spi.h"

void spi_init()
{
	/* https://blog.csdn.net/we_long/article/details/131782390 */
	spi_gpios_init();
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
	SPI_InitTypeDef spi_init_struct;
	
	spi_init_struct.SPI_Direction 			= SPI_Direction_2Lines_FullDuplex;		/* Both MOSI, MISO lines, full duplex. */
	spi_init_struct.SPI_Mode 				= SPI_Mode_Master;						/* Work Mode: Master. */
	spi_init_struct.SPI_DataSize			= SPI_DataSize_8b;						/* Data size to transmiit/receive. */
	spi_init_struct.SPI_FirstBit			= SPI_FirstBit_MSB;						/* Big endians or little endians. */
	spi_init_struct.SPI_CPOL				= SPI_CPOL_High;						/* Clock Polarity: The default level of idle Colock. */
	spi_init_struct.SPI_CPHA				= SPI_CPHA_2Edge;						/* Clock Phase: Sampled by the 2nd Mode Hopping (Rising or falling edge.). */
	spi_init_struct.SPI_NSS					= SPI_NSS_Soft;							/* Specifies whether the NSS signal is managed by
                                         hardware (NSS pin) or by software using the SSI bit. 
				#define SPI_NSS_Soft                    ((uint16_t)0x0200)
				#define SPI_NSS_Hard                    ((uint16_t)0x0000) */
	spi_init_struct.SPI_BaudRatePrescaler	= SPI_BaudRatePrescaler_4;			/*Baud Rate Prescaler. */
	spi_init_struct.SPI_CRCPolynomial		= 7;									/* This field (and CRC register) defines the CRC polynomial in a binary form. See the link http://en.wikipedia.org/wiki/Cyclic_redundancy_check for more. */
	
	/* Enable the SPI1. */
	SPI_Init(SPI1, &spi_init_struct);
	/* Enable the SPI1. */
	SPI_Cmd(SPI1, ENABLE);
}

/* Read and Write byte (In parallel) via SPI1. */
uint8_t spi_write_read(uint8_t byte)
{
	/* Wait until the send buffer is empty. */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
		;
	
	/* Transmit the byte via Serial Periph Interface 1. */
	SPI_I2S_SendData(SPI1, byte);
	
	/* Wait until the receive buffer is not empty. */
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
		;
		
	return SPI_I2S_ReceiveData(SPI1);;
}