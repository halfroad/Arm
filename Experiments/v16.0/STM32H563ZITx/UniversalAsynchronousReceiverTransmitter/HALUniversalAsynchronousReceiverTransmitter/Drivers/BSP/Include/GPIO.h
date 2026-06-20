#ifndef __GPIO_H
#define __GPIO_H

typedef enum
{
    LED_1_GREEN,
    LED_2_YELLOW,
    LED_3_RED
    
} BUILT_IN_LED;

void InitializeGeneralPurposeInputOutput(void);
void Toggle (BUILT_IN_LED led);

#endif  // __GPIO_H