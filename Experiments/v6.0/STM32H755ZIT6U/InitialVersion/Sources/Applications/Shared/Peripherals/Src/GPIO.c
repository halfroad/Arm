#include <stm32h7xx.h>

#include "GPIO.h"

void InitLEDs()
{
    /* Enable the RCC clock for Green and Yellow LEDs. */
    __HAL_RCC_GPIOB_CLK_ENABLE();
    
    GPIO_InitTypeDef GPIO_InitType;
    
    GPIO_InitType.Pin = GREEN_LED_GPIO_PIN | YELLOW_LED_GPIO_PIN;
    GPIO_InitType.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitType.Pull = GPIO_NOPULL;
    GPIO_InitType.Speed = GPIO_SPEED_FREQ_HIGH;
    
    HAL_GPIO_Init(GREEN_LED_GPIO_PORT, &GPIO_InitType);
    
    /* Enable the RCC clock for Red LED. */
    __HAL_RCC_GPIOE_CLK_ENABLE();
    
    GPIO_InitType.Pin = RED_LED_GPIO_PIN;
    
    HAL_GPIO_Init(RED_LED_GPIO_PORT, &GPIO_InitType);
}