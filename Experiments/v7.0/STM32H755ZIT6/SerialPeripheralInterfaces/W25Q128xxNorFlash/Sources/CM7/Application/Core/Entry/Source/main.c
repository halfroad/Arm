#include <stdio.h>
#include <string.h>

#include "../../Initializers/Include/SystemInitializer.h"

#include "../../../../Drivers/BSP/Peripherals/LEDs/Include/LED.h"
#include "../../../../Drivers/BSP/Peripherals/LEDs/Include/GPIO.h"

#include "../../../User/Peripherals/Include/Delay.h"
#include "../../../User/Peripherals/Include/OLED.h"
#include "../../../User/Peripherals/Include/SerialPeripheralInterface.h"

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
    
    InitSerialPeripheralInterface();
    
    uint8_t receivedByte;
    char *bytes = { "Hello, world!" };
    uint8_t length = sizeof bytes;
    
    uint8_t i = 0;
    
    while (1)
    {
        /*  PC13 Blue Push Button.  */
        
        if (0x2000 == (GPIOC -> IDR & 0x2000))
        {
            DelayMs(20);
            
            if (0x2000 == (GPIOC -> IDR & 0x2000))
            {
                while (0x2000 == (GPIOC -> IDR & 0x2000))
                    ;
                
                //while (length --)
                    Transceive(8, 0xDD);
            }
        }
        
        OLED_ShowString(0, 0, (uint8_t *)"Hit blue key please:\n", 8, 1);
        
        OLED_Refresh();
    
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
