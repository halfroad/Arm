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
        
    Bit 0 GPIOAEN: GPIOA peripheral clock enable
    Set and reset by software.
        0: GPIOA peripheral clock disabled (default after reset)
        1: GPIOA peripheral clock enabled
        */
    RCC -> AHB4ENR |= 0x01 << 0;
    
    /*
    
    Bits 31:0 MODER[15:0][1:0]: Port x configuration I/O pin y (y = 15 to 0)
    These bits are written by software to configure the I/O mode.
        00: Input mode
        01: General purpose output mode
        10: Alternate function mode
        11: Analog mode (reset state)
    
    */
    GPIOA -> MODER &= ~(0x03 << 0 * 2);
    GPIOA -> MODER |= 0x02 << 0 * 2;
    
    /*
    
    Bits 15:0 OT[15:0]: Port x configuration I/O pin y (y = 15 to 0)
    These bits are written by software to configure the I/O output type.
        0: Output push-pull (reset state)
        1: Output open-drain

    */
    
    GPIOA -> OTYPER &= ~(0x01 << 0);
    
    /*
    
    Bits 31:0 PUPDR[15:0][1:0]: Port x configuration I/O pin y (y = 15 to 0)
    These bits are written by software to configure the I/O pull-up or pull-down
        00: No pull-up, pull-down
        01: Pull-up
        10: Pull-down
        11: Reserved
    
    */
    GPIOA -> PUPDR &= ~(0x03 << 0 * 2);
    GPIOA -> PUPDR |= 0x02 << 0 * 2;
    
    /*
    
    /Product%20Specifications/stm32h755zi.pdf
    Table 9. Port A alternate functions
    
    Port    AF2
    PA0     TIM5_CH1
    
    
    Bits 31:0 AFR[7:0][3:0]: Alternate function selection for port x I/O pin y (y = 7 to 0)
    These bits are written by software to configure alternate function I/Os.
        0000: AF0
        0001: AF1
        0010: AF2
        0011: AF3
        0100: AF4
        0101: AF5
        0110: AF6
        0111: AF7
        1000: AF8
        1001: AF9
        1010: AF10
        1011: AF11
        1100: AF12
        1101: AF13
        1110: AF14
        1111: AF15

    Bits 31:0 AFR[15:8][3:0]: Alternate function selection for port x I/O pin y (y = 15 to 8)
    These bits are written by software to configure alternate function I/Os.
        0000: AF0
        0001: AF1
        0010: AF2
        0011: AF3
        0100: AF4
        0101: AF5
        0110: AF6
        0111: AF7
        1000: AF8
        1001: AF9
        1010: AF10
        1011: AF11
        1100: AF12
        1101: AF13
        1110: AF14
        1111: AF15
        
    */
    GPIOA -> AFR[0] &= ~(0x0F << 0);
    GPIOA -> AFR[0] |= 0x02 << 0;
}