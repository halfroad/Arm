#include "./Peripherals/Timers/Include/AdavancedTimer.h"
#include "../Include/TrapezoidalMotionAlgorithm.h"

#include "../Include/StepperMotorController.h"

#define PRESCALER                               170

#define STEP_SUB_DIVISION                       8
#define ANGLE_PER_STEP                          1.8f
#define STEPS_PER_ROUND                         200

#define PULSES_PER_ROUND                        (STEP_SUB_DIVISION * STEPS_PER_ROUND)
#define MINIMAL_ANGLE_PER_STEP                  (ANGLE_PER_STEP / STEP_SUB_DIVISION)

extern StepperMotorTypeDef stepperMotor;
extern TrapezoidalMotionAlgorithmTypeDef motions;

static void OnAdvancedTimerOutputDelayElapsedHandler(TIM_HandleTypeDef *htim);

void InitStepperMotorController(void)
{
    InitAdvancedTimer(PRESCALER - 1, 0xFFFF - 1, OnAdvancedTimerOutputDelayElapsedHandler);
    InitStepperMotor();
    
    if (stepperMotor.Init)
        stepperMotor.Init();
    
    InitTrapezoidalMotions(PRESCALER);
    
    if (motions.Init)
        motions.Init();
}

void OnTrapezoidMotionEstimatedHandler(int32_t rounds)
{
    stepperMotor.rotaryDirection = rounds > 0 ? RotaryDirectionClockwise: RotaryDirectionAntiClockwise;
    
    StartMotor(stepperMotor.number, stepperMotor.rotaryDirection);    
}

void ApplyTrapezoidalMotions(int16_t rounds, int8_t acceleration, int16_t velocity, int8_t deceleration)
{    
    MoveTrapezoid(rounds, acceleration, velocity, deceleration, OnTrapezoidMotionEstimatedHandler);
}

void OnNextPeriodEstimatedHandler(MotionStates newMotionState, uint16_t newPulsePeriod)
{
    switch (newMotionState)
    {
        case MotionStateAcceleration:
        case MotionStateUniformVelocity:
        case MotionStateDeceleration:

            stepperMotor.pulses ++;
        
            if (stepperMotor.rotaryDirection == RotaryDirectionClockwise)
                stepperMotor.absoluteScales ++;
            else
                stepperMotor.absoluteScales --;
        
            break;
        
        default:
            break;
    }
}

void OnMotionCompletedHandler(void)
{
    StopMotor(stepperMotor.number);
}

static void OnAdvancedTimerOutputDelayElapsedHandler(TIM_HandleTypeDef *htim)
{
    static uint8_t i = 0;
    
    switch (stepperMotor.number)
    {
        case ConnectorNumber0:
            AssignNewCompare(PulseWidthModulationOutputCompareChannel0, motions.estimatedNextPulsePeriod / 2);
            break;
        
        case ConnectorNumber1:
            AssignNewCompare(PulseWidthModulationOutputCompareChannel1, motions.estimatedNextPulsePeriod / 2);
            break;
        
        default:
            break;
    }
    
    
    if (++ i == 2)
    {
        EstimateNextPeriod(OnNextPeriodEstimatedHandler, OnMotionCompletedHandler);
        
        i = 0;
    }
}