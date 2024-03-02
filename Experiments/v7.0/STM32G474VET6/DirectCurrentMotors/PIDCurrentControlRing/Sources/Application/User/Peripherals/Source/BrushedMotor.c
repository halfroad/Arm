#include "../Include/AdvancedTimer.h"

#include "../Include/BrushedMotor.h"

#define ADVANCE_TIMER_CHANNEL                           TIM_CHANNEL_1

#define CONNECTOR_2_STOP_GPIO_PORT_CLOCK_ENABLE()       do { __HAL_RCC_GPIOD_CLK_ENABLE(); } while(0)

#define CONNECTOR_2_STOP_GPIO_PORT                      GPIOD
#define CONNECTOR_2_STOP_GPIO_PIN                       GPIO_PIN_11

#define ENABLE_MOTOR                                    HAL_GPIO_WritePin(CONNECTOR_2_STOP_GPIO_PORT, CONNECTOR_2_STOP_GPIO_PIN, GPIO_PIN_SET)
#define DISABLE_MOTOR                                   HAL_GPIO_WritePin(CONNECTOR_2_STOP_GPIO_PORT, CONNECTOR_2_STOP_GPIO_PIN, GPIO_PIN_RESET)

extern TIM_HandleTypeDef TIM8_Handle;

static void InitGPIOs(void);

static void DivertMotor(MotorRotationDirections direction);
static void RegulateMotor(uint32_t compare);

void InitMotor()
{
    InitGPIOs();

    InitAdvancedTimer(0, 8500 - 1, 0x00);
    
    DeactivateMotor();
    ActivateMotor();
}

static void InitGPIOs(void)
{
    CONNECTOR_2_STOP_GPIO_PORT_CLOCK_ENABLE();
    
    GPIO_InitTypeDef GPIO_InitType = { 0 };
    
    GPIO_InitType.Pin = CONNECTOR_2_STOP_GPIO_PIN;
    GPIO_InitType.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitType.Pull = GPIO_NOPULL;
    GPIO_InitType.Speed = GPIO_SPEED_HIGH;
    
    HAL_GPIO_Init(CONNECTOR_2_STOP_GPIO_PORT, &GPIO_InitType);
    HAL_GPIO_WritePin(CONNECTOR_2_STOP_GPIO_PORT, CONNECTOR_2_STOP_GPIO_PIN, GPIO_PIN_RESET);
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
    else if (direction == MotorRotationDirectionAntiClockwise)
        HAL_TIMEx_PWMN_Start(&TIM8_Handle, ADVANCE_TIMER_CHANNEL);
}

static void RegulateMotor(uint32_t compare)
{
    if (compare < __HAL_TIM_GetAutoreload(&TIM8_Handle) - 0x0F)
        __HAL_TIM_SetCompare(&TIM8_Handle, ADVANCE_TIMER_CHANNEL, compare);    
}

void RotateMotor(int32_t compare)
{
    if (compare > 0)
    {            
        DivertMotor(MotorRotationDirectionClockwise);
        RegulateMotor(compare);
    }
    else
    {
        DivertMotor(MotorRotationDirectionAntiClockwise);
        RegulateMotor(-compare);
    }
}