#include <stm32h7xx.h>

#include "SystemInterrupts.h"
#include "GPIO.h"
#include "OLED.h"
#include "Delay.h"

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
    
    if (!(RCC -> AHB4ENR & (0x01 << 2)))
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
}