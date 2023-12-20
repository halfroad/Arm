#include "SystemInitializer.h"
#include "Delay.h"
#include "GPIO.h"
#include "OLED.h"
#include "InputCaptures.h"

extern uint8_t TIM5_CHANNEL1_CAPTURE_STATUS;
extern uint32_t TIM5_CHANNEL1_CAPTURE_VALUE;

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
    InitGPIOs();
    OLED_Init();
    
    /* 500ms to reload. */
    InitInputCaptures(0xFFFFFFFF, 240 - 1);
    
    uint32_t duration = 0;

    while (1)
    {
        DelayMilliseconds(10);
        
        if (TIM5_CHANNEL1_CAPTURE_STATUS & 0x80)
        {
            duration = TIM5_CHANNEL1_CAPTURE_STATUS & 0x3F;
            duration *= 0xFFFFFFFF;
            duration += TIM5_CHANNEL1_CAPTURE_VALUE;
            
            TIM5_CHANNEL1_CAPTURE_STATUS = 0;
        }
        
        OLED_ShowString(8, 16, (uint8_t *)"Duration: ", 8, 1);
        OLED_ShowNum(8, 28, duration, 5, 8, 1);
        
        OLED_Refresh();
    }
}
