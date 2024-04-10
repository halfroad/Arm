#include <stm32h7xx.h>

#include "./Peripherals/CM7/Include/ClockOutput.h"

void InitClockOutput(void)
{
   // RCC -> APB2ENR
    
    /*
    
    Bits 24:22 MCO1SEL[2:0]: Micro-controller clock output 1
    Set and cleared by software. Clock source selection may generate glitches on MCO1.
    It is highly recommended to configure these bits only after reset, before enabling the external
    oscillators and the PLLs.
        000: HSI clock selected (hsi_ck) (default after reset)
        001: LSE oscillator clock selected (lse_ck)
        010: HSE clock selected (hse_ck)
        011: PLL1 clock selected (pll1_q_ck)
        100: HSI48 clock selected (hsi48_ck)
    others: reserved
    
    */
    
    RCC -> CFGR &= ~(0x07 << 22);
    RCC -> CFGR |= 0x02 << 22;
    
    
    /*
    
    Bits 21:18 MCO1PRE[3:0]: MCO1 prescaler
    Set and cleared by software to configure the prescaler of the MCO1. Modification of this prescaler
    may generate glitches on MCO1. It is highly recommended to change this prescaler only after reset,
    before enabling the external oscillators and the PLLs.
        0000: prescaler disabled (default after reset)
        0001: division by 1 (bypass)
        0010: division by 2
        0011: division by 3
        0100: division by 4
        ...
        1111: division by 15
    
    */
    RCC -> CFGR &= ~(0x0F << 18);
    RCC -> CFGR |= 0x0F << 18;

    /*
    
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
    
    GPIOA -> MODER &= ~(0x03 << 8 * 2);
    GPIOA -> MODER |= 0x02 << 8 * 2;
    
    /*
    
    STM32H755ZIT6/Product Specifications/stm32h755zi.pdf
    Table 9. Port A alternate functions
    
    Port    AF0
    PA8     MCO1
    
    GPIO alternate function high register (GPIOx_AFRH)
    (x = A to J)
    Address offset: 0x24
    Reset value: 0x0000 0000
    
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
    GPIOA -> AFR[1] &= ~(0x0F << 0 * 4);
}