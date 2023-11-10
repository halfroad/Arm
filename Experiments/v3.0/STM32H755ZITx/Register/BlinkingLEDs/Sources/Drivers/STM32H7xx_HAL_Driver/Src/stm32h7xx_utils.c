#include "stm32h7xx_utils.h"

#if defined (DUAL_CORE)
/**
  * @brief  This function configures the Cortex-M SysTick source to have 1ms time base.
  * @note   When a RTOS is used, it is recommended to avoid changing the Systick
  *         configuration by calling this function, for a delay use rather osDelay RTOS service.
  * @param  CPU_Frequency Core frequency in Hz
  * @note   CPU_Frequency can be calculated thanks to RCC helper macro or function
  *         @ref LL_RCC_GetSystemClocksFreq
  *         LL_RCC_GetSystemClocksFreq() is used to calculate the CM7 clock frequency
  *         and _RCC_CALC_HCLK_FREQ is used to calculate the CM4 clock frequency.
  * @retval None
  */
#else
/**
  * @brief  This function configures the Cortex-M SysTick source to have 1ms time base.
  * @note   When a RTOS is used, it is recommended to avoid changing the Systick
  *         configuration by calling this function, for a delay use rather osDelay RTOS service.
  * @param  CPU_Frequency Core frequency in Hz
  * @note   CPU_Frequency can be calculated thanks to RCC helper macro or function
  *         @ref LL_RCC_GetSystemClocksFreq
  * @retval None
  */
#endif /* DUAL_CORE */
void Init1msTick(uint32_t CPU_Frequency)
{
  /* Use frequency provided in argument */
  InitTick(CPU_Frequency, 1000U);
}


/**
  * @brief  This function provides accurate delay (in milliseconds) based
  *         on SysTick counter flag
  * @note   When a RTOS is used, it is recommended to avoid using blocking delay
  *         and use rather osDelay service.
  * @note   To respect 1ms timebase, user should call @ref Init1msTick function which
  *         will configure Systick to 1ms
  * @param  Delay specifies the delay time length, in milliseconds.
  * @retval None
  */
void mDelay(uint32_t Delay)
{
  uint32_t count = Delay;
  __IO uint32_t  tmp = SysTick->CTRL;  /* Clear the COUNTFLAG first */
  /* Add this code to indicate that local variable is not used */
  ((void)tmp);

  /* Add a period to guaranty minimum wait */
  if(count < MAX_DELAY)
  {
      count++;
  }

  while (count != 0U)
  {
    if((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) != 0U)
    {
        count--;
    }
  }
}

#if defined (DUAL_CORE)
/**
  * @brief  This function sets directly SystemCoreClock CMSIS variable.
  * @note   Variable can be calculated also through SystemCoreClockUpdate function.
  * @param  CPU_Frequency Core frequency in Hz
  * @note   CPU_Frequency can be calculated thanks to RCC helper macro or function
  *         @ref LL_RCC_GetSystemClocksFreq
  *         LL_RCC_GetSystemClocksFreq() is used to calculate the CM7 clock frequency
  *         and __LL_RCC_CALC_HCLK_FREQ is used to calculate the CM4 clock frequency.
  * @retval None
  */
#else
/**
  * @brief  This function sets directly SystemCoreClock CMSIS variable.
  * @note   Variable can be calculated also through SystemCoreClockUpdate function.
  * @param  CPU_Frequency Core frequency in Hz
  * @note   CPU_Frequency can be calculated thanks to RCC helper macro or function
  *         @ref LL_RCC_GetSystemClocksFreq
  * @retval None
  */
#endif /* DUAL_CORE */
void SetSystemCoreClock(uint32_t CPU_Frequency)
{
  /* HCLK clock frequency */
  SystemCoreClock = CPU_Frequency;
}