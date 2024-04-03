#ifndef __ADAVANCED_TIMER_H
#define __ADAVANCED_TIMER_H

#include <stm32g4xx.h>

typedef enum
{
    PulseWidthModulationOutputCompareChannelPseudo,
    PulseWidthModulationOutputCompareChannel0,
    PulseWidthModulationOutputCompareChannel1,
    
} PulseWidthModulationOutputCompareChannels;

void InitAdvancedTimer(uint32_t prescaler, uint16_t period, void (* onAdvancedTimerPeriodElapsedHandler)(TIM_HandleTypeDef *htim));

void StartPulseWidthModulation(PulseWidthModulationOutputCompareChannels channel);
void StopPulseWidthModulation(PulseWidthModulationOutputCompareChannels channel);

void AssignNewCompare(TIM_HandleTypeDef *htim);

#endif  /*  #ifndef __ADAVANCED_TIMER_H */