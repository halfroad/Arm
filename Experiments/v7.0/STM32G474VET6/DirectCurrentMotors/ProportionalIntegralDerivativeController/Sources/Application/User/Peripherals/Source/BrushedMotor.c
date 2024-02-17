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
    MotorRotationDirectionClockwise,
    MotorRotationDirectionAnticlockwise,
    
} MotorRotationDirections;

extern TIM_HandleTypeDef TIM8_Handle;

static void ChangeMotorDirection(MotorRotationDirections direction);
static void SetMotorVelocity(uint16_t compare);

VelocityEncoder_TypeDef VelocityEncoder_Type = { 0 };
Motor_TypeDef Motor_Type = { 0 };

void InitDirectCurrenBrushedtMotor(void)
{
    CONNECTOR_1_STOP_GPIO_PORT_CLOCK_ENABLE();
    
    GPIO_InitTypeDef GPIO_InitType = { 0 };
    
    GPIO_InitType.Pin = CONNECTOR_1_STOP_GPIO_PIN;
    GPIO_InitType.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitType.Pull = GPIO_NOPULL;
    GPIO_InitType.Speed = GPIO_SPEED_HIGH;
    
    HAL_GPIO_Init(CONNECTOR_1_STOP_GPIO_PORT, &GPIO_InitType);
    HAL_GPIO_WritePin(CONNECTOR_1_STOP_GPIO_PORT, CONNECTOR_1_STOP_GPIO_PIN, GPIO_PIN_RESET);
    
    StopMotor();
    ChangeMotorDirection(MotorRotationDirectionClockwise);
    SetMotorVelocity(0);
    StartMotor();
}

void StartMotor(void)
{
    ENABLE_MOTOR;
}

void StopMotor(void)
{
    HAL_TIM_PWM_Stop(&TIM8_Handle, ADVANCE_TIMER_CHANNEL);
    HAL_TIMEx_PWMN_Stop(&TIM8_Handle, ADVANCE_TIMER_CHANNEL);
    
    DISABLE_MOTOR;
}

static void ChangeMotorDirection(MotorRotationDirections direction)
{
    HAL_TIM_PWM_Stop(&TIM8_Handle, ADVANCE_TIMER_CHANNEL);
    HAL_TIMEx_PWMN_Stop(&TIM8_Handle, ADVANCE_TIMER_CHANNEL);
    
    if (direction == MotorRotationDirectionClockwise)
        HAL_TIM_PWM_Start(&TIM8_Handle, ADVANCE_TIMER_CHANNEL);
    else if (direction == MotorRotationDirectionClockwise)
        HAL_TIMEx_PWMN_Start(&TIM8_Handle, ADVANCE_TIMER_CHANNEL);
}

static void SetMotorVelocity(uint16_t compare)
{
    if (compare < __HAL_TIM_GetAutoreload(&TIM8_Handle) - 0x0F)
        __HAL_TIM_SetCompare(&TIM8_Handle, ADVANCE_TIMER_CHANNEL, compare);    
}

void AdaptMotorVelocityDirection(float compare)
{
    uint32_t speed = (uint32_t)compare;
    
    if (compare >= 0)
    {
        ChangeMotorDirection(MotorRotationDirectionClockwise);
        SetMotorVelocity(speed);
    }
    else
    {
        ChangeMotorDirection(MotorRotationDirectionAnticlockwise);
        SetMotorVelocity(-speed);
    }
}

void ComputeVelocity(int32_t counterNumber, uint8_t iterations)
{
    static uint8_t times = 0, filters = 0;
    static float velocities [100] = { 0.0 };
    
    uint8_t m, n;
    
    float cursor = 0.0f;
    
    if (times == iterations)
    {
        /*
        
        Steps to compute the Rounds per Minute:
        
        Step #1: calculate the delta (increments or decrements) of counter within [iterations] times (Milliseconds).
        Step #2: calculate the delta (increments or decrements) of counter within one minite.
        Step #3: divide the counter number when the encoder rotates per round.
        Step #4: divide the Gear Ratio to acquire the final velocity of the motor.
        
        */
        VelocityEncoder_Type.counterNumber = counterNumber;
        VelocityEncoder_Type.delta = VelocityEncoder_Type.counterNumber - VelocityEncoder_Type.previousCounterNumber;
        velocities[filters ++] = (float)(VelocityEncoder_Type.delta * (1000 / iterations * 60.0) / (GEAR_RATIO * FREQUENCY_MULTIPLIER * PULSES_PER_ROUND));
        VelocityEncoder_Type.previousCounterNumber = VelocityEncoder_Type.counterNumber;  /* Persist the curent Counter Number. */
        
        /* Filter after 10 consecutive velocities. */
        if (filters == 10)
        {
            /* Bubble sorting. */
            for (m = 10; m >= 1; m --)
            {
                for (n = 0; n < m - 1; n ++)
                {
                    if (velocities[n] > velocities[n + 1])
                    {
                        cursor = velocities[n];
                        velocities[n] = velocities[n + 1];
                        velocities[n + 1] = cursor;
                    }
                }
            }
            
            cursor = 0.0f;
            
            for (m = 2; m < 8; m ++)
                cursor += velocities[m];
            
            cursor = (float)(cursor / 6.0f);
            
            /*
            
            First order low pass filter: Y(n)= qX(n) + (1-q)Y(n-1),
            
            X(n): Current sampling value.
            Y(n-1): Previous filter output.
            Y(n): Current filter output.
            
            */
            
            Motor_Type.velocity = (float)(0.52f * Motor_Type.velocity + (1.0f - .052f) * cursor);
            
            filters = 0;
        }
        
        times = 0;
    }
    
    times ++;
}