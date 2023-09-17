#include "w25qxx.h"

#define CHIP_SELECT_GPIO_PIN						GPIO_Pin_4
#define CHIP_SELECT_LOW								SPI_CHIP_SELECT_LOW(GPIOA, CHIP_SELECT_GPIO_PIN)
#define CHIP_SELECT_HIGH							SPI_CHIP_SELECT_HIGH(GPIOA, CHIP_SELECT_GPIO_PIN)

#define W25QXX_JEDEC_DIRECTIVE_READ_BYTE			0x00

#define W25QXX_DIRECTIVE_WAIT_FLAG					0x01

#define W25QXX_DIRECTIVE_WRITE_STATUS_REGISTER_1	0x01	/* Byte0|byte1|byte2, 0x01|S7 ~ S0|S8 ~ S5, S means Status. */
#define W25QXX_DIRECTIVE_PAGE_PROGRAMMING			0x02	/* Byte0|byte1|byte2|byte3|byte4|...|byteN, 0x02|A23 ~ A16|A15 ~ A8|A7 ~ A0|D7 ~ D0. A is abbreviation of Addrerss, D is data. */
#define W25QXX_DIRECTIVE_READ						0x03	/* Byte0|byte1|byte2|byte3, 0x03|A23 ~ A16|A15 ~ A8|A7 ~ A0. */
#define W25QXX_DIRECTIVE_WRITE_DISABLE				0x04	/* Byte0, 0x04. */
#define W25QXX_DIRECTIVE_READ_STATUS_REGISTER_1		0x05	/* Byte0|byte1, 0x05|S7 ~ S0. */
#define W25QXX_DIRECTIVE_WRITE_ENABLE				0x06	/* Byte0, 0x06. */
#define W25QXX_DIRECTIVE_WRITE_STATUS_REGISTER_3	0x11	/* Byte0|byte1|byte2, 0x01|S7 ~ S0|S8 ~ S5, S means Status. */
#define W25QXX_DIRECTIVE_READ_STATUS_REGISTER_3		0x15	/* Byte0|byte1, 0x05|S7 ~ S0. */
#define W25QXX_DIRECTIVE_SECTOR_ERASE_4KB			0x20	/* Byte0|byte1|byte2|byte3, 0x20|A23 ~ A16|A15 ~ A8|A7 ~ A0. */
#define W25QXX_DIRECTIVE_WRITE_STATUS_REGISTER_2	0x31	/* Byte0|byte1|byte2, 0x01|S7 ~ S0|S8 ~ S5, S means Status. */
#define W25QXX_DIRECTIVE_PAGE_PROGRAMMING_4BITS		0x32	/* Byte0|byte1|byte2|byte3|byte4|...|byteN, 0x32|A23 ~ A16|A15 ~ A7|A7 ~ A0|D7 ~ D0. */
#define W25QXX_DIRECTIVE_READ_STATUS_REGISTER_2		0x35	/* Byte0|byte1, 0x35|S15 ~ S8. */
#define W25QXX_DIRECTIVE_BLOCK_ERASE_32KB			0x52	/* Byte0|byte1|byte2|byte3, 0x52|A23 ~ A16|A15 ~ A8|A7 ~ A0. */
#define W25QXX_DIRECTIVE_CHIP_WIPE					0x60	/* Byte0, 0xC7/0x60. */
#define W25QXX_DIRECTIVE_PAUSE_ERASE				0x75	/* Byte0, 0x75. */
#define W25QXX_DIRECTIVE_RESUME_ERASE				0x7A	/* Byte0, 0x7A. */
#define W25QXX_DIRECTIVE_HIGH_PERFORMENCE_MODE		0xA3	/* Byte0, 0xA3. */
#define W25QXX_DIRECTIVE_POWER_DOWN_MODE			0xB9	/* Byte0, 0xB9. */
#define W25QXX_DIRECTIVE_BLOCK_ERASE_64KB			0xD8	/* Byte0|byte1|byte2|byte3, 0xD8|A23 ~ A6|A15 ~ A8|A7 ~ A0. */

#define W25QXX_JEDEC_DIRECTIVE_READ_DEVICE_ID		0x90
#define W25QXX_DIRECTIVE_DUMMY_BYTE					0xFF

#define W25QXX_FLASH_PAGE_SIZE						256		/*  256 bytes per page. */
#define W25QXXX_FLASH_PAGES_NUMBER(capacity)		capacity / 8 * 1024 * 1024 / 256	/* W25Q128 has 16M Bytes. */

void w25qxx_init(void)
{
	spi_init();
}
/*

Read identifier.

*/
uint16_t w25qxx_flash_read_identifier(void)
{
	uint16_t identifier = 0x0000;
	
	/* Commence the transmission. */
	CHIP_SELECT_LOW;
	
	/* Send the READ ID directive. */
	spi_write_read(W25QXX_JEDEC_DIRECTIVE_READ_DEVICE_ID);
	
	/* Read a byte. */
	spi_write_read(W25QXX_JEDEC_DIRECTIVE_READ_BYTE);
	/* Read a byte. */
	spi_write_read(W25QXX_JEDEC_DIRECTIVE_READ_BYTE);
	/* Read a byte. */
	spi_write_read(W25QXX_JEDEC_DIRECTIVE_READ_BYTE);
	
	identifier |= spi_write_read(W25QXX_DIRECTIVE_DUMMY_BYTE) << 8;
	identifier |= spi_write_read(W25QXX_DIRECTIVE_DUMMY_BYTE);
	
	/* Stop the transmission. */
	CHIP_SELECT_HIGH;
	
	return identifier;
}

