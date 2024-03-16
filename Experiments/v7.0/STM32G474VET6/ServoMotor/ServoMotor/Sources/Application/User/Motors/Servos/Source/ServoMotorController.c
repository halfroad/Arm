#include <stm32g4xx.h>

#include "../Include/ServoMotorController.h"
#include "../Include/AnglesConvertor.h"

void InitServoMotorController(void)
{
    InitAdvancedTimer(170 - 1, 20 * 1e3 - 1);
}

void AssignAngle(ServoMotorNumber servoNumber, float angle)
{
    uint16_t compare = ConvertAngle(angle);
    
    AssignCompare(servoNumber, compare);
}