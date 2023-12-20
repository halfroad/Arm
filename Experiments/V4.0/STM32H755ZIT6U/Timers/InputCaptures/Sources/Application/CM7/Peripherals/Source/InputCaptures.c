#include "SystemInterrupts.h"
#include "InputCaptures.h"

uint8_t TIM5_CHANNEL1_CAPTURE_STATUS = 0;
uint32_t TIM5_CHANNEL1_CAPTURE_VALUE;
    
void InitInputCaptures(uint32_t prescaler, uint32_t autoReload)
{
    /*
    
    Bit 3  TIM5EN: TIM5 peripheral clock enable
    Set and reset by software.
        0: TIM5 peripheral clock disable (default after reset)
        1: TIM5 peripheral clock enabled
        
    */
    RCC -> APB1LENR |= 0x01 << 3;
    
    /*
    
    Bits 15:0 PSC[15:0]: Prescaler value
    The counter clock frequency CK_CNT is equal to fCK_PSC / (PSC[15:0] + 1).
    PSC contains the value to be loaded in the active prescaler register at each update event
    (including when the counter is cleared through UG bit of TIMx_EGR register or through
    trigger controller when configured in ¡°reset mode¡±).
    
    */
    TIM5 -> PSC = prescaler;
    
    /*
    
    Bits 31:16 ARR[31:16]: High auto-reload value (TIM2 and TIM5)
    
    Bits 15:0 ARR[15:0]: Low Auto-reload value
    ARR is the value to be loaded in the actual auto-reload register.
    Refer to the Section 41.3.1: Time-base unit on page 1805 for more details about ARR
    update and behavior.
    The counter is blocked while the auto-reload value is null
    
    */
    TIM5 -> ARR = autoReload;
    
    /*
    
    Bits 1:0 CC1S[1:0]: Capture/Compare 1 selection
    This bit-field defines the direction of the channel (input/output) as well as the used input.
        00: CC1 channel is configured as output
        01: CC1 channel is configured as input, IC1 is mapped on TI1
        10: CC1 channel is configured as input, IC1 is mapped on TI2
        11: CC1 channel is configured as input, IC1 is mapped on TRC. This mode is working only if
    an internal trigger input is selected through TS bit (TIMx_SMCR register)
    Note: CC1S bits are writable only when the channel is OFF (CC1E = 0 in TIMx_CCER)
        
    */
    TIM5 -> CCMR1 &= ~(0x03 << 0);
    TIM5 -> CCMR1 |= 0x01 << 0;
    
    /*
    
    Bits 3:2 IC1PSC[1:0]: Input capture 1 prescaler
    This bit-field defines the ratio of the prescaler acting on CC1 input (IC1). The prescaler is
    reset as soon as CC1E=0 (TIMx_CCER register).
        00: no prescaler, capture is done each time an edge is detected on the capture input
        01: capture is done once every 2 events
        10: capture is done once every 4 events
        11: capture is done once every 8 events
    
    */
    TIM5 -> CCMR1 &= ~(0x03 << 2);
    
    /*
    
    Bits 7:4 IC1F[3:0]: Input capture 1 filter
    This bit-field defines the frequency used to sample TI1 input and the length of the digital filter
    applied to TI1. The digital filter is made of an event counter in which N consecutive events
    are needed to validate a transition on the output:
        0000: No filter, sampling is done at fDTS
        0001: fSAMPLING=fCK_INT, N=2
        0010: fSAMPLING=fCK_INT, N=4
        0011: fSAMPLING=fCK_INT, N=8
        0100: fSAMPLING=fDTS/2, N=6
        0101: fSAMPLING=fDTS/2, N=8
        0110: fSAMPLING=fDTS/4, N=6
        0111: fSAMPLING=fDTS/4, N=8
        1000: fSAMPLING=fDTS/8, N=6
        1001: fSAMPLING=fDTS/8, N=8
        1010: fSAMPLING=fDTS/16, N=5
        1011: fSAMPLING=fDTS/16, N=6
        1100: fSAMPLING=fDTS/16, N=8
        1101: fSAMPLING=fDTS/32, N=5
        1110: fSAMPLING=fDTS/32, N=6
        1111: fSAMPLING=fDTS/32, N=8
        
    */
    TIM5 -> CCMR1 &= ~(0x0F << 4);
    
    /*
    
    Bit 3 CC1NP: Capture/Compare 1 output Polarity.
        CC1 channel configured as output: CC1NP must be kept cleared in this case.
        CC1 channel configured as input: This bit is used in conjunction with CC1P to define
    TI1FP1/TI2FP1 polarity. refer to CC1P description.

    */
    TIM5 -> CCER &= ~(0x01 << 3);
    
    /*
    
    Bit 1 CC1P: Capture/Compare 1 output Polarity.
    
        0: OC1 active high (output mode) / Edge sensitivity selection (input mode, see below)
        1: OC1 active low (output mode) / Edge sensitivity selection (input mode, see below)
        
    When CC1 channel is configured as input, both CC1NP/CC1P bits select the active
    polarity of TI1FP1 and TI2FP1 for trigger or capture operations.
    
        CC1NP=0, CC1P=0: non-inverted/rising edge. The circuit is sensitive to TIxFP1 rising edge
    (capture or trigger operations in reset, external clock or trigger mode),
    TIxFP1 is not inverted (trigger operation in gated mode or encoder
    mode).
    
        CC1NP=0, CC1P=1: inverted/falling edge. The circuit is sensitive to TIxFP1 falling edge
    (capture or trigger operations in reset, external clock or trigger mode),
    TIxFP1 is inverted (trigger operation in gated mode or encoder mode).
    
        CC1NP=1, CC1P=1: non-inverted/both edges. The circuit is sensitive to both TIxFP1 rising
    and falling edges (capture or trigger operations in reset, external clock
    or trigger mode), TIxFP1is not inverted (trigger operation in gated
    mode). This configuration must not be used in encoder mode.
    
        CC1NP=1, CC1P=0: This configuration is reserved, it must not be used.
    
    */
    TIM5 -> CCER &= ~(0x01 << 1);
    
    /*
    
    Bit 0 CC1E: Capture/Compare 1 output enable.
        0: Capture mode disabled / OC1 is not active
        1: Capture mode enabled / OC1 signal is output on the corresponding output pin

    */
    TIM5 -> CCER |= 0x01 << 0;
    
    /*
    
    Bit 1 CC1IE: Capture/Compare 1 interrupt enable
        0: CC1 interrupt disabled.
        1: CC1 interrupt enabled.
        
    Bit 0 UIE: Update interrupt enable
        0: Update interrupt disabled.
        1: Update interrupt enabled.
            
    */
    
    TIM5 -> DIER |= (0x01 << 1) | (0x01 << 0);
    
    /*
    
    Bit 0 CEN: Counter enable
        0: Counter disabled
        1: Counter enabled
    Note: External clock, gated mode and encoder mode can work only if the CEN bit has been
    previously set by software. However trigger mode can set the CEN bit automatically by
    hardware.
    CEN is cleared automatically in one-pulse mode, when an update event occurs
    
    */
    
    TIM5 -> CR1 |= 0x01 << 0;
    
    SetNestedVectoredInterruptPriorty(2, 2, TIM5_IRQn);
}

