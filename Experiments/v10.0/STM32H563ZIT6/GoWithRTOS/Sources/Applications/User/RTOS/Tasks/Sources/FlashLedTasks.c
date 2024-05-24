#include <stdio.h>

#include "../../../Middlewares/Third_Party/FreeRTOS/Source/include/FreeRTOS.h"
#include "../../../Middlewares/Third_Party/FreeRTOS/Source/include/task.h"


#include "./Communications/SerialCommunications/Include/SerialCommunications.h"

#include "./Peripherals/GPIOs/Include/GPIO.h"

#include "../Include/FlashLedTasks.h"

TaskHandle_t FlashGreenLedTaskHandle;
TaskHandle_t FlashYellowLedTaskHandle;
TaskHandle_t FlashRedLedTaskHandle;
TaskHandle_t PushButtonTaskHandle;

void FlashGreenLedTask(void *arguments)
{
    for ( ;; )
    {
        ToggleLED(GreenLED);
        
        printf("Green LED is toggling.\n");
        
        vTaskDelay(250);
    }
}

void FlashYellowLedTask(void *arguments)
{
    for ( ;; )
    {
        ToggleLED(YellowLED);
        
        printf("Yellow LED is toggling.\n");
        
        vTaskDelay(250);
    }
}

void FlashRedLedTask(void *arguments)
{
    for ( ;; )
    {
        ToggleLED(RedLED);
        
        printf("Red LED is toggling.\n");
        
        vTaskDelay(250);
    }
}

void PushButtonTask(void *arguments)
{
    for ( ;; )
    {
        if (0x2000 == (GPIOC -> IDR & 0x2000))
        {
            /*    0b 10 0000 0000 0000 = 0x2000    */
            while (0x2000 == (GPIOC -> IDR & 0x2000))
                ;
            
            if (FlashGreenLedTaskHandle)
            {
                vTaskDelete(FlashGreenLedTaskHandle);
                
                FlashGreenLedTaskHandle = NULL;
            }
        }
        
        vTaskDelay(250);
    }
}