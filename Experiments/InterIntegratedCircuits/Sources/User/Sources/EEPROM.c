#include <stdint.h>

#include "InterIntegratedCircuits.h"
#include "EEPROM.h"

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

void eeprom_write_bytes(uint16_t register_address /* AT24C02 */, uint8_t *bytes, uint16_t length)
{
	i2c_generate_start_conditions();				/* 1. Start conditions. */
	
	i2c_send((SLAVE_ADDRESS << 1) | EEPROM_WRITE, 1);						/* 2. Send Slave Address << WRITE/READ flag. */
	i2c_send(register_address, 1);					/* 3. Register address of data. */
	
	for (uint16_t i = 0; i < length; i++)
		i2c_send(bytes[i], 0);						/* 4. Data. */
	
	i2c_generate_stop_conditions();
}

uint8_t eeprom_read_bytes(uint16_t register_address, uint8_t *bytes, uint16_t length)
{
	i2c_generate_start_conditions();
	
	i2c_send((SLAVE_ADDRESS << 1) | EEPROM_WRITE, 1);
	i2c_send(register_address, 1);
	
	i2c_generate_start_conditions();				/*Start again */
	
	i2c_send((SLAVE_ADDRESS << 1) | EEPROM_READ, 1);
	
	/* Read bytes. */
	for (uint16_t i = 0; i < length; i ++)
		*(bytes + i) = i2c_read(1);					/* Read bytes and acknowledgement from Slave. */
	
	i2c_generate_stop_conditions();
	
	return 0;
}
