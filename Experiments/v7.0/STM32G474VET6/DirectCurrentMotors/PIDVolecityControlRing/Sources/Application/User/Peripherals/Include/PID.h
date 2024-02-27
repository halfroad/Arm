#ifndef __PID_H
#define __PID_H

#include <stm32g4xx.h>

#define USE_POSITIONAL_PID

#ifdef USE_POSITIONAL_PID

#define KP                      8.50f
#define KI                      5.00f
#define KD                      0.10f

#else

#define KP                      10.00f
#define KI                      6.00f
#define KD                      0.50f

#endif

#define PID_SAMPLING_CYCLE      50

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

    
} PIDTypeDef;

void InitPID(void);

__IO float ComposePID(PIDTypeDef *pid, float feebackValue);

#endif	/* #ifndef __PID_H */