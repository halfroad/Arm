#include <stdio.h>
#include <math.h>

#include "../Include/BrushedMotor.h"
#include "../Include/CurrentVoltageTemperatureGatherer.h"
#include "../Include/TickTimer.h"
#include "../Include/VelocityEncoder.h"
#include "../../../Middlewares/Third_Party/UpperHostCommunications/Include/UpperHostCommunications.h"

#include "../Include/CurrentRingPIDController.h"

MotorControlProtocol motorControlProtocol;
PIDTypeDef PIDType;

extern int32_t GetEncoderCounter(void);
extern void EvaluateVelocity(int32_t counter, uint8_t iterations, int16_t *velocity);

static void OnConversionCompletedHandler(float voltage, float current, float temperature);
static void OnPIDComposedHandler (float newPulseWidthModulation);

void InitCurrentRingPIDController(void)
{
    InitMotor();
    
    /*  PIDType.OnPIDComposedHandler = OnPIDComposedHandler;    */
    
    InitPID(&PIDType);
    InitTickTimer(170 - 1, 1000 - 1);
    
    InitVelocityEncoder(0, ENCODER_AUTO_RELOAD);
    InitCurrentVoltageTemperatureGatherer(OnConversionCompletedHandler);
    
    InitUpperHostCommunications(&motorControlProtocol, &PIDType);
}

void TickTimerPeriodElapsedHandler(TIM_HandleTypeDef *htim)
{
    int32_t counter = GetEncoderCounter();
        
    EvaluateVelocity(counter, 10, &motorControlProtocol.velocity);

    static uint8_t i = 0;
    float newPulseWidthModulation = 0.0f;
    
    if (i ++ % PID_SAMPLING_CYCLE == 0)
    {
#if RING_PID_CONTROL == 0
        newPulseWidthModulation = ComposePID(&PIDType, motorControlProtocol.velocity);
#elif RING_PID_CONTROL == 1
        newPulseWidthModulation = ComposePID(&PIDType, motorControlProtocol.currents[0]);
#else
        newPulseWidthModulation = ComposePID(&PIDType, motorControlProtocol.velocity);
#endif
        /*  PIDType.OnPIDComposedHandler(newPulseWidthModulation);  */

        OnPIDComposedHandler(newPulseWidthModulation);
    }
}

void AssignNewTarget(int16_t newTarget)
{
    PIDType.TargetValue += newTarget;
    
    if (PIDType.TargetValue <= -MOTOR_PEAK_SPEED)
        PIDType.TargetValue = -MOTOR_PEAK_SPEED;
    else if (PIDType.TargetValue >= MOTOR_PEAK_SPEED)
        PIDType.TargetValue = MOTOR_PEAK_SPEED;   
}

void Accelerate(void)
{
    AssignNewTarget(10);
}

void Decelerate(void)
{
    AssignNewTarget(-10);
}

void Brake(void)
{
    AssignNewTarget(-PIDType.TargetValue);
}

static void OnConversionCompletedHandler(float voltage, float current, float temperature)
{
    static uint8_t i = 0;
    
    motorControlProtocol.currents[0] = current;
    
    if (i ++ % 20 == 0)
    {
        printf("Voltage = %.1fV, current = %.1fmA, temperature = %.1fC.\n", voltage, current, temperature);
    }
}

static void OnPIDComposedHandler (float newPulseWidthModulation)
{
    if ( motorControlProtocol.state <= MotorStateRun)
    {
        if (newPulseWidthModulation >= ENCODER_PEAK_PULSE_WIDTH_MODULAION)
            motorControlProtocol.pulseWidthModulation = ENCODER_PEAK_PULSE_WIDTH_MODULAION;
        else if (newPulseWidthModulation <= -ENCODER_PEAK_PULSE_WIDTH_MODULAION)
            motorControlProtocol.pulseWidthModulation = -ENCODER_PEAK_PULSE_WIDTH_MODULAION;
        else
            motorControlProtocol.pulseWidthModulation = (int32_t)newPulseWidthModulation;
        
        RotateMotor(motorControlProtocol.pulseWidthModulation);
        
#ifdef UPPER_HOST_COMMUNICATIONS_ENABLED
        
       // ReportState(&motorControlProtocol, motorControlProtocol.state);
       // ReconcileInitialPIDs(&motorControlProtocol, MotorDriveBoardReportTypePID1, (float *)(&PIDType.TargetValue), PIDType.ProportionalFactor, PIDType.IntegralFactor, PIDType.DerivativeFactor);
       // ReportVelocity(&motorControlProtocol, motorControlProtocol.velocity);
        
      //  ReportWave(2, PIDType -> TargetValue);
       // ReportWave(3, motorControlProtocol.pulseWidthModulation * 100 / MOTOR_PEAK_SPEED);

#endif
        
    }   
}