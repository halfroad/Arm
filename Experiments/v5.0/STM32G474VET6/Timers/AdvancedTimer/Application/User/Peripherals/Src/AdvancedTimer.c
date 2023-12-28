#include "AdvancedTimer.h"

TIM_HandleTypeDef htim;

void (* PeriodArrived)(TIM_HandleTypeDef *TIM_HandleType);

void InitAdvancedTimer(uint16_t prescaler, uint16_t autoReload, void (* onPeriodArrived)(TIM_HandleTypeDef *htim))
{
    htim.Instance = TIM1;
    
    htim.Init.Prescaler = prescaler;
    htim.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim.Init.Period = autoReload;
    
    PeriodArrived = onPeriodArrived;
    
    HAL_TIM_Base_Init(&htim);
    HAL_TIM_Base_Start_IT(&htim);
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *TIM_HandleType)
{
    if (TIM_HandleType -> Instance == TIM1)
    {
        __HAL_RCC_TIM1_CLK_ENABLE();
        
        HAL_NVIC_SetPriority(TIM1_UP_TIM16_IRQn, 1, 1);
        HAL_NVIC_EnableIRQ(TIM1_UP_TIM16_IRQn);
    }
}

void TIM1_UP_TIM16_IRQHandler(void)
{
    if (__HAL_TIM_GET_FLAG(&htim, TIM_FLAG_UPDATE) == SET)
    {
        if (PeriodArrived)
            PeriodArrived(&htim);
        
        __HAL_TIM_CLEAR_IT(&htim, TIM_FLAG_UPDATE);
    }
}