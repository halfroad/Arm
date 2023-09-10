#include <stdint.h>

void eeprom_write_bytes(uint16_t register_address /* AT24C02 */, uint8_t *bytes, uint16_t length);
uint8_t eeprom_read_bytes(uint16_t register_address, uint8_t *bytes, uint16_t length);