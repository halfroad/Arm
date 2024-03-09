#include <stdio.h>
#include <string.h>

#include "../../Initializers//Include/SystemInitializer.h"
#include "../../Peripherals/Include/Delay.h"
#include "../../Peripherals/Include/LED.h"
#include "../../Peripherals/Include/OLED.h"
#include "../../Peripherals/Include/GPIO.h"
#include "../../Peripherals/Include/SHT20.h"

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
    
    InitHumidityTemperatureSensor();
    
    uint8_t i = 0;
    
    uint16_t temperature, humidity;
    
    char buffer[30];

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
                
                temperature = AcquireTemperature();
                sprintf(buffer, "Temperature: %dC", temperature);
                
                OLED_ShowString(0, 0, (uint8_t *)buffer, 8, 1);
                
                humidity = AcquireRelativeHumidity();
                sprintf(buffer, "Humidity: %d", humidity);
                
                OLED_ShowString(0, 8, (uint8_t *)buffer, 8, 1);

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
