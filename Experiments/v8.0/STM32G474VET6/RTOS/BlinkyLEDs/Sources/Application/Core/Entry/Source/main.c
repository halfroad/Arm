#include <stm32g4xx_hal.h>

#include "../../Initializers/Include/SystemInitializer.h"

#include "../../../User/FreeRTOS/Wrappers/Include/TasksWrapper.h"
#include "../../../Drivers/BSP/ATKDMG474/Include/LED.h"

int main(void)
{
    HAL_Init();
    
    SystemClockConfig();
    
    InitLEDs();
    DispatchRealTimeTasks();
    
    while(1)
    {
        
    }
    
    return 0;
}