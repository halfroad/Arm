#include "../Include/TickTimer.h"

#define TICK_TIMER                                          TIM5
#define TICK_TIMER_CLOCK_ENABLE()                           do { __HAL_RCC_TIM5_CLK_ENABLE(); } while (0)
#define TICK_TIMER_IRQN                                     TIM5_IRQn
#define TICK_TIMER_IRQHANDLER                               TIM5_IRQHandler

TIM_HandleTypeDef TickTimer_TIM_HandleType = { 0 };

void (* onPIDComposedEvent) (float newPulseWidthModulation);

void TickTimer_Base_MspInitCallback(TIM_HandleTypeDef *htim);
void TickTimerPeriodElapsedCallback(TIM_HandleTypeDef *htim);
__weak void TickTimerPeriodElapsedHandler(TIM_HandleTypeDef *htim);

void InitTickTimer(uint32_t prescaler, uint32_t autoReload)
{
    /* TIM_HandleTypeDefTickTimer_TIM_HandleType = { 0 }; */
    
    TickTimer_TIM_HandleType.Instance = TICK_TIMER;
    
    TickTimer_TIM_HandleType.Init.Prescaler = prescaler;
    TickTimer_TIM_HandleType.Init.CounterMode = TIM_COUNTERMODE_UP;
    TickTimer_TIM_HandleType.Init.Period = autoReload;
    TickTimer_TIM_HandleType.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
      
    TickTimer_TIM_HandleType.Base_MspInitCallback = TickTimer_Base_MspInitCallback;
    
    //TickTimer_TIM_HandleType.PeriodElapsedCallback = TickTimerPeriodElapsedCallback;
    
    HAL_TIM_Base_Init(&TickTimer_TIM_HandleType);
    HAL_TIM_RegisterCallback(&TickTimer_TIM_HandleType, HAL_TIM_PERIOD_ELAPSED_CB_ID, TickTimerPeriodElapsedCallback);

    HAL_TIM_Base_Start_IT(&TickTimer_TIM_HandleType);
    __HAL_TIM_CLEAR_IT(&TickTimer_TIM_HandleType, TIM_IT_UPDATE);
}

void StopTickTimer(void)
{
    HAL_TIM_Base_Stop(&TickTimer_TIM_HandleType);
    HAL_TIM_Base_Stop_IT(&TickTimer_TIM_HandleType);
}

/**
  * @brief  Initializes the TIM Base MSP.
  * @param  htim TIM Base handle
  * @retval None
  */
void TickTimer_Base_MspInitCallback(TIM_HandleTypeDef *htim)
{
  /* Prevent unused argument(s) compilation warning 
  UNUSED(htim);
    */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_TIM_Base_MspInit could be implemented in the user file
   */
    if (htim -> Instance == TICK_TIMER)
    {
        TICK_TIMER_CLOCK_ENABLE();
        
        HAL_NVIC_SetPriority(TICK_TIMER_IRQN, 2U, 2U);
        HAL_NVIC_EnableIRQ(TICK_TIMER_IRQN);
    }
}
/**
 * @brief  Period elapsed callback in non blocking mode
 * @note   This function is called  when TIM6 interrupt took place, inside
 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
 * a global variable "uwTick" used as application time base.
 * @param  htim : TIM handle
 * @retval None
 */
void TickTimerPeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    /* USER CODE BEGIN Callback 0 */

    /* USER CODE END Callback 0 */
    if (htim -> Instance == TICK_TIMER)
    {
        if (TickTimerPeriodElapsedHandler)
            TickTimerPeriodElapsedHandler(htim);
    }
    /* USER CODE BEGIN Callback 1 */

    /* USER CODE END Callback 1 */
}

void TICK_TIMER_IRQHANDLER(void)
{
    HAL_TIM_IRQHandler(&TickTimer_TIM_HandleType);
}

/**
  * @brief  OnTickTimerPeriodElapsed.
  * @param  htim TIM Base handle
  * @retval None
  */
__weak void TickTimerPeriodElapsedHandler(TIM_HandleTypeDef *htim)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(htim);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_TIM_Base_MspInit could be implemented in the user file
   */
}