#include <stdio.h>

#include "./Timers/Include/AdvancedTimer.h"
#include "../Include/StepperMotor.h"
#include "../Include/SCurveMotionProfiles.h"

#include "../Include/StepperMotorController.h"

#define PRESCALER                               170

#define MINIMUM_ROTATION_ANGLE                  1
#define MAXIMUM_ROTATION_ANGLE                  50

#define INITIAL_VELOCITY                        0
#define FINAL_VELOCITY                          300
#define ACCELERATION_DURATION                   3.5f
#define DECELERATION_DURATION                   1.5f

#define DEFAULT_ROTATION_ANGLE                  15

static void OnOutputCompareDelayElapsedHandler(TIM_HandleTypeDef *htim);

extern StepperMotorTypeDef stepperMotorType;

__IO uint16_t rotationAngle                 = DEFAULT_ROTATION_ANGLE;

void InitStepperMotorController(void)
{
    InitAdvancedTimer(PRESCALER - 1, 0xFFFF - 1, OnOutputCompareDelayElapsedHandler);
    InitStepperMotor();
    InitSCurveMotionProfiles(PRESCALER);
}

void RotateMotor(void)
{
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
    
    ActivateMotor(stepperMotorType.number, stepperMotorType.rotaryDirection);
}

void SwitchRotaryDirection(RotaryDirections rotaryDirection)
{
    stepperMotorType.rotaryDirection = rotaryDirection;
    
    DivertMotor(stepperMotorType.number, stepperMotorType.rotaryDirection);
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

static void NextPulseCycleEstimatedCallback(uint16_t cycle)
{
    switch (stepperMotorType.number)
    {
        case ConnectorNumber0:
            
            AssignNewCompare(OutputCompareChannel0, cycle);
                
            break;
        
        case ConnectorNumber1:
            
            AssignNewCompare(OutputCompareChannel1, cycle);
            
            break;
        
        default:
            break;
    }    
}

static void LastPulseArrivedCallback(void)
{
    StationMotor();
}

static void OnOutputCompareDelayElapsedHandler(TIM_HandleTypeDef *htim)
{
    EstimateNextPulseCycle(NextPulseCycleEstimatedCallback, LastPulseArrivedCallback);
}

void RotaryDirectionEstimatedCallback(SCurveMotionProfilesRotaryDirections rotaryDirection)
{
    switch (rotaryDirection)
    {
        case SCurveMotionProfilesRotaryDirectionClockwise:
            
            SwitchRotaryDirection(RotaryDirectionClockwise);
        
            break;
        
        case SCurveMotionProfilesRotaryDirectionAntiClockwise:
            
            SwitchRotaryDirection(RotaryDirectionAntiClockwise);
        
            break;
        
        default:
            break;
    }
}

void FirstPulseInitiatedCallback(uint16_t cycle)
{
    ResetCounter();
    
    switch (stepperMotorType.number)
    {
        case ConnectorNumber0:
            
            AssignNewCompare(OutputCompareChannel0, cycle);
            RotateMotor();
        
            break;
        
        case ConnectorNumber1:
            
            AssignNewCompare(OutputCompareChannel1, cycle);
            RotateMotor();
            
            break;
        
        default:
            break;
    }
}

void StartMotionProfiles(void)
{
    char error[100];
    
    if (HAL_OK != InitiateFirstPulse(rotationAngle * STEPS_PER_ROUND, INITIAL_VELOCITY, FINAL_VELOCITY, ACCELERATION_DURATION, DECELERATION_DURATION, error, RotaryDirectionEstimatedCallback, FirstPulseInitiatedCallback))
        printf("%s", error);
}

void IncreaseRotationAngle(void)
{
    rotationAngle += 1;
    
    if (rotationAngle > MAXIMUM_ROTATION_ANGLE)
        rotationAngle = MINIMUM_ROTATION_ANGLE;
}

void DecreaseRotationAngle(void)
{
    rotationAngle -= 1;
    
    if (rotationAngle < MINIMUM_ROTATION_ANGLE)
        rotationAngle = MAXIMUM_ROTATION_ANGLE;
}

void ResetRotationAngle(void)
{
    rotationAngle = DEFAULT_ROTATION_ANGLE;
}