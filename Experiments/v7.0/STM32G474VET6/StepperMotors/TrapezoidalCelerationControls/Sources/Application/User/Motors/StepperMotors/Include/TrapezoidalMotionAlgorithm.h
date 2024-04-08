#ifndef __TRAPEZOIDAL_MOTION_ALGORITHM_H
#define __TRAPEZOIDAL_MOTION_ALGORITHM_H

#include <stm32g4xx.h> 

typedef enum
{
    MotionStateIdle,
    MotionStateAcceleration,
    MotionStateUniformVelocity,
    MotionStateDeceleration,
    MotionStateArrived,
    
} MotionStates;

typedef struct
{
    /*  The cycle of next pulse, will be set on register of Auto Reload of timer.    */
    __IO int32_t nextPeriod;
    /*  Timer period register during uniform velocity phase.    */
    __IO int32_t periodDuringUniformVelocity;
    
    /*  The lcoation when the Uniform Velocity phase begins.  */
    __IO uint32_t stepWhenMaximumVelocityLimitationReaches;
    /*  The lcoation when the deceleration begins.  */
    __IO uint32_t stepWhenDecelerationMustBegin;
    
    /*  The lcoation when the deceleration begin.  */
    __IO uint32_t stepWhenDecelerationBegin;
    /*  Number of steps during deceleration phase.  */
    __IO int32_t stepsDuringDeceleration;
    
    /*  Count for the steps during both acceleration and deceleration.   */
    __IO int32_t stepsDuringCeleration;
    /*  Steps rotated during all phases.   */
    __IO int32_t rotatedSteps;
    
    /*  Motion state.   */
    MotionStates state;
    
    /*  Initializer */
    void (* Init)(void);
    
} TrapezoidalMotionsTypeDef;

void InitTrapezoidalMotions(void);

void GenerateTrapezoidalMotions(int16_t steps, uint32_t acceleration, uint32_t deceleration, uint32_t maximumVelocity);
void ComputeNextPeriod(void (* motionStateChangeOccurs)(MotionStates newMotionState, uint16_t newPeriod));

#endif  /*  #ifndef __TRAPEZOIDAL_MOTION_ALGORITHM_H   */