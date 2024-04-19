#ifndef __S_CURVE_MOTION_PROFILES_H
#define __S_CURVE_MOTION_PROFILES_H

#include <stm32g4xx.h>

#define MINIMAL_STEP_DIVIDOR                                                    8                                                   /*  Dividor of minimal step.    */
#define ANGLE_PER_STEP                                                          1.8                                                 /*  1.8бу/ step. */
#define STEPS_PER_ROUND                                                         (360 / (ANGLE_PER_STEP / MINIMAL_STEP_DIVIDOR))     /*  Steps (Pulses) per round.   */

typedef enum
{
    SCurveMotionProfilesStateIdle,
    
    /*  Velocity increases rapidly. */
    SCurveMotionProfilesStateAcceleration,
    
    /*  Acceleration increases: a increase, v also increases very rapidly.
    SCurveMotionProfilesStateJerkyAcceleration, */
    /*  Acceleration decreases: a decrease, v increases rapidly
    SCurveMotionProfilesStateNegativeJerkyAcceleration,. */
    
    /*  Uniform Velocity, steady: a equals zero (may be a positive to resist the friction.), velocity keeps unchanged.    */
    SCurveMotionProfilesStateUniformVelocity,
    
    /*  Velocity decreases rapidly. */
    SCurveMotionProfilesStateDeceleration,
    
    /*  Deceleration (Negative Acceleration) decreases: -a decrease, v also decreases rapidly.
    SCurveMotionProfilesStateNegativeJerkyDeceleration, */
    /*  Deceleration (Negative Acceleration) decreases: -a increased, v also decreases very rapidly.
    SCurveMotionProfilesStateJerkyDeceleration, */
    
    SCurveMotionProfilesStateArrived,
    
} SCurveMotionProfilesStates;

typedef enum
{
    SCurveMotionProfilesMotionPsuedo,
    SCurveMotionProfilesMotionModeAcceleration,
    SCurveMotionProfilesMotionModeDeceleration,
    
} SCurveMotionProfilesMotionModes;

typedef enum
{
    SCurveMotionProfilesRotaryDirectionClockwise,
    SCurveMotionProfilesRotaryDirectionAntiClockwise,
    
} SCurveMotionProfilesRotaryDirections;

typedef struct
{
    uint16_t clockPrescaler;
    
    SCurveMotionProfilesStates state;
    SCurveMotionProfilesMotionModes motionMode;

    int32_t initialVelocity;
    int32_t finalVelocity;
    
    uint32_t stepDecelerationBegins;
    
    uint32_t accelerationSteps;
    uint32_t decelerationSteps;
    
    uint32_t totalSteps;
    
    float *accelerationVelocityDistributions;
    float *decelerationVelocityDistributions;
    
    __IO uint32_t currentStep;
    
    float *velocityCursor;
    
} SCurveMotionProfilesTypeDef;

typedef struct
{
    __IO uint32_t pulses;
    __IO uint16_t cycle;
    
} NewPulseUpdateParametersTypeDef;

void InitSCurveMotionProfiles(uint16_t prescaler);

HAL_StatusTypeDef DistributeVelocities(int32_t initialVelocity, int32_t finalVelocity, float duration, char *error);

HAL_StatusTypeDef InitiateFirstPulse(int32_t steps, int32_t initialVelocity, int32_t finalVelocity, float accelerationDuration, float decelerationDuration, char *error, 
    void (* RotaryDirectionEstimatedCallback)(SCurveMotionProfilesRotaryDirections rotaryDirection), void (* FirstPulseInitiatedCallback)(uint16_t cycle));

void EstimateNextPulseCycle(void (* NextPulseCycleEstimatedCallback)(uint16_t cycle), void (* LastPulseArrivedCallback)(void));

#endif  /*  #ifndef __S_CURVE_MOTION_PROFILES_H */