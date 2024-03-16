#include <stm32g4xx.h>

#include "../Include/AdvancedTimer.h"

#define TIMER                                                   TIM8
#define TIMER_RCC_CLOCK_ENABLE()                                do  {   __HAL_RCC_TIM8_CLK_ENABLE();    }   while(0)
#define SERVO1_TIMER_PWM_OUTPUT_CAPTURE_MODE                    TIM_OCMODE_PWM2
#define SERVO1_TIMER_PWM_CHANNEL                                TIM_CHANNEL_1
#define SERVO2_TIMER_PWM_CHANNEL                                TIM_CHANNEL_2

#define SERVO1_TIMER_PWM_GPIO_PORT                              GPIOC
#define SERVO1_TIMER_PWM_GPIO_RCC_CLOCK_ENABLE()                do  {   __HAL_RCC_GPIOC_CLK_ENABLE();   }   while(0)
#define SERVO1_TIMER_PWM_GPIO_PIN                               GPIO_PIN_6

#define SERVO_TIMER_PWM_GPI_ALTERNATE_FUNCTION                  GPIO_AF4_TIM8

#define SERVO2_TIMER_PWM_GPIO_PORT                              GPIOC
#define SERVO2_TIMER_PWM_GPIO_RCC_CLOCK_ENABLE()                do  {   __HAL_RCC_GPIOC_CLK_ENABLE();   }   while(0)
#define SERVO2_TIMER_PWM_GPIO_PIN                               GPIO_PIN_7

TIM_HandleTypeDef TIM_HandleType;
TIM_OC_InitTypeDef TIM_OC_InitType;

void ConfigurePulseWidthModulation(uint16_t period);
void PWM_MspInitCallback(TIM_HandleTypeDef *htim);

void InitAdvancedTimer(uint32_t prescaler, uint16_t period)
{
    TIM_HandleType.Instance                 = TIMER;
    
    TIM_HandleType.Init.Prescaler           = prescaler;
    TIM_HandleType.Init.CounterMode         = TIM_COUNTERMODE_UP;
    TIM_HandleType.Init.Period              = period;
    TIM_HandleType.Init.ClockDivision       = TIM_CLOCKDIVISION_DIV1;
    TIM_HandleType.Init.AutoReloadPreload   = TIM_AUTOMATICOUTPUT_DISABLE;
    TIM_HandleType.Init.RepetitionCounter   = 0;
    
    TIM_HandleType.PWM_MspInitCallback      = PWM_MspInitCallback;
    
    HAL_TIM_PWM_Init(&TIM_HandleType);
    
    ConfigurePulseWidthModulation(period);
    
    /*  Start the PWM channels.  */
    HAL_TIM_PWM_Start(&TIM_HandleType, SERVO1_TIMER_PWM_CHANNEL);
    HAL_TIM_PWM_Start(&TIM_HandleType, SERVO2_TIMER_PWM_CHANNEL);
}

void ConfigurePulseWidthModulation(uint16_t period)
{
    TIM_OC_InitType.OCMode                  = SERVO1_TIMER_PWM_OUTPUT_CAPTURE_MODE;
    TIM_OC_InitType.Pulse                   = 0.5f / 20.0f * (period + 1);
    
    TIM_OC_InitType.OCPolarity              = TIM_OCPOLARITY_HIGH;
    TIM_OC_InitType.OCNPolarity             = TIM_OCNPOLARITY_HIGH;
    
    TIM_OC_InitType.OCFastMode              = TIM_OCFAST_DISABLE;
    TIM_OC_InitType.OCIdleState             = TIM_OCIDLESTATE_RESET;
    TIM_OC_InitType.OCNIdleState            = TIM_OCNIDLESTATE_RESET;
    
    HAL_TIM_PWM_ConfigChannel(&TIM_HandleType, &TIM_OC_InitType, SERVO1_TIMER_PWM_CHANNEL);
    HAL_TIM_PWM_ConfigChannel(&TIM_HandleType, &TIM_OC_InitType, SERVO2_TIMER_PWM_CHANNEL);
}

void AssignCompare(ServoMotorNumber servoNumber, uint16_t compare)
{
    switch (servoNumber)
    {
        case ServoMotorNumber0:
            __HAL_TIM_SetCompare(&TIM_HandleType, SERVO1_TIMER_PWM_CHANNEL, compare);
        
        break;
        
        case ServoMotorNumber1:
            __HAL_TIM_SetCompare(&TIM_HandleType, SERVO2_TIMER_PWM_CHANNEL, compare);
        
        default:
            break;
        
        break;
    }
}

void PWM_MspInitCallback(TIM_HandleTypeDef *htim)
{
    if (htim -> Instance == TIMER)
    {
       /*
        
        Table 12. STM32G474xB/xC/xE pin definition (continued)
        
        Pin name            Alternate functions
        
        PC6                     TIM8_CH1
        PC7                     TIM8_CH2
        
        */
        
        TIMER_RCC_CLOCK_ENABLE();
        
        GPIO_InitTypeDef GPIO_InitType      = { 0 };
        
        GPIO_InitType.Pin                   = SERVO1_TIMER_PWM_GPIO_PIN;
        GPIO_InitType.Mode                  = GPIO_MODE_AF_PP;
        GPIO_InitType.Pull                  = GPIO_NOPULL;
        GPIO_InitType.Alternate             = SERVO_TIMER_PWM_GPI_ALTERNATE_FUNCTION;
        
        SERVO1_TIMER_PWM_GPIO_RCC_CLOCK_ENABLE();
        HAL_GPIO_Init(SERVO1_TIMER_PWM_GPIO_PORT, &GPIO_InitType);
        
        GPIO_InitType.Pin                   = SERVO2_TIMER_PWM_GPIO_PIN;
        
        SERVO2_TIMER_PWM_GPIO_RCC_CLOCK_ENABLE();
        HAL_GPIO_Init(SERVO2_TIMER_PWM_GPIO_PORT, &GPIO_InitType);
    }
}