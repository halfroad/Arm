#include <math.h>

#include "../Include/StepperMotor.h"

#include "../Include/TrapezoidalMotionAlgorithm.h"

#define ANGLE_PER_STEP_SUB_DIVISION                     8
#define ANGLE_PER_STEP                                  1.8
/*  pulses/step     */
#define MINIMAL_ANGLE_PER_STEP                          ((float)(1.8f / ANGLE_PER_STEP_SUB_DIVISION))           /*  1.8f: rads/step
                                                                                                                    rads/pulse      */
#define PULSES_PER_ROUND                                (360 / MINIMAL_ANGLE_PER_STEP)                          /*  pulses/round or pulses/2π or pulses/360°    */
#define FREQUENCY                                       (SystemCoreClock / timerPrescaler)

#define PI                                              3.1415926f
/*

α = 2π/PULSES_PER_ROUND

*/
#define ANGLE_PER_PULSE                                 ((float)(2 * PI / PULSES_PER_ROUND))                    /*
                                                                                                                    PULSES_PER_ROUND        = pulses/rounds,
                                                                                                                    θ(rads)                 = 2π/round
                                                                                                                    θ / PULSES_PER_ROUND    = 2π/pulse
                                                                                                                                        = rads/pulse
                                                                                                                */

uint8_t timerPrescaler;

TrapezoidalMotionAlgorithmTypeDef motions;
TrapezoidalMotionIntermediateParametersTypeDef motionParameters;

void InitTrapezoidalMotionType(void)
{
    motions.estimatedNextPulsePeriod                                    = 0;
    motions.estimatedPeriodOfMaximumVelocity                            = 0;
    motions.estimatedStepWhenDecelerationBegins                          = 0;
    motions.estimatedTotalStepsDuringDeceleration                       = 0;
    motions.state                                                       = MotionStateIdle;
    
    motionParameters.intersectionOfAccelerationMaximumLinears           = 0;
    motionParameters.intersectionOfAccelerationDecelerationLinears      = 0;
    motionParameters.actualStepsOfDeceleration                          = 0;
    motionParameters.residual                                           = 0;
    motionParameters.movedSteps                                         = 0;
}

