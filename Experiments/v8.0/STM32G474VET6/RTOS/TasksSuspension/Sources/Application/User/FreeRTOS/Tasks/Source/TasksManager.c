#include "../../../Middlewares/Third_Party/FreeRTOS/Source/include/FreeRTOS.h"
#include "../../../Middlewares/Third_Party/FreeRTOS/Source/include/task.h"

#include "../../../Drivers/BSP/ATKDMG474/Include/LED.h"
#include "../../../Drivers/BSP/ATKDMG474/Include/PushButton.h"

#include "../../Wrappers/Include/TasksWrapper.h"

#include "../Include/TasksManager.h"

void InitTasksManager(void)
{
    InitLEDs();
    InitPushButtons();
    
    DispatchRealTimeTasks();
}