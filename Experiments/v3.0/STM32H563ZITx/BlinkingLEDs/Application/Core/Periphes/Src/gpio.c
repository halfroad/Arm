#include "stm32h5xx.h"

#include "gpio.h"

void led_gpios_init(void)
{
    // https://www.cnblogs.com/zc110747/p/10636273.html
    // Enable the AHB2 clock for GPIOB.
    RCC -> AHB2ENR |= 1 << 1;
    /*
    MODIFY_REG(RCC -> AHB2ENR, RCC_AHB2ENR_GPIOBEN, 1);
    */
    
     /*
    0b 0000 0011 << 28 = 0b00110000 00000000 00000000 00000000.
    ~(0b00110000 00000000 00000000 00000000) = 0b11001111 11111111 11111111 11111111.
    */
    
    /* Set to OUTPUT mode.
        1. Reset the Mode of MODER14[1:0] bit [28: 29].
        ~(0x03 << 28) = 0xCFFFFFFF
        0b11001111 11111111 11111111 11111111.
    
        2.  0x01 << 28) = 0x10000000
        0b00010000 00000000 00000000 00000000
    
       These bits are written by software to configure the I/O mode.
        00: Input mode
        01: General purpose output mode
        10: Alternate function mode
        11: Analog mode (reset state)
        Note: The bitfield is reserved and must be kept to reset value when the corresponding I/O is
        not available on the selected package.
    */
    GPIOB -> MODER &= ~(0x03 << 0);
    GPIOB -> MODER |= (0x01 << 0);
    
    /* Set Output type to Push-pull.
    
   These bits are written by software to configure the I/O output type.
    0: Output push-pull (reset state)
    1: Output open-drain
    Note: The bit is reserved and must be kept t

    */
    GPIOB -> OTYPER &= ~(0x01 << 0);
    
    /* Set Output speed to High Speed
    
    These bits are written by software to configure the I/O output speed.
        00: Low speed
        01: Medium speed
        10: High speed
        11: Very-high speed
        Note: Refer to the device datasheet for the frequency specifications and the power supply
        and load conditions for each speed.
        The bitfield is reserved and must be kept to reset value when the corresponding I/O is
        not available on the selected package.

    */
    GPIOB -> OSPEEDR &= ~(0x03 << 0);
    GPIOB -> OSPEEDR |= 0x02 << 0;
    
    /* Pull up and Pull down to No pull-up, no pull-down.
    
    These bits are written by software to configure the I/O pull-up or pull-down
        00: No pull-up, pull-down
        01: Pull-up
        10: Pull-down
        11: Reserved
        Note: The bitfield is reserved and must be kept to reset value when the corresponding I/O is
        not available on the selected package.

    */
    GPIOB -> PUPDR &= ~(0x03 << 0);
}
