#include "stm32h7xx.h"

void SetSystemClock(void)
{
    uint16_t retrials = 0;
    uint8_t status = 0;
    
    PWR -> CR3 &= ~(0x01 << 2);
}