void TIM5_IRQHandler(void)
{
    uint16_t status = TIM5 -> SR;
    
    if ((TIM5_CHANNEL1_CAPTURE_STATUS & 0x80) == 0)
    {
        /* Not yet captured. */
        if (status & 0x01)
        {
            if (TIM5_CHANNEL1_CAPTURE_STATUS & 0x40)
            {
                /* High voltage level is captured. */
                if ((TIM5_CHANNEL1_CAPTURE_STATUS & 0x3F) == 0x3F)
                {
                    /* High voltage is too long, mark as only once. */
                    TIM5_CHANNEL1_CAPTURE_STATUS |= 0x80;
                    TIM5_CHANNEL1_CAPTURE_VALUE = 0xFFFFFFFF;
                }
                else
                    TIM5_CHANNEL1_CAPTURE_STATUS ++;
            }
        }
        
        /* Capture Event occured on Channel 1.*/
        if (status & 0x02)
        {
            /* A Falling Edge is captured. */
            if (TIM5_CHANNEL1_CAPTURE_STATUS & 0x40)
            {
                /* Captured high voltage level pulse. */
                TIM5_CHANNEL1_CAPTURE_STATUS |= 0x80;
                TIM5_CHANNEL1_CAPTURE_VALUE = TIM5 -> CCR1;
                
                /*
    
                    Bit 1 CC1P: Capture/Compare 1 output Polarity.
                    
                        0: OC1 active high (output mode) / Edge sensitivity selection (input mode, see below)
                        1: OC1 active low (output mode) / Edge sensitivity selection (input mode, see below)
                        
                    When CC1 channel is configured as input, both CC1NP/CC1P bits select the active
                    polarity of TI1FP1 and TI2FP1 for trigger or capture operations.
                    
                        CC1NP=0, CC1P=0: non-inverted/rising edge. The circuit is sensitive to TIxFP1 rising edge
                    (capture or trigger operations in reset, external clock or trigger mode),
                    TIxFP1 is not inverted (trigger operation in gated mode or encoder
                    mode).
                    
                        CC1NP=0, CC1P=1: inverted/falling edge. The circuit is sensitive to TIxFP1 falling edge
                    (capture or trigger operations in reset, external clock or trigger mode),
                    TIxFP1 is inverted (trigger operation in gated mode or encoder mode).
                    
                        CC1NP=1, CC1P=1: non-inverted/both edges. The circuit is sensitive to both TIxFP1 rising
                    and falling edges (capture or trigger operations in reset, external clock
                    or trigger mode), TIxFP1is not inverted (trigger operation in gated
                    mode). This configuration must not be used in encoder mode.
                    
                        CC1NP=1, CC1P=0: This configuration is reserved, it must not be used.
                
                */
                TIM5 -> CCER &= ~(0x01 << 1);
            }
            else
            {
                TIM5_CHANNEL1_CAPTURE_STATUS = 0;
                TIM5_CHANNEL1_CAPTURE_VALUE = 0;
                
                /* A Rising Edge is captured. */
                TIM5_CHANNEL1_CAPTURE_STATUS |= 0x40;
                
                 /*
    
                    Bit 0 CEN: Counter enable
                        0: Counter disabled
                        1: Counter enabled
                    Note: External clock, gated mode and encoder mode can work only if the CEN bit has been
                    previously set by software. However trigger mode can set the CEN bit automatically by
                    hardware.
                    CEN is cleared automatically in one-pulse mode, when an update event occurs
                
                */
                TIM5 -> CR1 &= ~(0x01 << 0);
                
                /*
                
                    Bits 31:16 CNT[31:16]: Most significant part counter value (TIM2 and TIM5)
                    Bits 15:0 CNT[15:0]: Least significant part of counter value
                */
                TIM5 -> CNT = 0;
                
                /*
                
                    Bit 1 CC1P: Capture/Compare 1 output Polarity.
                    
                        0: OC1 active high (output mode) / Edge sensitivity selection (input mode, see below)
                        1: OC1 active low (output mode) / Edge sensitivity selection (input mode, see below)
                        
                    When CC1 channel is configured as input, both CC1NP/CC1P bits select the active
                    polarity of TI1FP1 and TI2FP1 for trigger or capture operations.
                    
                        CC1NP=0, CC1P=0: non-inverted/rising edge. The circuit is sensitive to TIxFP1 rising edge
                    (capture or trigger operations in reset, external clock or trigger mode),
                    TIxFP1 is not inverted (trigger operation in gated mode or encoder
                    mode).
                    
                        CC1NP=0, CC1P=1: inverted/falling edge. The circuit is sensitive to TIxFP1 falling edge
                    (capture or trigger operations in reset, external clock or trigger mode),
                    TIxFP1 is inverted (trigger operation in gated mode or encoder mode).
                    
                        CC1NP=1, CC1P=1: non-inverted/both edges. The circuit is sensitive to both TIxFP1 rising
                    and falling edges (capture or trigger operations in reset, external clock
                    or trigger mode), TIxFP1is not inverted (trigger operation in gated
                    mode). This configuration must not be used in encoder mode.
                    
                        CC1NP=1, CC1P=0: This configuration is reserved, it must not be used.
                
                */
                TIM5 -> CCER |= (0x01 << 1);
                
                /*
    
                    Bit 0 CEN: Counter enable
                        0: Counter disabled
                        1: Counter enabled
                    Note: External clock, gated mode and encoder mode can work only if the CEN bit has been
                    previously set by software. However trigger mode can set the CEN bit automatically by
                    hardware.
                    CEN is cleared automatically in one-pulse mode, when an update event occurs
                
                */
                
                TIM5 -> CR1 |= 0x01;
            }
        }
    }
    
    /*
    
    TIMx status register.
    
    */
    TIM5 -> SR = 0x00;
}