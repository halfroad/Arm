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

#define START_GIVE_REASOURCE_TO_SEMAPHORE_TASK_STACK_DEPTH              128
#define START_GIVE_REASOURCE_TO_SEMAPHORE_TASK_PRIORY                   2

#define START_TAKE_REASOURCE_FROM_SEMAPHORE_TASK_STACK_DEPTH            128
#define START_TAKE_REASOURCE_FROM_SEMAPHORE_TASK_PRIORY                 3

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* Definitions for StartGiveReasourceToSemaphoreTaskHandle */
TaskHandle_t StartGiveReasourceToSemaphoreTaskHandle;

/* Definitions for StartTakeReasourceFromSemaphoreTaskkHandle */
TaskHandle_t StartTakeReasourceFromSemaphoreTaskkHandle;

QueueHandle_t countingSemaphoreHandle;

/* USER CODE END Variables */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

static void StartGiveReasourceToSemaphoreTask(void *argument);
static void StartTakeReasourceFromSemaphoreTask(void *argument);

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
    /* add semaphores, ... 
    xSemaphoreCreateCounting( uxMaxCount, uxInitialCount )    xQueueCreateCountingSemaphore( ( uxMaxCount ), ( uxInitialCount ) )
    */
    countingSemaphoreHandle = xSemaphoreCreateCounting( 100, 0 );
    
    if (!countingSemaphoreHandle)
    {
        printf("Failed to create countingSemaphoreHandle.\n");
    }
    
    /* USER CODE END RTOS_SEMAPHORES */

    /* USER CODE BEGIN RTOS_TIMERS */
    /* start timers, add new ones, ... */
    /* USER CODE END RTOS_TIMERS */

    /* USER CODE BEGIN RTOS_QUEUES */
    /* add queues, ... */
    
    /* USER CODE END RTOS_QUEUES */
    /* creation of StartGiveReasourceToSemaphoreTask */
    xTaskCreate(StartGiveReasourceToSemaphoreTask, "StartGiveReasourceToSemaphoreTaskTaskName", START_GIVE_REASOURCE_TO_SEMAPHORE_TASK_STACK_DEPTH, NULL, START_GIVE_REASOURCE_TO_SEMAPHORE_TASK_PRIORY, &StartGiveReasourceToSemaphoreTaskHandle);

    /* creation of StartTakeReasourceFromSemaphoreTask */
    xTaskCreate(StartTakeReasourceFromSemaphoreTask, "StartTakeReasourceFromSemaphoreTaskName", START_TAKE_REASOURCE_FROM_SEMAPHORE_TASK_STACK_DEPTH, (void *)1, START_TAKE_REASOURCE_FROM_SEMAPHORE_TASK_PRIORY, &StartTakeReasourceFromSemaphoreTaskkHandle);
    
    /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
    /* USER CODE END RTOS_THREADS */

    /* USER CODE BEGIN RTOS_EVENTS */
    /* add events, ... */
    /* USER CODE END RTOS_EVENTS */
    
    vTaskDelete(NULL);
    
    vPortExitCritical();

}
/* USER CODE BEGIN Header_StartGiveReasourceToSemaphoreTask */
/**
 * @brief Function implementing the StartGiveReasourceToSemaphoreTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartGiveReasourceToSemaphoreTask */
void StartGiveReasourceToSemaphoreTask(void *argument)
{
    /* USER CODE BEGIN StartGiveReasourceToSemaphoreTask */
    
    static uint8_t counter = 0;
    static uint8_t numberOfAcquisitions = 0;
    
    /* Infinite loop */
    
    for (;;)
    {
        if (GPIO_PIN_SET == HAL_GPIO_ReadPin(PUSH_BUTTON_GPIO_Port, PUSH_BUTTON_Pin)
            && counter ++ >= 10)
        {
            if (pdPASS == xSemaphoreGive(countingSemaphoreHandle))
            {
                printf("Counting semaphore is released. Number of Acquisitions = %d\n\n", ++ numberOfAcquisitions);
                
                counter = 0;
                
            }
        }
        
        vTaskDelay(10);
    }
    /* USER CODE END StartGiveReasourceToSemaphoreTask */
}

/* USER CODE BEGIN Header_StartTakeReasourceFromSemaphoreTask */
/**
 * @brief Function implementing the StartTakeReasourceFromSemaphoreTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartTakeReasourceFromSemaphoreTask */
void StartTakeReasourceFromSemaphoreTask(void *argument)
{
    UBaseType_t availableResources;
    
    for (;;)
    {
        if (pdPASS == xSemaphoreTake(countingSemaphoreHandle, portMAX_DELAY))
        {
            printf("Counting Semaphore is acquired.\n");
            
            /*
            uxSemaphoreGetCount( xSemaphore )           uxQueueMessagesWaiting( ( QueueHandle_t ) ( xSemaphore ) )
            */
            availableResources = uxSemaphoreGetCount(countingSemaphoreHandle);
            
            printf("Available Resources on countingSemaphoreHandle is %ld.\n", availableResources);
        }
        
        vTaskDelay(1000);
    }
    /* USER CODE END StartTakeReasourceFromSemaphoreTask */
}

