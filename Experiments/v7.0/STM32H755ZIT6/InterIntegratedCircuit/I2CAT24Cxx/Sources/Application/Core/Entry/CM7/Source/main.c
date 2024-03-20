#include <stdio.h>
#include <string.h>

#include "../../../Initializers/CM7/Include/SystemInitializer.h"

#include "../../../../../Drivers/BSP/LEDs/CM7/Include/LED.h"
#include "../../../../../Drivers/BSP/LEDs/CM7/Include/GPIO.h"

#include "../../../../User/Peripherals/CM7/Include/Delay.h"
#include "../../../../User/Peripherals/CM7/Include/AT24Cxx.h"
#include "../../../../User/Peripherals/CM7/Include/OLED.h"



const uint8_t TEXT_TO_WRITE [] = { "Hello, AT24Cxx!\n"};

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
    InitDelay();
    InitGPIOs();
    InitLEDs();
    OLED_Init();
    
    InitAT24Cxx();
    
    uint8_t i = 0;
    uint8_t written = 0;
    uint8_t writtenWords[sizeof TEXT_TO_WRITE];
    
    while (1)
    {
        /*  PC13 Blue Push Button.  */
        
        if (0x2000 == (GPIOC -> IDR & 0x2000))
        {
            DelayMs(10);
            
            if (0x2000 == (GPIOC -> IDR & 0x2000))
            {
                while (0x2000 == (GPIOC -> IDR & 0x2000))
                    ;
                
                OLED_Clear();
                
                OLED_ShowString(0, 0, (uint8_t *)"Hit blue key please.\n", 8, 1);

                if (AT24CxxValidate(255, 0xAA))
                {
                    OLED_ShowString(0, 10, (uint8_t *)"No AT24Cxx device.\n", 8, 1);
                }
                else
                {
                    if (written)
                    {
                        OLED_ShowString(0, 10, (uint8_t *)"Wrote alreay, read:\n", 8, 1);
                        
                        AT24CxxReadBytes(0, writtenWords, sizeof TEXT_TO_WRITE);
                        
                        OLED_ShowString(0, 20, writtenWords, 8, 1);
                    }
                    else
                    {
                        OLED_ShowString(0, 10, (uint8_t *)"Detected, writing...\n", 8, 1);
                        
                        AT24CxxWriteBytes(0, (uint8_t *)TEXT_TO_WRITE, sizeof TEXT_TO_WRITE);
                        OLED_ShowString(0, 20, (uint8_t *)"Writing Done.\n", 8, 1);
                        written = 1;
                    }
                    
                }
                
                OLED_Refresh();
            }
        }
        
        DelayMs(10);
        
        if (i ++ == 25)
        {
            static uint8_t on = 0;
            
            on = on ? 0: 1;
        
            GPIOB -> ODR &= ~(0x01 << 0);
            GPIOB -> ODR |= on << 0;
            
            i = 0;
        }
    }
}
