#ifndef __STEPPER_MOTOR_H
#define __STEPPER_MOTOR_H

#include <stm32g4xx.h>

typedef enum
{
    ConnectorNumberPseudo,
    ConnectorNumber0,
    ConnectorNumber1,
    
} ConnectorNumbers;

typedef enum
{
    RotaryDirectionClockwise,
    RotaryDirectionAntiClockwise,
    
} RotaryDirections;

typedef enum
{
    Enabled,
    Disabled,
    
} Enablements;

typedef enum
{
    StateIdle,
    StateRun,
    StateStopped,
    
} States;

typedef struct
{
    ConnectorNumbers number;
    uint8_t enabled;
    RotaryDirections rotaryDirection;
    
    float angle;
    
    __IO float pulses;
    __IO float accumulativePulses;
    __IO float absoluteRotations;
    
    States state;
    
    void (* Init)();
    
} StepperMotorTypeDef;

void InitStepperMotor(void);

void StartMotor(ConnectorNumbers number, RotaryDirections rotaryDirection);
void StopMotor(ConnectorNumbers number);

#endif  /*  #ifndef __STEPPER_MOTOR_H   */