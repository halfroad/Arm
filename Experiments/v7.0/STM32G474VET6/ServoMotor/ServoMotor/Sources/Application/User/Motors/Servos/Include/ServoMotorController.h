#ifndef __SERVO_MOTOR_CONTROLLER_H
#define __SERVO_MOTOR_CONTROLLER_H

#include "../../../Peripherals/Timers/include/AdvancedTimer.h"

void InitServoMotorController(void);

void AssignAngle(ServoMotorNumber servoNumber, float angle);

#endif  /*  #ifndef __SERVO_MOTOR_CONTROLLER_H  */