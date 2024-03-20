#ifndef __AT24CXX_H
#define __AT24CXX_H

void InitAT24Cxx(void);

uint8_t AT24CxxReadByte(uint16_t address);

void AT24CxxReadBytes(uint16_t address, uint8_t *bytes, uint16_t length);

void AT24CxxWriteByte(uint16_t address, uint8_t byte);

void AT24CxxWriteBytes(uint16_t address, uint8_t *bytes, uint16_t length);

uint8_t AT24CxxValidate(uint8_t address, uint8_t notation);
    
#endif  /*  #ifndef __AT24CXX_H   */