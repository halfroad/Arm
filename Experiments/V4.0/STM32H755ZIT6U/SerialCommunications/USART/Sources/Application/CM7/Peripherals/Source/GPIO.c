#include <stm32h7xx.h>

#include "SystemInterrupts.h"
#include "ExtendedInterrupts.h"
#include "GPIO.h"
#include "OLED.h"
#include "Delay.h"

uint8_t buttonPressed = 0x00;

void InitGPIOs(void)
{
    /*
    
    B1 user (Blue button): the user button is connected to the I/O PC13 by default (Tamper
    support, SB82 ON and SB81 OFF) or PA0 (Wakeup support, SB81 ON and SB82 OFF) of
    the STM32H7 microcontroller.
        
    Bit 2 GPIOCEN: GPIOC peripheral clock enable
    Set and reset by software.
        0: GPIOC peripheral clock disabled (default after reset)
        1: GPIOC peripheral clock enabled
    */
    RCC -> AHB4ENR |= 0x01 << 2;
    
    /*
    
    Bits 31:0 MODER[15:0][1:0]: Port x configuration I/O pin y (y = 15 to 0)
    These bits are written by software to configure the I/O mode.
        00: Input mode
        01: General purpose output mode
        10: Alternate function mode
        11: Analog mode (reset state)
    
    */
    GPIOC -> MODER &= ~(0x03 << 13 * 2);
    
    /*
    
    Bits 31:0 PUPDR[15:0][1:0]: Port x configuration I/O pin y (y = 15 to 0)
    These bits are written by software to configure the I/O pull-up or pull-down
        00: No pull-up, pull-down
        01: Pull-up
        10: Pull-down
        11: Reserved
    
    */
    GPIOC -> PUPDR &= ~(0x03 << 13 * 2);
    GPIOC -> PUPDR |= 0x02 << 13 * 2;
    
    SetNestedVectoredInterruptPriorty(3, 2, EXTI15_10_IRQn);
    ConfigureExternalInterrupt(0x02, 13, 0x00);
}

void EXTI15_10_IRQHandler(void)
{
    EXTI_D1 -> PR1 = 0x01 << 13;
    
    OLED_Clear();
    
    if (buttonPressed & 0x80)
    {
        OLED_ShowString(8, 32, (uint8_t *)"Button released.", 8, 1);
        
        buttonPressed = 0x00;
    }
    else
    {
        OLED_ShowString(8, 32, (uint8_t *)"Button pressed.", 8, 1);
        
        buttonPressed = 0x80;
    }
    
    OLED_Refresh();
    
    DelayMilliseconds(100);
}