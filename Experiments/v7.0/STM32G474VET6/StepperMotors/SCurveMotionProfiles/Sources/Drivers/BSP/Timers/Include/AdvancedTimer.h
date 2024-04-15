#ifndef __ADVANCED_TIMER_H
#define __ADVANCED_TIMER_H

#include <stm32g4xx.h>

typedef enum
{
    OutputCompareChannelPseudo,
    OutputCompareChannel0,
    OutputCompareChannel1,
    
} OutputCompareChannels;

void InitAdvancedTimer(uint16_t prescaler, uint16_t period, void (* onOutputCompareDelayElapsedCallback)(TIM_HandleTypeDef *htim));
    
void StartOutputCompare(OutputCompareChannels channel);
void StopOutputCompare(OutputCompareChannels channel);

void AssignNewCompare(OutputCompareChannels channel, uint16_t increments);

#endif