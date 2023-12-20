#include <stdio.h>

#include "SystemInitializer.h"
#include "Delay.h"
#include "LED.h"
#include "BMP.h"
#include "OLED.h"
#include "RealTimeClock.h"

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
    InitRealTimeClock();
    RealTimeClockPeriodicWakeup(4, 0);
    
    uint8_t year, month, day, weekday, hour, minute, second, ampm, buffer[250], counter = 0;

    while (1)
    {
        counter ++;
        
        if (counter % 10 == 0)
        {
            RealTimeClockGetDate(&year, &month, &day, &weekday);
            RealTimeClockGetTime(&hour, &minute, &second, &ampm);
            
            sprintf((char *) buffer, "Date: 20%02d-%02d-%02d", year, month, day);
            
            OLED_ShowString(0, 8, buffer, 8, 1);
            
            sprintf((char *) buffer, "Week: %02d", weekday);
            
            OLED_ShowString(0, 24, buffer, 8, 1);
            
            sprintf((char *) buffer, "Time: %02d:%02d:%02d", hour, minute, second);
            
            OLED_ShowString(0, 40, buffer, 8, 1);
            
            OLED_Refresh();
            
            DelayMilliseconds(10);
        }

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
        
    }
}

void RTC_Alarm_IRQHandler(void)
{
    /*
    
    Bit 8 ALRAF: Alarm A flag
    This flag is set by hardware when the time/date registers (RTC_TR and RTC_DR) match the
    Alarm A register (RTC_ALRMAR).
    This flag is cleared by software by writing 0.
    
    */
    if (RTC -> ISR & (0x01 << 8))
    {
        RTC -> ISR &= ~(0x01 << 8);
    
        OLED_ShowString(0, 24, (uint8_t *)"Alarm A triggered.", 16, 1);
        OLED_Refresh();
    }
    
    EXTI -> PR1 |= 0x01 << 17;
}

void RTC_WKUP_IRQHandler(void)
{
    /*
    
    Bit 10 WUTF: Wakeup timer flag
    This flag is set by hardware when the wakeup auto-reload counter reaches 0.
    This flag is cleared by software by writing 0.
    This flag must be cleared by software at least 1.5 RTCCLK periods before WUTF is set to 1
    again.
    
    */
    if (RTC -> ISR & (0x01 << 10))
    {
        RTC -> ISR &= ~(0x01 << 10);
        
        OLED_ShowString(0, 32, (uint8_t *)"Wake Up triggered.", 16, 1);
        OLED_Refresh();
    }
    
    EXTI -> PR1 |= 0x01 << 19;
}
