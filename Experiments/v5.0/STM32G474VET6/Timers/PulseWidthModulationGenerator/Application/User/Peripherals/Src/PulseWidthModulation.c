#include "PulseWidthModulation.h"

TIM_HandleTypeDef TIM_HandleType = { 0 };

void InitPulseWidthModulation(uint16_t prescaler, uint16_t autoReload)
{    
    TIM_HandleType.Instance = TIM1;
    
    TIM_HandleType.Init.Prescaler = prescaler;
    TIM_HandleType.Init.CounterMode = TIM_COUNTERMODE_UP;
    TIM_HandleType.Init.Period = autoReload;
    TIM_HandleType.Init.RepetitionCounter = 0;
    
    HAL_TIM_PWM_Init(&TIM_HandleType);
    
    TIM_OC_InitTypeDef TIM_OC_InitType = { 0 };
    
    TIM_OC_InitType.OCMode = TIM_OCMODE_PWM1;
    TIM_OC_InitType.Pulse = (autoReload + 1) / 2;
    TIM_OC_InitType.OCPolarity = TIM_OCPOLARITY_HIGH;
    
    HAL_TIM_PWM_ConfigChannel(&TIM_HandleType, &TIM_OC_InitType, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&TIM_HandleType, TIM_CHANNEL_1);
}

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
    if (htim -> Instance == TIM1)
    {
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_TIM1_CLK_ENABLE();
        
        GPIO_InitTypeDef GPIO_InitType = { 0 };
        
        GPIO_InitType.Pin = GPIO_PIN_8;
        GPIO_InitType.Mode = GPIO_MODE_AF_PP;
        GPIO_InitType.Pull = GPIO_PULLUP;
        GPIO_InitType.Speed = GPIO_SPEED_FAST;
        GPIO_InitType.Alternate = GPIO_AF6_TIM1;
        
        HAL_GPIO_Init(GPIOA, &GPIO_InitType);
    }
}

void SetCompare(uint16_t newCompare)
{
    __HAL_TIM_SET_COMPARE(&TIM_HandleType, TIM_CHANNEL_1, newCompare);
}