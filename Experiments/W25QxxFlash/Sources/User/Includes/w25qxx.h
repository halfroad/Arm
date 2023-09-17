#include "spi.h"

#define W25QXX_FLASH_DEFALUT_VOLUME	128

void w25qxx_init(void);
	
/*

Read identifier.

*/
uint16_t w25qxx_flash_read_identifier(void);

/*

Erase the sector for the specific address.

Byte0|byte1|byte2|byte3, 0x20|A23 ~ A16|A15 ~ A8|A7 ~ A0.

@address, section address, i.e. 0x001A2B3Ch, byte0|byte1|byte2|byte3, 0x20h|A23 ~ A16|A15 ~ A8|A7 ~ A0.
*/
void w25qxx_flash_sector_erase(uint32_t address);

/*

Page Write

Byte0|byte1|byte2|byte3|byte4|...|byteN, 0x02|A23 ~ A16|A15 ~ A8|A7 ~ A0|D7 ~ D0. A is abbreviation of Addrerss, D is data.

@address, where to start writing the bytes, @sectorAddress, i.e. 0x001A2B3Ch, byte0|byte1|byte2|byte3|(D7 ~ D0), 0x02h|A23 ~ A16|A15 ~ A8|A7 ~ A0|D7 ~ D0
@bytes, the bytes to be written.
@number, number of bytes to be written.

*/
uint32_t w25qxx_flash_page_write(uint32_t address, uint8_t *bytes, uint32_t length);

/*

Multiple Pages Write.

@address, where to start writing the bytes, @sectorAddress, i.e. 0x001A2B3Ch, byte0|byte1|byte2|byte3|(D7 ~ D0), 0x02h|A23 ~ A16|A15 ~ A8|A7 ~ A0|D7 ~ D0
@bytes, the bytes to be written.
@number, Number of pages to be written.

*/
void w25qxx_flash_pages_write(uint32_t address, uint8_t *bytes, uint32_t number, uint16_t volume);

#define w25qxx_flash_pages_write_wrap(address, bytes, number,...) w25qxx_flash_pages_write( uint32_t address, uint8_t *bytes, uint32_t number, (W25QXX_FLASH_DEFALUT_VOLUME, ##__VA_ARGS__) )


/*

Read bytes.

Byte0|byte1|byte2|byte3, 0x03|A23 ~ A16|A15 ~ A8|A7 ~ A0.

*/
void w25qxx_flash_read(uint32_t address, uint8_t *bytes, uint32_t number);