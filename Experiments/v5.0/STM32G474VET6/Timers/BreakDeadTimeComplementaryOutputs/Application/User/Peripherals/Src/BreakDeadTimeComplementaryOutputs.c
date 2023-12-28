#include "BreakDeadTimeComplementaryOutputs.h"

static TIM_HandleTypeDef TIM_HandleType;
static TIM_BreakDeadTimeConfigTypeDef TIM_BreakDeadTimeConfigType;

void InitBreakDeadTimeComplementaryOutputs(uint16_t prescaler, uint16_t autoReload)
{   
    /*Initialize the TIMER. */
    
    __HAL_RCC_TIM1_CLK_ENABLE();
    
    TIM_HandleType.Instance = TIM1;
    
    TIM_HandleType.Init.Prescaler = prescaler;
    TIM_HandleType.Init.CounterMode = TIM_COUNTERMODE_UP;
    TIM_HandleType.Init.Period = autoReload;
    TIM_HandleType.Init.ClockDivision = TIM_CLOCKDIVISION_DIV4;
    TIM_HandleType.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    
    HAL_TIM_PWM_Init(&TIM_HandleType);
    
    /* Configure the Pulse Width Modulation. */
    
    TIM_OC_InitTypeDef TIM_OC_InitType;
    
    TIM_OC_InitType.OCMode = TIM_OCMODE_PWM1;
    TIM_OC_InitType.Pulse = autoReload / 2;
    TIM_OC_InitType.OCPolarity = TIM_OCPOLARITY_LOW;
    TIM_OC_InitType.OCNPolarity = TIM_OCNPOLARITY_LOW;

    HAL_TIM_PWM_ConfigChannel(&TIM_HandleType, &TIM_OC_InitType, TIM_CHANNEL_1);
    
    /* Configure the Break Dead-time, and enable the interrupt for Dead-time. */
    
    TIM_BreakDeadTimeConfigType.LockLevel = TIM_LOCKLEVEL_OFF;
    TIM_BreakDeadTimeConfigType.BreakState = TIM_BREAK_ENABLE;
    TIM_BreakDeadTimeConfigType.BreakPolarity = TIM_BREAKPOLARITY_LOW;
    TIM_BreakDeadTimeConfigType.AutomaticOutput = TIM_AUTOMATICOUTPUT_ENABLE;
    
    HAL_TIMEx_ConfigBreakDeadTime(&TIM_HandleType, &TIM_BreakDeadTimeConfigType);                                                                                                                                                                                 
    
    /* Enable the Output Compare and Complementary Output Compare. */
    HAL_TIM_PWM_Start(&TIM_HandleType, TIM_CHANNEL_1);
    HAL_TIMEx_PWMN_Start(&TIM_HandleType, TIM_CHANNEL_1);
    
}

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    
    GPIO_InitTypeDef GPIO_InitType;
    
    /* Main output channel. */
    GPIO_InitType.Pin = GPIO_PIN_8;
    GPIO_InitType.Mode = GPIO_MODE_AF_PP;
    GPIO_InitType.Pull = GPIO_PULLUP;
    GPIO_InitType.Speed = GPIO_SPEED_FAST;
    GPIO_InitType.Alternate = GPIO_AF6_TIM1;
    
    HAL_GPIO_Init(GPIOA, &GPIO_InitType);
    
    __HAL_RCC_GPIOB_CLK_ENABLE();
    
    /* Complementary output channel. */
    GPIO_InitType.Pin = GPIO_PIN_13;
    
    HAL_GPIO_Init(GPIOB, &GPIO_InitType);
    
    /* Break Dead-Time. */
    
    GPIO_InitType.Pin = GPIO_PIN_12;
    
    HAL_GPIO_Init(GPIOB, &GPIO_InitType);
}

void SetBreakDeadTimeCompare(uint8_t deadTime, uint16_t compare)
{
    TIM_BreakDeadTimeConfigType.DeadTime = deadTime;
    
    HAL_TIMEx_ConfigBreakDeadTime(&TIM_HandleType, &TIM_BreakDeadTimeConfigType);
    __HAL_TIM_MOE_ENABLE(&TIM_HandleType);
    
    TIM1 -> CCR1 = compare;
}