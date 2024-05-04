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

#include "../Include/GPIOs.h"

/* USER CODE BEGIN 0 */

#define BLUE_BUTTON_GPIO_Pin                GPIO_PIN_13
#define BLUE_BUTTON_GPIO_Port               GPIOC

#define RCC_BLUE_BUTTON_CLOCK_ENABLE()      do                                              \
                                            {                                               \
                                                __HAL_RCC_GPIOC_CLK_ENABLE();               \
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
void InitButtons(void)
{

    GPIO_InitTypeDef GPIO_InitStruct    = { 0 };

    /* GPIO Ports Clock Enable */
    RCC_BLUE_BUTTON_CLOCK_ENABLE();

    /*Configure GPIO pin : PtPin */
    GPIO_InitStruct.Pin                 = BLUE_BUTTON_GPIO_Pin;
    GPIO_InitStruct.Mode                = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull                = GPIO_NOPULL;
    GPIO_InitStruct.Speed               = GPIO_SPEED_FREQ_MEDIUM;

    HAL_GPIO_Init(BLUE_BUTTON_GPIO_Port, & GPIO_InitStruct);
}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */