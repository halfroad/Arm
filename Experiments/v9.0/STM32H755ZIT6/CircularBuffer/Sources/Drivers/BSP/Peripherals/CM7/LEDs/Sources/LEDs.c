/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    gpio.c
 * @brief   This file provides code for the configuration
 *          of all used GPIO pins.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include <stm32h7xx.h>

#include "../Include/LEDs.h"

/* USER CODE BEGIN 0 */

#define GREEN_LED_Pin                       GPIO_PIN_0
#define GREEN_LED_GPIO_Port                 GPIOB


#define GREEN_LED_CLOCK_ENABLE()            do                                              \
                                            {                                               \
                                                __HAL_RCC_GPIOB_CLK_ENABLE();               \
                                            }                                               \
                                            while   (0)                                     \


#define YELLOW_LED_Pin                      GPIO_PIN_1
#define YELLOW_LED_GPIO_Port                GPIOE
                                            
#define YELLOW_LED_CLOCK_ENABLE()           do                                              \
                                            {                                               \
                                                __HAL_RCC_GPIOE_CLK_ENABLE();               \
                                            }                                               \
                                            while   (0)                                     \
                                                
#define RED_LED_Pin                         GPIO_PIN_14
#define RED_LED_GPIO_Port                   GPIOB

#define RED_LED_CLOCK_ENABLE()              do                                              \
                                            {                                               \
                                                __HAL_RCC_GPIOB_CLK_ENABLE();               \
                                            }                                               \
                                            while   (0)                                     \

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins
 */
 
void InitLEDs(void)
{

    GPIO_InitTypeDef GPIO_InitStruct = { 0 };

    /*Configure GPIO pin : PtPin */
    GPIO_InitStruct.Pin = GREEN_LED_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    
    /* GPIO Ports Clock Enable */
    GREEN_LED_CLOCK_ENABLE();
    HAL_GPIO_Init(GREEN_LED_GPIO_Port, & GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = YELLOW_LED_Pin;

    /* GPIO Ports Clock Enable */
    YELLOW_LED_CLOCK_ENABLE();
    HAL_GPIO_Init(YELLOW_LED_GPIO_Port, & GPIO_InitStruct);

    GPIO_InitStruct.Pin = RED_LED_Pin;

    /* GPIO Ports Clock Enable */
    RED_LED_CLOCK_ENABLE();
    HAL_GPIO_Init(RED_LED_GPIO_Port, & GPIO_InitStruct);
}

void ToggleLED(LEDs led)
{
    switch (led)
    {
        case GREED_LED:
            
            HAL_GPIO_TogglePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin);
        
            break;
        
        case YELLOW_LED:
            
            HAL_GPIO_TogglePin(YELLOW_LED_GPIO_Port, YELLOW_LED_Pin);
        
            break;
        
        case RED_LED:
            
            HAL_GPIO_TogglePin(RED_LED_GPIO_Port, RED_LED_Pin);
        
            break;
        
        default:
            break;
    }
}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */