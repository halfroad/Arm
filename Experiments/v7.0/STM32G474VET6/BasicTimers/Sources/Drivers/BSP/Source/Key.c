#include "../Include/Key.h"

#define PUSH_BUTTON_GPIO_PORT           GPIOE

#define PUSH_BUTTON_0_GPIO_PIN_PORT     GPIO_PIN_12
#define PUSH_BUTTON_1_GPIO_PIN_PORT     GPIO_PIN_13
#define PUSH_BUTTON_2_GPIO_PIN_PORT     GPIO_PIN_14

void InitPushButtons(void)
{
    __HAL_RCC_GPIOE_CLK_ENABLE();
    
    GPIO_InitTypeDef GPIO_InitType = { 0 };
    
    GPIO_InitType.Pin = PUSH_BUTTON_0_GPIO_PIN_PORT | PUSH_BUTTON_1_GPIO_PIN_PORT | PUSH_BUTTON_2_GPIO_PIN_PORT;
    GPIO_InitType.Mode = GPIO_MODE_INPUT;
    GPIO_InitType.Pull = GPIO_PULLUP;
    GPIO_InitType.Speed = GPIO_SPEED_HIGH;
    
    HAL_GPIO_Init(PUSH_BUTTON_GPIO_PORT, &GPIO_InitType);
    
    HAL_NVIC_SetPriority(EXTI0_IRQn | EXTI1_IRQn | EXTI2_IRQn, 10, 0);
    HAL_NVIC_EnableIRQ(EXTI0_IRQn | EXTI1_IRQn | EXTI2_IRQn);
    
    TIM_Base_InitTypeDef TIM_Base_InitType =  { 0 };
    
//    TIM_Base_InitType.Prescaler = __HAL_TIM_CALC_PSC(SystemCoreClock, 1000);
//    TIM_Base_InitType.CounterMode = TIM_COUNTERMODE_UP;
//    TIM_Base_InitType.Period = __HAL_TIM_CALC_PERIOD(
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
//    switch (GPIO_Pin)
//    {
//        case PUSH_BUTTON_0_GPIO_PIN_PORT:
//            
//    }
    
    /*
    
    https://blog.csdn.net/m0_51274562/article/details/127475527
    
    */
    
    uint8_t i = 0;
}

static GPIO_PinState ReadPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    HAL_Delay(50);
    
    return HAL_GPIO_ReadPin(GPIOx, GPIO_Pin);
}

PressedPushButtons KeyScan(void)
{   
    if (GPIO_PIN_RESET == ReadPin(PUSH_BUTTON_GPIO_PORT, PUSH_BUTTON_0_GPIO_PIN_PORT))
        return PUSH_BUTTON_PRESSED_0;
    else if (GPIO_PIN_RESET == ReadPin(PUSH_BUTTON_GPIO_PORT, PUSH_BUTTON_1_GPIO_PIN_PORT))
        return PUSH_BUTTON_PRESSED_1;
    else if (GPIO_PIN_RESET == ReadPin(PUSH_BUTTON_GPIO_PORT, PUSH_BUTTON_2_GPIO_PIN_PORT))
        return PUSH_BUTTON_PRESSED_2;
    else
        return PUSH_BUTTON_PRESSED_NONE;
}