void InitTrapezoidalMotions(uint8_t prescaler)
{
    timerPrescaler                                                      = prescaler;
    motions.Init                                                        = InitTrapezoidalMotionType;
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
  * @note   MoveTrapezoid() function is called from StepperMotorController.
  *
  * @param rounds: Total rounds (rounds).
  * @param acceleration: the Acceleration value during acceleration phase.
  * @param velocity: the Velocity value during the motion of Uniform Velocity phase.
  * @param deceleration: the Acceleration value during deceleration phase.
  * @retval None
  */
void MoveTrapezoid(int32_t rounds, uint32_t acceleration, uint32_t velocity, uint32_t deceleration, void (* onTrapezoidMotionEstimated)(int32_t rounds))
{
    if (motions.state == MotionStateIdle ||
        motions.state == MotionStateArrived)
    {
        InitTrapezoidalMotionType();
        
        static int32_t steps                                                    = 0;
        steps                                                                   = rounds * PULSES_PER_ROUND;
        
        if (steps < 0)
            steps                                                               = -steps;
        
        if (steps == 1)
        {
            /*  Only 1 step, accelerate directly.   */
            motions.stepsCountingWhenCelerating                                 = -1;
            motions.state                                                       = MotionStateDeceleration;
            /*  Default velocity.   */
            motions.estimatedNextPulsePeriod                                    = 1000; /*  Give a default Compare to render a
                                                                                                default frequency of pulses, that means
                                                                                                a default velocity for the stepper motor  */
        }
        else if (steps > 1)
        {
            /*
                Compute period during Uniform Velocity phase. The period will keep unchanged unitl the Uniform Velocity phase is over.
                period = (α / t) / ω
            */
            motions.estimatedPeriodOfMaximumVelocity                            = (int32_t)(ANGLE_PER_PULSE * FREQUENCY / velocity);
            /*
                Compute the C₀ and set the period, the unit of acceleration is rad/second² ⑦
                C₀ = (1/Tₜ) * √(2α / ω)                                                 ⑦
            */
            motions.estimatedNextPulsePeriod                                    = (int32_t)(FREQUENCY * 0.96f * sqrt(2 * ANGLE_PER_PULSE / acceleration));
            motions.estimatedFirstPulsePeriod                                   = motions.estimatedNextPulsePeriod;
            /*
                Compute the step when the Maximum Velocity reaches.
                nₐ = Vₘₐₓ² / (2aₐα)                                                         ⑦
            
             motionParameters.intersectionOfAccelerationMaximumLinears = max_s_lim
            */
            motionParameters.intersectionOfAccelerationMaximumLinears           = (uint32_t)(velocity * velocity / (2 * ANGLE_PER_PULSE * acceleration));
            
            /*  Move 1 step at least.    */
            if (motionParameters.intersectionOfAccelerationMaximumLinears == 0)
                motionParameters.intersectionOfAccelerationMaximumLinears   = 1;
            
            /*
                Compute the step when the deceleration begins.
                nₑ  = (nₐ + nₑ)ωₑ / (ωₐ + ωₑ) = Total_Steps * ωₑ / (ωₐ + ωₑ)
            
                motionParameters.intersectionOfAccelerationDecelerationLinears = accl_lim
            */
            motionParameters.intersectionOfAccelerationDecelerationLinears      = (uint32_t)(steps * deceleration / (acceleration + deceleration));
            
            /*
                Move 1 step at least.
            */
            if (motionParameters.intersectionOfAccelerationDecelerationLinears == 0)
                motionParameters.intersectionOfAccelerationDecelerationLinears  = 1;
            
            if (motionParameters.intersectionOfAccelerationDecelerationLinears <= motionParameters.intersectionOfAccelerationMaximumLinears)
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
                motions.estimatedTotalStepsDuringDeceleration                  = motionParameters.intersectionOfAccelerationDecelerationLinears - steps;
            }
            else
            {
                /*
                    Condition(s),
                    The intersection of Acceleration Line Equation and Deceleration Line Equation is above
                    the intersection of Acceleration Line Equation and Maximum Velocity Line Equation.
                    
                    In this scenario the motor will be a <Triangular Motion>.
                    The equations are,
                    
                    ωₐnₐ                    = ωₑnₑ                                      ①
                    nₐ + nₘ + nₑ            = Total Steps                               ④
                */
                motions.estimatedTotalStepsDuringDeceleration                   = -(motionParameters.intersectionOfAccelerationMaximumLinears * acceleration / deceleration);
            }
            
            /*
                Move 1 step at least.
            */
            if (motions.estimatedTotalStepsDuringDeceleration == 0)
                motions.estimatedTotalStepsDuringDeceleration = -1;
            
            motions.estimatedStepWhenDecelerationBegins                         = steps + motions.estimatedTotalStepsDuringDeceleration;
            
            /*
                If the velocity of C₀ is greater than the maximum (Unifrom) velocity, rotate the stepper motor  ⑦
                with Uniform Velocity directly, but not accelerate anymore.
            */
            if (motions.estimatedNextPulsePeriod <= motions.estimatedPeriodOfMaximumVelocity)
            {
                motions.estimatedNextPulsePeriod                                = motions.estimatedPeriodOfMaximumVelocity;
                motions.state                                                   = MotionStateUniformVelocity;
            }
            else
                motions.state                                                   = MotionStateAcceleration;
            
            motions.stepsCountingWhenCelerating                                 = 0;
        }
        
        if (onTrapezoidMotionEstimated)
            onTrapezoidMotionEstimated(rounds);
    }
}

void EstimateNextPeriod(void (* onNextPeriodEstimated)(MotionStates newMotionState, uint16_t newPulsePeriod), void (* onMotionCompleted)(void))
{
    __IO static uint16_t lastPulsePeriod                     = 0;
    __IO static uint16_t newPulsePeriod                 = 0;
    
    switch (motions.state)
    {
        case MotionStateAcceleration:
        {
            motionParameters.movedSteps ++;
            motions.stepsCountingWhenCelerating ++;
            
            /*
            
                Cₙ = C₀ * (√(n + 1) - √n)                                                   ⑧
            
            */
            newPulsePeriod                              = motions.estimatedNextPulsePeriod - (2 * motions.estimatedNextPulsePeriod + motionParameters.residual) / (4 * motions.stepsCountingWhenCelerating + 1);
            motionParameters.residual                   = (2 * motions.estimatedNextPulsePeriod + motionParameters.residual) % (4 * motions.stepsCountingWhenCelerating + 1);
            /*  Check whether the step of deceleration reaches. */
            if (motionParameters.movedSteps >= motions.estimatedStepWhenDecelerationBegins)
            {
                motions.stepsCountingWhenCelerating     = motions.estimatedTotalStepsDuringDeceleration;
                motions.state                           = MotionStateDeceleration;
            }
            else if (newPulsePeriod <= motions.estimatedPeriodOfMaximumVelocity)
            {
                /*
                
                Check whether the Maximum Velocity reaches, the period is smaller, the frequency (Velocity) is higher.
                In then case the velocity reaches the maximum velocity, or higher than maximum velocity, the motor runs
                at Uniform Velocity.
                
                */
                lastPulsePeriod                         = newPulsePeriod;
                newPulsePeriod                          = motions.estimatedPeriodOfMaximumVelocity;
                
                motionParameters.residual               = 0;
                
                motions.state                           = MotionStateUniformVelocity;
            }
            
            onNextPeriodEstimated(motions.state, newPulsePeriod);
        }
    
            break;
        
        case MotionStateUniformVelocity:
        {
            motionParameters.movedSteps ++;
            
            newPulsePeriod                              = motions.estimatedPeriodOfMaximumVelocity;
            
            /*  Check whether the deceleration step reaches.    */
            if (motionParameters.movedSteps >= motions.estimatedStepWhenDecelerationBegins)
            {
                /*  The Steps During Celeration as the Acceleration/Deceleration steps. */
                motions.stepsCountingWhenCelerating     = motions.estimatedTotalStepsDuringDeceleration;
                /*  Assign the period of deceleration phase with the last period of Uniform Velocity phase. */
                newPulsePeriod                          = lastPulsePeriod;
                
                motions.state                           = MotionStateDeceleration;
            }
            
            onNextPeriodEstimated(motions.state, newPulsePeriod);
        }
            break;
        
        case MotionStateDeceleration:
        {
            motionParameters.movedSteps ++;
            motions.stepsCountingWhenCelerating ++;
            
            newPulsePeriod                              = motions.estimatedNextPulsePeriod - (2 * motions.estimatedNextPulsePeriod + motionParameters.residual) / (4 * motions.stepsCountingWhenCelerating + 1);
            motionParameters.residual                   = (2 * motions.estimatedNextPulsePeriod + motionParameters.residual) % (4 * motions.stepsCountingWhenCelerating + 1);
            
            /*  Check whether the last step reaches.    */
            if (motions.stepsCountingWhenCelerating >= 0)
            {
                motions.state                           = MotionStateArrived;
                motionParameters.movedSteps             = 0;
                
                motionParameters.residual               = 0;
            }
            
            onNextPeriodEstimated(motions.state, newPulsePeriod);
        }
            break;
        
        case MotionStateArrived:
        {
            motionParameters.residual                   = 0;
            motionParameters.movedSteps                 = 0;
        
            onMotionCompleted();
        }
        
            break;
                    
        default:
            break;
    }
    
    motions.estimatedNextPulsePeriod                    = newPulsePeriod;
}