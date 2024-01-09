#include "../../../Middlewares/Third_Party/FreeRTOS/Source/include/FreeRTOS.h"
#include "../../../Middlewares/Third_Party/FreeRTOS/Source/include/task.h"

#include "../../Peripherals/Include/gpio.h"
#include "../Include/FlashLEDTasks.h"

TaskHandle_t createTasksHandle;

TaskHandle_t createFlashGreenLEDTaskHandle;
TaskHandle_t createFlashRedLEDTaskHandle;
TaskHandle_t createFlashYellowLEDTaskHandle;


#define CREATE_TASKS_PRIORITY                       5

#define CREATE_TASKS_STACK_DEPTH                    128

#define CREATE_FLASH_GREEN_LED_TASK_STACK_DEPTH     128
#define CREATE_FLASH_RED_LED_TASK_STACK_DEPTH       128

#define CREATE_FLASH_GREEN_LED_TASK_PRIORITY        6
#define CREATE_FLASH_RED_LED_TASK_PRIORITY          7


#define CREATE_FLASH_YELLOW_LED_TASK_STACK_DEPTH    128

#define CREATE_FLASH_YELLOW_LED_TASK_PRIORITY       8


static void CreateTasks(void);

static void CreateFlashGreenLEDTask(void);
static void CreateFlashRedLEDTask(void);
static void CreateFlashYellowLEDTask(void* paramter);

void InitRtos()
{
    BaseType_t result = xTaskCreate((TaskFunction_t)CreateTasks, "CreateTasksName", CREATE_TASKS_STACK_DEPTH, NULL, CREATE_TASKS_PRIORITY, &createTasksHandle);
    
    if (result == pdTRUE)
        vTaskStartScheduler();
}

static void CreateTasks(void)
{
    /*
    
    void SysTick_Handler()
    void xPortSysTickHandler( void )
    
    */
    xTaskCreate((TaskFunction_t)CreateFlashGreenLEDTask, "CreateGreenLEDFlashTaskName", CREATE_FLASH_GREEN_LED_TASK_STACK_DEPTH, NULL, CREATE_FLASH_GREEN_LED_TASK_PRIORITY, &createFlashGreenLEDTaskHandle);
    xTaskCreate((TaskFunction_t)CreateFlashRedLEDTask, "CreateRedLEDFlashTaskName", CREATE_FLASH_RED_LED_TASK_STACK_DEPTH, NULL, CREATE_FLASH_RED_LED_TASK_PRIORITY, &createFlashRedLEDTaskHandle);
    xTaskCreate((TaskFunction_t)CreateFlashYellowLEDTask, "CreateFlashYellowLEDTaskName", CREATE_FLASH_YELLOW_LED_TASK_STACK_DEPTH, (void *) 1, CREATE_FLASH_YELLOW_LED_TASK_PRIORITY, &createFlashYellowLEDTaskHandle);
}

static void CreateFlashGreenLEDTask(void)
{
    HAL_GPIO_TogglePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin);
    
    vTaskDelay(500);
}

static void CreateFlashRedLEDTask(void)
{
    HAL_GPIO_TogglePin(RED_LED_GPIO_Port, RED_LED_Pin);
    
    vTaskDelay(500);
}

static void CreateFlashYellowLEDTask(void* paramter)
{
    HAL_GPIO_TogglePin(YELLOW_LED_GPIO_Port, YELLOW_LED_Pin);
    
    vTaskDelay(500);
}

