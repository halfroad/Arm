#include <stdio.h>

#include "SystemInterrupts.h"
#include "Timer.h"
#include "OLED.h"

void InitTimer(uint16_t prescaler, uint16_t reload)
{
    /*
    
    Bit 1 TIM3EN: TIM3 peripheral clock enable
    Set and reset by software.
        0: TIM3 peripheral clock disable (default after reset)
        1: TIM3 peripheral clock enabled
    
    */
    RCC -> APB1LENR |= 0x01 << 1;
    
    while ((RCC -> APB1LENR & (0x01 << 1)) == 0)
        ;
    
    /*
    
    TIMx slave mode control register (TIMx_SMCR)(x = 1, 8)
    
    Bits 16, 2, 1, 0 SMS[3:0]: Slave mode selection
    When external signals are selected the active edge of the trigger signal (TRGI) is linked to
    the polarity selected on the external input (see Input Control register and Control Register
    description.
        0000: Slave mode disabled - if CEN = ¡®1¡¯ then the prescaler is clocked directly by the internal
        clock.
        0001: Encoder mode 1 - Counter counts up/down on TI1FP1 edge depending on TI2FP2
        level.
        0010: Encoder mode 2 - Counter counts up/down on TI2FP2 edge depending on TI1FP1
        level.
        0011: Encoder mode 3 - Counter counts up/down on both TI1FP1 and TI2FP2 edges
        depending on the level of the other input.
        0100: Reset Mode - Rising edge of the selected trigger input (TRGI) reinitializes the counter
        and generates an update of the registers.
        0101: Gated Mode - The counter clock is enabled when the trigger input (TRGI) is high. The
        counter stops (but is not reset) as soon as the trigger becomes low. Both start and stop of
        the counter are controlled.
        0110: Trigger Mode - The counter starts at a rising edge of the trigger TRGI (but it is not
        reset). Only the start of the counter is controlled.
        0111: External Clock Mode 1 - Rising edges of the selected trigger (TRGI) clock the counter.
        1000: Combined reset + trigger mode - Rising edge of the selected trigger input (TRGI)
    reinitializes the counter, generates an update of the registers and starts the counter.
    Codes above 1000: Reserved.
    Note: The gated mode must not be used if TI1F_ED is selected as the trigger input
    (TS=00100). Indeed, TI1F_ED outputs 1 pulse for each transition on TI1F, whereas the
    gated mode checks the level of the trigger signal.
    Note: The clock of the slave peripherals (timer, ADC, ...) receiving the TRGO or the TRGO2
    signals must be enabled prior to receive events from the master timer, and the clock
    frequency (prescaler) must not be changed on-the-fly while triggers are received from
    the master timer

    */
    TIM3 -> SMCR = 0x00;
    
    /*
    
    TIMx DMA/interrupt enable register (TIMx_DIER)(x = 1, 8)
    
    Bit 0 UIE: Update interrupt enable
        0: Update interrupt disabled
        1: Update interrupt enabled
    */
    
    TIM3 -> DIER |= 0x01 << 0;
    
    /*
    
    Bits 15:0 PSC[15:0]: Prescaler value
    The counter clock frequency (CK_CNT) is equal to fCK_PSC / (PSC[15:0] + 1).
    PSC contains the value to be loaded in the active prescaler register at each update event
    (including when the counter is cleared through UG bit of TIMx_EGR register or through
    trigger controller when configured in ¡°reset mode¡±).

    */
    TIM3 -> PSC = prescaler;
    
    /*
    
    Bits 15:0 ARR[15:0]: Auto-reload value
    ARR is the value to be loaded in the actual auto-reload register.
    Refer to the Section 40.3.1: Time-base unit on page 1701 for more details about ARR
    update and behavior.
    The counter is blocked while the auto-reload value is null
    
    */
    TIM3 -> ARR = reload;
    
    /*
    
    Bits 6:5 CMS[1:0]: Center-aligned mode selection
        00: Edge-aligned mode. The counter counts up or down depending on the direction bit
        (DIR).
        01: Center-aligned mode 1. The counter counts up and down alternatively. Output compare
        interrupt flags of channels configured in output (CCxS=00 in TIMx_CCMRx register) are set
        only when the counter is counting down.
        10: Center-aligned mode 2. The counter counts up and down alternatively. Output compare
        interrupt flags of channels configured in output (CCxS=00 in TIMx_CCMRx register) are set
        only when the counter is counting up.
        11: Center-aligned mode 3. The counter counts up and down alternatively. Output compare
    interrupt flags of channels configured in output (CCxS=00 in TIMx_CCMRx register) are set
    both when the counter is counting up or down.
    Note: It is not allowed to switch from edge-aligned mode to center-aligned mode as long as
    the counter is enabled (CEN=1)
    
    */
    TIM3 -> CR1 &= ~(0x03 << 5);
    
    /*
    
    Bit 4 DIR: Direction
        0: Counter used as upcounter
        1: Counter used as downcounter
    Note: This bit is read only when the timer is configured in Center-aligned mode or Encoder
    mode.
    
    */
    TIM3 -> CR1 &= ~(0x01 << 4);
    
    /*
    
    Bit 0 CEN: Counter enable
        0: Counter disabled
        1: Counter enabled
    Note: External clock, gated mode and encoder mode can work only if the CEN bit has been
    previously set by software. However trigger mode can set the CEN bit automatically by
    hardware.
    CEN is cleared automatically in one-pulse mode, when an update event occurs.
    
    */
    TIM3 -> CR1 |= 0x01 << 0;
    
    SetNestedVectoredInterruptPriorty(2, 2, TIM3_IRQn);
}

void TIM3_IRQHandler(void)
{
    static uint8_t counter = 0;
    /*
    
    Bit 0 UIF: Update interrupt flag
    This bit is set by hardware on an update event. It is cleared by software.
        0: No update occurred.
        1: Update interrupt pending. This bit is set by hardware when the registers are updated:
    ¨C At overflow or underflow regarding the repetition counter value (update if repetition
    counter = 0) and if the UDIS=0 in the TIMx_CR1 register.
    ¨C When CNT is reinitialized by software using the UG bit in TIMx_EGR register, if URS=0
    and UDIS=0 in the TIMx_CR1 register.
    ¨C When CNT is reinitialized by a trigger event (refer to Section 40.4.3: TIMx slave mode
    control register (TIMx_SMCR)(x = 1, 8)), if URS=0 and UDIS=0 in the TIMx_CR1 register
    
    */
    if (TIM3 -> SR & 0x01)
    {
        if (GPIOB -> ODR & (0x01 << 0))
            GPIOB -> BSRR |= 0x01 << 16;
        else
            GPIOB -> BSRR |= 0x01 << 0;
        
        if (GPIOB -> ODR & (0x01 << 14))
            GPIOB -> BSRR |= 0x01 << (14 + 16);
        else
            GPIOB -> BSRR |= 0x01 << 14;
        
        if (GPIOE -> ODR & (0x01 << 1))
            GPIOE -> BSRR |= 0x01 << (1 + 16);
        else
            GPIOE -> BSRR |= 0x01 << 1;
        
        
        OLED_ShowNum(8, 16, counter ++, 3, 8, 1);
        OLED_Refresh();
    }
    
    /* Clear Update Interrupt Flag. */
    TIM3 -> SR &= ~(0x01 << 0);
}