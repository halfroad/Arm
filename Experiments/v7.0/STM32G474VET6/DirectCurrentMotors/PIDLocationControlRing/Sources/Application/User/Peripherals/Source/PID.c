#include "../Include/PID.h"

void InitPID(PIDTypeDef *PIDType)
{
#if RING_PID_CONTROL == 1
    PIDType -> TargetValue              = 20.0f;    /*  Intial current to launch the motor  */
#else
    PIDType -> TargetValue              = 0.0f;
#endif
    
    PIDType -> NextExpectedValue        = 0.0f;
    
    PIDType -> ProportionalFactor       = KP;
    PIDType -> IntegralFactor           = KI;
    PIDType -> DerivativeFactor         = KD;
    
    PIDType -> PreviousError            = 0.0f;
    PIDType -> LastError                = 0.0f;
    PIDType -> Error                    = 0.0f;
    
    PIDType -> AccumulativeErrors       = 0.0f;
    
    /*
    Use Encoder instead.
    InitUpdateTimer(prescaler, period);
    */
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