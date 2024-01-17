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

#include <stdio.h>

#include "../../../Middlewares/Third_Party/FreeRTOS/Source/include/FreeRTOS.h"
#include "../../../Middlewares/Third_Party/FreeRTOS/Source/include/task.h"
#include "../../RTOS/Include/TasksWrapper.h"
#include "../../User/Periperhals/Include/gpio.h"

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

#define START_FLASH_LEDS_TASK_STACK_DEPTH                   128
/*
#define START_FLASH_LEDS_TASK_STACK_DEPTH                   30
*/
#define START_FLASH_LEDS_TASK_PRIORITY                        2

/*
vTaskList(buffer);
*/
#define TASK_STATES_QUERY_TASK_STACK_DEPTH                  1024
/*
#define TASK_STATES_QUERY_TASK_STACK_DEPTH                  128
*/

#define TASK_STATES_QUERY_TASK_PRIORITY                        3

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* Definitions for StartFlashLEDsTaskHandle */
TaskHandle_t StartFlashLEDsTaskHandle;

/* Definitions for TaskStatesQueryTaskHandle */
TaskHandle_t TaskStatesQueryTaskHandle;

/* USER CODE END Variables */


/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

static void StartFlashLEDsTask(void *argument);
static void StartLTaskStatesQueryTask(void *argument);

/* USER CODE END FunctionPrototypes */

/**
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
    /* creation of StartFlashLEDsTask */
    xTaskCreate(StartFlashLEDsTask, "StartFlashLEDsTaskName", START_FLASH_LEDS_TASK_STACK_DEPTH, NULL, START_FLASH_LEDS_TASK_PRIORITY, &StartFlashLEDsTaskHandle);

    /* creation of StartLTaskStatesQueryTask */
    xTaskCreate(StartLTaskStatesQueryTask, "StartLTaskStatesQueryTaskName", TASK_STATES_QUERY_TASK_STACK_DEPTH, (void *)1, TASK_STATES_QUERY_TASK_PRIORITY, &TaskStatesQueryTaskHandle);

    /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
    /* USER CODE END RTOS_THREADS */

    /* USER CODE BEGIN RTOS_EVENTS */
    /* add events, ... */
    /* USER CODE END RTOS_EVENTS */
    
    vTaskDelete(NULL);
    
    vPortExitCritical();

}
/* USER CODE BEGIN Header_StartFlashLEDsTask */
/**
 * @brief Function implementing the StartFlashLEDsTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartFlashLEDsTask */
void StartFlashLEDsTask(void *argument)
{
    /* USER CODE BEGIN StartFlashLEDsTask */
    /* Infinite loop */
    for (;;)
    {
        HAL_GPIO_TogglePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin);

        vTaskDelay(50);
    }
    /* USER CODE END FlashGreenLEDTask */
}

/* USER CODE BEGIN Header_StartLTaskStatesQueryTask */
/**
 * @brief Function implementing the StartLTaskStatesQueryTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartLTaskStatesQueryTaskk */
void StartLTaskStatesQueryTask(void *argument)
{
    TickType_t previousWakeTime = xTaskGetTickCount();
    TickType_t timeIncrement = 100;
    BaseType_t shouldDelay = pdFALSE;
    
    /* Infinite loop */
    for (;;)
    {
        HAL_GPIO_TogglePin(YELLOW_LED_GPIO_Port, YELLOW_LED_Pin);
        HAL_GPIO_TogglePin(RED_LED_GPIO_Port, RED_LED_Pin);
        
        HAL_Delay(90);
        
        shouldDelay = xTaskDelayUntil( &previousWakeTime,
                                timeIncrement );
        
        printf("shouldDelay = %ld.\n", shouldDelay);
    }
    /* USER CODE END StartListAndListItemsTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
