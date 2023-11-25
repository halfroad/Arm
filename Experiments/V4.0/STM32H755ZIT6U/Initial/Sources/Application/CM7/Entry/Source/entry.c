#include "SystemInitializer.h"
#include "delay.h"
#include "LED.h"

extern uint32_t SystemCoreClock;

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
    
    InitSystem(4, 60, 2, 2, 2);
    InitDelay(240);
    InitLEDs();
        
    while (1)
    {
        /*
    
        Bits 31:16 BR[15:0]: Port x reset I/O pin y (y = 15 to 0)
        These bits are write-only. A read to these bits returns the value 0x0000.
            0: No action on the corresponding ODRx bit
            1: Resets the corresponding ODRx bit
        Note: If both BSx and BRx are set, BSx has priority.
        
        Bits 15:0 BS[15:0]: Port x set I/O pin y (y = 15 to 0)
        These bits are write-only. A read to these bits returns the value 0x0000.
            0: No action on the corresponding ODRx bit
            1: Sets the corresponding ODRx bit
        
        */
        GPIOB -> BSRR |= 0x01 << 0;
        GPIOB -> BSRR |= 0x01 << 14;
        GPIOE -> BSRR |= 0x01 << 1;
        
        DelayMilliseconds(500);
        
        GPIOB -> BSRR |= 0x01 << 16;
        GPIOB -> BSRR |= 0x01 << 30;
        GPIOE -> BSRR |= 0x01 << 17;
        
        DelayMilliseconds(500);
    }
}