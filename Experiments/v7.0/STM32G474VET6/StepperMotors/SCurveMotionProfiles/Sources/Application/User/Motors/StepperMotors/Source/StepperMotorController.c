#include "./Timers/Include/AdvancedTimer.h"
#include "../Include/StepperMotor.h"

#include "../Include/StepperMotorController.h"

static void OnOutputCompareDelayElapsedHandler(TIM_HandleTypeDef *htim);

extern StepperMotorTypeDef stepperMotorType;

void InitStepperMotorController(void)
{
    InitAdvancedTimer(170 - 1, 1e3, OnOutputCompareDelayElapsedHandler);
    InitStepperMotor();
}

void RotateMotor(void)
{
    ActivateMotor(stepperMotorType.number, stepperMotorType.rotaryDirection);
    
    switch (stepperMotorType.number)
    {
        case ConnectorNumber0:
            
            StartOutputCompare(OutputCompareChannel0);
        
            break;
        
        case ConnectorNumber1:
            
            StartOutputCompare(OutputCompareChannel1);
        
            break;
        
        default:
            break;
    }
}

void SwitchRotaryDirection(void)
{
    switch (stepperMotorType.rotaryDirection)
    {
        case RotaryDirectionClockwise:
            
            stepperMotorType.rotaryDirection = RotaryDirectionAntiClockwise;
        
            break;
        
        case RotaryDirectionAntiClockwise:
            
            stepperMotorType.rotaryDirection = RotaryDirectionClockwise;
        
            break;
        
        default:
        
            break;
    }
}

void StationMotor(void)
{
    InactivateMotor(stepperMotorType.number);
    
    switch (stepperMotorType.number)
    {
        case ConnectorNumber0:
            
            StopOutputCompare(OutputCompareChannel0);
        
            break;
        
        case ConnectorNumber1:
            
            StopOutputCompare(OutputCompareChannel1);
        
            break;
        
        default:
            break;
    }
}

static void OnOutputCompareDelayElapsedHandler(TIM_HandleTypeDef *htim)
{
    uint8_t i = 0;
}