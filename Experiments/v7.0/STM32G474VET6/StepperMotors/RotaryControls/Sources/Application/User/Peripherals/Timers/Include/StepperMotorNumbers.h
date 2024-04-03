#ifndef __STEPPER_MOTOR_NUMBERS_H
#define __STEPPER_MOTOR_NUMBERS_H

typedef enum
{
    StepperMotorNumberPseudo,
    StepperMotorNumber0,
    StepperMotorNumber1,
    
} StepperMotorNumbers;

typedef enum
{
    StepperMotorDirectionPseudo,
    StepperMotorDirectionClockwise,
    StepperMotorDirectionAntiClockwise,
    
} StepperMotorDirections;

typedef enum
{
    StepperMotorStatePseudo,
    StepperMotorStateActive,
    StepperMotorStateInactive,
    
} StepperMotorStates;

#endif  /*  #ifndef __STEPPER_MOTOR_NUMBERS_H   */