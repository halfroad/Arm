#ifndef __FLASH_LED_TASKS_H
#define __FLASH_LED_TASKS_H

#define FLASH_GREEN_LED_TASK_STACK_DEPTH                            128
#define FLASH_YELLOW_LED_TASK_STACK_DEPTH                           128
#define FLASH_RED_LED_TASK_STACK_DEPTH                              128
#define PUSH_BUTTON_TASK_STACK_DEPTH                                128

#define FLASH_GREEN_LED_TASK_PRIORITY                               1
#define FLASH_YELLOW_LED_TASK_PRIORITY                              2
#define FLASH_RED_LED_TASK_PRIORITY                                 3
#define PUSH_BUTTON_TASK_PRIORITY                                   4

void FlashGreenLedTask(void *arguments);
void FlashYellowLedTask(void *arguments);
void FlashRedLedTask(void *arguments);
void PushButtonTask(void *arguments);

#endif  /*  #ifndef __FLASH_LED_TASKS_H */