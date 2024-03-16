#ifndef __ADVANCED_TIMER_H
#define __ADVANCED_TIMER_H

#include <stm32g4xx.h>

typedef enum
{
    ServoMotorNumberPseudo,
    ServoMotorNumber0,
    ServoMotorNumber1,
    
} ServoMotorNumber;

void InitAdvancedTimer(uint32_t prescaler, uint16_t period);

void AssignCompare(ServoMotorNumber servoNumber, uint16_t compare);

#endif  /*  #ifndef __ADVANCED_TIMER_H  */
