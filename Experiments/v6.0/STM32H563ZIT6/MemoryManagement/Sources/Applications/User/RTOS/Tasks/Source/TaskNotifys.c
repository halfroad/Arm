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

#define START_MEMORY_MANAGE_TASK_STACK_DEPTH            128
#define START_MEMORY_MANAGE_PRIORITY                    2

#define START_TIMER_TASK_STACK_DEPTH                    128
#define START_TIMER_TASK_PRIORITY                       3

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* Definitions for StartMemoryManageTaskHandle */
TaskHandle_t StartMemoryManageTaskHandle;

/* Definitions for StartTimerTaskHandle */
TaskHandle_t StartTimerTaskHandle;

uint8_t *buffer;

/* USER CODE END Variables */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

static void StartMemoryManageTask(void *argument);
static void StartTimerTask(void *argument);

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
    
    /* creation of StartMemoryManageTask */
    xTaskCreate(StartMemoryManageTask, "StartMemoryManageTaskName", START_MEMORY_MANAGE_TASK_STACK_DEPTH, NULL, START_MEMORY_MANAGE_PRIORITY, &StartMemoryManageTaskHandle);
    
    /* creation of StartTimerTask */
    xTaskCreate(StartTimerTask, "StartTimerTaskName", START_TIMER_TASK_STACK_DEPTH, NULL, START_TIMER_TASK_PRIORITY, &StartTimerTaskHandle);
   
    /* USER CODE END RTOS_THREADS */

    /* USER CODE BEGIN RTOS_EventsS */
    /* add Eventss, ... */
    /* USER CODE END RTOS_EventsS */
    
    vTaskDelete(NULL);
    
    vPortExitCritical();

}
/* USER CODE BEGIN Header_StartMemoryManageTask */
/**
 * @brief Function implementing the StartMemoryManageTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartMemoryManageTask */
void StartMemoryManageTask(void *argument)
{
    /* USER CODE BEGIN StartMemoryManageTask */

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
                
                buffer = pvPortMalloc(30);
                
               if (buffer)
                   printf("Memory allocated with size succeeded.\n\n");
               else
                   printf("Failed to allot the Memory.\n\n");
            }
        }
        
        vTaskDelay(10);
    }
    /* USER CODE END StartMemoryManageTask */
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
        if (counter == 25)
            printf("The volume of residual memory is %d.\n\n", xPortGetFreeHeapSize());
        
        if (counter++ == 50)
        {
            if (buffer)
            {
                vPortFree(buffer);
                
                buffer = NULL;
                
                printf("Memory is freed.\n\n");
            }
            
            counter = 0;
        }
        
        vTaskDelay(100);
    }
    /* USER CODE END StartTimerTask */
}
