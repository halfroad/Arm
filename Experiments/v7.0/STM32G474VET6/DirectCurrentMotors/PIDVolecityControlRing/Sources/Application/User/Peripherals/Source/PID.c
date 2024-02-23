#include "../Include/PID.h"

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