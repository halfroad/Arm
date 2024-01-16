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
#include "../../../Middlewares/Third_Party/FreeRTOS/Source/include/semphr.h"
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

#define START_HIGH_PRIORTY_TASK_STACK_DEPTH         128
#define START_HIGH_PRIORTY_TASK_PRIORY              4

#define START_MEDIUM_PRIORTY_TASK_STACK_DEPTH       128
#define START_MEDIUM_PRIORTY_TASK_PRIORY            3

#define START_LOW_PRIORTY_TASK_STACK_DEPTH          128
#define START_LOW_PRIORTY_TASK_PRIORY               2

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* Definitions for highPriorityTaskHandle */
TaskHandle_t highPriorityTaskHandle;

/* Definitions for mediumPriorityTaskHandle */
TaskHandle_t mediumPriorityTaskHandle;

/* Definitions for lowPriorityTaskHandle */
TaskHandle_t lowPriorityTaskHandle;

QueueHandle_t binarySemaphore;

/* USER CODE END Variables */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

static void StartHighPriorityTask(void *argument);
static void StartMediumPriorityTask(void *argument);
static void StartLowPriorityTask(void *argument);

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
    binarySemaphore = xSemaphoreCreateBinary();
    
    if (binarySemaphore)
        xSemaphoreGive(binarySemaphore);
    else
        printf("Failed to create binarySemaphore.\n");
    
    /* USER CODE END RTOS_SEMAPHORES */

    /* USER CODE BEGIN RTOS_TIMERS */
    /* start timers, add new ones, ... */
    /* USER CODE END RTOS_TIMERS */

    /* USER CODE BEGIN RTOS_QUEUES */
    /* add queues, ... */
    
    /* USER CODE END RTOS_QUEUES */
    /* creation of StartHighPriorityTask */
    xTaskCreate(StartHighPriorityTask, "StartHighPriorityTaskName", START_HIGH_PRIORTY_TASK_STACK_DEPTH, NULL, START_HIGH_PRIORTY_TASK_PRIORY, &highPriorityTaskHandle);

    /* creation of StartMediumPriorityTask */
    xTaskCreate(StartMediumPriorityTask, "StartMediumPriorityTaskName", START_MEDIUM_PRIORTY_TASK_STACK_DEPTH, NULL, START_MEDIUM_PRIORTY_TASK_PRIORY, &mediumPriorityTaskHandle);

    /* creation of StartLowPriorityTask */
    xTaskCreate(StartLowPriorityTask, "StartLowPriorityTaskName", START_LOW_PRIORTY_TASK_STACK_DEPTH, NULL, START_LOW_PRIORTY_TASK_PRIORY, &lowPriorityTaskHandle);
    
    /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
    /* USER CODE END RTOS_THREADS */

    /* USER CODE BEGIN RTOS_EVENTS */
    /* add events, ... */
    /* USER CODE END RTOS_EVENTS */
    
    vTaskDelete(NULL);
    
    vPortExitCritical();

}
/* USER CODE BEGIN Header_StartHighPriorityTask */
/**
 * @brief Function implementing the StartHighPriorityTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartHighPriorityTask */
void StartHighPriorityTask(void *argument)
{
    /* USER CODE BEGIN StartHighPriorityTask */
    
    /* Infinite loop */
    
    for (;;)
    {
        printf("High Priority Task is attmpting to acquire the binary semaphore.\n");
        
        if (pdPASS == xSemaphoreTake(binarySemaphore, portMAX_DELAY))
        {
            printf("High Priority Task acquired the binary semaphore.\n");
            printf("High Priority Task delays with HAL_Delay(1000) for 1 seconds.\n");
            
            HAL_Delay(1000);
            
            if (pdPASS == xSemaphoreGive(binarySemaphore))
            {
                printf("High Priority Task released the binary semaphore.\n\n");
                
                vTaskDelay(1000);
            }
        }
    }
    /* USER CODE END StartHighPriorityTask */
}

/* USER CODE BEGIN Header_StartMediumPriorityTask */
/**
 * @brief Function implementing the StartMediumPriorityTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartMediumPriorityTask */
void StartMediumPriorityTask(void *argument)
{    
    for (;;)
    {
        printf("Medium Priority Task is running.\n\n");
        
        vTaskDelay(1000);
    }
    /* USER CODE END StartMediumPriorityTask */
}

/* USER CODE BEGIN Header_StartLowPriorityTask */
/**
 * @brief Function implementing the StartLowPriorityTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartLowPriorityTask */
void StartLowPriorityTask(void *argument)
{    
    for (;;)
    {
        printf("Low Priority Task is attmpting to acquire the binary semaphore.\n");
        
        if (pdPASS == xSemaphoreTake(binarySemaphore, portMAX_DELAY))
        {
            printf("Low Priority Task acquired the binary semaphore.\n");
            printf("High Priority Task delays with HAL_Delay(3000) for 3 seconds.\n");
            
            HAL_Delay(3000);
            
            if (pdPASS == xSemaphoreGive(binarySemaphore))
            {
                printf("Low Priority Task released the binary semaphore.\n\n");
                
                vTaskDelay(1000);
            }
        }
    }
    /* USER CODE END StartLowPriorityTask */
}

