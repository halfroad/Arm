#ifndef __TASKS_WRAPPER_H
#define __TASKS_WRAPPER_H

#include "../../../Middlewares/Third_Party/FreeRTOS/Source/include/FreeRTOS.h"
#include "../../../Middlewares/Third_Party/FreeRTOS/Source/include/task.h"

void vApplicationIdleHook (void);
void vApplicationTickHook (void);
void vApplicationMallocFailedHook (void);
void vApplicationDaemonTaskStartupHook (void);
void vApplicationStackOverflowHook (TaskHandle_t xTask, signed char *pcTaskName);

#endif