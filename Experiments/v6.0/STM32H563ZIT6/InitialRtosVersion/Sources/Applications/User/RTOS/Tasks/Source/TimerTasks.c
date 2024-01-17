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
#include "../../../Middlewares/Third_Party/FreeRTOS/Source/include/timers.h"
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

#define TIMER_CONTROLS_TASK_STACK_DEPTH             128
#define TIMER_CONTROLS_TASK_PRIORITY                  2

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */

/* Definitions for OneTimeTimerHandle */
TimerHandle_t OneTimeTimerHandle;

/* Definitions for PeriodicTimerHandle */
TimerHandle_t PeriodicTimerHandle;

/* Definitions for TimerControlsTask */
TaskHandle_t TimerControlsTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

void OneTimeTimerCallback(TimerHandle_t xTimer);
void PeriodicTimerCallback(TimerHandle_t xTimer);

static void TimerControlsTask(void *argument);

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
    
    OneTimeTimerHandle = xTimerCreate( "OneTimeTimerHandleName", /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
                                    1000,
                                    pdFALSE,
                                    (void *)1,
                                    OneTimeTimerCallback );
    PeriodicTimerHandle = xTimerCreate( "OneTimeTimerHandleName", /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
                                    1000,
                                    pdFALSE,
                                    (void *)1,
                                    OneTimeTimerCallback );
    /* USER CODE END RTOS_TIMERS */

    /* USER CODE BEGIN RTOS_QUEUES */
    /* add queues, ... */
    /* USER CODE END RTOS_QUEUES */
    /* creation of StartTask1 */
    xTaskCreate(TimerControlsTask, "TimerControlsTaskName", TIMER_CONTROLS_TASK_STACK_DEPTH, NULL, TIMER_CONTROLS_TASK_PRIORITY, &TimerControlsTaskHandle);
    
    /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
    /* USER CODE END RTOS_THREADS */

    /* USER CODE BEGIN RTOS_EVENTS */
    /* add events, ... */
    /* USER CODE END RTOS_EVENTS */
    
    vTaskDelete(NULL);
    
    vPortExitCritical();

}

void OneTimeTimerCallback(TimerHandle_t xTimer)
{
    static uint32_t times = 0;
    
    printf("The times occured on One Time Timer is %#x", ++ times);
}

void PeriodicTimerCallback(TimerHandle_t xTimer)
{
    static uint32_t times = 0;
    
    printf("The times occured on Periodic Timer is %#x", ++ times);
}

/* USER CODE BEGIN TimerControlsTask */
/**
 * @brief Function implementing the TimerControlsTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END TimerControlsTask */
void TimerControlsTask(void *argument)
{
    /* USER CODE BEGIN FlashRedLEDTask */
    /* Infinite loop */
    for (;;)
    {
       /* printf("Timer Controls Task is running.\n"); */
        
        eTaskState taskState = eTaskGetState(TimerControlsTaskHandle);
        
        if (GPIO_PIN_SET == HAL_GPIO_ReadPin(PUSH_BUTTON_GPIO_Port, PUSH_BUTTON_Pin))
        {
            xTimerStart(OneTimeTimerHandle, portMAX_DELAY);
            xTimerStart(PeriodicTimerHandle, portMAX_DELAY);
        }
        else
        {
            xTimerStop(OneTimeTimerHandle, portMAX_DELAY);
            xTimerStop(PeriodicTimerHandle, portMAX_DELAY);
        }
            
        vTaskDelay(10);
    }
    /* USER CODE END FlashRedLEDTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

