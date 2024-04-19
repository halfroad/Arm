#ifndef _STEPPER_MOTOR_CONTROLLER_H
#define _STEPPER_MOTOR_CONTROLLER_H

#include "../Include/StepperMotor.h"

void InitStepperMotorController(void);

void RotateMotor(void);
void SwitchRotaryDirection(RotaryDirections rotaryDirection);
void StationMotor(void);

void IncreaseRotationAngle(void);
void DecreaseRotationAngle(void);

void ResetRotationAngle(void);

void StartMotionProfiles(void);

#endif  /*  #ifndef _STEPPER_MOTOR_CONTROLLER_H */