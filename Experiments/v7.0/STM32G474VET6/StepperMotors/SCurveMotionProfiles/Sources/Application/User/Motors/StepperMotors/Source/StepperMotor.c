#include "./Timers/Include/AdvancedTimer.h"

#include "../Include/StepperMotor.h"

#define MOTOR_0_DIRECTION_GPIO_PORT                                 GPIOF
#define RCC_MOTOR_0_DIRECTION_GPIO_CLOCK_ENABLE()                   do                                                                                                              \
                                                                    {                                                                                                               \
                                                                        __HAL_RCC_GPIOF_CLK_ENABLE();                                                                               \
                                                                    }                                                                                                               \
                                                                    while   (0)

#define MOTOR_0_DIRECTION_GPIO_PIN                                  GPIO_PIN_2

#define MOTOR_0_ENABLEMENT_GPIO_PORT                                GPIOF

#define RCC_MOTOR_0_ENABLEMENT_GPIO_CLOCK_ENABLE()                  do                                                                                                              \
                                                                    {                                                                                                               \
                                                                        __HAL_RCC_GPIOF_CLK_ENABLE();                                                                               \
                                                                    }                                                                                                               \
                                                                    while   (0)

#define MOTOR_0_ENABLEMENT_GPIO_PIN                                 GPIO_PIN_9

                                                                    
#define MOTOR_1_DIRECTION_GPIO_PORT                                 GPIOE
#define RCC_MOTOR_1_DIRECTION_GPIO_CLOCK_ENABLE()                   do                                                                                                              \
                                                                    {                                                                                                               \
                                                                        __HAL_RCC_GPIOE_CLK_ENABLE();                                                                               \
                                                                    }                                                                                                               \
                                                                    while   (0)

#define MOTOR_1_DIRECTION_GPIO_PIN                                  GPIO_PIN_5

#define MOTOR_1_ENABLEMENT_GPIO_PORT                                GPIOE
#define RCC_MOTOR_1_ENABLEMENT_GPIO_CLOCK_ENABLE()                  do                                                                                                              \
                                                                    {                                                                                                               \
                                                                        __HAL_RCC_GPIOE_CLK_ENABLE();                                                                               \
                                                                    }                                                                                                               \
                                                                    while   (0)

#define MOTOR_1_ENABLEMENT_GPIO_PIN                                 GPIO_PIN_6
                                                                    
#define DIVERT_MOTOR(number, direction)                             do                                                                                                              \
                                                                    {                                                                                                               \
                                                                        switch (number)                                                                                             \
                                                                        {                                                                                                           \
                                                                            case ConnectorNumber0:                                                                                  \
                                                                            {                                                                                                       \
                                                                                if (RotaryDirectionClockwise == direction)                                                          \
                                                                                    HAL_GPIO_WritePin(MOTOR_0_DIRECTION_GPIO_PORT, MOTOR_0_DIRECTION_GPIO_PIN, GPIO_PIN_SET);       \
                                                                                else                                                                                                \
                                                                                    HAL_GPIO_WritePin(MOTOR_0_DIRECTION_GPIO_PORT, MOTOR_0_DIRECTION_GPIO_PIN, GPIO_PIN_RESET);     \
                                                                            }                                                                                                       \
                                                                            break;                                                                                                  \
                                                                                                                                                                                    \
                                                                            case ConnectorNumber1:                                                                                  \
                                                                            {                                                                                                       \
                                                                                if (RotaryDirectionClockwise == direction)                                                          \
                                                                                    HAL_GPIO_WritePin(MOTOR_1_DIRECTION_GPIO_PORT, MOTOR_1_DIRECTION_GPIO_PIN, GPIO_PIN_SET);       \
                                                                                else                                                                                                \
                                                                                    HAL_GPIO_WritePin(MOTOR_1_DIRECTION_GPIO_PORT,MOTOR_1_DIRECTION_GPIO_PIN,  GPIO_PIN_RESET);     \
                                                                            }                                                                                                       \
                                                                            break;                                                                                                  \
                                                                                                                                                                                    \
                                                                            default:                                                                                                \
                                                                                break;                                                                                              \
                                                                        }                                                                                                           \
                                                                    }                                                                                                               \
                                                                    while   (0)
                                                                        
