#include "SystemInitializer.h"
#include "Delay.h"
#include "LED.h"
#include "GPIO.h"
#include "IndependentWatchDog.h"

int main(void)
{
    /*
    
    User LD1: a green user LED is connected to the STM32H7 I/O PB0 (SB65 OFF and SB54
    ON) or PA5 (SB65 ON and SB54 OFF) corresponding to the ST Zio D13.
    
    */

    /*
    
    B1 user (Blue button): the user button is connected to the I/O PC13 by default (Tamper
    support, SB82 ON and SB81 OFF) or PA0 (Wakeup support, SB81 ON and SB82 OFF) of
    the STM32H7 microcontroll er.
    
    */
    
#ifdef USE_HSE
    InitSystem(1, 120, 2, 2, 2);
#else
    InitSystem(4, 60, 2, 2, 2);
#endif
    
    InitDelay(240);
    InitLEDs();
    InitGPIOs();
    
    DelayMilliseconds(100);
    
    /*
    
    Time(overflow) =  4 * 2^prescaler * reload / 32, maximum is 256.
    Time(overflow) = 4 * 2^4 * 500 / 32 = 1000ms = 1s.
    
    */
    InitIndependentWatchDog(0b00000100, 500);
    
    GPIOB -> BSRR |= 0x01 << 0;
    GPIOB -> BSRR |= 0x01 << 14;
    GPIOE -> BSRR |= 0x01 << 1;

    while (1)
    {
        if (GPIOC -> IDR & 0x01 << 13)
            FeedIndependentWatchDog();
            
        DelayMilliseconds(10);
        
    }
}
