#ifndef __KEY_H
#define __KEY_H

#include <stm32g4xx.h>

typedef enum
{
    PUSH_BUTTON_PRESSED_NONE,
    PUSH_BUTTON_PRESSED_0,
    PUSH_BUTTON_PRESSED_1,
    PUSH_BUTTON_PRESSED_2,
    
} PressedPushButtons;

void InitPushButtons(void);

PressedPushButtons KeyScan(void);

#endif /* #ifndef __KEY_H */