#ifndef __LED_H
#define __LED_H

typedef enum
{
    LightChoiceRedLED,
    LightChoiceGreenLED,
    
} LightChoices;

void InitLEDs(void);

void ToggleLED(LightChoices lightChoice);

#endif  /*  #ifndef __LED_H */