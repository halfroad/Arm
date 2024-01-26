#include <stm32g4xx.h>

#include "../Include/PushButton.h"

#define BASIC_TIMER_INSTANCE                        TIM7
#define TIMER_CLOCK_ENABLE()                        do { __HAL_RCC_TIM7_CLK_ENABLE(); } while(0)


#define PUSH_BUTTON_0_GPIO_PIN                      GPIO_PIN_12
#define PUSH_BUTTON_1_GPIO_PIN                      GPIO_PIN_13
#define PUSH_BUTTON_2_GPIO_PIN                      GPIO_PIN_14

#define PUSH_BUTTON_GPIO_PORT                       GPIOE
#define PUSH_BUTTON_GPIO_CLOCK_ENABLE()             do { __HAL_RCC_GPIOE_CLK_ENABLE(); } while(0)

#define PUSH_BUTTON_GPIO_EXTI_PORT                  EXTI_GPIOE

HAL_StatusTypeDef InitBasicTimer(uint32_t prescaler, uint32_t autoReload);

TIM_HandleTypeDef TIM_Handle = { 0 };
EXTI_HandleTypeDef EXTI_HandleType = { 0 };

uint8_t StateFlags = 0x00U;

void InitPushButtons(void)
{
    PUSH_BUTTON_GPIO_CLOCK_ENABLE();
    
    GPIO_InitTypeDef GPIO_InitType = { 0 };
    
    GPIO_InitType.Pin = PUSH_BUTTON_0_GPIO_PIN | PUSH_BUTTON_1_GPIO_PIN | PUSH_BUTTON_2_GPIO_PIN;
    GPIO_InitType.Mode = GPIO_MODE_IT_FALLING;
    GPIO_InitType.Pull = GPIO_PULLUP;
    
    HAL_GPIO_Init(PUSH_BUTTON_GPIO_PORT, &GPIO_InitType);
    
    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 10U, 0U);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
    
    uint32_t precaler = __HAL_TIM_CALC_PSC(SystemCoreClock, 1e3);
    uint32_t period = __HAL_TIM_CALC_PERIOD(SystemCoreClock, precaler, 4);
    
    InitBasicTimer(precaler, period);
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

void EXTI15_10_IRQHandler(void)
{
    if (GPIO_PIN_SET == HAL_GPIO_ReadPin(PUSH_BUTTON_GPIO_PORT, PUSH_BUTTON_0_GPIO_PIN))
        HAL_GPIO_EXTI_IRQHandler(PUSH_BUTTON_0_GPIO_PIN);
    
    if (GPIO_PIN_SET == HAL_GPIO_ReadPin(PUSH_BUTTON_GPIO_PORT, PUSH_BUTTON_1_GPIO_PIN))
        HAL_GPIO_EXTI_IRQHandler(PUSH_BUTTON_1_GPIO_PIN);
    
    if (GPIO_PIN_SET == HAL_GPIO_ReadPin(PUSH_BUTTON_GPIO_PORT, PUSH_BUTTON_2_GPIO_PIN))
        HAL_GPIO_EXTI_IRQHandler(PUSH_BUTTON_2_GPIO_PIN);
}

/**
  * @brief  EXTI line detection callback.
  * @param  GPIO_Pin: Specifies the port pin connected to corresponding EXTI line.
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  /* Prevent unused argument(s) compilation warning 
  UNUSED(GPIO_Pin);
    */

  /* NOTE: This function should not be modified, when the callback is needed,
           the HAL_GPIO_EXTI_Callback could be implemented in the user file
   */
    
    if (GPIO_Pin == PUSH_BUTTON_0_GPIO_PIN)
    {
        if ((StateFlags | 0x01U << 0) == 0x01U)
        {
            StateFlags &= ~(0x01U << 0);
            
            HAL_TIM_Base_Start_IT(&TIM_Handle);
        }
    }
    if (GPIO_Pin == PUSH_BUTTON_1_GPIO_PIN)
    {
        if ((StateFlags | 0x02U << 1) == 0x02U)
        {
            StateFlags &= ~(0x01U << 1);
            
            HAL_TIM_Base_Start_IT(&TIM_Handle);
        }
    }
    
    if (GPIO_Pin == PUSH_BUTTON_2_GPIO_PIN)
    {
        if ((StateFlags | 0x02U << 2) == 0x02U)
        {
            StateFlags &= ~(0x01U << 2);
            
            HAL_TIM_Base_Start_IT(&TIM_Handle);
        }
    }
    
}

void TimerPeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim -> Instance == BASIC_TIMER_INSTANCE)
    {
        StateFlags = 0x00U;
        
        if (GPIO_PIN_RESET == HAL_GPIO_ReadPin(PUSH_BUTTON_GPIO_PORT, PUSH_BUTTON_0_GPIO_PIN))
        {
            StateFlags |= 0x01U << 0;
            HAL_TIM_Base_Stop_IT(&TIM_Handle);
        }
        
        if (GPIO_PIN_RESET == HAL_GPIO_ReadPin(PUSH_BUTTON_GPIO_PORT, PUSH_BUTTON_1_GPIO_PIN))
        {
            StateFlags |= 0x01U << 1;
            HAL_TIM_Base_Stop_IT(&TIM_Handle);
        }
        
        if (GPIO_PIN_RESET == HAL_GPIO_ReadPin(PUSH_BUTTON_GPIO_PORT, PUSH_BUTTON_2_GPIO_PIN))
        {
            StateFlags |= 0x01U << 2;
            HAL_TIM_Base_Stop_IT(&TIM_Handle);
        }
    }
}

PushButtonPressStates ScanButton(void)
{
    switch (StateFlags)
    {
        case 0x01U:
            return PUSH_BUTTON_0_PRESSED;
        break;
        
        case 0x02U:
            return PUSH_BUTTON_1_PRESSED;
        break;
        
        case 0x04U:
            return PUSH_BUTTON_2_PRESSED;
        break;
        
        default:
            return PUSH_BUTTON_NONE_PRESSED;
        break;
            
    }
}