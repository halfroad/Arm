#ifndef __PID_H
#define __PID_H

#include <stm32g4xx.h>

#define USE_INCREMENTAL_PID_ALGORITHM

#ifdef USE_INCREMENTAL_PID_ALGORITHM

#define LOCATION_PROPORTIONAL_FACTOR                        0.06f
#define LOCATION_INTEGRAL_FACTOR                            0.00f
#define LOCATION_DERIVATIVE_FACTOR                          0.01f

#define VELOCITY_PROPORTIONAL_FACTOR                        5.00f
#define VELOCITY_INTEGRAL_FACTOR                            0.30f
#define VELOCITY_DERIVATIVE_FACTOR                          0.01f

#define CURRENT_PROPORTIONAL_FACTOR                         8.00f
#define CURRENT_INTEGRAL_FACTOR                             4.00f
#define CURRENT_DERIVATIVE_FACTOR                           1.00f

#else

#define LOCATION_PROPORTIONAL_FACTOR                        0.06f
#define LOCATION_INTEGRAL_FACTOR                            0.00f
#define LOCATION_DERIVATIVE_FACTOR                          0.01f

#define VELOCITY_PROPORTIONAL_FACTOR                        5.00f
#define VELOCITY_INTEGRAL_FACTOR                            0.30f
#define VELOCITY_DERIVATIVE_FACTOR                          0.01f

#define CURRENT_PROPORTIONAL_FACTOR                         8.00f
#define CURRENT_INTEGRAL_FACTOR                             4.00f
#define CURRENT_DERIVATIVE_FACTOR                           1.00f

#endif  /*  #ifdef USE_INCREMENTAL_PID_ALGORITHM    */

#define PID_SAMPLING_CYCLE                                  50

typedef struct
{
    __IO float TargetValue;
    __IO float NextExpectedValue;
    
    __IO float ProportionalFactor;
    __IO float IntegralFactor;
    __IO float DerivativeFactor;
    
    __IO float AccumulativeErrors;
    
    __IO float PreviousError;
    __IO float LastError;
    __IO float Error;
    
    void (* OnPIDComposedHandler)(float newPulseWidthModulation);

    
} PIDTypeDef;

void InitPID(PIDTypeDef *PIDType, float defaultTargetValue, float initialProportionalFactor, float initialIntegralFactor, float initialDerivativeFactor);

void ConstrainIntegrals(PIDTypeDef *PID, float lower, float upper);
__IO float ComposePID(PIDTypeDef *pid, float feebackValue);

#endif	/* #ifndef __PID_H */