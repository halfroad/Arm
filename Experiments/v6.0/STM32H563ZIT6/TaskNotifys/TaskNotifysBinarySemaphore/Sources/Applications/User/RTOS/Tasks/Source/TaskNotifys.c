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

#define START_NOTITY_GIVE_TASK_STACK_DEPTH          128
#define START_NOTITY_GIVE_PRIORITY                  4

#define START_NOTITY_TAKE_TASK_STACK_DEPTH          128
#define START_NOTITY_TAKE_PRIORITY                  2
    
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* Definitions for StartNotifyGiveTaskHandle */
TaskHandle_t StartNotifyGiveTaskHandle;

/* Definitions for StartNotifyTakeTaskHandle */
TaskHandle_t StartNotifyTakeTaskHandle;


/* USER CODE END Variables */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

static void StartNotifyGiveTask(void *argument);
static void StartNotifyTakeTask(void *argument);

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
    
    /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
    
    /* creation of StartNotifyGiveTask */
    xTaskCreate(StartNotifyGiveTask, "StartNotifyGiveTaskName", START_NOTITY_GIVE_TASK_STACK_DEPTH, NULL, START_NOTITY_GIVE_PRIORITY, &StartNotifyGiveTaskHandle);
    
    /* creation of StartNotifyTakeTask */
    xTaskCreate(StartNotifyTakeTask, "StartNotifyTakeTaskName", START_NOTITY_TAKE_TASK_STACK_DEPTH, NULL, START_NOTITY_TAKE_PRIORITY, &StartNotifyTakeTaskHandle);

    /* USER CODE END RTOS_THREADS */

    /* USER CODE BEGIN RTOS_EventsS */
    /* add Eventss, ... */
    /* USER CODE END RTOS_EventsS */
    
    vTaskDelete(NULL);
    
    vPortExitCritical();

}
/* USER CODE BEGIN Header_StartNotifyGiveTask */
/**
 * @brief Function implementing the StartNotifyGiveTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartNotifyGiveTask */
void StartNotifyGiveTask(void *argument)
{
    /* USER CODE BEGIN StartWriteEventsBitsTask */
    
    /* Infinite loop */
    
    for (;;)
    {
        if (GPIO_PIN_SET == HAL_GPIO_ReadPin(PUSH_BUTTON_GPIO_Port, PUSH_BUTTON_Pin))
        {
            HAL_Delay(0);
            
            if (GPIO_PIN_SET == HAL_GPIO_ReadPin(PUSH_BUTTON_GPIO_Port, PUSH_BUTTON_Pin))
            {
                while (GPIO_PIN_SET == HAL_GPIO_ReadPin(PUSH_BUTTON_GPIO_Port, PUSH_BUTTON_Pin))
                    ;
            }
            /*
            
            #define xTaskNotifyGive( xTaskToNotify ) \
    xTaskGenericNotify( ( xTaskToNotify ), ( tskDEFAULT_INDEX_TO_NOTIFY ), ( 0 ), eIncrement, NULL )
            
            */
            xTaskNotifyGive(StartNotifyTakeTaskHandle);
        }
        
        vTaskDelay(10);
    }
    /* USER CODE END StartNotifyTakeTask */
}

/* USER CODE BEGIN Header_StartNotifyTakeTask */
/**
 * @brief Function implementing the StartNotifyTakeTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartNotifyTakeTask */
void StartNotifyTakeTask(void *argument)
{
    /* USER CODE BEGIN StartNotifyTakeTask */
    
    /* Infinite loop */
    
    for (;;)
    {
        uint32_t notifys = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        
        if (notifys)
            printf("Number of notifys received: %d.\n\n", notifys);
    }
    /* USER CODE END StartNotifyTakeTask */
}
