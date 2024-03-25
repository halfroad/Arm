#include "../../../Middlewares/Third_Party/FreeRTOS/Source/include/FreeRTOS.h"
#include "../../../Middlewares/Third_Party/FreeRTOS/Source/include/task.h"

#define FLASH_GREEN_LED_TASK_STACK_DEPTH                                128
#define FLASH_GREEN_LED_TASK_PRIORITY                                   6

TaskHandle_t FlashGreenLedTaskHandle;

extern void FlashGreenLedTask(void *arguments);

#define FLASH_RED_LED_TASK_STACK_DEPTH                                  128
#define FLASH_RED_LED_TASK_PRIORITY                                     6

TaskHandle_t FlashRedLedTaskHandle;

extern void FlashRedLedTask(void *arguments);

void ScheduleTasks(void *arguments)
{
    portENTER_CRITICAL();
    
    xTaskCreate(FlashRedLedTask, "FlashRedLedTaskName", FLASH_RED_LED_TASK_STACK_DEPTH, NULL, FLASH_RED_LED_TASK_PRIORITY, &FlashRedLedTaskHandle);
    xTaskCreate(FlashGreenLedTask, "FlashGreenLedTaskName", FLASH_GREEN_LED_TASK_STACK_DEPTH, NULL, FLASH_GREEN_LED_TASK_PRIORITY, &FlashGreenLedTaskHandle);
    
    vTaskDelete(NULL);
    
    portEXIT_CRITICAL();
}