#include <math.h>

#include "../Include/StepperMotor.h"

#include "../Include/TrapezoidalMotionAlgorithm.h"

#define TIMER_FREQUENCY                                 (SystemCoreClock / 85)

#define ANGLE_PER_STEP_SUB_DIVISION                     8                                                   /*  pulses/step     */
#define MINIMAL_ANGLE_PER_STEP                          (1.8f / ANGLE_PER_STEP_SUB_DIVISION)                /*  1.8f: rads/step
                                                                                                                rads/pulse      */
#define STEPS_PER_ROUND                                 200                                                 /*  steps/round     */
#define PULSES_PER_ROUND                                (ANGLE_PER_STEP_SUB_DIVISION * STEPS_PER_ROUND)     /*  pulses/round or pulses/2π or pulses/360°    */

#define PI                                              3.1415926
/*

α = 2π/PULSES_PER_ROUND

*/
#define ANGLE_PER_PULSE                                 (2 * PI / PULSES_PER_ROUND)                         /*
                                                                                                                PULSES_PER_ROUND        = pulses/rounds,
                                                                                                                θ(rads)                 = 2π/round
                                                                                                                θ / PULSES_PER_ROUND    = 2π/pulse
                                                                                                                                        = rads/pulse
                                                                                                            */

TrapezoidalMotionAlgorithmTypeDef trapezoidalMotionAlgorithmType;

void InitTrapezoidalMotionType(void)
{
    trapezoidalMotionAlgorithmType.nextPeriod                                = 0;
    trapezoidalMotionAlgorithmType.periodDuringUniformVelocity               = 0;
    trapezoidalMotionAlgorithmType.stepWhenMaximumVelocityLimitationReaches  = 0;
    trapezoidalMotionAlgorithmType.stepWhenDecelerationMustBegin             = 0;
    trapezoidalMotionAlgorithmType.stepWhenDecelerationBegin                 = 0;
    trapezoidalMotionAlgorithmType.stepsDuringDeceleration                   = 0;
    trapezoidalMotionAlgorithmType.stepsDuringCeleration                     = 0;
    trapezoidalMotionAlgorithmType.rotatedSteps                              = 0;
    trapezoidalMotionAlgorithmType.state                                     = MotionStateIdle;
}

void InitTrapezoidalMotionAlgorithm(void)
{
    trapezoidalMotionAlgorithmType.Init                                      = InitTrapezoidalMotionType;
}

/*

1. If the Acceleration Vaue is set to low, and the Maximum Velocity is high:
    The stpper motor already ran a long distance, but the motor has not reached the Maximum
    Velocity. In the meanwhile, the terminal step gets closer and closer, the velocity of motor
    
    cannot decelerate to zero if the motor is still not decelerated.
    
    In this scenario, the motor will be a <Triangular Motion>.
    
    Condition(s),
    The intersection of Acceleration Line Equation and Deceleration Line Equation is beneath
    the intersection of Acceleration Line Equation and Maximum Velocity Line Equation.
    
    The equations are,
    ωₐnₐ                = ωₑnₑ                                          ①
    nₐ + nₑ             = Total_Steps                                   ②
    
    Note,
    ωₐ is the Acceleration Value during acceleration phase.
    nₐ is the number of pulses (steps) during acceleration phase.
    ωₑ is the Acceleration Value during deceleration phase.
    nₑ is the Acceleration Value during deceleration phase.
    
2. If the Acceleration Value is set to high, and Maximum Velocity is low, the motor reaches the
    Maximum Velocity quickly. In the meanwhile, the terminal step is far way. The motor runs with
    the Maximum (Uniform) Velocity for a while until the circumstances met, the motor decelerates
    until stops at the terminal step.
    
    In this scenario the motor will be a <Triangular Motion>.
    The equations are,
    
    Condition(s),
    The intersection of Acceleration Line Equation and Deceleration Line Equation is above
    the intersection of Acceleration Line Equation and Maximum Velocity Line Equation.
    
    
    
    ωₐnₐ                = ωₑnₑ                                                  ①
    nₐ + nₘ + nₑ        = Total Steps                                           ④
    
    Note,
    ωₐ is the Acceleration Value during acceleration phase.                     ①
    nₐ is the number of pulses (steps) during acceleration phase.               ①④
    nₘ is the number of pulses (steps) during uniform velocity phase.           ④
    ωₑ is the Acceleration Value during deceleration phase.                     ①
    nₑ is the Acceleration Value during deceleration phase.                     ①④
    
    Compute the Pulse Time
    
    CₙTₜ = √(2α/ω) * (√(n + 1) - √n)                                        ⑤
    Cₙ = (1/Tₜ) * √(2α/ω) * (√(n + 1) - √n)                                 ⑥

    C₀ = (1/Tₜ) * √(2α / ω)                                                     ⑦
    Cₙ = C₀ * (√(n + 1) - √n)                                                   ⑧
    
    Note,
    Cₙ is the count value (Auto Reload) for pulse (or step) #n.                 ⑤
    Tₜ is the cycle of a clock tick.                                            ⑥
    α is the angle of a single step.                                            ⑤
    ω is the acceleration value during acceleration or deceleration phase.      ⑤
    n is the number #n tick.                                                    ⑤
    C₀ is the first count value (Auto Reload).                                  ⑧
*/
/**
  * @brief  This function is used to create the Trapezoidal Velocity Control Parameters.
  * @note   CreateTrapezoidalVelocityControlParameters() function is called from StepperMotorController.
  *
  * @param steps: Total Steps.
  * @param acceleration: the Acceleration value during acceleration phase.
  * @param maximumVelocity: the Velocity value during the motion of Uniform Velocity phase.
  * @param deceleration: the Acceleration value during deceleration phase.
  * @retval None
  */
