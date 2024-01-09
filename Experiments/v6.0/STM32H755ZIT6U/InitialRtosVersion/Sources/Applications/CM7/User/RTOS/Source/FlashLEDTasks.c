#include "../../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os.h"

//#include "../../../Middlewares/Third_Party/FreeRTOS/Source/include/FreeRTOS.h"
//#include "../../../Middlewares/Third_Party/FreeRTOS/Source/include/task.h"

#include "../../Peripherals/Include/gpio.h"
#include "../Include/FlashLEDTasks.h"

static void CreateTasks(void);

static void StartFlashGreenLEDTask(void *argument);
static void StartFlashRedLEDTask(void *argument);

TaskHandle_t FlashGreenLEDTaskHandle;
TaskHandle_t FlashRedLEDTaskHandle;

void InitRtos()
{    
    CreateTasks();
}

void CreateTasks(void)
{
    /* USER CODE BEGIN Init */

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

    /* Create the thread(s) */
    /* creation of FlashGreenLEDTask */
    xTaskCreate(StartFlashGreenLEDTask, "StartFlashGreenLEDTaskName", 128, NULL, 24, &FlashGreenLEDTaskHandle);
    /* creation of FlashRedLEDTask */
    xTaskCreate(StartFlashRedLEDTask, "StartFlashRedLEDTaskName", 128, NULL, 24, &FlashRedLEDTaskHandle);

    /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
    /* USER CODE END RTOS_THREADS */

    /* USER CODE BEGIN RTOS_EVENTS */
    /* add events, ... */
    /* USER CODE END RTOS_EVENTS */
    
    vTaskStartScheduler();
}

/* USER CODE BEGIN Header_StartFlashGreenLEDTask */
/**
 * @brief  Function implementing the FlashGreenLEDTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartFlashGreenLEDTask */
void StartFlashGreenLEDTask(void *argument)
{
    /* USER CODE BEGIN StartFlashGreenLEDTask */
    /* Infinite loop */
    for (;;)
    {
        HAL_GPIO_TogglePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin);

        vTaskDelay(500);
    }
    /* USER CODE END StartFlashGreenLEDTask */
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
    /* USER CODE BEGIN StartFlashRedLEDTask */
    /* Infinite loop */
    for (;;)
    {
        HAL_GPIO_TogglePin(RED_LED_GPIO_Port, RED_LED_Pin);
        
        vTaskDelay(500);
    }
    /* USER CODE END StartFlashRedLEDTask */
}
