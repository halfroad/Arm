#include "spi.h"

uint16_t w25qxx_flash_read_identifier(void);

/* Write Enablement or Disablement. */
void w25qxx_flash_write_enable(uint8_t enablement);
	
/* Read Status Register. */
uint8_t w25qxx_flash_read_status_register1(void);