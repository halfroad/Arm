#ifndef __W25QXX_H
#define __W25QXX_H

#include <stm32g4xx.h>

void InitW25Qxx(void);

uint16_t ReadManufacturerIdentification(void);

void W25QxxRead(uint8_t *bytes, uint32_t address, uint16_t size);
void W25QxxWrite(uint8_t *bytes, uint32_t address, uint16_t size);

void EradicateChip(void);

#endif  /*  #ifndef __W25QXX_H  */