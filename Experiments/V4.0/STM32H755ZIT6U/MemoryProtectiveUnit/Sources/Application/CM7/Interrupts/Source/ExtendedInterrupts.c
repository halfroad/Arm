#include "ExtendedInterrupts.h"

void ConfigureExternalInterrupt(uint8_t GPIOx, uint8_t bitNumber, uint8_t triggerEdge)
{
    /*
    
    Bit 1 SYSCFGEN: SYSCFG peripheral clock enable
    Set and reset by software.
        0: SYSCFG peripheral clock disabled (default after reset)
        1: SYSCFG peripheral clock enabled
    
    */

    RCC -> APB4ENR |= 0x01 << 1;
    
    uint8_t offset = (bitNumber % 4) * 4;
    
    /*
    
    SYSCFG external interrupt configuration register 1
    (SYSCFG_EXTICR1)
    
    Bits 15:0 EXTIx[3:0]: EXTI x configuration (x = 0 to 3)
    These bits are written by software to select the source input for the EXTI input
    for external interrupt / event detection.
        0000: PA[x] pin
        0001: PB[x] pin
        0010: PC[x] pin
        0011: PD[x] pin
        0100: PE[x] pin
        0101: PF[x] pin
        0110: PG[x] pin
        0111: PH[x] pin
        1000: PI[x] pin
        1001: PJ[x] pin
        1010: PK[x] pin
    Other configurations: reserved
    
    */
    SYSCFG -> EXTICR[bitNumber / 4] &= ~(0x0F << offset);
    SYSCFG -> EXTICR[bitNumber / 4] |= GPIOx << offset;
    
    EXTI_D1 -> IMR1 = 0x01 << bitNumber;
    
    if (triggerEdge & 0x01)
        EXTI -> FTSR1 = 0x01 << bitNumber;
    else if (triggerEdge & 0x02)
        EXTI -> RTSR1 = 0x01 << bitNumber;
    else
    {
        EXTI -> FTSR1 = 0x01 << bitNumber;
        EXTI -> RTSR1 = 0x01 << bitNumber;
    }
}