#define ENABLE_MOTOR(number, enablement)                            do                                                                                                              \
                                                                    {                                                                                                               \
                                                                        switch (number)                                                                                             \
                                                                        {                                                                                                           \
                                                                            case ConnectorNumber0:                                                                                  \
                                                                            {                                                                                                       \
                                                                                if (Enabled == enablement)                                                                          \
                                                                                    HAL_GPIO_WritePin(MOTOR_0_ENABLEMENT_GPIO_PORT, MOTOR_0_ENABLEMENT_GPIO_PIN, GPIO_PIN_RESET);    \
                                                                                else                                                                                                \
                                                                                    HAL_GPIO_WritePin(MOTOR_0_ENABLEMENT_GPIO_PORT, MOTOR_0_ENABLEMENT_GPIO_PIN, GPIO_PIN_SET);     \
                                                                            }                                                                                                       \
                                                                            break;                                                                                                  \
                                                                                                                                                                                    \
                                                                            case ConnectorNumber1:                                                                                  \
                                                                            {                                                                                                       \
                                                                                if (Enabled == enablement)                                                                          \
                                                                                    HAL_GPIO_WritePin(MOTOR_1_ENABLEMENT_GPIO_PORT, MOTOR_1_ENABLEMENT_GPIO_PIN, GPIO_PIN_RESET);   \
                                                                                else                                                                                                \
                                                                                    HAL_GPIO_WritePin(MOTOR_1_ENABLEMENT_GPIO_PORT, MOTOR_1_ENABLEMENT_GPIO_PIN, GPIO_PIN_SET);     \
                                                                            }                                                                                                       \
                                                                            break;                                                                                                  \
                                                                                                                                                                                    \
                                                                            default:                                                                                                \
                                                                                break;                                                                                              \
                                                                        }                                                                                                           \
                                                                    }                                                                                                               \
                                                                    while   (0)
StepperMotorTypeDef stepperMotorType    = { 0 };

void InitStepperMotorType()
{
    stepperMotorType.number             = ConnectorNumber0;
    stepperMotorType.enabled            = Disabled;
    stepperMotorType.rotaryDirection    = RotaryDirectionClockwise;
    
    stepperMotorType.angle              = 0.0f;
    stepperMotorType.pulses             = 0;
    stepperMotorType.absoluteScales     = 0.0f;
    
    stepperMotorType.state              = StateIdle;
}

void InitStepperMotor(void)
{
    GPIO_InitTypeDef GPIO_InitType      = { 0 };
    
    GPIO_InitType.Pin                   = MOTOR_0_DIRECTION_GPIO_PIN;
    GPIO_InitType.Mode                  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitType.Pull                  = GPIO_PULLDOWN;
    
    RCC_MOTOR_0_DIRECTION_GPIO_CLOCK_ENABLE();
    HAL_GPIO_Init(MOTOR_0_DIRECTION_GPIO_PORT, &GPIO_InitType);
    
    GPIO_InitType.Pin                   = MOTOR_0_ENABLEMENT_GPIO_PIN;
    
    RCC_MOTOR_0_ENABLEMENT_GPIO_CLOCK_ENABLE();
    HAL_GPIO_Init(MOTOR_0_ENABLEMENT_GPIO_PORT, &GPIO_InitType);
    
    GPIO_InitType.Pin                   = MOTOR_1_DIRECTION_GPIO_PIN;
    
    RCC_MOTOR_1_DIRECTION_GPIO_CLOCK_ENABLE();
    HAL_GPIO_Init(MOTOR_1_DIRECTION_GPIO_PORT, &GPIO_InitType);
    
    GPIO_InitType.Pin                   = MOTOR_1_ENABLEMENT_GPIO_PIN;
    
    RCC_MOTOR_1_ENABLEMENT_GPIO_CLOCK_ENABLE();
    HAL_GPIO_Init(MOTOR_1_ENABLEMENT_GPIO_PORT, &GPIO_InitType);
    
    stepperMotorType.Init               = InitStepperMotorType;
    
    if (stepperMotorType.Init)
        stepperMotorType.Init();
}

void ActivateMotor(ConnectorNumbers number, RotaryDirections rotationDirection)
{
    ENABLE_MOTOR(number, Enabled);
    DIVERT_MOTOR(number, rotationDirection);
}

void InactivateMotor(ConnectorNumbers number)
{
    ENABLE_MOTOR(number, Disabled);
}

void DivertMotor(ConnectorNumbers number, RotaryDirections rotationDirection)
{
    DIVERT_MOTOR(number, rotationDirection);
}