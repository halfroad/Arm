#include "../Include/BasicTimer.h"

#define BASIC_TIMER_INSTANCE                                TIM7
#define TIMER_CLOCK_ENABLE()                                do { __HAL_RCC_TIM7_CLK_ENABLE(); } while(0)

TIM_HandleTypeDef TIM_Handle = { 0 };

void InitBasicTimer1(uint32_t prescaler, uint32_t autoReload)
{
    /*
    Initialize the Basic Timer TIM7.
    */
    
    TIMER_CLOCK_ENABLE();
    
    TIM_Handle.Instance = BASIC_TIMER_INSTANCE;
    
    /* TIM_Handle.Init.Prescaler = __HAL_TIM_CALC_PSC(SystemCoreClock, 1000); */
    TIM_Handle.Init.Prescaler = prescaler;
    TIM_Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
    TIM_Handle.Init.Period = autoReload;
    TIM_Handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    TIM_Handle.Init.RepetitionCounter = 0;
    TIM_Handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
       
    HAL_TIM_Base_Init(&TIM_Handle);
}

/**
  * @brief  This function configures the TIM6 as a time base source.
  *         The time source is configured  to have 1ms time base with a dedicated
  *         Tick interrupt priority.
  * @note   This function is called  automatically at the beginning of program after
  *         reset by HAL_Init() or at any time when clock is configured, by HAL_RCC_ClockConfig().
  * @param  TickPriority: Tick interrupt priority.
  * @retval HAL status
  */
HAL_StatusTypeDef InitBasicTimer(uint32_t prescaler, uint32_t autoReload)
{
  HAL_StatusTypeDef     status;

  /* Enable TIM6 clock */
  TIMER_CLOCK_ENABLE();

  /* Initialize TIM6 */
  TIM_Handle.Instance = BASIC_TIMER_INSTANCE;

  /* Initialize TIMx peripheral as follow:

  + Period = [(TIM6CLK/1000) - 1]. to have a (1/1000) s time base.
  + Prescaler = (uwTimclock/1000000 - 1) to have a 1MHz counter clock.
  + ClockDivision = 0
  + Counter direction = Up
  */
    TIM_Handle.Init.Prescaler = prescaler;
    TIM_Handle.Init.Period = autoReload;
    TIM_Handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    TIM_Handle.Init.CounterMode = TIM_COUNTERMODE_UP;

    status = HAL_TIM_Base_Init(&TIM_Handle);
    
    if (status == HAL_OK)
    {
        /* Start the TIM time Base generation in interrupt mode */
        status = HAL_TIM_Base_Start_IT(&TIM_Handle);
        
        if (status == HAL_OK)
        {
            /* Enable the TIM7 global Interrupt */
            HAL_NVIC_EnableIRQ(TIM7_DAC_IRQn);
            /* Configure the TIM IRQ priority */
            HAL_NVIC_SetPriority(TIM7_DAC_IRQn, 1UL << __NVIC_PRIO_BITS, 0U);
        }
    }

 /* Return function status */
  return status;
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{

    if (htim -> Instance == BASIC_TIMER_INSTANCE)
    {
    }
}

void TIM7_DAC_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM_Handle);
}

void TimerPeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim -> Instance == BASIC_TIMER_INSTANCE)
    {
        uint8_t i = 0;
    }
}


