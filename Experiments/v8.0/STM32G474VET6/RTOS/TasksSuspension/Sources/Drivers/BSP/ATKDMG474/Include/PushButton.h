#ifndef PUSH_BUTTONS_H
#define PUSH_BUTTONS_H

typedef enum
{
    KEY_NONE_PRESSED = 0x00U,
    KEY_0_PRESSED,
    KEY_1_PRESSED,
    KEY_2_PRESSED,
    
} KeyPressStates; 

void InitPushButtons(void);
KeyPressStates ScanButton(void);

#endif /* #ifndef __INIT_PUSH_BUTTONS_H */
