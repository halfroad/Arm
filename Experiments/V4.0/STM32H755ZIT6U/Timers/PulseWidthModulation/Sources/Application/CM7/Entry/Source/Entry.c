#include "SystemInitializer.h"
#include "Delay.h"
#include "LED.h"
#include "OLED.h"
#include "PulseWidthModulation.h"

int main(void)
{
    /*
    
    User LD1: a green user LED is connected to the STM32H7 I/O PB0 (SB65 OFF and SB54
    ON) or PA5 (SB65 ON and SB54 OFF) corresponding to the ST Zio D13.
    
    */

    /*
    
    B1 user (Blue button): the user button is connected to the I/O PC13 by default (Tamper
    support, SB82 ON and SB81 OFF) or PA0 (Wakeup support, SB81 ON and SB82 OFF) of
    the STM32H7 microcontroller.
    
    */
    
#ifdef USE_HSE
    InitSystem(1, 120, 2, 2, 2);
#else
    InitSystem(4, 60, 2, 2, 2);
#endif
    InitDelay(240);
    InitLEDs();
    
    OLED_Init();
    
    /* 500ms to reload. */
    InitPulseWdithModulation(500 - 1, 240 - 1);
    
    uint16_t pwm = 0;
    uint8_t direction = 1;

    while (1)
    {
        DelayMilliseconds(10);
        
        if (direction)
            pwm ++;
        else
            pwm --;
        
        if (pwm == 0)
            direction = 1;
        else if (pwm == 300)
            direction = 0;
        
        OLED_ShowNum(8, 16, pwm, 5, 8, 1);
        OLED_Refresh();
        
        SetCaptureCompare(pwm);
    }
}
