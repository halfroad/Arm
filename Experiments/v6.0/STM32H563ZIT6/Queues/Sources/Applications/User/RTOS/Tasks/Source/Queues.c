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
#include "../../../Middlewares/Third_Party/FreeRTOS/Source/include/queue.h"
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

#define START_ENQUEUE_TASK_STACK_DEPTH                      128
#define START_ENQUEUE_TASK_PRIORITY                           2

#define START_DEQUEUE_TASK_STACK_DEPTH                      1024
#define START_DEQUEUE_TASK_PRIORITY                           3

#define START_DEQUEUE_ADDRESS_TASK_STACK_DEPTH              128
#define START_DEQUEUE_ADDRESS_TASK_PRIORITY                   4

#define NUMBERS_QUEUE_LENGTH                                2
#define POINTERS_QUEUE_LENGTH                               1

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* Definitions for startEnqueueTaskHandle */
TaskHandle_t startEnqueueTaskHandle;

/* Definitions for startDequeueTaskHandle */
TaskHandle_t startDequeueTaskHandle;

/* Definitions for startDequeueAddressTaskHandle */
TaskHandle_t startDequeueAddressTaskHandle;

QueueHandle_t numbersQueueHandle;
QueueHandle_t pointersQueueHandle;

/* USER CODE END Variables */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

static void StartEnqueueTask(void *argument);
static void StartDequeueTask(void *argument);
static void StartDequeueAddressTask(void *argument);

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
    
    numbersQueueHandle = xQueueCreate(NUMBERS_QUEUE_LENGTH, sizeof(uint8_t));
    
    if (!numbersQueueHandle)
        printf("Failed to create numbersQueueHandle");
    
    pointersQueueHandle = xQueueCreate(POINTERS_QUEUE_LENGTH, sizeof(char *));
    
    if (!pointersQueueHandle)
        printf("Failed to create pointersQueueHandle");
    
    /* USER CODE END RTOS_QUEUES */
    /* creation of StartEnqueueTask */
    xTaskCreate(StartEnqueueTask, "StartEnqueueTaskName", START_ENQUEUE_TASK_STACK_DEPTH, NULL, START_ENQUEUE_TASK_PRIORITY, &startEnqueueTaskHandle);

    /* creation of StartDequeueTask */
    xTaskCreate(StartDequeueTask, "StartDequeueTaskName", START_DEQUEUE_TASK_STACK_DEPTH, (void *)1, START_DEQUEUE_TASK_PRIORITY, &startDequeueTaskHandle);
    
    /* creation of StartDequeueAddressTask */
    xTaskCreate(StartDequeueAddressTask, "StartDequeueAddressTaskName", START_DEQUEUE_ADDRESS_TASK_STACK_DEPTH, (void *)1, START_DEQUEUE_ADDRESS_TASK_PRIORITY, &startDequeueAddressTaskHandle);

    /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
    /* USER CODE END RTOS_THREADS */

    /* USER CODE BEGIN RTOS_EVENTS */
    /* add events, ... */
    /* USER CODE END RTOS_EVENTS */
    
    vTaskDelete(NULL);
    
    vPortExitCritical();

}
/* USER CODE BEGIN Header_StartEnqueueTask */
/**
 * @brief Function implementing the StartEnqueueTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartEnqueueTask */
void StartEnqueueTask(void *argument)
{
    /* USER CODE BEGIN StartEnqueueTask */
    /* Infinite loop */
    
    static int8_t counter = 1;
    
    for (;;)
    {
        GPIO_PinState pinState = HAL_GPIO_ReadPin(PUSH_BUTTON_GPIO_Port, PUSH_BUTTON_Pin);
        
        if (GPIO_PIN_SET == pinState)
        {
            /*
            if (pdTRUE != xQueueSend( numbersQueueHandle, &counter, portMAX_DELAY ))
            {
                printf("Faild to send the item to numbersQueueHandle.\n");
            }
            else
            {
                counter ++;
            }
            */
            
            char buffer[130] = {"This is large array, little bit large. Please be cautious about the scale of array, and make sure the array is not overflowed."};
            char *bufferPointer = &buffer[0];
                
            static uint16_t counter = 0;
            
            if (counter ++ >= 500)
            {
                if (pdTRUE != xQueueSend(pointersQueueHandle, &bufferPointer, portMAX_DELAY))
                {
                    printf("Faild to send the item to pointersQueueHandle.\n");
                }
            }
        }
        
        vTaskDelay(10);
    }
    /* USER CODE END FlashGreenLEDTask */
}

/* USER CODE BEGIN Header_StartDequeueTask */
/**
 * @brief Function implementing the StartDequeueTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDequeueTask */
void StartDequeueTask(void *argument)
{
    uint8_t number;
    
    for (;;)
    {
        if (pdTRUE == xQueueReceive( numbersQueueHandle,
                          &number,
                          portMAX_DELAY ))
            printf("Succeeded to dequeue number %d from numbersQueueHandle.\n", number);
        else
            printf("Failed to dequeue number from numbersQueueHandle.\n");
        
        vTaskDelay(10);
    }
    /* USER CODE END StartDequeueTask */
}

/* USER CODE BEGIN Header_StartDequeueAddressTask */
/**
 * @brief Function implementing the StartDequeueAddressTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDequeueAddressTask */
void StartDequeueAddressTask(void *argument)
{
    char *buffer;
    
    /* Infinite loop */
    for (;;)
    {
        if (pdTRUE == xQueueReceive( pointersQueueHandle,
                          &buffer,
                          portMAX_DELAY ))
            printf("Succeeded to dequeue number %s from pointersQueueHandle.\n", buffer);
        else
            printf("Failed to dequeue buffer from pointersQueueHandle.\n");
        
        vTaskDelay(10);
    }
    /* USER CODE END StartDequeueAddressTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

