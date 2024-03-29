#include <stm32h5xx.h>

#include "../../../Middlewares/Third_Party/FreeRTOS/Source/include/FreeRTOS.h"
#include "../../../Middlewares/Third_Party/FreeRTOS/Source/include/task.h"

#include "../Include/TasksWrapper.h"


/* USER CODE BEGIN 2 */

#if defined(SysTick)
#undef SysTick_Handler

/* CMSIS SysTick interrupt handler prototype */
extern void SysTick_Handler     (void);
/* FreeRTOS tick timer interrupt handler prototype */
extern void xPortSysTickHandler (void);

/* USER CODE END FunctionPrototypes */

/* USER CODE BEGIN 2 */

/*
  SysTick handler implementation that also clears overflow flag.
*/
#if (USE_CUSTOM_SYSTICK_HANDLER_IMPLEMENTATION == 0)
void SysTick_Handler (void) {
  /* Clear overflow flag */
  SysTick->CTRL;

  if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) {
    /* Call tick handler */
    xPortSysTickHandler();
  }
}
#endif
#endif /* SysTick */

/* USER CODE END 3 */

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/**
  Dummy implementation of the callback function vApplicationIdleHook().
*/
#if (configUSE_IDLE_HOOK == 1)
__WEAK void vApplicationIdleHook (void){}
#endif

/**
  Dummy implementation of the callback function vApplicationTickHook().
*/
#if (configUSE_TICK_HOOK == 1)
 __WEAK void vApplicationTickHook (void){}
#endif

/**
  Dummy implementation of the callback function vApplicationMallocFailedHook().
*/
#if (configUSE_MALLOC_FAILED_HOOK == 1)
__WEAK void vApplicationMallocFailedHook (void){}
#endif

/**
  Dummy implementation of the callback function vApplicationDaemonTaskStartupHook().
*/
#if (configUSE_DAEMON_TASK_STARTUP_HOOK == 1)
__WEAK void vApplicationDaemonTaskStartupHook (void){}
#endif

/**
  Dummy implementation of the callback function vApplicationStackOverflowHook().
*/
#if (configCHECK_FOR_STACK_OVERFLOW > 0)
__WEAK void vApplicationStackOverflowHook (TaskHandle_t xTask, signed char *pcTaskName)
{
  (void)xTask;
  (void)pcTaskName;
  configASSERT(0);
}
#endif

/*---------------------------------------------------------------------------*/
#if (configSUPPORT_STATIC_ALLOCATION == 1)
/* External Idle and Timer task static memory allocation functions */
extern void vApplicationGetIdleTaskMemory  (StaticTask_t **ppxIdleTaskTCBBuffer,  StackType_t **ppxIdleTaskStackBuffer,  uint32_t *pulIdleTaskStackSize);
extern void vApplicationGetTimerTaskMemory (StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize);

/*
  vApplicationGetIdleTaskMemory gets called when configSUPPORT_STATIC_ALLOCATION
  equals to 1 and is required for static memory allocation support.
*/
__WEAK void vApplicationGetIdleTaskMemory (StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize)
{
  /* Idle task control block and stack */
  static StaticTask_t Idle_TCB;
  static StackType_t  Idle_Stack[configMINIMAL_STACK_SIZE];

  *ppxIdleTaskTCBBuffer   = &Idle_TCB;
  *ppxIdleTaskStackBuffer = &Idle_Stack[0];
  *pulIdleTaskStackSize   = (uint32_t)configMINIMAL_STACK_SIZE;
}

/*
  vApplicationGetTimerTaskMemory gets called when configSUPPORT_STATIC_ALLOCATION
  equals to 1 and is required for static memory allocation support.
*/
__WEAK void vApplicationGetTimerTaskMemory (StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize)
{
  /* Timer task control block and stack */
  static StaticTask_t Timer_TCB;
  static StackType_t  Timer_Stack[configTIMER_TASK_STACK_DEPTH];

  *ppxTimerTaskTCBBuffer   = &Timer_TCB;
  *ppxTimerTaskStackBuffer = &Timer_Stack[0];
  *pulTimerTaskStackSize   = (uint32_t)configTIMER_TASK_STACK_DEPTH;
}
#endif

/* USER CODE END Application */

#define SCHEDULE_TASKS_TASK_STACK_DEPTH             128
#define SCHEDULE_TASKS_PRIORITY                       15

TaskHandle_t ScheduleTasksHandle;
StackType_t ScheduleTasksStack[SCHEDULE_TASKS_TASK_STACK_DEPTH];
StaticTask_t ScheduleTasksTaskControlBlock;

extern void ScheduleTasks(void *argument);

void InitRtos(void)
{
    /* Start scheduler if initialized and not started before */
    
#if (configSUPPORT_STATIC_ALLOCATION == 1)
    
#define CREATE_TASKS_STATICALLY   1
    
#if (CREATE_TASKS_STATICALLY == 1)
    
    ScheduleTasksHandle = xTaskCreateStatic(ScheduleTasks,
                                    "ScheduleTasksName", /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
                                    SCHEDULE_TASKS_TASK_STACK_DEPTH,
                                    NULL,
                                    SCHEDULE_TASKS_PRIORITY,
                                    ScheduleTasksStack,
                                    &ScheduleTasksTaskControlBlock );
    if (ScheduleTasksHandle)
        vTaskStartScheduler();
#else
    if (pdPASS == xTaskCreate(ScheduleTasks, "ScheduleTasksName", SCHEDULE_TASKS_TASK_STACK_DEPTH, NULL, SCHEDULE_TASKS_PRIORITY, &ScheduleTasksHandle))
        vTaskStartScheduler();
    
#endif
    
#else
        
    if (pdPASS == xTaskCreate(ScheduleTasks, "ScheduleTasksName", SCHEDULE_TASKS_TASK_STACK_DEPTH, NULL, SCHEDULE_TASKS_PRIORITY, &ScheduleTasksHandle))
        vTaskStartScheduler();
#endif

}