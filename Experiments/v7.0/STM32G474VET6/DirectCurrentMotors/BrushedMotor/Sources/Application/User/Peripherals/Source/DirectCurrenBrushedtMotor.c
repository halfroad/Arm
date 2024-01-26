#include "../Include/AdvancedTimer.h"
#include "../Include/DirectCurrenBrushedtMotor.h"

#define ADVANCE_TIMER_CHANNEL                           TIM_CHANNEL_1

#define CONNECTOR_1_STOP_GPIO_PORT_CLOCK_ENABLE()       do { __HAL_RCC_GPIOD_CLK_ENABLE(); } while(0)

#define CONNECTOR_1_STOP_GPIO_PORT                      GPIOD
#define CONNECTOR_1_STOP_GPIO_PIN                       GPIO_PIN_11

#define ENABLE_MOTOR                                    HAL_GPIO_WritePin(CONNECTOR_1_STOP_GPIO_PORT, CONNECTOR_1_STOP_GPIO_PIN, GPIO_PIN_SET)
#define DISABLE_MOTOR                                   HAL_GPIO_WritePin(CONNECTOR_1_STOP_GPIO_PORT, CONNECTOR_1_STOP_GPIO_PIN, GPIO_PIN_RESET)

typedef enum
{
    MotorRotationDirectionClockwise,
    MotorRotationDirectionAnticlockwise,
    
} MotorRotationDirections;

extern TIM_HandleTypeDef TIM_Handle;

static void ChangeMotorDirection(MotorRotationDirections direction);
static void SetMotorSpeed(uint16_t compare);

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
    SetMotorSpeed(0);
    StartMotor();
}

void StartMotor(void)
{
    ENABLE_MOTOR;
}

void StopMotor(void)
{
    HAL_TIM_PWM_Stop(&TIM_Handle, ADVANCE_TIMER_CHANNEL);
    HAL_TIMEx_PWMN_Stop(&TIM_Handle, ADVANCE_TIMER_CHANNEL);
    
    DISABLE_MOTOR;
}

static void ChangeMotorDirection(MotorRotationDirections direction)
{
    HAL_TIM_PWM_Stop(&TIM_Handle, ADVANCE_TIMER_CHANNEL);
    HAL_TIMEx_PWMN_Stop(&TIM_Handle, ADVANCE_TIMER_CHANNEL);
    
    if (direction == MotorRotationDirectionClockwise)
        HAL_TIM_PWM_Start(&TIM_Handle, ADVANCE_TIMER_CHANNEL);
    else if (direction == MotorRotationDirectionClockwise)
        HAL_TIMEx_PWMN_Start(&TIM_Handle, ADVANCE_TIMER_CHANNEL);
}

static void SetMotorSpeed(uint16_t compare)
{
    if (compare < __HAL_TIM_GetAutoreload(&TIM_Handle) - 0x0F)
        __HAL_TIM_SetCompare(&TIM_Handle, ADVANCE_TIMER_CHANNEL, compare);    
}

void AdaptMotorSpeedDirection(float compare)
{
    uint32_t velocity = (uint32_t)compare;
    
    if (compare >= 0)
    {
        ChangeMotorDirection(MotorRotationDirectionClockwise);
        SetMotorSpeed(velocity);
    }
    else
    {
        ChangeMotorDirection(MotorRotationDirectionAnticlockwise);
        SetMotorSpeed(-velocity);
    }
}