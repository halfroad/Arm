#include "../Include/Delay.h"

// For store tick counts in us
static __IO uint32_t usTicks;

void InitDelay()
{
    // Configure the SysTick timer to overflow every 1 us
    SysTick_Config(SystemCoreClock / 1000000);
}

void DelayUs(uint32_t us)
{
    // Reload us value
    usTicks = us;
    // Wait until usTick reach zero
    while (usTicks)
        ;
}

void DelayMs(uint32_t ms)
{
    // Wait until ms reach zero
    while (ms--)
    {
        // Delay 1ms
        DelayUs(1000);
    }
}

#if (USE_CUSTOM_SYSTICK_HANDLER_IMPLEMENTATION == 0)

// SysTick_Handler function will be called every 1 us
void SysTickPeriodElapsedHandler (void)
    
#else

void SysTick_Handler (void)

#endif  /*  #if (USE_CUSTOM_SYSTICK_HANDLER_IMPLEMENTATION == 0)    */

{
    if (usTicks != 0)
    {
        usTicks--;
    }
}