#include "../Include/Key.h"

#define PUSH_BUTTON_GPIO_PORT           GPIOE

#define PUSH_BUTTON_0_GPIO_PIN_PORT     GPIO_PIN_12
#define PUSH_BUTTON_1_GPIO_PIN_PORT     GPIO_PIN_13
#define PUSH_BUTTON_2_GPIO_PIN_PORT     GPIO_PIN_14

PressedPushButtons pressedPushButton = PUSH_BUTTON_PRESSED_NONE;

void InitPushButtons(void)
{
    __HAL_RCC_GPIOE_CLK_ENABLE();
    
    GPIO_InitTypeDef GPIO_InitType = { 0 };
    
    GPIO_InitType.Pin = PUSH_BUTTON_0_GPIO_PIN_PORT | PUSH_BUTTON_1_GPIO_PIN_PORT | PUSH_BUTTON_2_GPIO_PIN_PORT;
    GPIO_InitType.Mode = GPIO_MODE_INPUT;
    GPIO_InitType.Pull = GPIO_PULLUP;
    GPIO_InitType.Speed = GPIO_SPEED_HIGH;
    
    HAL_GPIO_Init(PUSH_BUTTON_GPIO_PORT, &GPIO_InitType);
    
    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 10, 0);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
    
    TIM_Base_InitTypeDef TIM_Base_InitType =  { 0 };
    
//    TIM_Base_InitType.Prescaler = __HAL_TIM_CALC_PSC(SystemCoreClock, 1000);
//    TIM_Base_InitType.CounterMode = TIM_COUNTERMODE_UP;
//    TIM_Base_InitType.Period = __HAL_TIM_CALC_PERIOD(
}

void EXTI15_10_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(EXTI15_10_IRQn);
}

/**
  * @brief  EXTI line detection callback.
  * @param  GPIO_Pin: Specifies the port pin connected to corresponding EXTI line.
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    switch (GPIO_Pin)
    {
        case GPIO_PIN_12:
            pressedPushButton = PUSH_BUTTON_PRESSED_0;
        
        case GPIO_PIN_13:
            pressedPushButton = PUSH_BUTTON_PRESSED_1;
        
        case GPIO_PIN_14:
            pressedPushButton = PUSH_BUTTON_PRESSED_2;
        
        default:
            pressedPushButton = PUSH_BUTTON_PRESSED_NONE;
    }
}


PressedPushButtons KeyScan(void)
{
    return pressedPushButton;
}