/* Write Enablement or Disablement. 

	Byte0, 0x04.
	Byte0, 0x06.
*/
void w25qxx_flash_write_enable(uint8_t enablement)
{
	/* Commence the transmission. */
	CHIP_SELECT_LOW;
	
	/* Send enable/disable directive. */
	if (enablement)
		spi_write_read(W25QXX_DIRECTIVE_WRITE_ENABLE);
	else
		spi_write_read(W25QXX_DIRECTIVE_WRITE_DISABLE);
	
	/* Stop the transmission. */
	CHIP_SELECT_HIGH;
}

/* Read Status Register.

 Byte0|byte1, 0x05|S7 ~ S0

*/
uint8_t w25qxx_flash_read_status_register1(void)
{	
	/* Commence the transmission. */
	CHIP_SELECT_LOW;
	
	uint8_t status = spi_write_read(W25QXX_DIRECTIVE_READ_STATUS_REGISTER_1);
	
	/* Stop the transmission. */
	CHIP_SELECT_HIGH;
	
	return status;
}

/* Wait the compeltion of write. */
void w25qxx_flash_wait_until_write_end(void)
{
	uint8_t status = 0;
	
	/* Lower the Chip Select register to commence the transmission. */
	CHIP_SELECT_LOW;
	
	uint16_t result = spi_write_read(W25QXX_DIRECTIVE_WRITE_STATUS_REGISTER_1);
	
	/* Wait until the W25QXX Flash not busy. */
	do
		/* Query the Read Status Regisetr 1. */
		status = spi_write_read(W25QXX_DIRECTIVE_DUMMY_BYTE);
	
	while ((status & W25QXX_DIRECTIVE_WAIT_FLAG) == SET)
		;
	
	/* High the Chip Select register to stop the transmission. */
	CHIP_SELECT_HIGH;
}

/*

Erase the sector for the specific address.

Byte0|byte1|byte2|byte3, 0x20|A23 ~ A16|A15 ~ A8|A7 ~ A0.

@address, section address, i.e. 0x001A2B3Ch, byte0|byte1|byte2|byte3, 0x20h|A23 ~ A16|A15 ~ A8|A7 ~ A0.
*/
void w25qxx_flash_sector_erase(uint32_t address)
{
	/* Send the Write Enablement directive. */
	w25qxx_flash_write_enable(1);
	/* Wait the end of write. */
	w25qxx_flash_wait_until_write_end();
	
	/* Lower the Chip Select register to commence the transmission. */
	CHIP_SELECT_LOW;
	
	/* Byte #0, 0x20h: Send the directive of Erase Sector. */
	spi_write_read(W25QXX_DIRECTIVE_SECTOR_ERASE_4KB);
	/*
	Byte #1, A23 ~ A16： Send the MSB of the sector address. 
	address & 0x00FF0000 = 0x001A2B3C & 0x00FF0000 = 0x001A0000
	(address & 0x00FF0000) >> 16 = 0x001A0000 >> 16 = 0x0000001A
	*/
	spi_write_read((address & 0x00FF0000) >> 16);
	/*
	Byte #2, A15 ~ A8: Send the 2nd byte of the sector address.
	address & 0x0000FF00 = 0x001A2B3C & 0x0000FF00 = 0x00002B00
	(address & 0x0000FF00) >> 8 = 0x00002B00 >> 8 = 0x0000002B
	*/
	spi_write_read((address & 0x0000FF00) >> 8);
	/*
	Byte #3, A7 ~ A0: Send the LSB byte of the secto address.
	address & 0x000000FF = 0x001A2B3C & 0x000000FF = 0x0000003C
	*/
	spi_write_read(address & 0x000000FF);
	
	/* High the Chip Select register to stop the transmission. */
	CHIP_SELECT_HIGH;
	
	/* Wait until erase done. */
	w25qxx_flash_wait_until_write_end();
}

