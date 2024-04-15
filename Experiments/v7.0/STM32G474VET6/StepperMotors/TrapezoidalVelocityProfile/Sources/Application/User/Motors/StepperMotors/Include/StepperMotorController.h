#ifndef _STEPPER_MOTOR_CONTROLLER_H
#define _STEPPER_MOTOR_CONTROLLER_H

#include "../Include/StepperMotor.h"

void InitStepperMotorController(void);


void SelectMotorNumber(uint8_t number);
void ApplyTrapezoidalMotions(int16_t steps, int8_t acceleration, int16_t velocity, int8_t deceleration);

#endif  /*  #ifndef _STEPPER_MOTOR_CONTROLLER_H */