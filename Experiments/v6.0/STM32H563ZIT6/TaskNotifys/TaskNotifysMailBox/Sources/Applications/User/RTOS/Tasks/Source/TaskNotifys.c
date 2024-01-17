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

#define START_TIMER_TASK_STACK_DEPTH                128
#define START_TIMER_TASK_PRIORITY                   3

#define START_NOTITY_TAKE_TASK_STACK_DEPTH          128
#define START_NOTITY_TAKE_PRIORITY                  2
    
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* Definitions for StartNotifyGiveTaskHandle */
TaskHandle_t StartNotifyGiveTaskHandle;

/* Definitions for StartTimerTaskHandle */
TaskHandle_t StartTimerTaskHandle;

/* Definitions for StartNotifyTakeTaskHandle */
TaskHandle_t StartNotifyTakeTaskHandle;

typedef enum
{
    NotificationValueFromPushButton = 10,
    NotificationValueFromTimer      = 50,
    
} eNotificationValues;


/* USER CODE END Variables */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

static void StartNotifyGiveTask(void *argument);
static void StartTimerTask(void *argument);
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
    
    /* creation of StartTimerTask */
    xTaskCreate(StartTimerTask, "StartTimerTaskName", START_TIMER_TASK_STACK_DEPTH, NULL, START_TIMER_TASK_PRIORITY, &StartTimerTaskHandle);
   
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
            HAL_GPIO_TogglePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin);
            HAL_GPIO_TogglePin(RED_LED_GPIO_Port, RED_LED_Pin);
            HAL_GPIO_TogglePin(YELLOW_LED_GPIO_Port, YELLOW_LED_Pin);
            
            HAL_Delay(10);
            
            if (GPIO_PIN_SET == HAL_GPIO_ReadPin(PUSH_BUTTON_GPIO_Port, PUSH_BUTTON_Pin))
            {
                while (GPIO_PIN_SET == HAL_GPIO_ReadPin(PUSH_BUTTON_GPIO_Port, PUSH_BUTTON_Pin))
                    ;
            }
            /*
            
            BaseType_t xTaskGenericNotify( TaskHandle_t xTaskToNotify,
                               UBaseType_t uxIndexToNotify,
                               uint32_t ulValue,
                               eNotifyAction eAction,
                               uint32_t * pulPreviousNotificationValue ) PRIVILEGED_FUNCTION;
#define xTaskNotify( xTaskToNotify, ulValue, eAction ) \
    xTaskGenericNotify( ( xTaskToNotify ), ( tskDEFAULT_INDEX_TO_NOTIFY ), ( ulValue ), ( eAction ), NULL )
            
            */
            xTaskNotify(StartNotifyTakeTaskHandle, NotificationValueFromPushButton, eSetValueWithOverwrite);
        }
        
        vTaskDelay(10);
    }
    /* USER CODE END StartNotifyTakeTask */
}

/* USER CODE BEGIN Header_StartTimerTask */
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
        if (counter ++ == 50)
        {
            xTaskNotify(StartNotifyTakeTaskHandle, 50, eSetValueWithOverwrite);
            
            counter = 0;
        }
        
        vTaskDelay(100);
    }
    /* USER CODE END StartTimerTask */
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
    
    uint32_t notificationValue;
    
    /* Infinite loop */
    
    for (;;)
    {
        /*
        
        BaseType_t xTaskGenericNotifyWait( UBaseType_t uxIndexToWaitOn,
                                   uint32_t ulBitsToClearOnEntry,
                                   uint32_t ulBitsToClearOnExit,
                                   uint32_t * pulNotificationValue,
                                   TickType_t xTicksToWait ) PRIVILEGED_FUNCTION;
#define xTaskNotifyWait( ulBitsToClearOnEntry, ulBitsToClearOnExit, pulNotificationValue, xTicksToWait ) \
    xTaskGenericNotifyWait( tskDEFAULT_INDEX_TO_NOTIFY, ( ulBitsToClearOnEntry ), ( ulBitsToClearOnExit ), ( pulNotificationValue ), ( xTicksToWait ) )

        
        */
        
        xTaskNotifyWait(0xFFFFFFFF, NULL, &notificationValue, portMAX_DELAY);
        
        switch (notificationValue)
        {
            case NotificationValueFromPushButton:
            {
                printf("Push Button is pressed. NotificationValue = %d\n", notificationValue);
                break;
            }
            
            case NotificationValueFromTimer:
            {
                printf("Timer is reached. NotificationValue = %d\n", notificationValue);
                break;
            }
            
            default:
                break;
        }
        
        vTaskDelay(1000);
    }
    /* USER CODE END StartNotifyTakeTask */
}
