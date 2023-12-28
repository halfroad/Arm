#include <stdio.h>

#include "SystemInitializer.h"
#include "Delay.h"
#include "LED.h"
#include "OLED.h"
#include "RandomGenerator.h"

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
    InitRandomGenerator();
    
    char buffer[40];
    
    while (1)
    {
        uint32_t randomNumber = GenerateRandomNumber();
        uint32_t rangedRandomNumber = GenerateRangedRandomNumber(100, 999);
        
        sprintf(buffer, "Random: %02d", randomNumber);
        
        OLED_ShowString(0, 8, (uint8_t *)buffer, 8, 1);
        
        sprintf(buffer, "Random Ranged: %02d", rangedRandomNumber);
        
        OLED_ShowString(0, 16, (uint8_t *)buffer, 8, 1);
        
        OLED_Refresh();
        
        DelayMilliseconds(100);

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
        
        GPIOB->BSRR |= 0x01 << 0;
        GPIOB->BSRR |= 0x01 << 14;
        GPIOE->BSRR |= 0x01 << 1;

        DelayMilliseconds(500);

        GPIOB->BSRR |= 0x01 << (0 + 16);
        GPIOB->BSRR |= 0x01 << (14 + 16);
        GPIOE->BSRR |= 0x01 << (1 + 16);

        DelayMilliseconds(500);
        
    }
}
