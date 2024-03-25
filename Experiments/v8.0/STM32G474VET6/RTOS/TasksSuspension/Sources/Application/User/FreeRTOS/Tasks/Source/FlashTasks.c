#include <stm32g4xx.h>

#include "../../../../Middlewares/Third_Party/FreeRTOS/Source/include/FreeRTOS.h"
#include "../../../../Middlewares/Third_Party/FreeRTOS/Source/include/task.h"

#include "../Include/FlashTasks.h"

#include "../../../Drivers/BSP/ATKDMG474/Include/LED.h"

extern TaskHandle_t FlashGreenLedTaskHandle;
extern TaskHandle_t FlashRedLedTaskHandle;

void FlashRedLedTask(void *arguments)
{
    while (1)
    {
        HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_0);
        
        vTaskDelay(250);
    }
}

void FlashGreenLedTask(void *arguments)
{
    while (1)
    {
        HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_1);
        
        vTaskDelay(250);
    }
}

void SuspendFlashGreenLedTask(void)
{
    vTaskSuspend(FlashGreenLedTaskHandle);
}

void ResumeFlashGreenLedTask(void)
{
    vTaskResume(FlashGreenLedTaskHandle);
}
