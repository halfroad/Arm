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
#include "../../../Middlewares/Third_Party/FreeRTOS/Source/include/FreeRTOS.h"
#include "../../../Middlewares/Third_Party/FreeRTOS/Source/include/task.h"

#include "./RTOS/Include/TasksWrapper.h"
#include "./Peripherals/GPIOs/Include/GPIO.h"
#include "./Peripherals/Displays/DSS1603/Include/OLED.h"

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

#define START_FLASH_GREEN_LED_TASK_STACK_DEPTH          128
#define START_FLASH_GREEN_LED_TASK_PRIORITY             7

#define START_FLASH_YELLOW_LED_TASK_STACK_DEPTH         128
#define START_FLASH_YELLOW_LED_TASK_PRIORITY            7

#define START_FLASH_RED_LED_TASK_STACK_DEPTH            128
#define START_FLASH_RED_LED_TASK_PRIORITY               7

#define START_OLED_DISPLAY_TASK_STACK_DEPTH             128
#define START_OLED_DISPLAY_TASK_PRIORITY                6

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */

/* Definitions for FlashGreenLEDTask */
TaskHandle_t StartFlashGreenLEDTaskHandle;

/* Definitions for FlashYellowLEDTask */
TaskHandle_t StartFlashYellowLEDTaskHandle;

/* Definitions for FlashRedLEDTask */
TaskHandle_t StartFlashRedLEDTaskHandle;

/* Definitions for OLEDDisplayTask */
TaskHandle_t StartOLEDDisplayTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

static void StartFlashGreenLEDTask(void *argument);
static void StartFlashYellowLEDTask(void *argument);
static void StartFlashRedLEDTask(void *argument);
static void StartOLEDDisplayTask(void *argument);

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
    /* creation of FlashGreenLEDTask */
    xTaskCreate(StartFlashGreenLEDTask, "StartFlashGreenLEDTaskName", START_FLASH_GREEN_LED_TASK_STACK_DEPTH, NULL, START_FLASH_GREEN_LED_TASK_PRIORITY, &StartFlashGreenLEDTaskHandle);
    
    /* creation of FlashYellowLEDTask */
    xTaskCreate(StartFlashYellowLEDTask, "StartFlashYellowLEDTaskName", START_FLASH_YELLOW_LED_TASK_STACK_DEPTH, NULL, START_FLASH_YELLOW_LED_TASK_PRIORITY, &StartFlashYellowLEDTaskHandle);

    /* creation of FlashRedLEDTask */
    xTaskCreate(StartFlashRedLEDTask, "StartFlashRedLEDTaskName", START_FLASH_RED_LED_TASK_STACK_DEPTH, (void *)1, START_FLASH_RED_LED_TASK_PRIORITY, &StartFlashRedLEDTaskHandle);
    
    /* creation of OLEDDisplayTask */
    xTaskCreate(StartOLEDDisplayTask, "StartOLEDDisplayTaskName", START_OLED_DISPLAY_TASK_STACK_DEPTH, (void *)1, START_OLED_DISPLAY_TASK_PRIORITY, &StartOLEDDisplayTaskHandle);

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
void StartFlashGreenLEDTask(void *argument)
{
    /* USER CODE BEGIN FlashGreenLEDTask */
    /* Infinite loop */
    for (;;)
    {
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
void StartFlashYellowLEDTask(void *argument)
{
    /* USER CODE BEGIN FlashYellowLEDTask */
    /* Infinite loop */
    for (;;)
    {
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
void StartFlashRedLEDTask(void *argument)
{
    /* USER CODE BEGIN FlashRedLEDTask */
    /* Infinite loop */
    for (;;)
    {
        HAL_GPIO_TogglePin(RED_LED_GPIO_Port, RED_LED_Pin);
        
        vTaskDelay(500);
    }
    /* USER CODE END FlashRedLEDTask */
}

/* USER CODE BEGIN Header_StartOLEDDisplayTask */
/**
 * @brief Function implementing the OLEDDisplayTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartOLEDDisplayTask */
void StartOLEDDisplayTask(void *argument)
{
    /* USER CODE BEGIN OLEDDisplayTask */
    /* Infinite loop */
    for (;;)
    {
        OLED_ShowChinese(0, 0, 0, 16, 1);
        OLED_ShowChinese(18 * 1, 0, 1, 16, 1);
        OLED_ShowChinese(18 * 2, 0, 2, 16, 1);
        OLED_ShowChinese(18 * 3, 0, 3, 16, 1);
        OLED_ShowChinese(18 * 4, 0, 4, 16, 1);
        
        OLED_Refresh();
    }
    /* USER CODE END OLEDDisplayTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

