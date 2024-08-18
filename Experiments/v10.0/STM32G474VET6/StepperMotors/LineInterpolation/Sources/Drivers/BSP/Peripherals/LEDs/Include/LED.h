#ifndef __LED_H
#define __LED_H

typedef enum
{
    RedLED,
    GreenLED,
    
} LEDs;

void InitLEDs(void);

void ToggleLED(LEDs led);

#endif  /*  #ifndef __LED_H */