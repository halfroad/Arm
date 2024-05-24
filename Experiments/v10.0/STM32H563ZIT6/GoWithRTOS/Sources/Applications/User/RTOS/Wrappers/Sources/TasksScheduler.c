#include "../../../Middlewares/Third_Party/FreeRTOS/Source/include/FreeRTOS.h"
#include "../../../Middlewares/Third_Party/FreeRTOS/Source/include/task.h"

#include "./RTOS/Tasks/Include/FlashLedTasks.h"

#include "../Include/TasksScheduler.h"

extern TaskHandle_t FlashGreenLedTaskHandle;
extern TaskHandle_t FlashYellowLedTaskHandle;
extern TaskHandle_t FlashRedLedTaskHandle;
extern TaskHandle_t PushButtonTaskHandle;

void ScheduleTasks(void)
{
    taskENTER_CRITICAL();
    
    xTaskCreate(FlashGreenLedTask, "FlashGreenLedTaskName", FLASH_GREEN_LED_TASK_STACK_DEPTH, (void *)1, FLASH_GREEN_LED_TASK_PRIORITY, &FlashGreenLedTaskHandle);
    xTaskCreate(FlashYellowLedTask, "FlashYellowLedTaskName", FLASH_YELLOW_LED_TASK_STACK_DEPTH, (void *)1, FLASH_YELLOW_LED_TASK_PRIORITY, &FlashYellowLedTaskHandle);
    xTaskCreate(FlashRedLedTask, "FlashRedLedTaskName", FLASH_RED_LED_TASK_STACK_DEPTH, (void *)1, FLASH_RED_LED_TASK_PRIORITY, &FlashRedLedTaskHandle);
    xTaskCreate(PushButtonTask, "PushButtonTaskName", PUSH_BUTTON_TASK_STACK_DEPTH, (void *)1, PUSH_BUTTON_TASK_PRIORITY, &PushButtonTaskHandle);
    
    vTaskDelete(NULL);
    
    taskEXIT_CRITICAL();
}