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
#define START_FLASH_LEDS_TASK_PRIORY                        2

/*
vTaskList(buffer);
*/
#define TASK_STATES_QUERY_TASK_STACK_DEPTH                  500
/*
#define TASK_STATES_QUERY_TASK_STACK_DEPTH                  128
*/

#define TASK_STATES_QUERY_TASK_PRIORY                        3

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
    xTaskCreate(StartFlashLEDsTask, "StartFlashLEDsTaskName", START_FLASH_LEDS_TASK_STACK_DEPTH, NULL, START_FLASH_LEDS_TASK_PRIORY, &StartFlashLEDsTaskHandle);

    /* creation of StartLTaskStatesQueryTask */
    xTaskCreate(StartLTaskStatesQueryTask, "StartLTaskStatesQueryTaskName", TASK_STATES_QUERY_TASK_STACK_DEPTH, (void *)1, TASK_STATES_QUERY_TASK_PRIORY, &TaskStatesQueryTaskHandle);

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
        HAL_GPIO_TogglePin(YELLOW_LED_GPIO_Port, YELLOW_LED_Pin);
        HAL_GPIO_TogglePin(RED_LED_GPIO_Port, RED_LED_Pin);

        vTaskDelay(500);
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
    /* USER CODE BEGIN StartLTaskStatesQueryTask */
    UBaseType_t StartFlashLEDsTaskPriority = uxTaskPriorityGet( StartFlashLEDsTaskHandle );
    printf("StartFlashLEDsTaskPriority is %ld\n", StartFlashLEDsTaskPriority);
    
    printf("\n");
    
    vTaskPrioritySet(StartFlashLEDsTaskHandle, 3);
    StartFlashLEDsTaskPriority = uxTaskPriorityGet( StartFlashLEDsTaskHandle );
    printf("StartFlashLEDsTaskPriority is %ld\n", StartFlashLEDsTaskPriority);
    
    printf("\n");
    
    UBaseType_t numberOfTasks = uxTaskGetNumberOfTasks( );
    printf("numberOfTasks is %ld\n", numberOfTasks);
    
    printf("\n");
    
    TaskStatus_t *TaskStatuses = pvPortMalloc(sizeof(TaskStatus_t) * numberOfTasks);
    configRUN_TIME_COUNTER_TYPE pulTotalRunTime;
    
    numberOfTasks = uxTaskGetSystemState( TaskStatuses,
                                      numberOfTasks,
                                      &pulTotalRunTime );
    
    printf("numberOfTasks = %ld, pulTotalRunTime = %d\n ", numberOfTasks, pulTotalRunTime);
    
    printf("\n");
    
    for (uint8_t i = 0; i < numberOfTasks; i ++)
        printf("Name = %s, Piority = %ld, Number = %ld\n", TaskStatuses[i].pcTaskName, TaskStatuses[i].uxCurrentPriority, TaskStatuses[i].xTaskNumber);
    
    TaskStatus_t taskStatus;
    
    vTaskGetInfo( StartFlashLEDsTaskHandle,
                       &taskStatus,
                       pdTRUE,
                       eInvalid );
                       
    printf("Name = %s, Piority = %ld, Number = %ld, eCurrentState = %d, StackHighWaterMark = %d \n", taskStatus.pcTaskName, taskStatus.uxCurrentPriority, taskStatus.xTaskNumber, taskStatus.eCurrentState, taskStatus.usStackHighWaterMark);
    
    printf("\n");
    
    TaskHandle_t TaskHandle = xTaskGetHandle("StartFlashLEDsTaskName");
    
    if (TaskHandle)
    {
        printf("TaskHandle = %#x.\n", (int)TaskHandle);
        printf("TaskHandle = 0x%p.\n", &TaskHandle);
        
        printf("StartFlashLEDsTaskHandle = 0x%p.\n", &StartFlashLEDsTaskHandle);
    }
    
    printf("\n");
    
    UBaseType_t StackHighWaterMark = uxTaskGetStackHighWaterMark( TaskHandle );
    printf("StackHighWaterMark = %ld.\n", StackHighWaterMark);
    
    printf("\n");
    
    eTaskState taskState = eTaskGetState( TaskHandle );
    printf("taskState = %d\n", taskState);
    
    printf("\n");
    
    char buffer[500];
    
    vTaskList(buffer);

    printf("buffer = %s.\n", buffer);
    
    /* Infinite loop */
    for (;;)
    {
        vTaskDelay(500);
    }
    /* USER CODE END StartListAndListItemsTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
