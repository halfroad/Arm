#ifndef __LED_H
#define __LED_H

typedef enum
{
    GREEN_LED,
    YELLOW_LED,
    RED_LED,
    
} LEDs;

void InitLEDs (void);
void ToggleLED (LEDs led);

#endif