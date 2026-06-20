#include <stm32h5xx.h>

#include "../Include/SystemConfigurations.h"

#define GREEN_LED_GPIO_PORT                                 GPIOB
#define GREEN_LED_GPIO_PIN                                  GPIO_PIN_0

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    
    /*
    
    11.8.27 RCC AHB2 peripheral clock register (RCC_AHB2ENR)
    Address offset: 0x08C
    Reset value: 0xC000 0000
    
    */
    
    RCC -> AHB2ENR |= (0x01 << 1);
    
    /*
    
    13.4.1 GPIO port mode register (GPIOx_MODER) (x = A to I)
    
    31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16
    MODE15[1:0] MODE14[1:0] MODE13[1:0] MODE12[1:0] MODE11[1:0] MODE10[1:0] MODE9[1:0] MODE8[1:0]
    rw rw rw rw rw rw rw rw rw rw rw rw rw rw rw rw
    15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
    MODE7[1:0] MODE6[1:0] MODE5[1:0] MODE4[1:0] MODE3[1:0] MODE2[1:0] MODE1[1:0] MODE0[1:0]
    rw rw rw rw rw rw rw rw rw rw rw rw rw rw rw rw
    
    Bits 31:0MODEy[1:0]: Port x configuration I/O pin y (y = 15 to 0)
    These bits are written by software to configure the I/O mode.
        00: Input mode
        01: General purpose output mode
        10: Alternate function mode
        11: Analog mode (reset state)
    Note: The bitfield is reserved and must be kept to reset value when the corresponding I/O is 
    not available on the selected package.

    
    */
    
    GREEN_LED_GPIO_PORT -> MODER &= ~(0x03 << 0);
    GREEN_LED_GPIO_PORT -> MODER |= (0x01 << 0);
    
    
    while (1)
    {
        /*
        
        13.4.7 GPIO port bit set/reset register (GPIOx_BSRR) (x = A to I)
        
        31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16
        BR15 BR14 BR13 BR12 BR11 BR10 BR9 BR8 BR7 BR6 BR5 BR4 BR3 BR2 BR1 BR0
        w w w w w w w w w w w w w w w w
        15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0
        BS15 BS14 BS13 BS12 BS11 BS10 BS9 BS8 BS7 BS6 BS5 BS4 BS3 BS2 BS1 BS0
        w w w w w w w w w w w w w w w w
        
        Bits 31:16BRy: Port x reset I/O pin y (y = 15 to 0)
        These bits are write-only. A read to these bits returns the value 0x0000.
            0: No action on the corresponding ODy bit
            1: Resets the corresponding ODy bit
        Note: If both BSy and BRy are set, BSy has priority.
        The bit is reserved and must be kept to reset value when the corresponding I/O is not 
        available on the selected package.
        
        Bits 15:0BSy: Port x set I/O pin y (y = 15 to 0)
        These bits are write-only. A read to these bits returns the value 0x0000.
            0: No action on the corresponding ODy bit
            1: Sets the corresponding ODy bit
        Note: The bit is reserved and must be kept to reset value when the corresponding I/O is not 
        available on the selected package.

        
        */
        GREEN_LED_GPIO_PORT -> BSRR &= ~(0x01 << 0);
        GREEN_LED_GPIO_PORT -> BSRR |= 0x01 << 0;
        
        HAL_Delay(250);
        
        GREEN_LED_GPIO_PORT -> BSRR &= ~(0x01 << (0 + 16));
        GREEN_LED_GPIO_PORT -> BSRR |= 0x01 << (0 + 16);
        
        HAL_Delay(250);
    }
}
