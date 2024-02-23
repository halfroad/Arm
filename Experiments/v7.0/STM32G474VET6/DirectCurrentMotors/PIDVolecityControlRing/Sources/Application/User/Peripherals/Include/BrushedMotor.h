#ifndef __DIRECT_CURREN_BRUSHEDT_MOTOR_H
#define __DIRECT_CURREN_BRUSHEDT_MOTOR_H

#include <stm32g4xx.h>

typedef struct
{
    uint8_t state;      /* Motor state. */
    float current;      /* Motor current. */
    float voltage;      /* Voltage on VBUS. */
    float power;
    float velocity;
    int32_t pulseWidthModulation;
    
} MotorTypeDef;

void InitDirectCurrenBrushedMotor(void);

void ActivateMotor(void);
void DeactivateMotor(void);

void RotateMotor(int32_t compare);

#endif /* #ifndef __DIRECT_CURREN_BRUSHEDT_MOTOR_H */