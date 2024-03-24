#include <stm32g4xx.h>

#include "../Include/LED.h"

#define LED0_RED_GPIO_PORT                                  GPIOE
#define LED0_RED_GPIO_PORT_CLOCK_ENABLE()                   do  {   __HAL_RCC_GPIOE_CLK_ENABLE();   }   while(0)
#define LED0_RED_GPIO_PIN                                   GPIO_PIN_0


#define LED1_GREEN_GPIO_PORT                                GPIOE
#define LED1_GREEN_GPIO_PORT_CLOCK_ENABLE()                 do  {   __HAL_RCC_GPIOE_CLK_ENABLE();   }   while(0)
#define LED1_GREEN_GPIO_PIN                                 GPIO_PIN_1

void InitLEDs(void)
{
    GPIO_InitTypeDef GPIO_InitType = { 0 };
    
    GPIO_InitType.Pin = LED0_RED_GPIO_PIN;
    GPIO_InitType.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitType.Pull = GPIO_PULLUP;
    
    LED0_RED_GPIO_PORT_CLOCK_ENABLE();
    HAL_GPIO_Init(LED0_RED_GPIO_PORT, &GPIO_InitType);
    
    GPIO_InitType.Pin = LED1_GREEN_GPIO_PIN;
    
    LED1_GREEN_GPIO_PORT_CLOCK_ENABLE();
    HAL_GPIO_Init(LED1_GREEN_GPIO_PORT, &GPIO_InitType);
}

void ToggleLED(LightChoices lightChoice)
{
    switch (lightChoice)
    {
        case LightChoiceRedLED:
            HAL_GPIO_TogglePin(LED0_RED_GPIO_PORT, LED0_RED_GPIO_PIN);
        break;
        
        case LightChoiceGreenLED:
            HAL_GPIO_TogglePin(LED1_GREEN_GPIO_PORT, LED1_GREEN_GPIO_PIN);
        break;
    }
   
}