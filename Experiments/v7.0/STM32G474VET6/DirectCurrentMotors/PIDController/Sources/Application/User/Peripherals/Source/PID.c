#include "../Include/PID.h"


#define USE_POSITIONAL_PID

#ifdef USE_POSITIONAL_PID

#define KP                                                              8.50f
#define KI                                                              5.00f
#define KD                                                              0.10f

#else

#define KP                                                              10.00f
#define KI                                                              6.00f
#define KD                                                              0.50f

#endif


PIDTypeDef PIDType = { 0 };

void InitPID(void)
{
    PIDType.TargetValue = 0.0f;
    PIDType.ActualValue = 0.0f;
    
    PIDType.ProportionalFactor = KP;
    PIDType.IntegralFactor = KI;
    PIDType.DerivativeFactor = KD;
    
    PIDType.LastSecondError = 0.0f;
    PIDType.LastFirstError = 0.0f;
    PIDType.Error = 0.0f;
    
    PIDType.AccumulativeErrors = 0.0f;
    
    /*
    Use Encoder instead.
    InitUpdateTimer(prescaler, period);
    */
}

static __IO float ComposePositionalPID(PIDTypeDef *pid, float feebackValue)
{
    pid -> Error = pid -> TargetValue - feebackValue;
    pid -> AccumulativeErrors += pid -> Error;
    
    pid -> ActualValue = pid -> ProportionalFactor * pid -> Error + pid -> IntegralFactor * pid -> AccumulativeErrors + pid -> DerivativeFactor * (pid -> Error - pid -> LastFirstError);
    
    pid -> LastFirstError = pid -> Error;
    
    return pid -> ActualValue;
}

static __IO float ComposeIncrementalPID(PIDTypeDef *pid, float feebackValue)
{
    pid -> Error = pid -> TargetValue - feebackValue;
    
    pid -> ActualValue += pid -> ProportionalFactor * (pid -> Error - pid -> LastFirstError) + pid -> IntegralFactor * pid -> Error + pid -> DerivativeFactor * (pid -> Error - 2.0 * pid -> LastFirstError + pid -> LastSecondError);
    
    pid -> LastFirstError = pid -> Error;
    pid -> LastSecondError = pid -> LastFirstError;
    
    return pid -> ActualValue;
}

__IO float ComposePID(PIDTypeDef *pid, float feebackValue)
{
#ifdef USE_POSITIONAL_PID
    return ComposePositionalPID(pid, feebackValue);
#else
    return ComposeIncrementalPID(pid, feebackValue);
#endif
}