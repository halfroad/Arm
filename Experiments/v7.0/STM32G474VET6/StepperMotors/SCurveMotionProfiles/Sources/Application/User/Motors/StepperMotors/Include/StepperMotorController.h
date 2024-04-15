#ifndef _STEPPER_MOTOR_CONTROLLER_H
#define _STEPPER_MOTOR_CONTROLLER_H

#include "../Include/StepperMotor.h"

void InitStepperMotorController(void);

void RotateMotor(void);
void SwitchRotaryDirection(void);
void StationMotor(void);

#endif  /*  #ifndef _STEPPER_MOTOR_CONTROLLER_H */