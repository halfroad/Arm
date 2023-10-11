#include <stdint.h>

#include "InterIntegratedCircuits.h"
#include "EEPROM.h"
#include "Timer.h"
#include "USART.h"

#define EEPROM_WORD_ADDRESS_SIZE	8
#define EEPROM_WORD_PAGE_SIZE		8

#define SLAVE_ADDRESS				0b0101000

#define EEPROM_WRITE				0x00
#define EEPROM_READ					0x01

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

void eeprom_write_byte(uint8_t register_address /* AT24C02 */, uint8_t byte)
{
	i2c_generate_start_conditions();						/* 1. Start conditions. */
	
	i2c_send(0xA0, 1);										/* 2. Send Slave Address << WRITE/READ flag. */
	i2c_send(register_address, 1);							/* 3. Register address of data. */
	i2c_send(byte, 1);										/* 4. Data. */
	
	i2c_generate_stop_conditions();
}

void eeprom_write_bytes(uint8_t register_address /* AT24C02 */, uint8_t *bytes, uint16_t length)
{
	uint8_t address = register_address;
	
	for (uint16_t i = 0; i < length; i++)
	{
		eeprom_write_byte(address, bytes[i]);
		
		address++;
		
		delay_ms(5);
	}
}

void eeprom_read_byte(uint8_t register_address, uint8_t *byte)
{
	i2c_generate_start_conditions();
	
	i2c_send(0xA0, 1);
	i2c_send(register_address, 1);
	
	i2c_generate_start_conditions();						/*Start again */
	
	i2c_send(0xA1, 1);
	
	/* Read byte and acknowledge. */
	*byte = i2c_read(0);
	
	i2c_generate_stop_conditions();
}

void eeprom_read_bytes(uint8_t register_address, uint8_t *bytes, uint16_t length)
{
	uint8_t address = register_address;
	
	for (uint16_t i = 0; i < 100; i ++)
	{
		eeprom_read_byte(address, &bytes[i]);				/* Read bytes. */
		
		address ++;
	}
}
