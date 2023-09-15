#include "w25qxx.h"

#define W25QXX_CS_LOW								SPI_CHIP_SELECT_LOW(GPIOA, GPIO_Pin_9)
#define W25QXX_CS_HIGH								SPI_CHIP_SELECT_HIGH(GPIOA, GPIO_Pin_9)

#define W25QXX_JEDEC_DIRECTIVE_READ_BYTE			0x00

#define W25QXX_DIRECTIVE_WAIT_FLAG					0x01

#define W25QXX_DIRECTIVE_WRITE_STATUS_REGISTER		0x01
#define W25QXX_DIRECTIVE_PAGE_WRITE					0x02
#define W25QXX_DIRECTIVE_READ						0x03
#define W25QXX_DIRECTIVE_WRITE_DISABLE				0x04
#define W25QXX_DIRECTIVE_READ_STATUS_REGISTER_1		0x05
#define W25QXX_DIRECTIVE_WRITE_ENABLE				0x06
#define W25QXX_DIRECTIVE_SECTOR_ERASE_4KB			0x20
#define W25QXX_DIRECTIVE_PAGE_WRITE_4BITS			0x32
#define W25QXX_DIRECTIVE_READ_STATUS_REGISTER_2		0x35
#define W25QXX_DIRECTIVE_BLOCK_ERASE_32KB			0x52
#define W25QXX_DIRECTIVE_FULL_WIPE					0x60	/* 0xC7*/
#define W25QXX_DIRECTIVE_PAUSE_ERASE				0x75
#define W25QXX_DIRECTIVE_RESUME_ERASE				0x7A
#define W25QXX_DIRECTIVE_HIGH_PERFORMENCE_MODE		0xA3
#define W25QXX_DIRECTIVE_POWER_DOWN_MODE			0xB9
#define W25QXX_DIRECTIVE_BLOCK_ERASE_64KB			0xD8

#define W25QXX_JEDEC_DIRECTIVE_READ_DEVICE_ID		0x90
#define W25QXX_DIRECTIVE_DUMMY_BYTE					0xFF

uint16_t w25qxx_flash_read_identifier(void)
{
	uint32_t identifier = 0x00000000, identifier0 = 0x00000000, identifier1 = 0x00000000, identifier2 = 0x00000000;
	
	/* Commence the transmission. */
	W25QXX_CS_LOW;
	
	/* Send the READ ID directive. */
	spi_read_write(W25QXX_JEDEC_DIRECTIVE_READ_DEVICE_ID);
	
	/* Read a byte. */
	identifier0 = spi_read_write(W25QXX_JEDEC_DIRECTIVE_READ_BYTE);
	/* Read a byte. */
	identifier1 = spi_read_write(W25QXX_JEDEC_DIRECTIVE_READ_BYTE);
	/* Read a byte. */
	identifier2 = spi_read_write(W25QXX_JEDEC_DIRECTIVE_READ_BYTE);
	
	/* Stop the transmission. */
	W25QXX_CS_LOW;
	
	/* Combine all the bytes, and return. */
	identifier = (identifier0 << 16) | (identifier1 << 8) | identifier;
	
	return identifier;
}

/* Write Enablement or Disablement. */
void w25qxx_flash_write_enable(uint8_t enablement)
{
	/* Commence the transmission. */
	W25QXX_CS_LOW;
	
	/* Send enable/disable directive. */
	if (enablement)
		spi_read_write(W25QXX_DIRECTIVE_WRITE_ENABLE);
	else
		spi_read_write(W25QXX_DIRECTIVE_WRITE_DISABLE);
	
	/* Stop the transmission. */
	W25QXX_CS_HIGH;
}

/* Read Status Register. */
uint8_t w25qxx_flash_read_status_register1(void)
{
	uint8_t status = 0;
	
	/* Commence the transmission. */
	W25QXX_CS_LOW;
	
	spi_read_write(W25QXX_DIRECTIVE_READ_STATUS_REGISTER_1);
	
	/* Wait until the W25QXX Flash not busy. */
	do
		/* Query the Read Status Regisetr 1. */
		status = spi_read_write(W25QXX_DIRECTIVE_DUMMY_BYTE);
	
	while ((status & W25QXX_DIRECTIVE_WAIT_FLAG) == SET);
	
	/* Stop the transmission. */
	W25QXX_CS_HIGH;
	
	return status;
}