#include <stdio.h>

#include "../../Initializers//Include/SystemInitializer.h"
#include "../../Peripherals/Include/Delay.h"
#include "../../Peripherals/Include/LED.h"
#include "../../Peripherals/Include/OLED.h"
#include "../../Peripherals/Include/GPIO.h"
#include "../../Peripherals/Include/AnalogyDigitalConversion.h"
#include "../../Peripherals/Include/DigitalAnalogyConversion.h"

#define ADC1_CHANNEL     19

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
    
    InitGPIOs();
    
    InitAnalogyDigitalConversion();
    InitDigitalAnalogyConversion();
    
    uint8_t counter = 0;
    
    float DACVoltage, ADCVoltage = 0.0;
    
    char buffer[30];
    
    while (1)
    {
        /*
        PC13 Blue Push Button.
        */
        if (0x2000 == (GPIOC -> IDR & 0x2000))
        {
            DelayMilliseconds(10);
            
            if (0x2000 == (GPIOC -> IDR & 0x2000))
            {
                while (0x2000 == (GPIOC -> IDR & 0x2000))
                    ;
                
                if (DAC1 -> DHR12R1 < 4000)
                    DAC1 -> DHR12R1 += 200;
            }
        }
        
        if (counter == 1)
        {
            OLED_Clear();
            
            sprintf(buffer, "Register Value: %d.", DAC1 -> DHR12R1);
            OLED_ShowString(0, 8, (uint8_t *)buffer, 8, 1);
            
            DACVoltage = (float)DAC1 -> DHR12R1 * (3.3 / 4096);
            
            sprintf(buffer, "DAC Voltage: %.1fv", DACVoltage);
            OLED_ShowString(0, 20, (uint8_t *)buffer, 8, 1);
            
            ADCVoltage = (float)GetAverageAnalogyDigitalConversion(ADC1_CHANNEL, 20) * (3.3 / 65536);
            
            sprintf(buffer, "ADC Voltage: %.1fv", ADCVoltage);
            OLED_ShowString(0, 34, (uint8_t *)buffer, 8, 1);
            
            OLED_Refresh();
            
            counter = 0;
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
        
        GPIOB->BSRR |= 0x01 << 0;
        GPIOB->BSRR |= 0x01 << 14;
        GPIOE->BSRR |= 0x01 << 1;

        DelayMilliseconds(250);

        GPIOB->BSRR |= 0x01 << (0 + 16);
        GPIOB->BSRR |= 0x01 << (14 + 16);
        GPIOE->BSRR |= 0x01 << (1 + 16);

        DelayMilliseconds(250);
        
        counter ++;
    }
}