/*

Page Write

Byte0|byte1|byte2|byte3|byte4|...|byteN, 0x02|A23 ~ A16|A15 ~ A8|A7 ~ A0|D7 ~ D0. A is abbreviation of Addrerss, D is data.

@address, where to start writing the bytes, @sectorAddress, i.e. 0x001A2B3Ch, byte0|byte1|byte2|byte3|(D7 ~ D0), 0x02h|A23 ~ A16|A15 ~ A8|A7 ~ A0|D7 ~ D0
@bytes, the bytes to be written.
@number, number of bytes to be written.

*/
uint32_t w25qxx_flash_page_write(uint32_t address, uint8_t *bytes, uint32_t length)
{
	/* Send the Write Enablement directive. */
	w25qxx_flash_write_enable(1);
	
	/* Lower the Chip Select register to commence the transmission. */
	CHIP_SELECT_LOW;
	
	/* Byte #0: 0x02h: Send the Page Write directive. */
	uint32_t result = spi_write_read(W25QXX_DIRECTIVE_PAGE_PROGRAMMING);
	/* Byte #1, A23 ~ A16： Send the MSB of the write address.
	address & 0x00FF0000 = 0x001A2B3C & 0x00FF0000 = 0x001A0000
	(address & 0x00FF0000) >> 16 = 0x001A0000 >> 16 = 0x0000001A
	*/
	result += spi_write_read((address & 0x00FF0000) >> 16);
	/*
	Byte #2, A15 ~ A8: Send the 2nd byte of the write address.
	address & 0x0000FF00 = 0x001A2B3C & 0x0000FF00 = 0x00002B00
	(address & 0x0000FF00) >> 8 = 0x00002B00 >> 8 = 0x0000002B
	*/
	result += spi_write_read((address & 0x0000FF00) >> 8);
	/*
	Byte #3, A7 ~ A0: Send the LSB of the write address.
	address & 0x000000FF = 0x001A2B3C & 0x000000FF = 0x0000003C
	*/
	result += spi_write_read(address & 0x000000FF);
	
	/* 256 bytes per page. */
	if (length > W25QXX_FLASH_PAGE_SIZE)
		length =  W25QXX_FLASH_PAGE_SIZE;
	
	while (length --)
		result += spi_write_read(*bytes ++);
	
	/* High the Chip Select register to stop the transmission. */
	CHIP_SELECT_HIGH;
	
	/* Write disable. */
	w25qxx_flash_write_enable(0);
	
	w25qxx_flash_wait_until_write_end();
	
	return result;
}

/*

Multiple Pages Write.

@address, where to start writing the bytes, @sectorAddress, i.e. 0x001A2B3Ch, byte0|byte1|byte2|byte3|(D7 ~ D0), 0x02h|A23 ~ A16|A15 ~ A8|A7 ~ A0|D7 ~ D0
@bytes, the bytes to be written.
@number, Number of pages to be written.

*/
void w25qxx_flash_pages_write(uint32_t address, uint8_t *bytes, uint32_t number, uint16_t volume)
{	
	/* Compute the number of data to be written for this time. */
	uint32_t currentNumber = number > (W25QXXX_FLASH_PAGES_NUMBER(volume) - address % W25QXXX_FLASH_PAGES_NUMBER(volume)) ? W25QXXX_FLASH_PAGES_NUMBER(volume) - address % W25QXXX_FLASH_PAGES_NUMBER(volume) : number;
	
	/* Compute the current address. */
	uint32_t currentAddress = address;
	
	while (number)
	{
		/* Single Page Wite. */
		w25qxx_flash_page_write(currentAddress, bytes, currentNumber);
		
		/* Compute the residual number of bytes to be written. */
		currentAddress += currentNumber;
		bytes += currentNumber;
		number -= currentNumber;
		
		currentNumber = number > W25QXXX_FLASH_PAGES_NUMBER(volume) ? W25QXXX_FLASH_PAGES_NUMBER(volume) : number;
	}
}

/*

Read bytes.

Byte0|byte1|byte2|byte3, 0x03|A23 ~ A16|A15 ~ A8|A7 ~ A0.

*/
void w25qxx_flash_read(uint32_t address, uint8_t *bytes, uint32_t number)
{
	/* Lower the Chip Select register to commence the transmission. */
	CHIP_SELECT_LOW;
	
	/* Byte #0: Send Read Data directive. */
	spi_write_read(W25QXX_DIRECTIVE_READ);
	/* Byte #1, A23 ~ A16： Send the MSB of the read address.
	address & 0x00FF0000 = 0x001A2B3C & 0x00FF0000 = 0x001A0000
	(address & 0x00FF0000) >> 16 = 0x001A0000 >> 16 = 0x0000001A
	*/
	spi_write_read((address & 0x00FF0000) << 16);
	
	/* Byte #2, A15 ~ A8: Send the 2nd byte of the read address.
	address & 0X0000FF00 = 0x001A2B3C & 0x0000FF00 = 0x00002B00
	(address & 0X0000FF00) << 8 = 0x0000002B.
	*/
	spi_write_read((address & 0x0000FF00) >> 8);
	
	/* Byte #3, A7 ~ A0: Send the LSB byte of the read address.
	address & 0x000000FF = 0x001A2B3C & 0x000000FF = 0x0000003C.
	*/
	spi_write_read(address & 0x000000FF);
	
	/* Read bytes. */
	while (number --)
	{
		/* Read a byte. */
		*bytes = spi_write_read(W25QXX_DIRECTIVE_DUMMY_BYTE);
		/* Move to next byte buffer. */
		bytes ++;
	}
	
	/* High the Chip Select register to stop the transmission. */
	CHIP_SELECT_HIGH;
}