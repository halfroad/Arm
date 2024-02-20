#ifndef __PID_H
#define __PID_H

#include <stm32g4xx.h>

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

void InitPID(void);

__IO float ComposePID(PIDTypeDef *pid, float feebackValue);

#endif	/* #ifndef __PID_H */