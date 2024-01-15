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
#define START_FLASH_LEDS_TASK_PRIORY                        2

#define START_LIST_AND_LIST_ITEMS_TASK_STACK_DEPTH          128
#define START_LIST_AND_LIST_ITEMS_TASK_PRIORY               3

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* Definitions for StartFlashLEDsTaskHandle */
TaskHandle_t StartFlashLEDsTaskHandle;

/* Definitions for StartListAndListItemsTaskHandle */
TaskHandle_t StartListAndListItemsTaskHandle;

/* Definitions for List. */
List_t list;

/* Definitions for ListItems. */
ListItem_t listItem1;
ListItem_t listItem2;
ListItem_t listItem3;

/* USER CODE END Variables */


/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

static void StartFlashLEDsTask(void *argument);
static void StartListAndListItemsTask(void *argument);

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

    /* creation of StartListAndListItemsTask */
    xTaskCreate(StartListAndListItemsTask, "StartListAndListItemsTaskName", START_LIST_AND_LIST_ITEMS_TASK_STACK_DEPTH, (void *)1, START_LIST_AND_LIST_ITEMS_TASK_PRIORY, &StartListAndListItemsTaskHandle);

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

/* USER CODE BEGIN Header_StartListAndListItemsTask */
/**
 * @brief Function implementing the StartListAndListItemsTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartListAndListItemsTask */
void StartListAndListItemsTask(void *argument)
{
    /* USER CODE BEGIN StartListAndListItemsTask */
    
    vListInitialise( &list );
    
    vListInitialiseItem(&listItem1);
    vListInitialiseItem(&listItem2);
    vListInitialiseItem(&listItem3);
    
    listItem1.xItemValue = 40;
    listItem2.xItemValue = 60;
    listItem3.xItemValue = 50;
    
    printf("Print the addresses of list and its list items.\n");
    printf("Name:\t\t\tAddress:\n");
    
    printf("list\t\t\t0x%p\t\n", &list);
    printf("list.pxIndex\t\t0x%p\t\n", list.pxIndex);
    printf("list.xListEnd\t\t0x%p\t\n", &list.xListEnd);
    
    
    printf("listItem1\t\t0x%p\t\n", &listItem1);
    printf("listItem2\t\t0x%p\t\n", &listItem2);
    printf("listItem3\t\t0x%p\t\n", &listItem3);
    
    printf("\n");
    
    printf("Insert the listItem1.\n");
    
    vListInsert(&list, &listItem1);
    
    printf("Name:\t\t\tAdress:\n");
    
    printf("list.xListEnd.pxNext\t0x%p\n", list.xListEnd.pxNext);
    printf("listItem1.pxNext\t0x%p\n", listItem1.pxNext);
    printf("listItem1.pxPrevious\t0x%p\n", listItem1.pxPrevious);
    
    printf("\n");
    
    printf("Insert the listItem 2.\n");
    
    vListInsert(&list, &listItem2);
    
    printf("list.xListEnd.pxNext\t0x%p\n", list.xListEnd.pxNext);
    
    printf("listItem1.pxNext\t0x%p\n", listItem1.pxNext);
    printf("listItem2.pxNext\t0x%p\n", listItem2.pxNext);
    
    printf("list.xListEnd.pxPrevious\t0x%p\n", list.xListEnd.pxPrevious);
    
    printf("listItem1.pxPrevious\t0x%p\n", listItem1.pxPrevious);
    printf("listItem2.pxPrevious\t0x%p\n", listItem2.pxPrevious);
    
    printf("\n");
    
    printf("Insert the listItem 3.\n");
    
    vListInsert(&list, &listItem3);
    
    printf("list.xListEnd.pxNext\t0x%p\n", list.xListEnd.pxNext);
    
    printf("listItem1.pxNext\t\t0x%p\n", listItem1.pxNext);
    printf("listItem2.pxNext\t\t0x%p\n", listItem2.pxNext);
    printf("listItem3.pxNext\t\t0x%p\n", listItem3.pxNext);
    
    printf("list.xListEnd.pxPrevious\t0x%p\n", list.xListEnd.pxPrevious);
    
    printf("listItem1.pxPrevious\t\t0x%p\n", listItem1.pxPrevious);
    printf("listItem2.pxPrevious\t\t0x%p\n", listItem2.pxPrevious);
    printf("listItem3.pxPrevious\t\t0x%p\n", listItem3.pxPrevious);
    
    /* Infinite loop */
    for (;;)
    {
        //HAL_GPIO_TogglePin(RED_LED_GPIO_Port, RED_LED_Pin);
        
        vTaskDelay(500);
    }
    /* USER CODE END StartListAndListItemsTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

