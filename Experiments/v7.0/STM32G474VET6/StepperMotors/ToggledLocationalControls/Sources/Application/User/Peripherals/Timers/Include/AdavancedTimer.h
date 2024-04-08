#ifndef __ADAVANCED_TIMER_H
#define __ADAVANCED_TIMER_H

#include <stm32g4xx.h>

typedef enum
{
    PulseWidthModulationOutputCompareChannelPseudo,
    PulseWidthModulationOutputCompareChannel0,
    PulseWidthModulationOutputCompareChannel1,
    
} PulseWidthModulationOutputCompareChannels;

void InitAdvancedTimer(uint32_t prescaler, uint16_t period, void (* advancedTimerOutputDelayElapsedHandler)(TIM_HandleTypeDef *htim));

void StartPulseWidthModulation(PulseWidthModulationOutputCompareChannels channel);
void StopPulseWidthModulation(PulseWidthModulationOutputCompareChannels channel);

void AssignNewCompare(uint16_t increments);

#endif  /*  #ifndef __ADAVANCED_TIMER_H */