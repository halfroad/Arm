#ifndef _STEPPER_MOTOR_CONTROLLER_H
#define _STEPPER_MOTOR_CONTROLLER_H

#include "../Include/StepperMotor.h"

void InitStepperMotorController(void);

void AssignAngle(ConnectorNumbers number, RotaryDirections rotaryDirection, float angle);

#endif  /*  #ifndef _STEPPER_MOTOR_CONTROLLER_H */