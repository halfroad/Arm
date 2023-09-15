#include "W25Qxx.h"

#define PAGE_WRITE				0x02
#define READ_DATA				0x03
#define WRITE_DISABLE			0x04
#define READ_STATUS_REGISTER1	0x05	/* byte0: 0x05|byte1: S7 ~ S0. */
#define WRITE_ENABLE			0x06
#define SECTOR_ERASE			0x20

#define JEDEC_DEVICE_ID			0x90

#define TOTAL_ADDRESSES			256

void w25qxx_flash_init()
{
	init_serial_peripheral_interface();
	
	uint32_t identifier = w25qxx_flash_read_identifier();
}

uint32_t w25qxx_flash_read_identifier(void)
{
	uint32_t byte0 = 0x00000000, byte1 = 0x00000000, byte2 = 0x00000000, byte3 = 0x00000000;
	
	/* Chip Select. */
	SPI1_CS_LOW;
	
	/* Send JEDEC directive to read the identifier. */
	spi_read_write(JEDEC_DEVICE_ID);
	
	byte0 = spi_transmit(0);
	byte1 = spi_transmit(0);
	byte2 = spi_transmit(0);
	
	uint32_t byte = spi_transmit(0xff) << 8;	/* Vendor ID. */
	byte |= spi_transmit(0xff);					/* Product ID. */
	
	/* Stop Chip Select. */
	SPI1_CS_HIGH;
	
	return byte;
}

/* Enable Write. */
void  w25qxx_flash_write_enable(uint8_t enable)
{
	/* Chip Select. */
	SPI1_CS_LOW;
	
	if (enable)
		/* Send the Enablement directive. */
		spi_transmit(WRITE_ENABLE);
	else
		/* Send the Disablement directive. */
		spi_transmit(WRITE_DISABLE);
	
	/* Stop Chip Select. */
	SPI1_CS_HIGH;
}

/* Query the Status Flag Register. */
uint8_t w25qxx_flash_read_status(void)
{
	/* Chip Select. */
	SPI1_CS_LOW;
	
	spi_transmit(READ_STATUS_REGISTER1);
	uint8_t status = spi_transmit(0xff);
	
	/* Stop Chip Select. */
	SPI1_CS_HIGH;
	
	return status;
}

/* Erase a Sector. */
void w25qxx_flash_sector_erase(uint32_t address)
{
	/* Enable Write. */
	w25qxx_flash_write_enable(1);
	
	/* Chip Select. */
	SPI1_CS_LOW;
	
	/*uint32_t address like-wise: 0x00 11 22 33, 0b 0000 0000, 0001 0001, 0010 0010, 0011 0011 */
	spi_transmit(SECTOR_ERASE);		/* Send the directive, byte0. */
	spi_transmit(address >> 16);	/* Byte 1: 1st address of sector to be erased, 0x00 00 00 11 (uint8_t, 0x11). */
	spi_transmit(address >> 8);		/* Byte 2: 2nd address of sector to be erased. 0x00 00 11 22 (uint8_t, 0x22. */
	spi_transmit(address);			/* Byte 3: 3rd address of sector to be erased. 0x00 11 22 33 (uint8_t, 0x33. */
	
	/* Stop Chip Select. */
	SPI1_CS_HIGH;
	
	/* Disable Write. */
	w25qxx_flash_write_enable(0);
	
	while (w25qxx_flash_read_status() & 0x01)
		;
}

void w25qxx_flash_page_write(uint32_t address, uint32_t number, uint8_t *byte)
{
	/* Enable Write. */
	w25qxx_flash_write_enable(1);
	
	/* Chip Select. */
	SPI1_CS_LOW;
	
	spi_transmit(PAGE_WRITE);					/* Send the Page Write Directive. */
	// w25qxx_flash_sector_erase(address);		/* Erase the sector before new bytes write. */
	spi_transmit(address >> 16);				/* Byte 1: 1st address of page to be written, 0x00 00 00 11 (uint8_t, 0x11). */
	spi_transmit(address >> 8);					/* Byte 2: 2nd address of page to be written. 0x00 00 11 22 (uint8_t, 0x22. */
	spi_transmit(address);						/* Byte 3: 3rd address of page to be written. 0x00 11 22 33 (uint8_t, 0x33. */
	
	while (number --)
		spi_transmit(*byte ++);
	
	/* Stop Chip Select. */
	SPI1_CS_HIGH;
	/* Disable Write. */
	w25qxx_flash_write_enable(1);
	
	while (w25qxx_flash_read_status() & 0x01)
		;
}

/*
Input,

	address: Internal address of device.
	number: number of data to write.

*/
void w25qxxx_flash_pages_write(uint32_t address, uint32_t number, uint8_t *byte)
{
	uint32_t residualAddresses;
	
	while (1)
	{
		residualAddresses = TOTAL_ADDRESSES - address % TOTAL_ADDRESSES;
		
		if (number <= residualAddresses)	/* Write can be compelted in current page. */
			residualAddresses = number;
		
		/* Write cannot be completed in the current page, then complete the current page. */
		w25qxx_flash_page_write(address, residualAddresses, byte);
		
		if (number == residualAddresses)
			break;
		
		number -= residualAddresses;		/* Number of bytes that are not written. */
		address += residualAddresses;		/* Next interal address to write. */
		byte += residualAddresses;			/* Next data address to write. */
	}
}

void w25qxx_flash_read_bytes(uint32_t address, uint32_t number, uint8_t *byte)
{
	/* Chip Select. */
	SPI1_CS_LOW;
	
	spi_transmit(READ_DATA);				/* Send Read directive. */
	spi_transmit(address >> 16);
	spi_transmit(address >> 8);
	spi_transmit(address);
	
	while (number --)
		*byte ++ = spi_transmit(0xff);
	
	/* Stop Chip Select. */
	SPI1_CS_HIGH;
}