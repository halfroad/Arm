#include "PulseWidthModulation.h"

void InitPulseWdithModulation(uint32_t prescaler, uint32_t autoReload)
{
    /*
    
    Bit 1 TIM3EN: TIM3 peripheral clock enable
    Set and reset by software.
        0: TIM3 peripheral clock disable (default after reset)
        1: TIM3 peripheral clock enabled
        
    */
    RCC -> APB1LENR|= 0x01 << 1;
    
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
    The counter is blocked while the auto-reload value is null.
    
    */
    TIM3 -> ARR = autoReload;
    
    /*
    
    Output compare mode:
    
    Bits 16, 6:4 OC3M[3:0]: Output compare 3 mode
    Refer to OC1M description (bits 6:4 in TIMx_CCMR1 register)
    
    Bits 16, 6:4 OC1M[3:0]: Output compare 1 mode
    These bits define the behavior of the output reference signal OC1REF from which OC1 and
    OC1N are derived. OC1REF is active high whereas OC1 and OC1N active level depends
    on CC1P and CC1NP bits.
        0000: Frozen - The comparison between the output compare register TIMx_CCR1 and the
        counter TIMx_CNT has no effect on the outputs.(this mode is used to generate a timing
        base).
        0001: Set channel 1 to active level on match. OC1REF signal is forced high when the
        counter TIMx_CNT matches the capture/compare register 1 (TIMx_CCR1).
        0010: Set channel 1 to inactive level on match. OC1REF signal is forced low when the
        counter TIMx_CNT matches the capture/compare register 1 (TIMx_CCR1).
        0011: Toggle - OC1REF toggles when TIMx_CNT=TIMx_CCR1.
        0100: Force inactive level - OC1REF is forced low.
        0101: Force active level - OC1REF is forced high.
        0110: PWM mode 1 - In upcounting, channel 1 is active as long as TIMx_CNT<TIMx_CCR1
        else inactive. In downcounting, channel 1 is inactive (OC1REF=¡®0) as long as
        TIMx_CNT>TIMx_CCR1 else active (OC1REF=1).
        0111: PWM mode 2 - In upcounting, channel 1 is inactive as long as
        TIMx_CNT<TIMx_CCR1 else active. In downcounting, channel 1 is active as long as
        TIMx_CNT>TIMx_CCR1 else inactive.
        1000: Retriggerable OPM mode 1 - In up-counting mode, the channel is active until a trigger
        event is detected (on TRGI signal). Then, a comparison is performed as in PWM mode 1
        and the channels becomes inactive again at the next update. In down-counting mode, the
        channel is inactive until a trigger event is detected (on TRGI signal). Then, a comparison is
        performed as in PWM mode 1 and the channels becomes inactive again at the next update.
        1001: Retriggerable OPM mode 2 - In up-counting mode, the channel is inactive until a
        trigger event is detected (on TRGI signal). Then, a comparison is performed as in PWM
        mode 2 and the channels becomes inactive again at the next update. In down-counting
        mode, the channel is active until a trigger event is detected (on TRGI signal). Then, a
        comparison is performed as in PWM mode 1 and the channels becomes active again at the
        next update.
        1010: Reserved,
        1011: Reserved,
        1100: Combined PWM mode 1 - OC1REF has the same behavior as in PWM mode 1.
        OC1REFC is the logical OR between OC1REF and OC2REF.
        1101: Combined PWM mode 2 - OC1REF has the same behavior as in PWM mode 2.
        OC1REFC is the logical AND between OC1REF and OC2REF.
        1110: Asymmetric PWM mode 1 - OC1REF has the same behavior as in PWM mode 1.
        OC1REFC outputs OC1REF when the counter is counting up, OC2REF when it is counting
        down.
        1111: Asymmetric PWM mode 2 - OC1REF has the same behavior as in PWM mode 2.
    OC1REFC outputs OC1REF when the counter is counting up, OC2REF when it is counting
    down.
    Note: In PWM mode, the OCREF level changes only when the result of the comparison
    changes or when the output compare mode switches from ¡°frozen¡± mode to ¡°PWM¡±
    mode.
    */
    TIM3 -> CCMR2 &= ~(0x0F << 4);
    TIM3 -> CCMR2 |= (0x06 << 4);
    
    /*
    
    Bit 3 OC3PE: Output compare 3 preload enable
    
    */
    TIM3 -> CCMR2 |= 0x01 << 3;
    
    /*
    
    Bit 8 CC3E: Capture/Compare 3 output enable.
    Refer to CC1E description
    
    */
    TIM3 -> CCER |= 0x01 << 8;
    
    /*
    
    Bit 9 CC3P: Capture/Compare 3 output Polarity.
    Refer to CC1P description
    
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
    TIM3 -> CCER |= 0x01 << 9;
    
    /*
    
    Bit 7 ARPE: Auto-reload preload enable
        0: TIMx_ARR register is not buffered
        1: TIMx_ARR register is buffered

    */
    TIM3 -> CR1 |= 0x01 << 7;
    
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
}

void SetCaptureCompare(uint16_t newCompare)
{
    /*
    
    Bits 31:16 CCR4[31:16]: High Capture/Compare 4 value (TIM2 and TIM5)
    Bits 15:0 CCR4[15:0]: Low Capture/Compare value
    
    1. if CC4 channel is configured as output (CC4S bits):
    CCR4 is the value to be loaded in the actual capture/compare 4 register (preload value).
    It is loaded permanently if the preload feature is not selected in the TIMx_CCMR2
    register (bit OC4PE). Else the preload value is copied in the active capture/compare 4
    register when an update event occurs.
    The active capture/compare register contains the value to be compared to the counter
    TIMx_CNT and signalled on OC4 output.
    
    2. if CC4 channel is configured as input (CC4S bits in TIMx_CCMR4 register):
    CCR4 is the counter value transferred by the last input capture 4 event (IC4). The
    TIMx_CCR4 register is read-only and cannot be programmed.
    */
    TIM3 -> CCR4 = newCompare;
}