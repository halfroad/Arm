#ifndef _STEPPER_MOTOR_CONTROLLER_H
#define _STEPPER_MOTOR_CONTROLLER_H

#include "../Include/StepperMotor.h"

void InitStepperMotorController(void);


void SelectMotorNumber(uint8_t number);
void RotateMotor(int16_t steps, uint32_t accelerations, uint32_t decelerations, uint32_t velocity);

#endif  /*  #ifndef _STEPPER_MOTOR_CONTROLLER_H */