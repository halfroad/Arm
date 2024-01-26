/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : app_freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
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
#include "../../../Middlewares/Third_Party/FreeRTOS/Source/include/FreeRTOS.h"
#include "../../../Middlewares/Third_Party/FreeRTOS/Source/include/task.h"

#include "../../Peripherals/Include/gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

#define START_FLASH_RED_LED_TASK_STACK_DEPTH        128
#define START_FLASH_RED_LED_TASK_PRIORTY            3

#define START_FLASH_GREEN_LED_TASK_STACK_DEPTH      128
#define START_FLASH_GREEN_LED_TASK_PRIORTY          4

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

TaskHandle_t StartFlashRedLedTaskHandle;
TaskHandle_t StartFlashGreenLedTaskHandle;

/* USER CODE END Variables */
/* Definitions for FlashRedLedTask */

/* Definitions for FlashGreenLedTask */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

void StartFlashRedLedTask(void *argument);
void StartFlashGreenLedTask(void *argument);

/* USER CODE END FunctionPrototypes */

/*
void ScheduleTasks(void);  (MISRA C 2004 rule 8.1) */

/*
 * @brief  FreeRTOS initialization
 * @param  None
 * @retval None
 */
void ScheduleTasks(void)
{
    /* USER CODE BEGIN Init */
    
    vPortEnterCritical();
    
    /* USER CODE END Init */

    /* USER CODE BEGIN RTOS_MUTEX */
    /* add mutexes, ... */
    /* USER CODE END RTOS_MUTEX */

    /* USER CODE BEGIN RTOS_SEMAPHORES */
    /* add semaphores, ... */
    /* USER CODE END RTOS_SEMAPHORES */

    /* USER CODE BEGIN RTOS_TIMERS */
    /* start timers, add new ones, ... */
    /* USER CODE END RTOS_TIMERS */

    /* USER CODE BEGIN RTOS_QUEUES */
    /* add queues, ... */
    /* USER CODE END RTOS_QUEUES */

    /* Create the thread(s) */
    /* creation of FlashRedLedTask */
    
    xTaskCreate(StartFlashRedLedTask, "StartFlashRedLedTaskName", START_FLASH_RED_LED_TASK_STACK_DEPTH, NULL, START_FLASH_RED_LED_TASK_PRIORTY, &StartFlashRedLedTaskHandle);

    /* creation of FlashGreenLedTask */
    xTaskCreate(StartFlashGreenLedTask, "StartFlashGreenLedTaskName", START_FLASH_GREEN_LED_TASK_STACK_DEPTH, (void *)1, START_FLASH_GREEN_LED_TASK_PRIORTY, &StartFlashGreenLedTaskHandle);
    
    vTaskDelete(NULL);
    /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
    /* USER CODE END RTOS_THREADS */

    /* USER CODE BEGIN RTOS_EVENTS */
    /* add events, ... */
    /* USER CODE END RTOS_EVENTS */
    
    vPortExitCritical();
}

/* USER CODE BEGIN Header_StartFlashRedLedTask */
/**
 * @brief  Function implementing the FlashRedLedTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartFlashRedLedTask */
void StartFlashRedLedTask(void *argument)
{
    /* USER CODE BEGIN StartFlashRedLedTask */
    /* Infinite loop */
    for (;;)
    {
        HAL_GPIO_TogglePin(RED_LED_GPIO_Port, RED_LED_Pin);

        vTaskDelay(500);
    }
    /* USER CODE END StartFlashRedLedTask */
}

/* USER CODE BEGIN Header_StartFlashGreenLedTask */
/**
 * @brief Function implementing the FlashGreenLedTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartFlashGreenLedTask */
void StartFlashGreenLedTask(void *argument)
{
    /* USER CODE BEGIN StartFlashGreenLedTask */
    /* Infinite loop */
    for (;;)
    {
        HAL_GPIO_TogglePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin);

        vTaskDelay(500);
    }
    /* USER CODE END StartFlashGreenLedTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

