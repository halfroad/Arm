#include "../Include/AdvancedTimer.h"
#include "../Include/VelocityEncoder.h"
#include "../Include/BrushedMotor.h"

#define ADVANCE_TIMER_CHANNEL                           TIM_CHANNEL_1

#define CONNECTOR_1_STOP_GPIO_PORT_CLOCK_ENABLE()       do { __HAL_RCC_GPIOD_CLK_ENABLE(); } while(0)

#define CONNECTOR_1_STOP_GPIO_PORT                      GPIOD
#define CONNECTOR_1_STOP_GPIO_PIN                       GPIO_PIN_11

#define ENABLE_MOTOR                                    HAL_GPIO_WritePin(CONNECTOR_1_STOP_GPIO_PORT, CONNECTOR_1_STOP_GPIO_PIN, GPIO_PIN_SET)
#define DISABLE_MOTOR                                   HAL_GPIO_WritePin(CONNECTOR_1_STOP_GPIO_PORT, CONNECTOR_1_STOP_GPIO_PIN, GPIO_PIN_RESET)

#define FREQUENCY_MULTIPLIER                            4   /* Sampling multiplier. */
#define PULSES_PER_ROUND                                11  /* Pulses per Round. */
#define GEAR_RATIO                                      30  /* Ratio of Velocity Reduction.*/

typedef enum
{
    MotorRotationDirectionClockwise         = 0x00,
    MotorRotationDirectionAnticlockwise     = 0x01,
    
} MotorRotationDirections;

extern TIM_HandleTypeDef TIM8_Handle;

VelocityEncoderTypeDef velocityEncoderType = { 0 };
MotorTypeDef motorType = { 0 };

static void InitGPIOs(void);

static void DivertMotor(MotorRotationDirections direction);
static void RegulateMotor(uint32_t compare);

void InitDirectCurrenBrushedMotor(void)
{
    InitGPIOs();
    InitAdvancedTimer(0, 8500 - 1, 0x00);
    InitVelocityEncoder(0, ENCODER_AUTO_RELOAD);
    InitCalculatorTimer(170 - 1, 1000 - 1);
    
    DeactivateMotor();
    DivertMotor(MotorRotationDirectionClockwise);
    RegulateMotor(0);
    ActivateMotor();
}

static void InitGPIOs(void)
{
    CONNECTOR_1_STOP_GPIO_PORT_CLOCK_ENABLE();
    
    GPIO_InitTypeDef GPIO_InitType = { 0 };
    
    GPIO_InitType.Pin = CONNECTOR_1_STOP_GPIO_PIN;
    GPIO_InitType.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitType.Pull = GPIO_NOPULL;
    GPIO_InitType.Speed = GPIO_SPEED_HIGH;
    
    HAL_GPIO_Init(CONNECTOR_1_STOP_GPIO_PORT, &GPIO_InitType);
    HAL_GPIO_WritePin(CONNECTOR_1_STOP_GPIO_PORT, CONNECTOR_1_STOP_GPIO_PIN, GPIO_PIN_RESET);
}

void ActivateMotor(void)
{
    ENABLE_MOTOR;
}

void DeactivateMotor(void)
{
    HAL_TIM_PWM_Stop(&TIM8_Handle, ADVANCE_TIMER_CHANNEL);
    HAL_TIMEx_PWMN_Stop(&TIM8_Handle, ADVANCE_TIMER_CHANNEL);
    
    DISABLE_MOTOR;
}

static void DivertMotor(MotorRotationDirections direction)
{
    HAL_TIM_PWM_Stop(&TIM8_Handle, ADVANCE_TIMER_CHANNEL);
    HAL_TIMEx_PWMN_Stop(&TIM8_Handle, ADVANCE_TIMER_CHANNEL);
    
    if (direction == MotorRotationDirectionClockwise)
        HAL_TIM_PWM_Start(&TIM8_Handle, ADVANCE_TIMER_CHANNEL);
    else if (direction == MotorRotationDirectionAnticlockwise)
        HAL_TIMEx_PWMN_Start(&TIM8_Handle, ADVANCE_TIMER_CHANNEL);
}

static void RegulateMotor(uint32_t compare)
{
    if (compare < __HAL_TIM_GetAutoreload(&TIM8_Handle) - 0x0F)
        __HAL_TIM_SetCompare(&TIM8_Handle, ADVANCE_TIMER_CHANNEL, compare);    
}

void RotateMotor(int32_t compare)
{   
    if (compare >= 0)
    {
        DivertMotor(MotorRotationDirectionClockwise);
        RegulateMotor(compare);
    }
    else
    {
        DivertMotor(MotorRotationDirectionAnticlockwise);
        RegulateMotor(-compare);
    }
}

void EstimateVelocity(int32_t counter, uint8_t iterations)
{
    static uint8_t times = 0, filters = 0;
    static float velocities [10] = { 0.0 };
    
    uint8_t m, n;
    
    float total = 0.0f;
    
    if (times == iterations)
    {
        /*
        
        Steps to compute the Rounds per Minute:
        
        Step #1: calculate the delta (increments or decrements) of counter within [iterations] times (Milliseconds).
        Step #2: calculate the delta (increments or decrements) of counter within one minite.
        Step #3: divide the counter number when the encoder rotates per round.
        Step #4: divide the Gear Ratio to acquire the final velocity of the motor.
        
        */
        velocityEncoderType.counterNumber = counter;
        velocityEncoderType.delta = velocityEncoderType.counterNumber - velocityEncoderType.previousCounterNumber;
        velocities[filters ++] = (float)(velocityEncoderType.delta * (1000.0f / iterations * 60.0) / (GEAR_RATIO * FREQUENCY_MULTIPLIER * PULSES_PER_ROUND));
        
        velocityEncoderType.previousCounterNumber = velocityEncoderType.counterNumber;  /* Persist the curent Counter Number. */
        
        /* Filter after 10 consecutive velocities. */
        if (filters >= 10)
        {
            /* Bubble sorting. */
            for (m = 10; m >= 1; m --)
            {
                for (n = 0; n < m - 1; n ++)
                {
                    if (velocities[n] > velocities[n + 1])
                    {
                        total = velocities[n];
                        velocities[n] = velocities[n + 1];
                        velocities[n + 1] = total;
                    }
                }
            }
            
            total = 0.0f;
            
            for (m = 2; m < 8; m ++)
                total += velocities[m];
            
            total = total / 6.0f;
            
            /*
            
            First order low pass filter: Y(n)= qX(n) + (1-q)Y(n-1),
            
            X(n): Current sampling value.
            Y(n-1): Previous filter output.
            Y(n): Current filter output.
            
            */
            
            motorType.velocity = (float)(0.52f * motorType.velocity + (1.0f - 0.52f) * total);
            
            filters = 0;
        }
        
        times = 0;
    }
    
    times ++;
}