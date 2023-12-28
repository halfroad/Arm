#include "Delay.h"
#include "RandomGenerator.h"

#define MAX_RETRIALS  1000

uint8_t InitRandomGenerator(void)
{    
    /*
    
    Bit 6 RNGEN: RNG peripheral clocks enable
    Set and reset by software.
        0: RNG peripheral clocks disabled (default after reset)
        1: RNG peripheral clocks enabled:
    The peripheral clocks of the RNG are: the kernel clock selected by RNGSEL and provided to
    rng_ker_ck input, and the rcc_hclk2 bus interface clock.
        
    */
    RCC -> AHB2ENR |= 0x01 << 6;
    
    /*
    
    RCC domain 2 kernel clock configuration register (RCC_D2CCIP2R) .
    
    Bits 9:8 RNGSEL[1:0]: RNG kernel clock source selection
    Set and reset by software.
        00: hsi48_ck clock is selected as kernel clock (default after reset)
        01: pll1_q_ck clock is selected as kernel clock
        10: lse_ck clock is selected as kernel clock
        11: lsi_ck clock is selected as kernel clock

    */
    RCC -> D2CCIP2R &= ~(0x03 << 8);
    RCC -> D2CCIP2R |= 0x01 << 8;
    
    /*
    
    Bit 2 RNGEN: True random number generator enable
        0: True random number generator is disabled. Analog noise sources are powered off and
        logic clocked by the RNG clock is gated.
        1: True random number generator is enabled.

    */
    RNG -> CR |= 0x01 << 2;
    
    /*
    
    Bit 0 DRDY: Data Ready
        0: The RNG_DR register is not yet valid, no random data is available.
        1: The RNG_DR register contains valid random data.
    Once the output buffer becomes empty (after reading the RNG_DR register), this bit returns to
    0 until a new random value is generated.
    Note: The DRDY bit can rise when the peripheral is disabled (RNGEN=0 in the RNG_CR
    register).
    If IE=1 in the RNG_CR register, an interrupt is generated when DRDY=1.

    */
    
    uint16_t retrials = 0;

    while (((RNG -> SR << 0) & 0x01) == 0 && retrials < MAX_RETRIALS)
    {
        retrials ++;
        
        DelayMicroseconds(100);
    }
    
    if (retrials >= MAX_RETRIALS)
        return 1;
    
    return 0;
}

uint32_t GenerateRandomNumber(void)
{
    while (((RNG -> SR << 0) & 0x01) == 0)
        ;
    
    return RNG -> DR;
}

uint32_t GenerateRangedRandomNumber(uint32_t minimum, uint32_t maximum)
{
    return GenerateRandomNumber() % (maximum - minimum + 1) + minimum;
}