void CreateTrapezoidalVelocityControlParameters(int16_t steps, uint32_t acceleration, uint32_t deceleration, uint32_t maximumVelocity)
{
    steps                                                                                   *= PULSES_PER_ROUND;
    
    if (steps < 0)
        steps                                                                               = -steps;
    else
    {
        if (steps == 1)
        {
            /*  Only 1 step, accelerate directly.   */
            trapezoidalMotionAlgorithmType.stepsDuringCeleration                            = -1;
            trapezoidalMotionAlgorithmType.state                                            = MotionStateDeceleration;
            /*  Default velocity.   */
            trapezoidalMotionAlgorithmType.nextPeriod                                       = 1000; /*  Give a default Auto Reload to render a
                                                                                default frequency of pulses, that means
                                                                                a default velocity for the stepper motor  */
        }
        else if (steps > 1)
        {
            /*
                Compute period during Uniform Velocity phase. The period will keep unchanged unitl the Uniform Velocity phase is over.
                period = (α / t) / ω
            */
            trapezoidalMotionAlgorithmType.periodDuringUniformVelocity                      = ANGLE_PER_PULSE * TIMER_FREQUENCY / maximumVelocity;
            /*
                Compute the C₀ and set the period, the unit of acceleration is rad/second² ⑦
                C₀ = (1/Tₜ) * √(2α / ω)                                                     ⑦
            */
            trapezoidalMotionAlgorithmType.nextPeriod                                       = TIMER_FREQUENCY * sqrt(2 * ANGLE_PER_PULSE / acceleration);
            /*
                Compute the step when the Maximum Velocity reaches.
                nₐ = Vₘₐₓ² / (2aₐα)                                                         ⑦
            */
            trapezoidalMotionAlgorithmType.stepWhenMaximumVelocityLimitationReaches = maximumVelocity * maximumVelocity / (2 * acceleration * ANGLE_PER_PULSE);
            
            /*  Move 1 step at least.    */
            if (trapezoidalMotionAlgorithmType.stepWhenMaximumVelocityLimitationReaches == 0)
                trapezoidalMotionAlgorithmType.stepWhenMaximumVelocityLimitationReaches     = 1;
            
            /*
                Compute the step when the deceleration begins.
                nₑ  = (nₐ + nₑ)ωₑ / (ωₐ + ωₑ) = Total_Steps * ωₑ / (ωₐ + ωₑ)
            */
            trapezoidalMotionAlgorithmType.stepWhenDecelerationMustBegin                    = steps * deceleration / (acceleration + deceleration);
            
            /*
                Move 1 step at least.
            */
            if (trapezoidalMotionAlgorithmType.stepWhenDecelerationMustBegin == 0)
                trapezoidalMotionAlgorithmType.stepWhenDecelerationMustBegin                = 1;
            
            if (trapezoidalMotionAlgorithmType.stepWhenDecelerationMustBegin <= trapezoidalMotionAlgorithmType.stepWhenMaximumVelocityLimitationReaches)
            {
                /*
                    In this scenario, the motor will be a <Triangular Motion>.

                    Condition(s),
                    The intersection of Acceleration Line Equation and Deceleration Line Equation is beneath
                    the intersection of Acceleration Line Equation and Maximum Velocity Line Equation.
                    
                    The equations are,
                    ωₐnₐ                = ωₑnₑ                                          ①
                    nₐ + nₑ             = Total_Steps                                   ②
                
                */
                trapezoidalMotionAlgorithmType.stepsDuringDeceleration                      = trapezoidalMotionAlgorithmType.stepWhenDecelerationMustBegin - steps;
            }
            else
            {
                /*
                    Condition(s),
                    The intersection of Acceleration Line Equation and Deceleration Line Equation is above
                    the intersection of Acceleration Line Equation and Maximum Velocity Line Equation.
                    
                    In this scenario the motor will be a <Triangular Motion>.
                    The equations are,
                    
                    ωₐnₐ                = ωₑnₑ                                              ①
                    nₐ + nₘ + nₑ        = Total Steps                                       ④
                */
                trapezoidalMotionAlgorithmType.stepsDuringDeceleration                      = -(trapezoidalMotionAlgorithmType.stepWhenMaximumVelocityLimitationReaches * acceleration / deceleration);
            }
            
            /*
                Move 1 step at least.
            */
            if (trapezoidalMotionAlgorithmType.stepsDuringDeceleration == 0)
                trapezoidalMotionAlgorithmType.stepsDuringDeceleration = -1;
            
            trapezoidalMotionAlgorithmType.stepWhenDecelerationBegin                        = steps + trapezoidalMotionAlgorithmType.stepsDuringCeleration;
            
            /*
                If the velocity of C₀ is greater than the maximum (Unifrom) velocity, rotate the stepper motor  ⑦
                with Uniform Velocity directly, but not accelerate anymore.
            */
            if (trapezoidalMotionAlgorithmType.nextPeriod <= trapezoidalMotionAlgorithmType.periodDuringUniformVelocity)
            {
                trapezoidalMotionAlgorithmType.nextPeriod                                   = trapezoidalMotionAlgorithmType.periodDuringUniformVelocity;
                trapezoidalMotionAlgorithmType.state                                        = MotionStateUniformVelocity;
            }
            else
                trapezoidalMotionAlgorithmType.state                                        = MotionStateAcceleration;
            
            trapezoidalMotionAlgorithmType.stepsDuringCeleration                            = 0;
        }
    }
}

