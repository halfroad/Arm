#include "SystemInitializer.h"
#include "Delay.h"
#include "LED.h"
#include "BMP.h"
#include "OLED.h"

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
    InitDelay(480);
    InitLEDs();
    
    OLED_Init();
    
    uint8_t t = ' ';

    while (1)
    {
        OLED_ShowPicture(0, 0, 128, 64, BMP1, 1);
        
        OLED_Refresh();
        
        DelayMilliseconds(500);
        
        OLED_Clear();
        
        OLED_ShowChinese(0, 0, 0, 16, 1); //中
        OLED_ShowChinese(18, 0, 1, 16, 1); //景
        OLED_ShowChinese(36, 0, 2, 16, 1); //园
        OLED_ShowChinese(54, 0, 3, 16, 1); //电
        OLED_ShowChinese(72, 0, 4, 16, 1); //子
        OLED_ShowChinese(90, 0, 5, 16, 1); //技
        OLED_ShowChinese(108, 0, 6, 16, 1); //术
        
        OLED_ShowString(8, 16, (uint8_t *)"ZHONGJINGYUAN", 16, 1);
        OLED_ShowString(20, 32, (uint8_t *)"2014/05/01", 16, 1);
        OLED_ShowString(0, 48, (uint8_t *)"ASCII:", 16, 1);
        OLED_ShowString(63, 48, (uint8_t *)"CODE:", 16, 1);
        
        OLED_ShowChar(48, 48, t, 16, 1); //显示ASCII字符
        
        t++;
        
        if (t > '~')
            t = ' ';
        
        OLED_ShowNum(103, 48, t, 3, 16, 1);
        
        OLED_Refresh();
        
        DelayMilliseconds(500);
        
        OLED_Clear();
        
        OLED_ShowChinese(0, 0, 0, 16, 1);  //16*16 中
        OLED_ShowChinese(16, 0, 0, 24, 1); //24*24 中
        OLED_ShowChinese(24, 20, 0, 32, 1); //32*32 中
        OLED_ShowChinese(64, 0, 0, 64, 1); //64*64 中
        
        OLED_Refresh();
        
        DelayMilliseconds(500);
        
        OLED_Clear();
        
        OLED_ShowString(0, 0, (uint8_t *)"ABC", 8, 1); //6*8 “ABC”
        OLED_ShowString(0, 8, (uint8_t *)"ABC", 12, 1); //6*12 “ABC”
        OLED_ShowString(0, 20, (uint8_t *)"ABC", 16, 1); //8*16 “ABC”
        OLED_ShowString(0, 36, (uint8_t *)"ABC", 24, 1); //12*24 “ABC”
        
        OLED_ShowString(0, 42, (uint8_t *)"Hello World!", 16, 1); //12*24 “ABC”
        
        OLED_Refresh();
        
        DelayMilliseconds(500);
        
        OLED_ScrollDisplay(11, 4, 1);

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

        DelayMilliseconds(250);

        GPIOB->BSRR |= 0x01 << (0 + 16);
        GPIOB->BSRR |= 0x01 << (14 + 16);
        GPIOE->BSRR |= 0x01 << (1 + 16);

        DelayMilliseconds(250);
        
    }
}
