#include "../../../Peripherals/Timers/Include/AdvancedTimer.h"
#include "../Include/StepperMotorConroller.h"

void InitStepperMotorConroller(void)
{
    InitStepperMotor();
    InitAdvancedTimer(170 - 1, 1000 - 1);
}

void StartMotor(StepperMotorNumbers stepperMotorNumber)
{
    StartPulseWidthModulation(stepperMotorNumber);
}

void StoptMotor(StepperMotorNumbers stepperMotorNumber)
{
    StopPulseWidthModulation(stepperMotorNumber);
}

void DivertMotor(StepperMotorNumbers stepperMotorNumber, StepperMotorDirections stepperMotorDirection)
{
    DivertStepperMotor(stepperMotorNumber, stepperMotorDirection);
}