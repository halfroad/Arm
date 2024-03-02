#ifndef __DIRECT_CURREN_BRUSHEDT_MOTOR_H
#define __DIRECT_CURREN_BRUSHEDT_MOTOR_H

#include <stm32g4xx.h>

#define MOTOR_PEAK_SPEED                                    300

typedef enum
{
    MotorRotationDirectionNeutral                           = 0x00,
    MotorRotationDirectionClockwise                         = 0x01,
    MotorRotationDirectionAntiClockwise                     = 0x02,
    
} MotorRotationDirections;


void InitMotor();

void ActivateMotor(void);
void DeactivateMotor(void);

void RotateMotor(int32_t compare);

#endif /* #ifndef __DIRECT_CURREN_BRUSHEDT_MOTOR_H */