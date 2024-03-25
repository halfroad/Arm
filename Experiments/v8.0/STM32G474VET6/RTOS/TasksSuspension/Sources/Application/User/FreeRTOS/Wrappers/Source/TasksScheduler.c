#include "../../../Middlewares/Third_Party/FreeRTOS/Source/include/FreeRTOS.h"
#include "../../../Middlewares/Third_Party/FreeRTOS/Source/include/task.h"

#define FLASH_GREEN_LED_TASK_STACK_DEPTH                                128
#define FLASH_GREEN_LED_TASK_PRIORITY                                   6

#define FLASH_RED_LED_TASK_STACK_DEPTH                                  128
#define FLASH_RED_LED_TASK_PRIORITY                                     6

#define BUTTONS_PRESS_TASK_STACK_DEPTH                                  128
#define BUTTONS_PRESS_TASK_PRIORITY                                     6

TaskHandle_t FlashGreenLedTaskHandle;
TaskHandle_t FlashRedLedTaskHandle;
TaskHandle_t ButtonPressTaskHandle;

extern void FlashGreenLedTask(void *arguments);
extern void FlashRedLedTask(void *arguments);
extern void ButtonPressTask(void *arguments);

void ScheduleTasks(void *arguments)
{
    portENTER_CRITICAL();
    
    xTaskCreate(FlashRedLedTask, "FlashRedLedTaskName", FLASH_RED_LED_TASK_STACK_DEPTH, NULL, FLASH_RED_LED_TASK_PRIORITY, &FlashRedLedTaskHandle);
    xTaskCreate(FlashGreenLedTask, "FlashGreenLedTaskName", FLASH_GREEN_LED_TASK_STACK_DEPTH, NULL, FLASH_GREEN_LED_TASK_PRIORITY, &FlashGreenLedTaskHandle);
    xTaskCreate(ButtonPressTask, "ButtonPressTaskName", BUTTONS_PRESS_TASK_STACK_DEPTH, NULL, BUTTONS_PRESS_TASK_PRIORITY, &ButtonPressTaskHandle);
    
    vTaskDelete(NULL);
    
    portEXIT_CRITICAL();
}