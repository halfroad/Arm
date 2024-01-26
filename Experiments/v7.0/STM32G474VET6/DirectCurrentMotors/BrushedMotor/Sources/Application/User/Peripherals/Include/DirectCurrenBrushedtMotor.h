#ifndef __DIRECT_CURREN_BRUSHEDT_MOTOR_H
#define __DIRECT_CURREN_BRUSHEDT_MOTOR_H

void InitDirectCurrenBrushedtMotor(void);

void StartMotor(void);
void StopMotor(void);

void AdaptMotorSpeedDirection(float compare);

#endif /* #ifndef __DIRECT_CURREN_BRUSHEDT_MOTOR_H */