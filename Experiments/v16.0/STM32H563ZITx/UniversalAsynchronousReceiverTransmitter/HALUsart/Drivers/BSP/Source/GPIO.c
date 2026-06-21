#include <stm32h5xx.h>

#include "../Include/GPIO.h"


#define ENABLE_GREEN_LED_GPIO_RCC_CLOCK()               do                                  \
                                                        {                                   \
                                                            __HAL_RCC_GPIOB_CLK_ENABLE();   \
                                                        }                                   \
                                                        while (0)                           \

                                            
#define GREEN_LED_GPIO_PORT                             GPIOB
#define GREEN_LED_GPIO_PIN                              GPIO_PIN_0

#define ENABLE_YELLOW_LED_GPIO_RCC_CLOCK()              do                                  \
                                                        {                                   \
                                                            __HAL_RCC_GPIOF_CLK_ENABLE();   \
                                                        }                                   \
                                                        while (0)                           \

                                            
#define YELLOW_LED_GPIO_PORT                            GPIOF
#define YELLOW_LED_GPIO_PIN                             GPIO_PIN_4

#define ENABLE_RED_LED_GPIO_RCC_CLOCK()                 do                                  \
                                                        {                                   \
                                                            __HAL_RCC_GPIOG_CLK_ENABLE();   \
                                                        }                                   \
                                                        while (0)                           \

                                            
#define RED_LED_GPIO_PORT                               GPIOG
#define RED_LED_GPIO_PIN                                GPIO_PIN_4


void InitializeGeneralPurposeInputOutput(void)
{
    GPIO_InitTypeDef GPIO_InitType = { 0 };
    
    ENABLE_GREEN_LED_GPIO_RCC_CLOCK ();
    
    GPIO_InitType.Pin = GREEN_LED_GPIO_PIN;
    GPIO_InitType.Mode = GPIO_MODE_OUTPUT_PP;
    
    HAL_GPIO_Init(GREEN_LED_GPIO_PORT, &GPIO_InitType);
    
    ENABLE_YELLOW_LED_GPIO_RCC_CLOCK();
    
    GPIO_InitType.Pin = YELLOW_LED_GPIO_PIN;
    
    HAL_GPIO_Init(YELLOW_LED_GPIO_PORT, &GPIO_InitType);
    
    ENABLE_RED_LED_GPIO_RCC_CLOCK();
    
    GPIO_InitType.Pin = RED_LED_GPIO_PIN;
    
    HAL_GPIO_Init(RED_LED_GPIO_PORT, &GPIO_InitType);
}

void Toggle (BUILT_IN_LED led)
{
    switch (led)
    {
        case LED_1_GREEN:
            HAL_GPIO_TogglePin (GREEN_LED_GPIO_PORT, GREEN_LED_GPIO_PIN);
        break;
        
        case LED_2_YELLOW:
            HAL_GPIO_TogglePin (YELLOW_LED_GPIO_PORT, YELLOW_LED_GPIO_PIN);
        break;
        
        case LED_3_RED:
            HAL_GPIO_TogglePin (RED_LED_GPIO_PORT, RED_LED_GPIO_PIN);
        break;
    }
}