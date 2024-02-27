#ifndef __DIRECT_CURREN_BRUSHEDT_MOTOR_H
#define __DIRECT_CURREN_BRUSHEDT_MOTOR_H

#include <stm32g4xx.h>

#define MOTOR_PEAK_SPEED                300

void InitMotor();

void ActivateMotor(void);
void DeactivateMotor(void);

void RotateMotor(int32_t compare);

#endif /* #ifndef __DIRECT_CURREN_BRUSHEDT_MOTOR_H */