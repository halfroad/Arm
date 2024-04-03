#include "../../../Peripherals/Timers/Include/AdavancedTimer.h"

#include "../Include/StepperMotorController.h"

#define PULSES_PER_ROUND                        (1600.0f)
#define MINIMAL_ANGLE_PER_STEP                  (1.8f / 8.0f)

extern StepperMotorTypeDef stepperMotor;

static void onAdvancedTimerPeriodElapsedHandler(TIM_HandleTypeDef *htim);

void InitStepperMotorController(void)
{
    InitAdvancedTimer(170 - 1, 1000 - 1, onAdvancedTimerPeriodElapsedHandler);
    InitStepperMotor();
    
    if (stepperMotor.Init)
        stepperMotor.Init();
}

static void onAdvancedTimerPeriodElapsedHandler(TIM_HandleTypeDef *htim)
{
    static uint8_t i = 0;
    
    if (++ i % 2 == 0)
    {
        i = 0;
        
        stepperMotor.pulses --;
        stepperMotor.state = StateRun;
        
        if (stepperMotor.rotaryDirection == RotaryDirectionClockwise)
            stepperMotor.accumulativePulses ++;
        else
            stepperMotor.accumulativePulses --;
        
        if (stepperMotor.pulses <= 0)
        {
            stepperMotor.state = StateStopped;
            
            StopMotor(stepperMotor.number);
        }
    }
    
    AssignNewCompare(htim);
}

void AssignAngle(ConnectorNumbers number, RotaryDirections rotaryDirection, float angle)
{
    stepperMotor.pulses = angle / MINIMAL_ANGLE_PER_STEP;
    
    if (stepperMotor.pulses == 0)
        StopMotor(number);
    else
        StartMotor(number, rotaryDirection);
}