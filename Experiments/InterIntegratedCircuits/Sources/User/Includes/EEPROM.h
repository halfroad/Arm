#include <stdint.h>

void eeprom_write_byte(uint8_t register_address /* AT24C02 */, uint8_t byte);
void eeprom_write_bytes(uint8_t register_address /* AT24C02 */, uint8_t *bytes, uint16_t length);

void eeprom_read_byte(uint8_t register_address, uint8_t *byte);
void eeprom_read_bytes(uint8_t register_address, uint8_t *bytes, uint16_t length);