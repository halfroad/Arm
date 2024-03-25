#include <stm32g4xx.h>

#include "../../../Middlewares/Third_Party/FreeRTOS/Source/include/FreeRTOS.h"
#include "../../../Middlewares/Third_Party/FreeRTOS/Source/include/task.h"

#include "../Include/TasksWrapper.h"

#define SCHEDULE_TASKS_STACK_DEPTH                      128
#define SCHEDULE_TASKS_PRIORITY                         5

TaskHandle_t ScheduleTasksTaskHandle;

extern void ScheduleTasks(void *arguments);

void DispatchRealTimeTasks(void)
{
    if (pdPASS == xTaskCreate(ScheduleTasks, "ScheduleTasksName", SCHEDULE_TASKS_STACK_DEPTH, (void *)__func__, SCHEDULE_TASKS_PRIORITY, &ScheduleTasksTaskHandle))
       vTaskStartScheduler();
}

#if defined(SysTick)
#undef SysTick_Handler

/* CMSIS SysTick interrupt handler prototype */
extern void SysTick_Handler     (void);
/* FreeRTOS tick timer interrupt handler prototype */
extern void xPortSysTickHandler (void);

/*
  SysTick handler implementation that also clears overflow flag.
*/
#if (USE_CUSTOM_SYSTICK_HANDLER_IMPLEMENTATION == 0)
void SysTick_Handler (void)
{
  /* Clear overflow flag */
  SysTick -> CTRL;

  if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) {
    /* Call tick handler */
    xPortSysTickHandler();
  }
}
#endif
#endif /* SysTick */

/*

https://www.freertos.org/a00110.html

*/
__WEAK void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{

    static StaticTask_t IdleStaticTaskControlBlockBuffer;
    static StackType_t IdleTaskStackBuffer[configMINIMAL_STACK_SIZE];
    
    *ppxIdleTaskTCBBuffer   = &IdleStaticTaskControlBlockBuffer;
    *ppxIdleTaskStackBuffer = IdleTaskStackBuffer;
    *pulIdleTaskStackSize   = configMINIMAL_STACK_SIZE;
}

#if (configUSE_TIMERS == 1)

__WEAK void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize )
{
    static StaticTask_t TimerTaskControlBlockBuffer;
    static StackType_t TimerTaskStackBuffer[configTIMER_TASK_STACK_DEPTH];
    
    *ppxTimerTaskTCBBuffer      = &TimerTaskControlBlockBuffer;
    *ppxTimerTaskStackBuffer    = TimerTaskStackBuffer;
    *pulTimerTaskStackSize      = configTIMER_TASK_STACK_DEPTH;
}

#endif  /*  #if (configUSE_TIMERS == 1) */

__WEAK void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
{
    
}