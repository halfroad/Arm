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

#define START_TASK1_STACK_DEPTH             128
#define START_TASK1_PRIORY                  2

#define START_TASK2_STACK_DEPTH             128
#define START_TASK2_PRIORY                  3

#define START_TASK3_STACK_DEPTH             128
#define START_TASK3_PRIORY                  4

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */

/* Definitions for StartTask1Handle */
TaskHandle_t StartTask1Handle;

/* Definitions for StartTask2Handle */
TaskHandle_t StartTask2Handle;

/* Definitions for StartTask3Handle */
TaskHandle_t StartTask3Handle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

static void StartTask1(void *argument);
static void StartTask2(void *argument);
static void StartTask3(void *argument);

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
    /* creation of StartTask1 */
    xTaskCreate(StartTask1, "StartTask1Name", START_TASK1_STACK_DEPTH, NULL, START_TASK1_PRIORY, &StartTask1Handle);
    
    /* creation of StartTask2 */
    xTaskCreate(StartTask2, "StartTask2Name", START_TASK2_STACK_DEPTH, NULL, START_TASK2_PRIORY, &StartTask2Handle);

    /* creation of StartTask3 */
    xTaskCreate(StartTask3, "StartTask3Name", START_TASK3_STACK_DEPTH, (void *)1, START_TASK3_PRIORY, &StartTask3Handle);

    /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
    /* USER CODE END RTOS_THREADS */

    /* USER CODE BEGIN RTOS_EVENTS */
    /* add events, ... */
    /* USER CODE END RTOS_EVENTS */
    
    vTaskDelete(NULL);
    
    vPortExitCritical();

}
/* USER CODE BEGIN Header_StartFlashGreenLEDTask */
/**
 * @brief Function implementing the FlashGreenLEDTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartFlashGreenLEDTask */
void StartTask1(void *argument)
{
    /* USER CODE BEGIN FlashGreenLEDTask */
    /* Infinite loop */
    for (;;)
    {
        printf("Task 1 is running.\n");
        
        HAL_GPIO_TogglePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin);

        vTaskDelay(500);
    }
    /* USER CODE END FlashGreenLEDTask */
}

/* USER CODE BEGIN Header_StartFlashYellowLEDTask */
/**
 * @brief Function implementing the FlashYellowLEDTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartFlashYellowLEDTask */
void StartTask2(void *argument)
{
    /* USER CODE BEGIN FlashYellowLEDTask */
    /* Infinite loop */
    for (;;)
    {
        printf("Task 2 is running.\n");
        
        HAL_GPIO_TogglePin(YELLOW_LED_GPIO_Port, YELLOW_LED_Pin);

        vTaskDelay(500);
    }
    /* USER CODE END FlashYellowLEDTask */
}

/* USER CODE BEGIN Header_StartFlashRedLEDTask */
/**
 * @brief Function implementing the FlashRedLEDTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartFlashRedLEDTask */
void StartTask3(void *argument)
{
    /* USER CODE BEGIN FlashRedLEDTask */
    /* Infinite loop */
    for (;;)
    {
        printf("Task 3 is running.\n");
        
        eTaskState taskState = eTaskGetState(StartTask1Handle);
        
        if (GPIO_PIN_SET == HAL_GPIO_ReadPin(PUSH_BUTTON_GPIO_Port, PUSH_BUTTON_Pin))
        {   
            if (taskState == eRunning || taskState == eReady)
                vTaskSuspend(StartTask1Handle);            
        }
        else
        {
            if (taskState == eSuspended)
                vTaskResume(StartTask1Handle);
        }
            
        vTaskDelay(500);
    }
    /* USER CODE END FlashRedLEDTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

