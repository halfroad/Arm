#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "../../../Middlewares/Third_Party/AlienTek/Malloc/Include/Malloc.h"
#include "./SerialCommunications/Include/SerialCommunications.h"

#include "../Include/SCurveMotionProfiles.h"

#define CLOCK_FREQUENCY                                                         (SystemCoreClock / motionProfiles.clockPrescaler)

#define ROUNDS_PER_MINUTE_TO_STEPS_PER_SECOND(rpm)                              (rpm * STEPS_PER_ROUND / 60)                        /*  Convert rpm to Steps (Pulses) per second.   */

#define MINIMUM_VELOCITY                                                        (CLOCK_FREQUENCY / 65535.0f)
/*

Since the S(distance travelled) and v(elocity) are not the linear changes, here it is not allowed to compute the S = v * t directly. Here the Intergral is exploited,

S = ¡Ò¡÷v dt                             ¢Ù

Since the a(celeration) is not the liear change, here it is not allowed to compute the v = a * t directly. Here the Intergral is exploited,

¡÷v = ¡Òa dt                             ¢Ú

Integrate the ¢Ù¢Ú,

S = 1/2 * a

Since the J(erk) is the liear change, here it is allowed to compute the a = J * t directly.

a = J * t                                 ¢Û

Integrate the ¢Ú¢Û

¡÷v = ¡Ò J t dt = 1/2 * J * t^2.          ¢Ü

Integrate the ¢Ù¢Ü

S = ¡Ò(1/2 * J t^2) dt = 1/6 * J * t^3   ¢Ý

*/
#define JERKY_ACCELERATION_STEPS(jerk, duration)                                (jerk * pow(duration, 3) / 6.0f)

SCurveMotionProfilesTypeDef motionProfiles                          = { 0 };
NewPulseUpdateParametersTypeDef newPulseUpdateParameters            = { 0 };

void onByteReceivedHandler(void *protocol, uint8_t *byte);

void InitSCurveMotionProfiles(uint16_t prescaler)
{
    motionProfiles.clockPrescaler                                   = prescaler;
    
    motionProfiles.state                                            = SCurveMotionProfilesStateIdle;
    
    InitSerialCommunications(115200, NULL, onByteReceivedHandler);
}

void onByteReceivedHandler(void *protocol, uint8_t *byte)
{
    uint8_t i = 0;
}

//SCurveMotionProfiles_AlienTek
//SCurveMotionProfiles_Dev

HAL_StatusTypeDef DistributeVelocities(int32_t initialVelocity, int32_t finalVelocity, float duration, char *error)
{
    SCurveMotionProfilesMotionModes motionMode                      = SCurveMotionProfilesMotionPsuedo;
    
    if (initialVelocity <= finalVelocity)
    {
        /*  Circumanstence: Acceleration phase.  */
        motionProfiles.motionMode                                   = SCurveMotionProfilesMotionModeAcceleration;
        
        motionProfiles.initialVelocity                              = ROUNDS_PER_MINUTE_TO_STEPS_PER_SECOND(initialVelocity);
        motionProfiles.finalVelocity                                = ROUNDS_PER_MINUTE_TO_STEPS_PER_SECOND(finalVelocity);
    }
    else
    {
        /*  Circumanstence: Deceleration phase.  */
        motionProfiles.motionMode                                   = SCurveMotionProfilesMotionModeDeceleration;
        
        motionProfiles.initialVelocity                              = ROUNDS_PER_MINUTE_TO_STEPS_PER_SECOND(finalVelocity);
        motionProfiles.finalVelocity                                = ROUNDS_PER_MINUTE_TO_STEPS_PER_SECOND(initialVelocity);
    }

    duration                                                        /= 2;                                                                                                       /*  Durations of Jerky Acceleration and Negative Jerky Acceleration are identical.  */
    
    int32_t intermediateVelocity                                   = (motionProfiles.initialVelocity + motionProfiles.finalVelocity) / 2;                                      /*  Compute the intermediate velocity, equals the average velocity. */

    /*
    
    acceleration    = ¡÷v / ¡÷t
    jerk            = acceleration / ¡÷t = ¡÷v / ¡÷t^2
    
    */
    float jerk                                                      = fabs(2.0f * (intermediateVelocity - motionProfiles.initialVelocity) / (duration * duration));

    uint32_t jerkyAccelerationSteps                                 = motionProfiles.initialVelocity * duration + JERKY_ACCELERATION_STEPS(jerk, duration);
    uint32_t nagativeJerkyAccelerationSteps                         = (motionProfiles.finalVelocity + motionProfiles.initialVelocity) * duration - jerkyAccelerationSteps;
    uint32_t totalAccelerationSteps                                 = jerkyAccelerationSteps + nagativeJerkyAccelerationSteps;
     
    if (totalAccelerationSteps % 2)
        totalAccelerationSteps ++;
    
    float *velocityDistributions                                    = (float *) custom_malloc(SRAMIN, (totalAccelerationSteps + 1) * sizeof (float));
    
    if (velocityDistributions)
    {
        /*
        
            Compute the velovity for first step, then compute the arrival time according to the velocity.
        
            S = ¡Ò(1/2 * J t^2) dt       = 1/6 * J * t^3       ¢Ý
            1/6 * J * t^3               = 1
            t^3 =                       6 * 1 / J
        
        */
        float timeOfSpecificStep                                    = 6.0f * 1.0f / jerk;
        
        timeOfSpecificStep                                          = pow(timeOfSpecificStep, 1 / 3.0f);
        
        float totalDuration                                         = timeOfSpecificStep;
        
        /*
        
            ¡÷v = ¡Ò J t dt = 1/2 * J * t^2.                  ¢Ü
        
        */
        float velocityIncrements                                    = 1.0f / 2.0f * jerk * pow(totalDuration, 2);
        
        velocityDistributions[0]                                    = motionProfiles.initialVelocity + velocityIncrements;
        
        if (velocityDistributions[0] <= MINIMUM_VELOCITY)
            velocityDistributions[0]                                = MINIMUM_VELOCITY;
        
        for (uint32_t i = 1; i < totalAccelerationSteps; i ++)
        {
            /*  Compute the time of number of i step.   */
            timeOfSpecificStep                                      = 1.0f / velocityDistributions[i - 1];
            
            /*  Jerky acceleration phase: a(cceleration) increases, also velocity increases.   */
            if (i < jerkyAccelerationSteps)
            {
                totalDuration                                       += timeOfSpecificStep;
                
                /*  ¡÷v = ¡Ò J t dt = 1/2 * J * t^2.          ¢Ü   */
                velocityIncrements                                  = 1.0f / 2.0f * jerk * pow(totalDuration, 2);
                
                /*  Store the computed velocity for the step i.  */
                velocityDistributions[i]                            = motionProfiles.initialVelocity + velocityIncrements;
                                
                /*
                
                    The last step of Jerky Acceleration phase, the totalDuration may not equal the duration explicitly,
                    Here the maneuver could do the tricky to tackle with the inconstency, then the computed totalDuration is the
                    duration of Negative Jerky Acceleration phase.
                */
                if (i == jerkyAccelerationSteps - 1)
                    totalDuration = fabs(totalDuration - duration);
            }
            else
            {
                /*  Negative Jerky Acceleration phase: a(cceleration) decreases, but velocity increases.   */
                totalDuration                                       += timeOfSpecificStep;
                /*  ¡÷v = ¡Ò J t dt = 1/2 * J * t^2.          ¢Ü   */
                velocityIncrements                                  = 1.0f / 2.0f * jerk * pow(duration - totalDuration, 2);
                /*  Store the computed velocity for the step i.  */
                velocityDistributions[i]                            = motionProfiles.finalVelocity - velocityIncrements;
                
                if (velocityDistributions[i] >= motionProfiles.finalVelocity)
                {
                    /*  Actual acceleration steps.  */
                    totalAccelerationSteps = i;
                    
                    break;
                }
            }
        }
        
        if (motionProfiles.motionMode == SCurveMotionProfilesMotionModeAcceleration)
        {
            motionProfiles.accelerationSteps                            = totalAccelerationSteps;
            motionProfiles.accelerationVelocityDistributions            = velocityDistributions;
        }
        else if (motionProfiles.motionMode == SCurveMotionProfilesMotionModeDeceleration)
        {
            float velocity                                              = 0.0f;
            
            /*  Sort the table by descending order.    */
            for (uint32_t i = 0; i < (totalAccelerationSteps / 2); i ++)
            {
                velocity                                                = velocityDistributions[i];
                
                velocityDistributions[i]                                = velocityDistributions[totalAccelerationSteps - i - 1];
                velocityDistributions[totalAccelerationSteps - i - 1]   = velocity;
            }
            
            motionProfiles.decelerationSteps                            = totalAccelerationSteps;
            motionProfiles.decelerationVelocityDistributions            = velocityDistributions;
        }
        
        return HAL_OK;
    }
    else
    {
        sprintf(error, "Memory is not sufficient to alloc the velocities.\n");
        
        return HAL_ERROR;
    }
}

