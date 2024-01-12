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
#define START_TASK1_PRIORY                  5

#define START_TASK2_STACK_DEPTH             128
#define START_TASK2_PRIORY                  5

#define START_TASK3_STACK_DEPTH             128
#define START_TASK3_PRIORY                  5

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */

/* Definitions for StartTask1Handle */
TaskHandle_t StartTask1Handle;
StackType_t StartTask1Stack[START_TASK1_STACK_DEPTH];
StaticTask_t StartTask1TaskControlBlock;

/* Definitions for StartTask2Handle */
TaskHandle_t StartTask2Handle;
StackType_t StartTask2Stack[START_TASK2_STACK_DEPTH];
StaticTask_t StartTask2TaskControlBlock;

/* Definitions for StartTask3Handle */
TaskHandle_t StartTask3Handle;
StackType_t StartTask3Stack[START_TASK3_STACK_DEPTH];
StaticTask_t StartTask3TaskControlBlock;

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
    
#if (configSUPPORT_STATIC_ALLOCATION == 1)

    StartTask1Handle = xTaskCreateStatic(StartTask1,
                                    "StartTask1Name", /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
                                    START_TASK1_STACK_DEPTH,
                                    NULL,
                                    START_TASK1_PRIORY,
                                    StartTask1Stack,
                                    &StartTask1TaskControlBlock );
    StartTask2Handle = xTaskCreateStatic(StartTask2,
                                    "StartTask2Name", /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
                                    START_TASK2_STACK_DEPTH,
                                    NULL,
                                    START_TASK2_PRIORY,
                                    StartTask2Stack,
                                    &StartTask2TaskControlBlock );
                                    
    StartTask3Handle = xTaskCreateStatic(StartTask3,
                                    "StartTask3Name", /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
                                    START_TASK3_STACK_DEPTH,
                                    NULL,
                                    START_TASK3_PRIORY,
                                    StartTask3Stack,
                                    &StartTask3TaskControlBlock );
    
#else

#endif

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
        
        if (GPIO_PIN_SET == HAL_GPIO_ReadPin(PUSH_BUTTON_GPIO_Port, PUSH_BUTTON_Pin))
        {
            eTaskState taskState = eTaskGetState(StartTask1Handle);
            
            if (taskState != eDeleted)
                vTaskDelete(StartTask1Handle);
        }
            
        vTaskDelay(10);
    }
    /* USER CODE END FlashRedLEDTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

