#include "LED.h"

void InitLEDs(void)
{
    /* Enable the RCC clock for GPIOB on AHB4.
    
    Bit 1 GPIOBEN: GPIOB peripheral clock enable
    Set and reset by software.
        0: GPIOB peripheral clock disabled (default after reset)
        1: GPIOB peripheral clock enabled
    */
    RCC -> AHB4ENR |= 0x01 << 1;
    
    /*
    
    Bits 31:0 MODER[15:0][1:0]: Port x configuration I/O pin y (y = 15 to 0)
    These bits are written by software to configure the I/O mode.
        00: Input mode
        01: General purpose output mode
        10: Alternate function mode
        11: Analog mode (reset state)
    
    Green LED is connected to PB0.
    Red LED is connected to PB14.
    */
    
    GPIOB -> MODER &= ~((0x03 << 0) | (0x03 << 14 * 2));
    GPIOB -> MODER |= (0x01 << 0) | (0x01 << 14 * 2);
    
    /*
    
    Bits 15:0 OT[15:0]: Port x configuration I/O pin y (y = 15 to 0)
    These bits are written by software to configure the I/O output type.
        0: Output push-pull (reset state)
        1: Output open-drain

    */
    GPIOB -> OTYPER &= ~((0x01 << 0) | (0x01 << 14));
    
    /*
    
    Bits 31:0 OSPEEDR[15:0][1:0]: Port x configuration I/O pin y (y = 15 to 0)
    These bits are written by software to configure the I/O output speed.
        00: Low speed
        01: Medium speed
        10: High speed
        11: Very high speed
    Note: Refer to the product datasheets for the values of OSPEEDRy bits versus VDD range
    and external load.
    
    */
    GPIOB -> OSPEEDR &= ~((0x03 << 0) | (0x03 << 14 * 2));
    GPIOB -> OSPEEDR |= ~((0x02 << 0) | (0x02 << 14 * 2));
    
    /*
    
    Bits 31:0 PUPDR[15:0][1:0]: Port x configuration I/O pin y (y = 15 to 0)
    These bits are written by software to configure the I/O pull-up or pull-down
        00: No pull-up, pull-down
        01: Pull-up
        10: Pull-down
        11: Reserved
    
    */
    GPIOB -> PUPDR &= ~((0x03 << 0) | (0x03 << 14 * 2));
    
    /*
    
    Bit 4 GPIOEEN: GPIOE peripheral clock enable
    Set and reset by software.
        0: GPIOE peripheral clock disabled (default after reset)
        1: GPIOE peripheral clock enabled

    */
    RCC -> AHB4ENR |= 0x01 << 4;
    
     /*
    
    Bits 31:0 MODER[15:0][1:0]: Port x configuration I/O pin y (y = 15 to 0)
    These bits are written by software to configure the I/O mode.
        00: Input mode
        01: General purpose output mode
        10: Alternate function mode
        11: Analog mode (reset state)
    
    Yellow LED is connected to PE1.
    */
    GPIOE -> MODER &= ~(0x03 << 2);
    GPIOE -> MODER |= 0x01 << 2;
    
    /*
    
    Bits 15:0 OT[15:0]: Port x configuration I/O pin y (y = 15 to 0)
    These bits are written by software to configure the I/O output type.
        0: Output push-pull (reset state)
        1: Output open-drain

    */
    GPIOE -> OTYPER &= ~(0x01 << 1);
    
     /*
    
    Bits 31:0 OSPEEDR[15:0][1:0]: Port x configuration I/O pin y (y = 15 to 0)
    These bits are written by software to configure the I/O output speed.
        00: Low speed
        01: Medium speed
        10: High speed
        11: Very high speed
    Note: Refer to the product datasheets for the values of OSPEEDRy bits versus VDD range
    and external load.
    
    */
    GPIOE -> OSPEEDR &= ~(0x03 << 2);
    GPIOE -> OSPEEDR |= ~(0x02 << 2);
    /*
    
    Bits 31:0 PUPDR[15:0][1:0]: Port x configuration I/O pin y (y = 15 to 0)
    These bits are written by software to configure the I/O pull-up or pull-down
        00: No pull-up, pull-down
        01: Pull-up
        10: Pull-down
        11: Reserved
    
    */
    GPIOE -> PUPDR &= ~(0x03 << 2);
}
