#include "../Include/PID.h"

void InitPID(PIDTypeDef *PIDType, float defaultTargetValue, float initialProportionalFactor, float initialIntegralFactor, float initialDerivativeFactor)
{
    PIDType -> TargetValue              = defaultTargetValue;
    PIDType -> NextExpectedValue        = 0.0f;
    
    PIDType -> ProportionalFactor       = initialProportionalFactor;
    PIDType -> IntegralFactor           = initialIntegralFactor;
    PIDType -> DerivativeFactor         = initialDerivativeFactor;
    
    PIDType -> PreviousError            = 0.0f;
    PIDType -> LastError                = 0.0f;
    PIDType -> Error                    = 0.0f;
    
    PIDType -> AccumulativeErrors       = 0.0f;
    
    /*
    Use Encoder instead.
    InitUpdateTimer(prescaler, period);
    */
}

void ConstrainIntegrals(PIDTypeDef *PID, float lower, float upper)
{
    if (PID -> AccumulativeErrors < lower)
        PID -> AccumulativeErrors = lower;
    else if (PID -> AccumulativeErrors > upper)
        PID -> AccumulativeErrors = lower;
}

__IO float ComposePID(PIDTypeDef *pid, float feebackValue)
{
    pid -> Error = pid -> TargetValue - feebackValue;
    
#ifdef USE_INCREMENTAL_PID_ALGORITHM
    
    pid -> NextExpectedValue += (pid -> ProportionalFactor * (pid -> Error - pid -> LastError) + pid -> IntegralFactor * pid -> Error + pid -> DerivativeFactor * (pid -> Error - 2.0 * pid -> LastError + pid -> PreviousError));
    
    pid -> LastError = pid -> Error;
    pid -> PreviousError = pid -> LastError;

#else
    
    pid -> AccumulativeErrors += pid -> Error;
    
    pid -> NextExpectedValue = pid -> ProportionalFactor * pid -> Error + pid -> IntegralFactor * pid -> AccumulativeErrors + pid -> DerivativeFactor * (pid -> Error - pid -> LastError);
    pid -> LastError = pid -> Error;
    
#endif
       
    return pid -> NextExpectedValue;
}