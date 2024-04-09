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
    __IO int32_t estimatedNextPulsePeriod;
    /*  The cycle of C0, will be set on register of Auto Reload of timer.   */
    __IO int32_t estimatedFirstPulsePeriod;
    /*  Timer period during uniform velocity phase.    */
    __IO int32_t estimatedPeriodOfMaximumVelocity;
    /*  The lcoation when the deceleration begin.  */
    __IO uint32_t estimatedStepWhenDecelerationBegins;
    /*  Number of steps during deceleration phase.  */
    __IO int32_t estimatedTotalStepsDuringDeceleration;
    
    /*  Count for the steps during both acceleration and deceleration.   */
    __IO int32_t stepsCountingWhenCelerating;
    /*  Steps rotated during all phases.   */

    
    /*  Motion state.   */
    MotionStates state;
    
    /*  Initializer */
    void (* Init)(void);
    
} TrapezoidalMotionAlgorithmTypeDef;

typedef struct
{
    /*  The estimate step (location, a.k.a the intersection of acceleration linear and maximum velocity linear) when the
        motor is accelerated from 0 to maximum velocity even though the motor does not reach the maximum velocity.
    
        intersectionOfAccelerationMaximumLinears = max_s_lim 
    */
    __IO uint32_t intersectionOfAccelerationMaximumLinears;
    
    /*  The estimated step (location, a.k.a the intersection of acceleration linear and deceleration linear) when the
        motor has to decelerate immediately.
    
        intersectionOfAccelerationDecelerationLinears = acc_lim 
    */
    __IO uint32_t intersectionOfAccelerationDecelerationLinears;
    
    /*
        Actual steps during deceleration.
    */
    __IO int32_t actualStepsOfDeceleration;
    
    /*  Residual of computation.    */
    __IO int32_t residual;
    
    /*
        Steps moved
    */
    __IO uint16_t movedSteps;
    
} TrapezoidalMotionIntermediateParametersTypeDef;

void InitTrapezoidalMotions(uint8_t prescaler);

void MoveTrapezoid(int32_t rounds, uint32_t acceleration, uint32_t velocity, uint32_t deceleration, void (* onTrapezoidMotionEstimated)(int32_t steps));
void EstimateNextPeriod(void (* onNextPeriodEstimated)(MotionStates newMotionState, uint16_t newPulsePeriod), void (* onMotionCompleted)(void));

#endif  /*  #ifndef __TRAPEZOIDAL_MOTION_ALGORITHM_H   */