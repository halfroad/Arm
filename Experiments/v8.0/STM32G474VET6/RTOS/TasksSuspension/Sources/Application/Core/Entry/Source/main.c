#include <stm32g4xx_hal.h>

#include "../../Initializers/Include/SystemInitializer.h"
#include "../../../User/FreeRTOS/Tasks/Include/TasksManager.h"

int main(void)
{
    HAL_Init();
    SystemClockConfig();
    
    InitTasksManager();
    
    while(1)
    {
    }
    
    return 0;
}