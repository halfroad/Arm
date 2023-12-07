#include "SystemInterrupts.h"
#include "WindowWatchDog.h"

#define DOWNCOUNTER     0x7F

/*

Precaler: Bits 13:11 WDGTB[2:0]: Timer base
Frequency(wwdg) = pclk3 / (4096 * 2^prescaler)
Time(wwdg) = 4096 * 2^prescaler * (counter - 0x3F) / pclk3

*/
void InitWindowWatchDog(uint8_t prescaler, uint8_t counter, uint8_t window)
{
    /*
    
    0x50003000 - 0x50003FFF WWDG1
    APB3
    (D1)
    
    
    Bit 6 WWDG1EN: WWDG1 Clock Enable
    Set by software, and reset by hardware when a system reset occurs.
    In order to work properly, before enabling the WWDG1, the bit WW1RSC must be set to ¡®1¡¯.
        0: WWDG1 peripheral clock disable (default after reset)
        1: WWDG1 peripheral clock enabled
    There is no protection to prevent the CPU2 to set the bit WWDG1EN to ¡¯1¡¯.
    It is not recommended to enable the WWDG1 clock for CPU2 operating modes in
    RCC_C2_APB3ENR register. 
    
    */
    
    RCC -> APB3ENR |= 0x01 << 6;
    
    /*
    
    Bit 0 WW1RSC: WWDG1 reset scope control
    This bit can be set by software but is cleared by hardware during a system reset
        0: The WWDG1 generates a reset of CPU1, when a timeout occurs. (default after reset)
        1: The WWDG1 generates a system reset, when a timeout occurs. 
        
    */
    
    RCC -> GCR |= 0x01 << 0;
    
    /*
    
    Bits 13:11 WDGTB[2:0]: Timer base
    The timebase of the prescaler can be modified as follows:
        000: CK Counter Clock (PCLK div 4096) div 1
        001: CK Counter Clock (PCLK div 4096) div 2
        010: CK Counter Clock (PCLK div 4096) div 4
        011: CK Counter Clock (PCLK div 4096) div 8
        100: CK Counter Clock (PCLK div 4096) div 16
        101: CK Counter Clock (PCLK div 4096) div 32
        110: CK Counter Clock (PCLK div 4096) div 64
        111: CK Counter Clock (PCLK div 4096) div 128
    
    WWDG1 -> CFR &= ~(0x07 << 11);
    */
    WWDG1 -> CFR &= 0xFF80; 
    WWDG1 -> CFR |= (prescaler << 11);
    
    /*
    
    Bits 6:0 W[6:0]: 7-bit window value
    These bits contain the window value to be compared with the down-counter.
    
    */
    
    WWDG1 -> CFR &= ~(0x7F << 6);
    WWDG1 -> CFR |= window;
    
    /*
    
    Bit 9 EWI: Early wakeup interrupt
    When set, an interrupt occurs whenever the counter reaches the value 0x40. This interrupt is
    only cleared by hardware after a reset.   
    
    */
    
    WWDG1 -> CFR |= 0x01 << 9;
    
    /*
    Bits 6:0 T[6:0]: 7-bit counter (MSB to LSB)
    These bits contain the value of the watchdog counter, decremented every
    (4096 x 2^WDGTB[2:0]) PCLK cycles. A reset is produced when it is decremented from 0x40 to
    0x3F (T6 becomes cleared).

    */
    WWDG1 -> CR |= DOWNCOUNTER;
    
        /*
    
    Bit 7 WDGA: Activation bit
    This bit is set by software and only cleared by hardware after a reset. When WDGA = 1, the
    watchdog can generate a reset.
        0: Watchdog disabled
        1: Watchdog enabled
    */
    
    WWDG1 -> CR |= 0x01 << 7;
    
    SetNestedVectoredInterruptPriorty(2, 2, WWDG_IRQn);
    
    /*
    
    Bit 0 EWIF: Early wakeup interrupt flag
    This bit is set by hardware when the counter has reached the value 0x40. It must be cleared
    by software by writing ¡®0¡¯. Writing ¡®1¡¯ has no effect. This bit is also set if the interrupt is not
    enabled. 

    */
    WWDG1 -> SR = 0x00;
}

static void SetWindowWatchDogCounter(uint8_t newCounter)
{
    WWDG1 -> CR = newCounter & 0x7F;
}

void WWDG_IRQHandler(void)
{
    static uint8_t LEDStatus = 0;
    
    /* Check whether a Early Wakeup Interrupt occurs. */
    if (WWDG1 -> SR & 0x01)
    {
        /* Clear Early Wakeup Flag. */
        WWDG1 -> SR = 0x00;
        
        SetWindowWatchDogCounter(DOWNCOUNTER);
        
        LEDStatus ^= 1;
    }
    
    if (LEDStatus)
    {
        GPIOB -> BSRR |= 0x01 << 0;
        GPIOB -> BSRR |= 0x01 << 14;
        GPIOE -> BSRR |= 0x01 << 1;
    }
    else
    {
        GPIOB -> BSRR |= 0x01 << (0 + 16);
        GPIOB -> BSRR |= 0x01 << (14 + 16);
        GPIOE -> BSRR |= 0x01 << (1 + 16);
    }
}