#include "../../../Peripherals/Timers/Include/AdavancedTimer.h"
#include "../Include/TrapezoidalMotionAlgorithm.h"

#include "../Include/StepperMotorController.h"

#define PULSES_PER_ROUND                        (1600.0f)
#define MINIMAL_ANGLE_PER_STEP                  (1.8f / 8.0f)

extern StepperMotorTypeDef stepperMotor;
extern TrapezoidalMotionAlgorithmTypeDef trapezoidalMotionAlgorithmType;

static void onAdvancedTimerPeriodElapsedHandler(TIM_HandleTypeDef *htim);
static void onMotionStateChangeOccursHandler(MotionStates newMotionState, uint16_t newPeriod);

void InitStepperMotorController(void)
{
    InitAdvancedTimer(170 - 1, 1000 - 1, onAdvancedTimerPeriodElapsedHandler);
    InitStepperMotor();
    InitTrapezoidalMotionAlgorithm();
    
    if (stepperMotor.Init)
        stepperMotor.Init();
    if (trapezoidalMotionAlgorithmType.Init)
        trapezoidalMotionAlgorithmType.Init();
}

static void onAdvancedTimerPeriodElapsedHandler(TIM_HandleTypeDef *htim)
{
    AssignNewCompare(trapezoidalMotionAlgorithmType.nextPeriod / 2);
    
    static uint8_t i = 0;
    
    if (++ i == 2)
    {
        i = 0;
        
        StartMotor(stepperMotor.number, stepperMotor.rotaryDirection);
        AssignNextPeriod(onMotionStateChangeOccursHandler);
    }
}

static void onMotionStateChangeOccursHandler(MotionStates newMotionState, uint16_t newPeriod)
{
    switch (newMotionState)
    {
        case MotionStateIdle:
        case MotionStateArrived:
        {
            StopMotor(stepperMotor.number);
        
            stepperMotor.state          = StateStopped;
        }
        
        case MotionStateAcceleration:
        case MotionStateUniformVelocity:
        case MotionStateDeceleration:
        {
            stepperMotor.accumulativePulses ++;
            
            if (stepperMotor.rotaryDirection == RotaryDirectionClockwise)
                stepperMotor.absoluteRotations ++;
            else
                stepperMotor.absoluteRotations --;
            
            stepperMotor.state          = StateRun;
        }
        
        
        default:
            break;
    }
}

void SelectMotorNumber(uint8_t number)
{
    switch (number)
    {
        case 0:
            stepperMotor.number = ConnectorNumber0;
        break;
        
        case 1:
            stepperMotor.number = ConnectorNumber1;
        break;
        
        default:
            break;
    }
}

void RotateMotor(int16_t steps, uint32_t accelerations, uint32_t decelerations, uint32_t velocity)
{
    stepperMotor.rotaryDirection = steps > 0 ? RotaryDirectionClockwise: RotaryDirectionAntiClockwise;

    CreateTrapezoidalVelocityControlParameters(steps, accelerations, decelerations, velocity);
    
    StartMotor(stepperMotor.number, stepperMotor.rotaryDirection);
}