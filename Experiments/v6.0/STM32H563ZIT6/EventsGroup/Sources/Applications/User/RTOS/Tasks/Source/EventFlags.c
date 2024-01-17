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
#include "../../../Middlewares/Third_Party/FreeRTOS/Source/include/event_groups.h"
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

#define START_WRITE_EVENTS_GROUP_TASK_STACK_DEPTH       128
#define START_WRITE_EVENTS_GROUP_PRIORITY                 4

#define START_TIMER_TASK_STACK_DEPTH                    128
#define START_TIMER_PRIORITY                              3

#define START_READ_EVENTS_GROUP_TASK_STACK_DEPTH        128
#define START_READ_EVENTS_GROUP_PRIORITY                  2
    
#define EVENT_BIT_PUSH_BUTTON_MASK                      0x01
    
#define EVENT_BIT_PUSH_BUTTON_PRESSED                   EVENT_BIT_PUSH_BUTTON_MASK << 0
#define EVENT_BIT_PUSH_BUTTON_RELEASED                  EVENT_BIT_PUSH_BUTTON_MASK << 1
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* Definitions for StartWriteEventsGroupTaskHandle */
TaskHandle_t StartWriteEventsGroupTaskHandle;

/* Definitions for StartTimerTaskHandle */
TaskHandle_t StartTimerTaskHandle;

/* Definitions for StartReadEventsGroupTaskHandle */
TaskHandle_t StartReadEventsGroupTaskHandle;

EventGroupHandle_t eventsGroupHandle;

/* USER CODE END Variables */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

static void StartWriteEventsGroupTask(void *argument);
static void StartTimerTask(void *argument);
static void StartReadEventsGroupTask(void *argument);

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
    
    eventsGroupHandle = xEventGroupCreate( );
    
    if (!eventsGroupHandle)
        printf("Failed to create EventsGroupHandle.\n");
    
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
    
    /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
    
    /* creation of StartWriteEventsGroupTask */
    xTaskCreate(StartWriteEventsGroupTask, "StartWriteEventsGroupTaskName", START_WRITE_EVENTS_GROUP_TASK_STACK_DEPTH, NULL, START_WRITE_EVENTS_GROUP_PRIORITY, &StartWriteEventsGroupTaskHandle);
    
    /* creation of StartWriteEventsGroupTask */
    xTaskCreate(StartTimerTask, "StartTimerTaskName", START_TIMER_TASK_STACK_DEPTH, NULL, START_TIMER_PRIORITY, &StartTimerTaskHandle);

    /* creation of StartReadEventsGroupTask */
    xTaskCreate(StartReadEventsGroupTask, "StartReadEventsGroupTaskName", START_READ_EVENTS_GROUP_TASK_STACK_DEPTH, NULL, START_READ_EVENTS_GROUP_PRIORITY, &StartReadEventsGroupTaskHandle);

    /* USER CODE END RTOS_THREADS */

    /* USER CODE BEGIN RTOS_EventsS */
    /* add Eventss, ... */
    /* USER CODE END RTOS_EventsS */
    
    vTaskDelete(NULL);
    
    vPortExitCritical();

}
/* USER CODE BEGIN Header_StartWriteEventsGroupTask */
/**
 * @brief Function implementing the StartWriteEventsGroupTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartWriteEventsGroupTask */
void StartWriteEventsGroupTask(void *argument)
{
    /* USER CODE BEGIN StartWriteEventsBitsTask */
    
    EventBits_t eventBits;
    
    /* Infinite loop */
    
    for (;;)
    {
        if (GPIO_PIN_SET == HAL_GPIO_ReadPin(PUSH_BUTTON_GPIO_Port, PUSH_BUTTON_Pin))
        {
            eventBits = xEventGroupSetBits(eventsGroupHandle, EVENT_BIT_PUSH_BUTTON_PRESSED);
            
            printf("The eventBits has read is %#x.\n", eventBits);
        }
        
        vTaskDelay(100);
    }
    /* USER CODE END StartReadEventsGroupTask */
}

/* USER CODE BEGIN StartTimerTask */
/**
 * @brief Function implementing the StartTimerTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartTimerTask */
void StartTimerTask(void *argument)
{
    /* USER CODE BEGIN StartTimerTask */
    
    static uint8_t counter = 0;
    
    /* Infinite loop */
    
    for (;;)
    {
        if (counter == 50)
        {
            xEventGroupSetBits(eventsGroupHandle, EVENT_BIT_PUSH_BUTTON_RELEASED);
            
            counter = 0;
        }
        
        counter ++;
        
        vTaskDelay(100);
    }
    /* USER CODE END StartTimerTask */
}

/* USER CODE BEGIN Header_StartReadEventsGroupTask */
/**
 * @brief Function implementing the StartReadEventsGroupTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartReadEventsGroupTask */
void StartReadEventsGroupTask(void *argument)
{
    /* USER CODE BEGIN StartReadEventsBitsTask */
    
    EventBits_t eventBits;
    
    /* Infinite loop */
    
    for (;;)
    {
        /*
        eventBits = xEventGroupGetBits(eventsGroupHandle);
        */
        eventBits = xEventGroupWaitBits(eventsGroupHandle, EVENT_BIT_PUSH_BUTTON_PRESSED | EVENT_BIT_PUSH_BUTTON_RELEASED, pdTRUE, pdTRUE, portMAX_DELAY);
        
        printf("The eventBits has read is %#x.\n", eventBits);
        
        vTaskDelay(500);
    }
    /* USER CODE END StartReadEventsGroupTask */
}
