#include <stm32h7xx.h>

#include "../../../Middlewares/Third_Party/FreeRTOS/Source/include/FreeRTOS.h"
#include "../../../Middlewares/Third_Party/FreeRTOS/Source/include/task.h"
#include "../../../Applications/Shared/Peripherals/Inc/GPIO.h"
#include "../Inc/TasksOperator.h"

#define CREATE_TASKS_STACK_DEPTH                        128     /* Word (uint32_t) based size. */
#define CREATE_TASKS_TASK_PRIORITY                      1

TaskHandle_t create_tasks_task_handle;

static void create_tasks(void *parameters);

#define CREATE_GREEN_LED_TASK_STACK_DEPTH               128     /* Word (uint32_t) based size. */
#define CREATE_GREEN_LED_TASK_PRIORITY                  2

TaskHandle_t create_green_led_task_handle;

static void create_green_led_task(void *parameters);

#define CREATE_YELLOW_LED_TASK_STACK_DEPTH               128     /* Word (uint32_t) based size. */
#define CREATE_YELLOW_LED_TASK_PRIORITY                  3

TaskHandle_t create_yellow_led_task_handle;

static void create_yellow_led_task(void *parameters);

#define CREATE_RED_LED_TASK_STACK_DEPTH                 128     /* Word (uint32_t) based size. */
#define CREATE_RED_LED_TASK_PRIORITY                    4

TaskHandle_t create_red_led_task_handle;

static void create_red_led_task(void *parameters);

void InitRTOS()
{
    BaseType_t baseType = xTaskCreate((TaskFunction_t)create_tasks, (const char *)"create_tasks_name", CREATE_TASKS_STACK_DEPTH, (void *)1, CREATE_TASKS_TASK_PRIORITY, &create_tasks_task_handle);
    
    if (baseType == pdPASS)
        vTaskStartScheduler();
}

static void create_tasks(void *parameters)
{
    xTaskCreate((TaskFunction_t)create_green_led_task, (const char *)"create_green_led_task_name", CREATE_GREEN_LED_TASK_STACK_DEPTH, (void *)1, CREATE_GREEN_LED_TASK_PRIORITY, &create_green_led_task_handle);
    xTaskCreate((TaskFunction_t)create_yellow_led_task, (const char *)"create_yellow_led_task_name", CREATE_YELLOW_LED_TASK_STACK_DEPTH, (void *)2, CREATE_YELLOW_LED_TASK_PRIORITY, &create_yellow_led_task_handle);
    xTaskCreate((TaskFunction_t)create_red_led_task, (const char *)"create_red_led_task_name", CREATE_RED_LED_TASK_STACK_DEPTH, (void *)2, CREATE_RED_LED_TASK_PRIORITY, &create_red_led_task_handle);
    
    /* Delete the task itself. */
    vTaskDelete(NULL);
}

static void create_green_led_task(void *parameters)
{
    for(;;)
    {
        HAL_GPIO_TogglePin(GREEN_LED_GPIO_PORT, GREEN_LED_GPIO_PIN);
        
        vTaskDelay(500);
    }
}

static void create_yellow_led_task(void *parameters)
{
    while (1)
    {
        HAL_GPIO_TogglePin(GREEN_LED_GPIO_PORT, YELLOW_LED_GPIO_PIN);
        
        vTaskDelay(500);
    }
}

static void create_red_led_task(void *parameters)
{
    HAL_GPIO_TogglePin(RED_LED_GPIO_PORT, RED_LED_GPIO_PIN);
    
    vTaskDelay(500);
}