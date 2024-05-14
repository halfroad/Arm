#ifndef __W25QXX_H
#define __W25QXX_H

#include <stm32h5xx.h>

typedef enum
{
    W25QxxStatusRegister1WriteInProgress        = 0x00  ,
    W25QxxStatusRegister1WriteEnableLatch               ,
    W25QxxStatusRegister1BlockProtectBit0               ,
    W25QxxStatusRegister1BlockProtectBit1               ,
    W25QxxStatusRegister1BlockProtectBit2               ,
    W25QxxStatusRegister1TopOrBottomProtect             ,
    W25QxxStatusRegister1SectorProtect                  ,
    
} W25QxxStatusRegister1BitFields;

typedef enum
{
    StatusRegiste1FlagReset                             = 0x00,
    StatusRegister1FlagSet                              = 0x01,
    
} StatusRegister1Flags;

void InitW25Qxx(void);

uint16_t W25QxxReadManufactureIdentifier(void);

HAL_StatusTypeDef W25QxxEraseChip(void);
HAL_StatusTypeDef EraseSector(uint32_t sectorNumber);

HAL_StatusTypeDef W25QxxRead(uint8_t *bytes, uint32_t address, uint16_t length);
void W25QxxWrite(uint8_t *bytes, uint32_t address, uint16_t length);

#endif  /*  #ifndef __W25QXX_H  */