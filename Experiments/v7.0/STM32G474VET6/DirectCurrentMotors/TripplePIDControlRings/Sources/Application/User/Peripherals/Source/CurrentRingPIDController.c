#include <stdio.h>

#include "../Include/BrushedMotor.h"
#include "../Include/CurrentVoltageTemperatureGatherer.h"
#include "../Include/TickTimer.h"
#include "../Include/VelocityEncoder.h"
#include "../../../Middlewares/Third_Party/UpperHostCommunications/Include/UpperHostCommunications.h"

#include "../Include/CurrentRingPIDController.h"

#define PULSES_PER_ROUND                                11 * 4 * 30
#define PEAK_TARGET_PULSES                              6600

#define USE_CURRENT_RING
#define MOTOR_DRIVE_LEAST_CURRENT                       40

MotorControlProtocol motorControlProtocol;

PIDTypeDef LocationPIDType;
PIDTypeDef VelocityPIDType;

#ifdef USE_CURRENT_RING
PIDTypeDef CurrentPIDType;
#endif  /*  #ifdef USE_CURRENT_RING */

extern int32_t GetEncoderCounter(void);
extern void EvaluateVelocity(int32_t counter, uint8_t iterations, int16_t *velocity);

static void OnConversionCompletedHandler(float voltage, float current, float temperature);
static void OnPIDComposedHandler (float newPulseWidthModulation);

void InitCurrentRingPIDController(void)
{
    InitMotor();
    
    /*  PIDType.OnPIDComposedHandler = OnPIDComposedHandler;    */
    
    InitPID(&LocationPIDType, 0.0f, LOCATION_PROPORTIONAL_FACTOR, LOCATION_INTEGRAL_FACTOR, LOCATION_DERIVATIVE_FACTOR);
    InitPID(&VelocityPIDType, 0.0f, VELOCITY_PROPORTIONAL_FACTOR, VELOCITY_INTEGRAL_FACTOR, VELOCITY_DERIVATIVE_FACTOR);
    
#ifdef USE_CURRENT_RING
    InitPID(&CurrentPIDType, 6.7, CURRENT_PROPORTIONAL_FACTOR, CURRENT_INTEGRAL_FACTOR, CURRENT_DERIVATIVE_FACTOR);
#endif  /*  #ifdef USE_CURRENT_RING */
    
    InitTickTimer(170 - 1, 1000 - 1);
    
    InitVelocityEncoder(0, ENCODER_AUTO_RELOAD);
    InitCurrentVoltageTemperatureGatherer(OnConversionCompletedHandler);
    
    InitUpperHostCommunications(&motorControlProtocol, &LocationPIDType);
}

void AssignNewTarget(int16_t newTarget)
{
    LocationPIDType.TargetValue += newTarget;
    
    if (LocationPIDType.TargetValue <= -PEAK_TARGET_PULSES)
        LocationPIDType.TargetValue = -PEAK_TARGET_PULSES;
    else if (LocationPIDType.TargetValue >= PEAK_TARGET_PULSES)
        LocationPIDType.TargetValue = PEAK_TARGET_PULSES;
    
    if (LocationPIDType.TargetValue > 0.0 || LocationPIDType.TargetValue < 0.0)
        motorControlProtocol.state = MotorStateRun;
    else
        motorControlProtocol.state = MotorStateBraked;
}

void Accelerate(void)
{
    AssignNewTarget(PULSES_PER_ROUND);
}

void Decelerate(void)
{
    AssignNewTarget(-PULSES_PER_ROUND);
}

void Brake(void)
{
    AssignNewTarget(-LocationPIDType.TargetValue);
}

void TickTimerPeriodElapsedHandler(TIM_HandleTypeDef *htim)
{
    int32_t counter = GetEncoderCounter();
        
    EvaluateVelocity(counter, 10, &motorControlProtocol.velocity);
    
    static uint8_t i = 0;
    float newPulseWidthModulation = motorControlProtocol.pulseWidthModulation;
    
    if (i ++ % PID_SAMPLING_CYCLE == 0)
    {
        /*  Constrain the Integrals.    */
        ConstrainIntegrals(&LocationPIDType, 1000, -1000);
        ConstrainIntegrals(&VelocityPIDType, 200, -200);
        
#ifdef USE_CURRENT_RING
        ConstrainIntegrals(&CurrentPIDType, 150, -150);
#endif  /*  #ifdef USE_CURRENT_RING */
        
        /*  Set the dead time for the Location Ring Control to get rid of tiny vibration.   */
        if (LocationPIDType.Error <= 20 && LocationPIDType.Error >= -20)
        {
            LocationPIDType.Error = 0.0f;
            LocationPIDType.AccumulativeErrors = 0.0f;
        }
        
        /*  Acquire the current counter of encoder, which is going to be used in Location Ring Control. */
        motorControlProtocol.hallSensorLocation = counter;
        
        /*  Location Ring Control (Outer Ring Control).  */
        newPulseWidthModulation = ComposePID(&LocationPIDType, motorControlProtocol.hallSensorLocation);
        
        /*  Constrain the new Pulse Width Modulation evaluated by Location Ring Control (Outer Ring Control) to confine the Velocity. 
        if (newPulseWidthModulation > 120)
            newPulseWidthModulation = 120;
        else if (newPulseWidthModulation < -120)
            newPulseWidthModulation = -120;
        */
        /*  Set the new target for the Velocity Ring Control (Inferior Outer Ring Control) by passing the new Pulse Width Modulation evaluated by Location Ring Control (Outer Ring Control).  */
        VelocityPIDType.TargetValue = newPulseWidthModulation;
        
        /*  Velocity Ring Control (Inferior Outer Ring Control).    */
        newPulseWidthModulation = ComposePID(&VelocityPIDType, motorControlProtocol.velocity);
        
#ifdef USE_CURRENT_RING
        
        /*  Constrain the new Pulse Width Modulation evaluated by Velocity Ring Control (Inferior Outer Ring Control) to confine the Current. */
        if (newPulseWidthModulation > 100)
            newPulseWidthModulation = 100;
                
        /*  Set the new target for the Current Ring Control (Inner Ring Control) by passing the new Pulse Width Modulation evaluated by Velocity Ring Control ( InferiorOuter Ring Control).  */
        CurrentPIDType.TargetValue = newPulseWidthModulation;
        
        /*  Current Ring Control (Inner Ring Control).    */
        newPulseWidthModulation = ComposePID(&CurrentPIDType, motorControlProtocol.currents[0]);
        
        if (newPulseWidthModulation < 0)
            newPulseWidthModulation = 0.0f;
        
#endif  /*  #ifdef USE_CURRENT_RING */
        
        /*  PIDType.OnPIDComposedHandler(newPulseWidthModulation);  */
        
        OnPIDComposedHandler(newPulseWidthModulation);
    }
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
            motorControlProtocol.pulseWidthModulation = newPulseWidthModulation;
        
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