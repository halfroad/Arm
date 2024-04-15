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
    
    __IO uint32_t pulses;
    __IO float absoluteScales;
    
    States state;
    
    void (* Init)();
    
} StepperMotorTypeDef;

void InitStepperMotor(void);

void ActivateMotor(ConnectorNumbers number, RotaryDirections rotationDirection);
void InactivateMotor(ConnectorNumbers number);

void DivertMotor(ConnectorNumbers number, RotaryDirections rotationDirection);

#endif  /*  #ifndef __STEPPER_MOTOR_H   */