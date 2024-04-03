#ifndef __STEPPER_MOTOR_CONROLLER_H
#define __STEPPER_MOTOR_CONROLLER_H

#include "../../../Peripherals/Timers/Include/StepperMotorNumbers.h"
#include "../Include/StepperMotor.h"

void InitStepperMotorConroller(void);

void StartMotor(StepperMotorNumbers stepperMotorNumber);
void StoptMotor(StepperMotorNumbers stepperMotorNumber);
void DivertMotor(StepperMotorNumbers stepperMotorNumber, StepperMotorDirections stepperMotorDirection);

#endif  /*  #ifndef __STEPPER_MOTOR_CONROLLER_H */