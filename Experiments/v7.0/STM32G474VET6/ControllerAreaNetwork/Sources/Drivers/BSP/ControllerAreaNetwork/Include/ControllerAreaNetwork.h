#ifndef __CONTROLLER_AREA_NETWORK_H
#define __CONTROLLER_AREA_NETWORK_H

#include <stm32g4xx.h>

typedef enum
{
    FDCANOperatingModeNone,
    FDCANOperatingModeInternalLoopback,
    FDCANOperatingModeExternalLoopback,
    FDCANOperatingModeNormal,
    
} FDCANOperatingModes;

void InitControllerAreaNetworkOnInternalLoopbackMode(uint32_t nominalPrescaler, uint32_t nominalSyncJumpWidth, uint32_t nominalTimeSegment1, uint32_t nominalTimeSegment2);
void InitControllerAreaNetworkOnExternalLoopbackMode(uint32_t nominalPrescaler, uint32_t nominalSyncJumpWidth, uint32_t nominalTimeSegment1, uint32_t nominalTimeSegment2);
void InitControllerAreaNetworkOnNormalMode(uint32_t nominalPrescaler, uint32_t nominalSyncJumpWidth, uint32_t nominalTimeSegment1, uint32_t nominalTimeSegment2);

HAL_StatusTypeDef FDCANTransmitFixedLengthMessage(uint8_t *message);
HAL_StatusTypeDef FDCANReceiveMessage(uint8_t *buffer, uint8_t *length);

#endif  /*  #ifndef __CONTROLLER_AREA_NETWORK_H */