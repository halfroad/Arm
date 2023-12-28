#include "BaseTimer.h"

TIM_HandleTypeDef timerHandleType;

void (*onPeriodArrived)(TIM_HandleTypeDef *htim);

/**
*
*   @brief
*   @note
*
*   @param prescaler
*   @param auto reload
*
*   @retval
*/
void InitTimer(uint16_t prescaler, uint16_t autoReload, void (* peroidArrived)(TIM_HandleTypeDef *htim))
{
#define TIMER_INSTANCE  TIM6;
    
    timerHandleType.Instance = TIMER_INSTANCE
    
    /** @defgroup TIM_AutoReloadPreload TIM Auto-Reload Preload
      * @{
    #define TIM_AUTORELOAD_PRELOAD_DISABLE                0x00000000U               !< TIMx_ARR register is not buffered 
    #define TIM_AUTORELOAD_PRELOAD_ENABLE                 TIM_CR1_ARPE              !< TIMx_ARR register is buffered
    
    timerHandleType.Init.AutoReloadPreload;
    */
    
    timerHandleType.Init.Prescaler = prescaler;
    timerHandleType.Init.CounterMode = TIM_COUNTERMODE_UP;
    timerHandleType.Init.Period = autoReload;
    
    onPeriodArrived = peroidArrived;
    
    HAL_TIM_Base_Init(&timerHandleType);
    
    HAL_TIM_Base_Start_IT(&timerHandleType);
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if (htim -> Instance == TIM6)
    {
        __HAL_RCC_TIM6_CLK_ENABLE();
        
        HAL_NVIC_SetPriority(TIM6_DAC_IRQn, 1, 3);
        HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
    }
}

void TIM6_DAC_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&timerHandleType);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim -> Instance == TIM6)
    {
        if (onPeriodArrived)
            onPeriodArrived(htim);
    }
        
}