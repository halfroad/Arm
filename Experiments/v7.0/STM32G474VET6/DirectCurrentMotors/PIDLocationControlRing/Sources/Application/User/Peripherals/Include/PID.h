#ifndef __PID_H
#define __PID_H

#include <stm32g4xx.h>

#define USE_INCREMENTAL_PID_ALGORITHM
#define RING_PID_CONTROL                2   /*  0: Velocity PID, 1: Current PID, 2: Location PID. */

#if RING_PID_CONTROL == 0

#ifdef USE_INCREMENTAL_PID_ALGORITHM
        
#define KP                              10.00f
#define KI                              6.00f
#define KD                              0.50f

#else
    
#define KP                              8.50f
#define KI                              5.00f
#define KD                              0.10f

#endif  /*  #if RING_PID_CONTROL == 0    */
    
#elif RING_PID_CONTROL == 1

#ifdef USE_INCREMENTAL_PID_ALGORITHM

#define KP                              10.0f
#define KI                              7.0f
#define KD                              2.0f

#else

#define KP                              0.0f
#define KI                              6.0f
#define KD                              4.0f

#endif  /*  #elif RING_PID_CONTROL == 1    */
    
#elif RING_PID_CONTROL == 2

#ifdef USE_INCREMENTAL_PID_ALGORITHM

#define KP                              15.0f
#define KI                              0.0f
#define KD                              7.5f

#else

#define KP                      1       5.0f
#define KI                              0.0f
#define KD                              7.5f

#endif
    
#endif  /*  #if RING_PID_CONTROL == 0   */

#define PID_SAMPLING_CYCLE              50

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

void InitPID(PIDTypeDef *PIDType);

__IO float ComposePID(PIDTypeDef *pid, float feebackValue);

#endif	/* #ifndef __PID_H */