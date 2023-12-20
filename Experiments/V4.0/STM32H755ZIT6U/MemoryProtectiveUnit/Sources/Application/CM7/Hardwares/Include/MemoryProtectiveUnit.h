#ifndef MEMORY_PROTECTIVE_UNIT_H

#define MEMORY_PROTECTIVE_UNIT_H

#include <stm32h7xx.h>

#define MPU_REGION_NO_ACCESS    0x00 // No Access
#define MPU_REGION_PRIV_RW      0x01 // Priviledge read/write only.
#define MPU_REGION_PRIV_RW_URO  0x02 //User read/write disallowed£¨Priviledge read/write.£©
#define MPU_REGION_FULL_ACCESS  0x03 // Full Access
#define MPU_REGION_PRIV_RO      0x05 // Priviledge read only.
#define MPU_REGION_PRIV_RO_URO  0x06 // Read only


uint8_t SetMemoryProtectiveRegion(uint32_t baseAddress, uint32_t size, uint8_t regionNunber, uint8_t instructiveEnabled, uint8_t accessPrivilege, uint8_t shareEnabled, uint8_t cacheEnabled, uint8_t bufferEnabled);
void SetProtectiveRegions(void);

#endif