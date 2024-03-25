#include <stm32g4xx.h>

#include "../../../../Middlewares/Third_Party/FreeRTOS/Source/include/FreeRTOS.h"
#include "../../../../Middlewares/Third_Party/FreeRTOS/Source/include/task.h"

#include "../../../Drivers/BSP/ATKDMG474/Include/LED.h"

void SuspendFlashGreenLedTask(void);
void ResumeFlashGreenLedTask(void);