void AssignNextPeriod(void (* motionStateChangeOccurs)(MotionStates newMotionState, uint16_t newPeriod))
{
    __IO static uint16_t lastPeriod                                                         = 0;
    __IO uint32_t newPeriod                                                                 = 0;
    
    switch (trapezoidalMotionAlgorithmType.state)
    {
        case MotionStateIdle:
        case MotionStateArrived:
            trapezoidalMotionAlgorithmType.stepsDuringCeleration                            = 0;
        
            break;
                
        case MotionStateAcceleration:
        {
            trapezoidalMotionAlgorithmType.rotatedSteps ++;
            trapezoidalMotionAlgorithmType.stepsDuringCeleration ++;
            
            newPeriod = trapezoidalMotionAlgorithmType.nextPeriod * (sqrt(trapezoidalMotionAlgorithmType.stepsDuringCeleration + 1) - sqrt(trapezoidalMotionAlgorithmType.stepsDuringCeleration));
            
            /*  Check whether the step of deceleration reaches. */
            if (trapezoidalMotionAlgorithmType.rotatedSteps >= trapezoidalMotionAlgorithmType.stepWhenDecelerationBegin)
            {
                trapezoidalMotionAlgorithmType.stepsDuringCeleration                        = trapezoidalMotionAlgorithmType.stepsDuringDeceleration;
                trapezoidalMotionAlgorithmType.state                                        = MotionStateDeceleration;
            }
            else if (newPeriod <= trapezoidalMotionAlgorithmType.periodDuringUniformVelocity)
            {
                /*
                
                Check whether the Maximum Velocity reaches, the period is smaller, the frequency (Velocity) is higher.
                In then case the velocity reaches the maximum velocity, or higher than maximum velocity, the motor runs
                at Uniform Velocity.
                
                */
                lastPeriod                                                                  = newPeriod;
                newPeriod                                                                   = trapezoidalMotionAlgorithmType.periodDuringUniformVelocity;
                
                trapezoidalMotionAlgorithmType.state                                        = MotionStateUniformVelocity;
            }
        }
    
            break;
        
        case MotionStateUniformVelocity:
        {
            trapezoidalMotionAlgorithmType.rotatedSteps ++;
            
            newPeriod                                                                       = trapezoidalMotionAlgorithmType.periodDuringUniformVelocity;
            
            /*  Check whether the deceleration step reaches.    */
            if (trapezoidalMotionAlgorithmType.rotatedSteps >= trapezoidalMotionAlgorithmType.stepWhenDecelerationBegin)
            {
                /*  The Steps During Celeration as the Acceleration/Deceleration steps. */
                trapezoidalMotionAlgorithmType.stepsDuringCeleration                        = trapezoidalMotionAlgorithmType.stepsDuringDeceleration;
                /*  Assign the period of deceleration phase with the last period of Uniform Velocity phase. */
                newPeriod                                                                   = lastPeriod;
                
                trapezoidalMotionAlgorithmType.state                                        = MotionStateDeceleration;
            }
        }
            break;
        
        case MotionStateDeceleration:
        {
            trapezoidalMotionAlgorithmType.rotatedSteps ++;
            
            trapezoidalMotionAlgorithmType.stepsDuringCeleration ++;
            newPeriod = trapezoidalMotionAlgorithmType.nextPeriod * (sqrt(trapezoidalMotionAlgorithmType.stepsDuringCeleration + 1) - sqrt(trapezoidalMotionAlgorithmType.stepsDuringCeleration));
            
            /*  Check whether the last step reaches.    */
            if (trapezoidalMotionAlgorithmType.stepsDuringCeleration >= 0)
            {
                trapezoidalMotionAlgorithmType.state                                        = MotionStateArrived;
                trapezoidalMotionAlgorithmType.rotatedSteps                                 = 0;
            }
        }
            break;
                    
        default:
            break;
    }
    
    trapezoidalMotionAlgorithmType.nextPeriod                                               = newPeriod;
    
    motionStateChangeOccurs(trapezoidalMotionAlgorithmType.state, newPeriod);
}