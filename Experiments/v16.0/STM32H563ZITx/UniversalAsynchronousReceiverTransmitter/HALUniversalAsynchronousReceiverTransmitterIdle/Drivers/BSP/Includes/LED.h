#pragma once

typedef enum
{
    GREEN_LED,
    YELLOW_LED,
    RED_LED,
    
} LEDs;

void InitLEDs(void);
void Toggle (LEDs led);