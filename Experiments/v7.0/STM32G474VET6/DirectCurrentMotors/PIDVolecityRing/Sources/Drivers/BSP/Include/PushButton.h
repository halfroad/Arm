#ifndef __INIT_PUSH_BUTTONS_H
#define __INIT_PUSH_BUTTONS_H

typedef enum
{
    PUSH_BUTTON_NONE_PRESSED = 0x00U,
    PUSH_BUTTON_0_PRESSED,
    PUSH_BUTTON_1_PRESSED,
    PUSH_BUTTON_2_PRESSED,
    
} PushButtonPressStates; 

void InitPushButtons(void);
PushButtonPressStates ScanButton(void);

#endif /* #ifndef __INIT_PUSH_BUTTONS_H */
