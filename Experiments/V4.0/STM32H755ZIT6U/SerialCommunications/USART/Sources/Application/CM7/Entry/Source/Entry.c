#include "SystemInitializer.h"
#include "Delay.h"
#include "LED.h"
#include "BMP.h"
#include "OLED.h"
#include "GPIO.h"
#include "USART.h"

void Init(void);

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
    
    
    Init();
    
    uint8_t buffer[] = "Hello, world!\n";
    
    while (1)
    {
        for (uint8_t i = 0; i < sizeof buffer; i ++)
        {
            SendByte(buffer[i]);
        }
        
        DelayMilliseconds(500);
        
    }
}

void onReceived(uint8_t *buffer, uint8_t length)
{
    OLED_Clear();
    
    OLED_ShowString(8, 16, buffer, 16, 1);
    
    OLED_Refresh();
}

void Init(void)
{
#ifdef USE_HSE
    InitSystem(1, 120, 2, 15, 2);
#else
    InitSystem(4, 60, 2, 2, 2);
#endif
    
    InitDelay(240);
    InitLEDs();
    OLED_Init();
    InitGPIOs();
    InitUSART(60, 115200, onReceived);
}
