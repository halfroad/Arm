#ifndef __PID_H
#define __PID_H

#include <stm32g4xx.h>

#define USE_POSITIONAL_PID

typedef struct
{
    __IO float TargetValue;
    __IO float ActualValue;
    
    __IO float ProportionalFactor;
    __IO float IntegralFactor;
    __IO float DerivativeFactor;
    
    __IO float AccumulativeErrors;
    
    __IO float LastSecondError;
    __IO float LastFirstError;
    __IO float Error;

    
} PIDTypeDef;

#endif	/* #ifndef __PID_H */