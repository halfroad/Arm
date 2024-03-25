#include <stm32g4xx.h>

#include "../Include/PushButton.h"

#define BASIC_TIMER_INSTANCE                        TIM7
#define TIMER_CLOCK_ENABLE()                        do { __HAL_RCC_TIM7_CLK_ENABLE(); } while(0)
#define BASIC_TIMER_DAC_IRQN                        TIM7_DAC_IRQn
#define BASIC_TIMER_IRQN_HANDLER                    TIM7_DAC_IRQHandler

#define PUSH_BUTTON_0_GPIO_PIN                      GPIO_PIN_12
#define PUSH_BUTTON_1_GPIO_PIN                      GPIO_PIN_13
#define PUSH_BUTTON_2_GPIO_PIN                      GPIO_PIN_14

#define PUSH_BUTTON_GPIO_PORT                       GPIOE
#define PUSH_BUTTON_GPIO_CLOCK_ENABLE()             do { __HAL_RCC_GPIOE_CLK_ENABLE(); } while(0)

#define PUSH_BUTTON_GPIO_EXTI_PORT                  EXTI_GPIOE

#define PUSH_BUTTON_EXTI_IRQN_HANDLER               EXTI15_10_IRQHandler

HAL_StatusTypeDef InitBasicTimer(uint32_t prescaler, uint32_t autoReload);

static TIM_HandleTypeDef TIM7_Handle = { 0 };

uint8_t StateFlags = 0x00U;

void InitPushButtons(void)
{
    PUSH_BUTTON_GPIO_CLOCK_ENABLE();
    
    GPIO_InitTypeDef GPIO_InitType = { 0 };
    
    GPIO_InitType.Pin = PUSH_BUTTON_0_GPIO_PIN | PUSH_BUTTON_1_GPIO_PIN | PUSH_BUTTON_2_GPIO_PIN;
    GPIO_InitType.Mode = GPIO_MODE_IT_FALLING;
    GPIO_InitType.Pull = GPIO_PULLUP;
    
    HAL_GPIO_Init(PUSH_BUTTON_GPIO_PORT, &GPIO_InitType);
    
    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 5U, 0U);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
    
    uint32_t precaler = __HAL_TIM_CALC_PSC(SystemCoreClock, 1e3);
    uint32_t period = __HAL_TIM_CALC_PERIOD(SystemCoreClock, precaler, 100);
    
    InitBasicTimer(precaler, period);
}

void TIM7_Base_MspInitCallback(TIM_HandleTypeDef *htim)
{
    if (htim -> Instance == BASIC_TIMER_INSTANCE)
    {
    }
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
  TIM7_Handle.Instance = BASIC_TIMER_INSTANCE;

  /* Initialize TIMx peripheral as follow:

  + Period = [(TIM6CLK/1000) - 1]. to have a (1/1000) s time base.
  + Prescaler = (uwTimclock/1000000 - 1) to have a 1MHz counter clock.
  + ClockDivision = 0
  + Counter direction = Up
  */
    TIM7_Handle.Init.Prescaler = prescaler;
    TIM7_Handle.Init.Period = autoReload;
    TIM7_Handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    TIM7_Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
    
    TIM7_Handle.Base_MspInitCallback = TIM7_Base_MspInitCallback;

    status = HAL_TIM_Base_Init(&TIM7_Handle);
    
    if (status == HAL_OK)
    {
        /* Start the TIM time Base generation in interrupt mode */
        status = HAL_TIM_Base_Start_IT(&TIM7_Handle);
        
        if (status == HAL_OK)
        {
            /* Enable the TIM7 global Interrupt */
            HAL_NVIC_EnableIRQ(TIM7_DAC_IRQn);
            /* Configure the TIM IRQ priority 
            HAL_NVIC_SetPriority(TIM7_DAC_IRQn, 1UL << __NVIC_PRIO_BITS, 0U);
            */
            HAL_NVIC_SetPriority(TIM7_DAC_IRQn, 5U, 0U);
        }
    }

 /* Return function status */
  return status;
}

void BASIC_TIMER_IRQN_HANDLER(void)
{
    HAL_TIM_IRQHandler(&TIM7_Handle);
}

void PUSH_BUTTON_EXTI_IRQN_HANDLER(void)
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
        if ((StateFlags & 0x01U << 0) == 0x00U)
        {
            StateFlags &= ~(0x01U << 0);
            StateFlags |= 0x01U << 0;
            
            HAL_TIM_Base_Start_IT(&TIM7_Handle);
        }
    }
    if (GPIO_Pin == PUSH_BUTTON_1_GPIO_PIN)
    {
        if ((StateFlags & 0x02U << 1) == 0x00U)
        {
            StateFlags &= ~(0x01U << 1);
            StateFlags |= 0x01U << 1;
            
            HAL_TIM_Base_Start_IT(&TIM7_Handle);
        }
    }
    
    if (GPIO_Pin == PUSH_BUTTON_2_GPIO_PIN)
    {
        if ((StateFlags & 0x02U << 2) == 0x00U)
        {
            StateFlags &= ~(0x01U << 2);
            StateFlags |= 0x01U << 2;
            
            HAL_TIM_Base_Start_IT(&TIM7_Handle);
        }
    }
}

void TimerPeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim -> Instance == BASIC_TIMER_INSTANCE)
    {
        if (GPIO_PIN_RESET == HAL_GPIO_ReadPin(PUSH_BUTTON_GPIO_PORT, PUSH_BUTTON_0_GPIO_PIN))
        {
            StateFlags |= 0x01U << 0;
            HAL_TIM_Base_Stop_IT(&TIM7_Handle);
        }
        
        if (GPIO_PIN_RESET == HAL_GPIO_ReadPin(PUSH_BUTTON_GPIO_PORT, PUSH_BUTTON_1_GPIO_PIN))
        {
            StateFlags |= 0x01U << 1;
            HAL_TIM_Base_Stop_IT(&TIM7_Handle);
        }
        
        if (GPIO_PIN_RESET == HAL_GPIO_ReadPin(PUSH_BUTTON_GPIO_PORT, PUSH_BUTTON_2_GPIO_PIN))
        {
            StateFlags |= 0x01U << 2;
            HAL_TIM_Base_Stop_IT(&TIM7_Handle);
        }
    }
}

KeyPressStates ScanButton(void)
{
    KeyPressStates state = KEY_NONE_PRESSED;
    
    switch (StateFlags)
    {
        case 0x01U:
            state = KEY_0_PRESSED;
        break;
        
        case 0x02U:
            state = KEY_1_PRESSED;
        break;
        
        case 0x04U:
            state = KEY_2_PRESSED;
        break;
        
        default:
            state = KEY_NONE_PRESSED;
        break;
            
    }
    
    StateFlags = 0x00U;
      
    return state;
}