#ifndef __ADVANCED_TIMER_H
#define __ADVANCED_TIMER_H

#include <stm32g4xx.h>
#include "../Include/StepperMotorNumbers.h"

void InitAdvancedTimer(uint32_t prescaler, uint16_t autoReload);

void StartPulseWidthModulation(StepperMotorNumbers stepperMotorNumber);
void StopPulseWidthModulation(StepperMotorNumbers stepperMotorNumber);

#endif  /*  #ifndef __ADVANCED_TIMER_H  */