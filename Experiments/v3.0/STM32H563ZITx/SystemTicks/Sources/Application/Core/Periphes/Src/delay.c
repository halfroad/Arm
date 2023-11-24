#include "stm32h5xx.h"

#include "delay.h"


static uint16_t fac_microseconds = 0;

void delay_init(uint16_t ticks)
{
     /* __IOM uint32_t CTRL;                   !< Offset: 0x000 (R/W)  SysTick Control and Status Register */
     /* __IOM uint32_t LOAD;                   !< Offset: 0x004 (R/W)  SysTick Reload Value Register */
     /* __IOM uint32_t VAL;                    !< Offset: 0x008 (R/W)  SysTick Current Value Register */
     /* __IM  uint32_t CALIB;                  !< Offset: 0x00C (R/ )  SysTick Calibration Register */
    
    SysTick -> CTRL |= 0x01 << 2;
    
    fac_microseconds = ticks;
}

void delay_us(uint32_t numberOfMicroseconds)
{
    uint32_t times;
    
    SysTick -> LOAD = numberOfMicroseconds * fac_microseconds;  /* Load the durartion of delay. */
    SysTick -> VAL = 0x00;                                      /* Clear the counter. */
    SysTick -> CTRL |= 0x01 << 0;                               /* Enbale the countdown. */
    
    do
    {
        times = SysTick -> CTRL;
    }
    while ((times & 0x01) && !(times & (0x01 << 16)));          /* Wait till the countdown reaches the end. */
    
    SysTick -> CTRL &= ~(0x01 << 0);                            /* Disable the countdown. */
    SysTick -> VAL = 0x00;                                      /* Reset the counter. */
}