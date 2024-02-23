#ifndef __DIRECT_CURREN_BRUSHEDT_MOTOR_H
#define __DIRECT_CURREN_BRUSHEDT_MOTOR_H

#include <stm32g4xx.h>

void InitDirectCurrenBrushedtMotor(void);

void StartMotor(void);
void StopMotor(void);

void AdaptMotorVelocityDirection(float compare);

#endif /* #ifndef __DIRECT_CURREN_BRUSHEDT_MOTOR_H */