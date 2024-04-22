#include "../Include/Delay.h"

#define DELAY_TIMER                                                         TIM6
#define DELAY_TIMER_RCC_CLOCK_ENABLE_REGISTER                               APB1ENR1
#define DELAY_TIMER_RCC_CLOCK_ENABLE_REGISTER_BIT_FIELD_NUMBER              4

#define DELAY_TIME_IRQN                                                     TIM6_DAC_IRQn
#define DELAY_TIME_IRQHANDLER                                               TIM6_DAC_IRQHandler

#define PRESCALER                                                           17
#define AUTO_RELOAD                                                         10

// For store tick counts in us
static __IO uint32_t usTicks;

void InitTimer(uint16_t prescaler, uint32_t autoReload);

void InitDelay(void)
{
    InitTimer(PRESCALER - 1, AUTO_RELOAD - 1);
}

void InitTimer(uint16_t prescaler, uint32_t autoReload)
{   
    /*
    
    Bit 4 TIM6EN: TIM6 timer clock enable
    Set and cleared by software.
        0: TIM6 clock disabled
        1: TIM6 clock enabled
    
    */
    RCC -> DELAY_TIMER_RCC_CLOCK_ENABLE_REGISTER |= 0x01 << DELAY_TIMER_RCC_CLOCK_ENABLE_REGISTER_BIT_FIELD_NUMBER;
    
    /*
    
    TIMx prescaler (TIMx_PSC)(x = 6 to 7)
    
    Bits 15:0 PSC[15:0]: Prescaler value
    The counter clock frequency ftim_cnt_ck is equal to f tim_psc_ck / (PSC[15:0] + 1).
    PSC contains the value to be loaded into the active prescaler register at each update event.
    (including when the counter is cleared through UG bit of TIMx_EGR register.
        
    */
    DELAY_TIMER -> PSC &= ~(0xFFFF << 0);
    DELAY_TIMER -> PSC |= prescaler;
    
    /*
    
    TIMx auto-reload register (TIMx_ARR)(x = 6 to 7)
    
    Bits 19:0 ARR[19:0]: Auto-reload value
    ARR is the value to be loaded into the actual auto-reload register.
    Refer to Section 31.3.4: Time-base unit on page 1444 for more details about ARR update
    and behavior.
    The counter is blocked while the auto-reload value is null.
    Non-dithering mode (DITHEN = 0)
    The register holds the auto-reload value in ARR[15:0]. The ARR[19:16] bits are reserved.
    Dithering mode (DITHEN = 1)
    The register holds the integer part in ARR[19:4]. The ARR[3:0] bitfield contains the dithered
    part.
    
    */
    DELAY_TIMER -> ARR &= ~(0x0FFFFF << 0);
    DELAY_TIMER -> ARR |= autoReload;
    
    /*
    
    TIMx DMA/Interrupt enable register (TIMx_DIER)(x = 6 to 7)
    
    Bit 0 UIE: Update interrupt enable
        0: Update interrupt disabled.
        1: Update interrupt enabled
    
    */
    
    DELAY_TIMER -> DIER |= 0x01 << 0;
    
    /*
    
    TIMx control register 1 (TIMx_CR1)(x = 6 to 7)
    
    Bit 0 CEN: Counter enable
        0: Counter disabled
        1: Counter enabled
    CEN is cleared automatically in one-pulse mode, when an update event occurs.
    
    */
    
    DELAY_TIMER -> CR1 |= 0x01 << 0;
    
    NVIC_SetPriority(DELAY_TIME_IRQN, 1);
    NVIC_EnableIRQ(DELAY_TIME_IRQN);
}

void DelayUs(uint32_t us)
{
    // Reload us value
    usTicks = us;
    // Wait until usTick reach zero
    while (usTicks);
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

// SysTick_Handler function will be called every 1 us
void DELAY_TIME_IRQHANDLER()
{
    if (usTicks != 0)
    {
        usTicks --;
    }
}