HAL_StatusTypeDef InitiateFirstPulse(int32_t steps, int32_t initialVelocity, int32_t finalVelocity, float accelerationDuration, float decelerationDuration, char *error, 
    void (* RotaryDirectionEstimatedCallback)(SCurveMotionProfilesRotaryDirections rotaryDirection), void (* FirstPulseInitiatedCallback)(uint16_t cycle))
{   
    if (HAL_OK == DistributeVelocities(initialVelocity, finalVelocity, accelerationDuration, error) &&
        HAL_OK == DistributeVelocities(finalVelocity, initialVelocity, decelerationDuration, error))
    {        
        if (steps < 0)
        {
            steps = -steps;
            
            RotaryDirectionEstimatedCallback(SCurveMotionProfilesRotaryDirectionAntiClockwise);
        }
        else
            RotaryDirectionEstimatedCallback(SCurveMotionProfilesRotaryDirectionClockwise);
        
        if (steps >= motionProfiles.accelerationSteps + motionProfiles.decelerationSteps)
        {
            motionProfiles.totalSteps                                   = steps;
            motionProfiles.stepDecelerationBegins                       = motionProfiles.totalSteps - motionProfiles.decelerationSteps;
            
            motionProfiles.currentStep                                  = 0;
            motionProfiles.state                                        = SCurveMotionProfilesStateAcceleration;
            motionProfiles.velocityCursor                               = motionProfiles.accelerationVelocityDistributions;
                        
            newPulseUpdateParameters.cycle                              = CLOCK_FREQUENCY / (* motionProfiles.velocityCursor);

            motionProfiles.velocityCursor ++;
            
            FirstPulseInitiatedCallback(newPulseUpdateParameters.cycle / 2);
            
            return HAL_OK;
        }
        else
        {
            custom_free(SRAMIN, motionProfiles.accelerationVelocityDistributions);
            custom_free(SRAMIN, motionProfiles.decelerationVelocityDistributions);
            
            sprintf(error, "Steps passed is not sufficient, please assign the parameters again.\n");
            
            return HAL_ERROR;
        }
    }
    
    return HAL_ERROR;
}

void EstimateNextPulseCycle(void (* NextPulseCycleEstimatedCallback)(uint16_t cycle), void (* LastPulseArrivedCallback)(void))
{
    static uint8_t i = 0;
    
    i ++;
    
    if (i == 2)
    {
        i = 0;
                        
        switch (motionProfiles.state)
        {
            case SCurveMotionProfilesStateAcceleration:
            {
                motionProfiles.currentStep ++;
                newPulseUpdateParameters.pulses ++;
                
                /*  Compute the counter of timer for the given velocity from velocities distribution.    */
                newPulseUpdateParameters.cycle                          = CLOCK_FREQUENCY / (* motionProfiles.velocityCursor);
                newPulseUpdateParameters.cycle                          /= 2;
                
                motionProfiles.velocityCursor ++;
                                
                if (motionProfiles.currentStep >= motionProfiles.accelerationSteps)
                {
                    /*  Release the allocated Velocities Distribution memory.   */
                    custom_free(SRAMIN, motionProfiles.accelerationVelocityDistributions);
                    
                    /*  Transtion to Uniform Velocity phase. */
                    motionProfiles.state                                = SCurveMotionProfilesStateUniformVelocity;
                }
            }
            
                break;
            
            case SCurveMotionProfilesStateUniformVelocity:
            {
                motionProfiles.currentStep ++;
                newPulseUpdateParameters.pulses ++;
                
                /*  Compute the counter of timer for the final velocity velocity as Uniform Velocity.    */
                newPulseUpdateParameters.cycle                          = CLOCK_FREQUENCY / motionProfiles.finalVelocity;
                newPulseUpdateParameters.cycle                          /= 2;
                
                if (motionProfiles.currentStep >= motionProfiles.stepDecelerationBegins)
                {
                    /*  Assign the Velocities Distribution of Deceleration phase to pointer of velocities.  */
                    motionProfiles.velocityCursor                       = motionProfiles.decelerationVelocityDistributions;
                    motionProfiles.state                                = SCurveMotionProfilesStateDeceleration;
                }
            }
            
                break;
            
            case SCurveMotionProfilesStateDeceleration:
            {
                motionProfiles.currentStep ++;
                newPulseUpdateParameters.pulses ++;
                
                /*  Compute the counter of timer for the given velocity from velocities distribution.    */
                newPulseUpdateParameters.cycle                          = CLOCK_FREQUENCY / (* motionProfiles.velocityCursor);
                newPulseUpdateParameters.cycle                          /= 2;
                
                motionProfiles.velocityCursor ++;
                                
                if (motionProfiles.currentStep >= motionProfiles.totalSteps)
                {
                    /*  Release the allocated Velocities Distribution memory.   */
                    custom_free(SRAMIN, motionProfiles.decelerationVelocityDistributions);
                    
                    motionProfiles.state                                = SCurveMotionProfilesStateArrived;
                }
            }
            
                break;
            
            case SCurveMotionProfilesStateArrived:
            {
                LastPulseArrivedCallback();
                
                motionProfiles.state                                    = SCurveMotionProfilesStateIdle;
            }
            
                break;
            
            default:
                break;
        }
    }
    
    NextPulseCycleEstimatedCallback(newPulseUpdateParameters.cycle);
}