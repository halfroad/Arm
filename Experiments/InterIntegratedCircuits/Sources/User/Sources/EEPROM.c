#include <stdint.h>

#include "InterIntegratedCircuits.h"
#include "EEPROM.h"

#define EEPROM_WRITE				0x00
#define EEPROM_READ					0x01

#define EEPROM_WORD_ADDRESS_SIZE	8
#define EEPROM_WORD_PAGE_SIZE		8

/*

	1. Length of data address varies on different chips;

		8 bits:
			AT24C01, AT24C02.
		16 bits:
			AT24C04, AT24C08, AT24C16.
		32 bits;
			AT24C32, AT24C64.
		64 bits;
			AT24C128, AT24C256.
		128 bits;
			AT24C512.
		
	2. Length of page address varies on different chips:
	
		8 bits:
			AT24C01, AT24C02.
		16 bits:
			AT24C04, AT24C08, AT24C16.
		32 bits;
			AT24C32, AT24C64.
		64 bits;
			AT24C128, AT24C256.
		128 bits;
			AT24C512.
*/

uint8_t eeprom_write_bytes(uint8_t slave_address, uint16_t register_address /* AT24C02 */, uint8_t *bytes, uint16_t length)
{
	i2c_start();															/* 1. Start. */
		
	uint8_t acknowledgement = i2c_send(slave_address | EEPROM_WRITE);		/* 2. Send Slave Address << WRITE/READ flag. */
	
	if (NO_ACKNOWLEDGEMENT == acknowledgement)
	{
		i2c_stop();
		
		return 1;
	}
	
	/* Write address of Data. */
#if (EEPROM_WORD_ADDRESS_SIZE == 8)
	acknowledgement = i2c_send((uint8_t)(register_address & 0x00ff));			/* 3.1. Data address with 8 bits. */
#else
	acknowledgement = i2c_send((uint8_t)(register_address >> 8));				/* 3.2.1 Data address with 16 bits, first 8 bits.*/
	
	if (NO_ACKNOWLEDGEMENT == acknowledgement)
	{
		i2c_stop();
		
		return 2;
	}
	
	acknowledgement = i2c_send((uint8_t)(register_address & 00ff));				/* 3.2.2 Data address with 16 bits, last 8 bits*/
	
	if (NO_ACKNOWLEDGEMENT == acknowledgement)
	{
		i2c_stop();
		
		return 3;
	}
	
#endif
	
	/* Write Data. */
	for (uint16_t i = 0; i < length; i ++)
	{
		acknowledgement = i2c_send(bytes[i]);
		
		if (NO_ACKNOWLEDGEMENT == acknowledgement)
		{
			i2c_stop();
			
			break;
		}
	}
	
	i2c_stop();
	
	return 0;
}

uint8_t eeprom_read_bytes(uint8_t slave_address, uint16_t register_address, uint8_t *bytes, uint16_t length)
{
	i2c_start();															/* 1. Start. */
	
	uint8_t acknowledgement = i2c_send(slave_address | EEPROM_WRITE);		/* 2. Send Slave Address << WRITE/READ flag. */
	
	if (NO_ACKNOWLEDGEMENT == acknowledgement)
	{
		i2c_stop();
		
		return 1;
	}
	
#if (8 == EEPROM_WORD_ADDRESS_SIZE)
	
	acknowledgement = i2c_send((uint8_t)(register_address & 0x00ff));		/* 3.1. Data address with 8 bits. */
	
	if (NO_ACKNOWLEDGEMENT == acknowledgement)
	{
		i2c_stop();
		
		return 2;
	}
	
#else
	
	acknowledgement  = i2c_send((uint8_t)(register_address << 8));			/* 3.2.1 Data address with 16 bits, first 8 bits.*/
	
	if (NO_ACKNOWLEDGEMENT == acknowledgement)
	{
		i2c_stop();
		
		return 3;
	}
	
	acknowledgement = i2c_send((uint8_t)(register_address & 0x00ff));		/* 3.2.2 Data address with 16 bits, last 8 bits*/
	
	if (NO_ACKNOWLEDGEMENT == acknowledgement)
	{
		i2c_stop();
		
		return 4;
	}
	
#endif

	/* Start again. */
	i2c_start();
	
	acknowledgement = i2c_send(slave_address | EEPROM_READ);
	
	if (NO_ACKNOWLEDGEMENT == acknowledgement)
	{
		i2c_stop();
		
		return 5;
	}
	
	/* Read bytes. */
	for (uint16_t i = 0; i < length; i ++)
		*(bytes + 1) = i2c_read(ACKNOWLEDGEMENT);							/* Read bytes and acknowledgement from Slave. */
	
	*(bytes + length - 1) = i2c_read(NO_ACKNOWLEDGEMENT);					/* Read last byte and no acknowledgement from Slave. */
	
	i2c_stop();
	
	return 0;
}
