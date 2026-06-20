#include <stm32h5xx.h>

#include "../Includes/LED.h"

/*

LEDs

User green LED (LD1)
The user green LED (LD1) is connected to the PB0 STM32 I/O (SB43 ON and SB51 OFF) or PA5 (SB51 ON and 
SB43 OFF) corresponding to the D13 ST Zio.

User yellow LED (LD2)
The user yellow LED (LD2) is connected to PF4.

User red LED (LD3)
The user red LED (LD3) is connected to PG4.
These user LEDs are ON when the I/O is in the HIGH state, and are OFF when the I/O is in the LOW state.

*/
    
#define ENABLE_GREEN_LED_GPIO_RCC_CLOCK()           do                                  \
                                                    {                                   \
                                                        __HAL_RCC_GPIOB_CLK_ENABLE();   \
                                                    }                                   \
                                                    while (0)                           \

#define GREEN_LED_PORT                              GPIOB
#define GREEN_LED_PIN                               GPIO_PIN_0

#define ENABLE_YELLOW_LED_GPIO_RCC_CLOCK()          do                                  \
                                                    {                                   \
                                                        __HAL_RCC_GPIOF_CLK_ENABLE();   \
                                                    }                                   \
                                                    while (0)                           \

#define YELLOW_LED_PORT                             GPIOF
#define YELLOW_LED_PIN                              GPIO_PIN_4

#define ENABLE_RED_LED_GPIO_RCC_CLOCK()             do                                  \
                                                    {                                   \
                                                        __HAL_RCC_GPIOG_CLK_ENABLE();   \
                                                    }                                   \
                                                    while (0)                           \

#define RED_LED_PORT                                GPIOG
#define RED_LED_PIN                                 GPIO_PIN_4


void InitLEDs (void)
{    
    GPIO_InitTypeDef GPIO_InitType = { 0 };
    
    GPIO_InitType.Mode = GPIO_MODE_OUTPUT_PP;
    
    GPIO_InitType.Pin = GREEN_LED_PIN;
    
    ENABLE_GREEN_LED_GPIO_RCC_CLOCK();
    HAL_GPIO_Init(GREEN_LED_PORT, &GPIO_InitType);
    
    GPIO_InitType.Pin = YELLOW_LED_PIN;
    
    ENABLE_YELLOW_LED_GPIO_RCC_CLOCK();
    HAL_GPIO_Init(YELLOW_LED_PORT, &GPIO_InitType);
    
    GPIO_InitType.Pin = RED_LED_PIN;
    
    ENABLE_RED_LED_GPIO_RCC_CLOCK();
    HAL_GPIO_Init(RED_LED_PORT, &GPIO_InitType);
}

void Toggle (LEDs led)
{
    switch (led)
    {
        case GREEN_LED:
            HAL_GPIO_TogglePin(GREEN_LED_PORT, GREEN_LED_PIN);
        break;
        
        case YELLOW_LED:
            HAL_GPIO_TogglePin(YELLOW_LED_PORT, YELLOW_LED_PIN);
        break;
        
        case RED_LED:
            HAL_GPIO_TogglePin(RED_LED_PORT, RED_LED_PIN);
        break;
        
        default:
            break;
    }
}

