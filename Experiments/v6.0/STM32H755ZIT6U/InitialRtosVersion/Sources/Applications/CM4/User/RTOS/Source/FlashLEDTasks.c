#include "../../../Middlewares/Third_Party/FreeRTOS/Source/include/FreeRTOS.h"
#include "../../../Middlewares/Third_Party/FreeRTOS/Source/include/task.h"

#include "../../Peripherals/Include/gpio.h"
#include "../Include/FlashLEDTasks.h"


#define CREATE_TASKS_STACK_DEPTH                    128
#define CREATE_TASKS_PRIORITY                       5

#define CREATE_FLASH_YELLOW_LED_TASK_STACK_DEPTH    128
#define CREATE_FLASH_YELLOW_LED_TASK_PRIORITY       6

TaskHandle_t createTasksHandle;
TaskHandle_t createFlashYellowLEDTaskHandle;

static void CreateTasks(void);
static void CreateFlashYellowLEDTask(void* paramter);

void InitRtos()
{
    BaseType_t result = xTaskCreate((TaskFunction_t)CreateTasks, "CreateTasksName", CREATE_TASKS_STACK_DEPTH, NULL, CREATE_TASKS_PRIORITY, &createTasksHandle);
    
    if (result == pdPASS)
        vTaskStartScheduler();
}

static void CreateTasks(void)
{
    /*
    
    void SysTick_Handler()
    void xPortSysTickHandler( void )
    
    */
    xTaskCreate((TaskFunction_t)CreateFlashYellowLEDTask, "CreateYellowLEDFlashTaskName", CREATE_FLASH_YELLOW_LED_TASK_STACK_DEPTH, NULL, CREATE_FLASH_YELLOW_LED_TASK_PRIORITY, &createFlashYellowLEDTaskHandle);
}

static void CreateFlashYellowLEDTask(void* paramter)
{
    while (1)
    {
        HAL_GPIO_TogglePin(YELLOW_LED_GPIO_Port, YELLOW_LED_Pin);
        
        HAL_Delay(500);
    }
}

