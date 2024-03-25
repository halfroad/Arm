#include <stm32g4xx.h>

#include "../Include/LED.h"

void InitLEDs()
{
    __HAL_RCC_GPIOE_CLK_ENABLE();
    
    GPIO_InitTypeDef GPIO_InitType = { 0 };
    
    GPIO_InitType.Pin = GPIO_PIN_0 | GPIO_PIN_1;
    GPIO_InitType.Mode = GPIO_MODE_OUTPUT_PP;
    
    HAL_GPIO_Init(GPIOE, &GPIO_InitType);
}
