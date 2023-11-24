#include "SystemInitializer.h"
#include "delay.h"

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
    
    DelayInit(480);
    
    static uint8_t i = 0;
    
    while (1)
    {
        DelayMilliseconds(1000);
        
        i ++;
        
        if (i > 10)
        {
            i = 0;
        }
    }
}