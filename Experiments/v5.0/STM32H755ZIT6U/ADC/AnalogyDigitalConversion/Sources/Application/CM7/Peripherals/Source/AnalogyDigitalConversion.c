#include "Delay.h"
#include "SystemInterrupts.h"
#include "AnalogyDigitalConversion.h"

void InitAnalogyDigitalConversion(void)
{
    /*
    
    AHB1(D2):
    0x40022000 - 0x400223FF ADC1 - ADC2 Section 26.7: ADC common registers
    
    */
    
    /*
    
    Bit 5 ADC12EN: Enable of ADC1/2 peripheral clocks
    Set and reset by software.
        0: ADC1 and 2 peripheral clocks disabled (default after reset)
        1: ADC1 and 2 peripheral clocks enabled
    The peripheral clocks of the ADC1 and 2 are: the kernel clock selected by ADCSEL and provided to
    adc_ker_ck_input, and the rcc_hclk1 bus interface clock.
    
    */
    RCC -> AHB1ENR |= 0x01 << 5;
    
    /*
    
    Table 8. STM32H755xI pin/ball definition (continued)
    
    44 P6 50 58 T3 PA5 I/O TT_
    ha -
    D2PWREN,
    TIM2_CH1/TIM2_ETR,
    TIM8_CH1N,
    SPI1_SCK/I2S1_CK,
    SPI6_SCK,
    OTG_HS_ULPI_CK,
    LCD_R4, EVENTOUT
    ADC12_INN18,
    ADC12_INP19,
    DAC1_OUT2
    */
    
    /*
    
    AHB4(D3):
    0x58020000 - 0x580203FF GPIOA Section 12.4: GPIO registers
    
    */
    
    /*
    
    Bit 0 GPIOAEN: GPIOA peripheral clock enable
    Set and reset by software.
        0: GPIOA peripheral clock disabled (default after reset)
        1: GPIOA peripheral clock enabled

    */
    RCC -> AHB4ENR |= 0x01 << 0;
    
    /*
    
    Bits 31:0 MODER[15:0][1:0]: Port x configuration I/O pin y (y = 15 to 0)
    These bits are written by software to configure the I/O mode.
        00: Input mode
        01: General purpose output mode
        10: Alternate function mode
        11: Analog mode (reset state)

    */
    GPIOA -> MODER &= ~(0x03 << 2 * 5);
    GPIOA -> MODER |= 0x03 << 2 * 5;
    
    /*
    
    Bits 31:0 PUPDR[15:0][1:0]: Port x configuration I/O pin y (y = 15 to 0)
    These bits are written by software to configure the I/O pull-up or pull-down
        00: No pull-up, pull-down
        01: Pull-up
        10: Pull-down
        11: Reserved

    */
    GPIOA -> PUPDR &= ~(0x03 << 5 * 2);
    
    /*
    
    RCC AHB1 peripheral reset register(RCC_AHB1RSTR)
    
    Bit 5 ADC12RST: ADC1and 2 block reset
    Set and reset by software.
        0: does not reset ADC1 and 2 block (default after reset)
        1: resets ADC1and 2 block

    */
    RCC -> AHB1RSTR |= 0x01 << 5;
    RCC -> AHB1RSTR &= ~(0x01 << 5);
    
    /*
    
    RCC domain 3 kernel clock configuration register (RCC_D3CCIPR) 

    Bits 17:16 ADCSEL[1:0]: SAR ADC kernel clock source selection
    Set and reset by software.
        00: pll2_p_ck clock selected as kernel peripheral clock (default after reset)
        01: pll3_r_ck clock selected as kernel peripheral clock
        10: per_ck clock selected as kernel peripheral clock
        others: reserved, the kernel clock is disabled
    */
    RCC -> D3CCIPR &= ~(0x03 << 16);
    /* per_ck clock = hsi_ket_ck, with frequenct 64MHz. */
    RCC -> D3CCIPR |= 0x02 << 16;
    
    /*
    
    Bits 17:16 CKMODE[1:0]: ADC clock mode
    These bits are set and cleared by software to define the ADC clock scheme (which is
    common to both master and slave ADCs):
        00: CK_ADCx (x=1 to 23) (Asynchronous clock mode), generated at product level (refer to
    Section Reset and Clock Control (RCC))
        01: adc_sclk/1 (Synchronous clock mode).
        10: adc_sclk/2 (Synchronous clock mode)
        11: adc_sclk/4 (Synchronous clock mode)
    Whatever CKMODE[1:0] settings, an additional divider factor of 2 is applied to the clock
    delivered to the analog ADC block.
    In synchronous clock mode, when adc_ker_ck = 2 x adc_hclk, there is no jitter in the delay
    from a timer trigger to the start of a conversion.
    Note: The software is allowed to write these bits only when the ADCs are disabled
    (ADCAL=0, JADSTART=0, ADSTART=0, ADSTP=0, ADDIS=0 and ADEN=0).

    */
    
    ADC12_COMMON -> CCR &= ~(0x03 << 16);
    
    /*
    
    ADC x common control register (ADCx_CCR) (x=1/2 or 3)
    Address offset: 0x08
    Reset value: 0x0000 0000
    The address offset is relative to the master ADC base address + 0x300.
    ADC1 and ADC2 are controlled by the same interface, while ADC3 is controlled separately. 
    
    Bits 21:18 PRESC[3:0]: ADC prescaler
    These bits are set and cleared by software to select the frequency of the clock to the ADC.
    The clock is common for all the ADCs.
        0000: input ADC clock not divided
        0001: input ADC clock divided by 2
        0010: input ADC clock divided by 4
        0011: input ADC clock divided by 6
        0100: input ADC clock divided by 8
        0101: input ADC clock divided by 10
        0110: input ADC clock divided by 12
        0111: input ADC clock divided by 16
        1000: input ADC clock divided by 32
        1001: input ADC clock divided by 64
        1010: input ADC clock divided by 128
        1011: input ADC clock divided by 256
    Others: Reserved, must not be used
    Note: The software is allowed to write these bits only when the ADC is disabled (ADCAL=0,
    JADSTART=0, ADSTART=0, ADSTP=0, ADDIS=0 and ADEN=0). The ADC prescaler
    value is applied only when CKMODE[1:0] = 0b00.

    */
    ADC12_COMMON -> CCR &= ~(0x0F << 18);
    ADC12_COMMON -> CCR |= 0x01 << 18;
    
    /*
    
    ADC control register (ADC_CR)
    
    Bit 0 ADEN: ADC enable control
    This bit is set by software to enable the ADC. The ADC will be effectively ready to operate once the
    flag ADRDY has been set.
    It is cleared by hardware when the ADC is disabled, after the execution of the ADDIS command.
        0: ADC is disabled (OFF state)
        1: Write 1 to enable the ADC.
    Note: The software is allowed to set ADEN only when all bits of ADC_CR registers are 0 (ADCAL=0,
    JADSTART=0, ADSTART=0, ADSTP=0, ADDIS=0 and ADEN=0) except for bit ADVREGEN
    which must be 1 (and the software must have wait for the startup time of the voltage regulator)
    
    Bit 29 DEEPPWD: Deep-power-down enable
    This bit is set and cleared by software to put the ADC in deep-power-down mode.
        0: ADC not in deep-power down
        1: ADC in deep-power-down (default reset state)
    Note: The software is allowed to write this bit only when the ADC is disabled (ADCAL=0,
    JADSTART=0, JADSTP=0, ADSTART=0, ADSTP=0, ADDIS=0 and ADEN=0).
    
    By default, the ADC is in deep-power-down mode where its supply is internally switched off
    to reduce the leakage currents (the reset state of bit DEEPPWD is 1 in the ADC_CR
    register).
    To start ADC operations, it is first needed to exit deep-power-down mode by clearing bit
    DEEPPWD=0.
    Then, it is mandatory to enable the ADC internal voltage regulator by setting the bit
    ADVREGEN=1 into ADC_CR register. The software must wait for the startup time of the
    ADC voltage regulator (TADCVREG_STUP) before launching a calibration or enabling the
    ADC. This delay must be implemented by software.
    The LDO status can be verified by checking the LDORDY bit in ADC_ISR register (refer to
    Section 26.3: ADC implementation for the availability of the LDO regulator status).
    For the startup time of the ADC voltage regulator, refer to device datasheet for
    TADCVREG_STUP parameter.
    After ADC operations are complete, the ADC can be disabled (ADEN=0). It is possible to
    save power by also disabling the ADC voltage regulator. This is done by writing bit
    ADVREGEN=0.
    Then, to save more power by reducing the leakage currents, it is also possible to re-enter in
    ADC deep-power-down mode by setting bit DEEPPWD=1 into ADC_CR register. This is
    particularly interesting before entering Stop mode.
    Note: Writing DEEPPWD=1 automatically disables the ADC voltage regulator and bit ADVREGEN
    is automatically cleared.
    Note: When the internal voltage regulator is disabled (ADVREGEN=0), the internal analog
    calibration is kept.
    In ADC deep-power-down mode (DEEPPWD=1), the internal analog calibration is lost and it
    is necessary to either relaunch a calibration or apply again the calibration factor which was
    previously saved (refer to Section 26.4.8: Calibration (ADCAL, ADCALDIF, ADCALLIN,
    ADC_CALFACT)).

    */
    ADC1 -> CR = 0;
    
    /*
    
    Bit 28 ADVREGEN: ADC voltage regulator enable
    This bits is set by software to enable the ADC voltage regulator.
    Before performing any operation such as launching a calibration or enabling the ADC, the ADC
    voltage regulator must first be enabled and the software must wait for the regulator start-up time.
        0: ADC Voltage regulator disabled
        1: ADC Voltage regulator enabled.
    For more details about the ADC voltage regulator enable and disable sequences, refer to
    Section 26.4.6: ADC deep-power-down mode (DEEPPWD) and ADC voltage regulator
    (ADVREGEN).
    The software can program this bitfield only when the ADC is disabled (ADCAL=0, JADSTART=0,
    ADSTART=0, ADSTP=0, ADDIS=0 and ADEN=0).

    */
    ADC1 -> CR |= 0x01 << 28;
    
    /* There is not any flag in ADC_ISR ,so just wait for a short while to ensure the completion of ADC voltage regulator enablement. */
    DelayMilliseconds(10);
    
    /*
    
    Bits 9:8 BOOST[1:0]: Boost mode control
    This bitfield is set and cleared by software to enable/disable the Boost mode.
        00: used when ADC clock ¡Ü 6.25 MHz
        01: used when 6.25 MHz < ADC clock frequency ¡Ü 12.5 MHz
        10: used when 12.5 MHz < ADC clock ¡Ü 25.0 MHz
        11: used when 25.0 MHz < ADC clock ¡Ü 50.0 MHz
    Note: The software is allowed to write this bit only when ADSTART=0 and JADSTART=0 (which
    ensures that no conversion is ongoing).
    When dual mode is enabled (bits DAMDF of ADCx_CCR register are not equal to zero), the
    BOOST bitfield of the slave ADC is no more writable and its content must be equal to the
    master ADC BOOST bitfield. 
    
    BOOST must be enabled in the case the frequency of ADC is greater than 20MHz.
    
    Be noted that, for the MCU STM32H750VBT6, this bitfields are clarified as follows,
    
    Bit 8 BOOST: Boost mode control
    This bit is set and cleared by software to enable/disable the Boost mode.
        0: Boost mode off. Used when ADC clock < 20 MHz to save power at lower clock frequency.
        1: Boost mode on. Must be used when ADC clock > 20 MHz.
    Note: The software is allowed to write this bit only when ADSTART=0 and JADSTART=0 (which
    ensures that no conversion is ongoing).
    When dual mode is enabled (bits DAMDF of ADCx_CCR register are not equal to zero), the bit
    BOOST of the slave ADC is no more writable and its content must be equal to the master ADC
    BOOST bit.
    
    ADC1 -> CR &= ~(0x03 << 8);
    
    */
    
    ADC1 -> CR |= 0x03 << 8;
    
    /*
    
    Bit 13 CONT: Single / continuous conversion mode for regular conversions
    This bit is set and cleared by software. If it is set, regular conversion takes place continuously until it
    is cleared.
        0: Single conversion mode
        1: Continuous conversion mode
    Note: It is not possible to have both discontinuous mode and continuous mode enabled: it is forbidden
    to set both DISCEN=1 and CONT=1.
    The software is allowed to write this bit only when ADSTART=0 (which ensures that no regular
    conversion is ongoing).
    When dual mode is enabled (DAMDF bits in ADCx_CCR register are not equal to zero), the bit
    CONT of the slave ADC is no more writable and its content is equal to the bit CONT of the
    master ADC.

    */
    ADC1 -> CFGR &= ~(0x01 << 13);
    
    /*
    
    Bit 12 OVRMOD: Overrun Mode
    This bit is set and cleared by software and configure the way data overrun is managed.
        0: ADC_DR register is preserved with the old data when an overrun is detected.
        1: ADC_DR register is overwritten with the last conversion result when an overrun is detected.
    Note: The software is allowed to write this bit only when ADSTART=0 (which ensures that no regular
    conversion is ongoing).

    */
    ADC1 -> CFGR |= 0x01 << 12;
    
    /*
    
    Bits 11:10 EXTEN[1:0]: External trigger enable and polarity selection for regular channels
    These bits are set and cleared by software to select the external trigger polarity and enable the
    trigger of a regular group.
        00: Hardware trigger detection disabled (conversions can be launched by software)
        01: Hardware trigger detection on the rising edge
        10: Hardware trigger detection on the falling edge
        11: Hardware trigger detection on both the rising and falling edges
    Note: The software is allowed to write these bits only when ADSTART=0 (which ensures that no
    regular conversion is ongoing).
    */
    
    ADC1 -> CFGR &= ~(0x03 << 10);
    
    /*
    
    Bits 9:5 EXTSEL[4:0]: External trigger selection for regular group
    These bits select the external event used to trigger the start of conversion of a regular group:
        00000: Event 0
        00001: Event 1
        00010: Event 2
        00011: Event 3
        00100: Event 4
        00101: Event 5
        00110: Event 6
        00111: Event 7
        ...
        11111: Event 31
    Note: The software is allowed to write these bits only when ADSTART=0 (which ensures that no
    regular conversion is ongoing).
    
    */
    
    ADC1 -> CFGR &= ~(0x1F << 5);
    
    /*
    
    Bits 4:2 RES[2:0]: Data resolution
    These bits are written by software to select the resolution of the conversion.
        000: 16 bits
        001: 14 bits in legacy mode (not optimized power consumption)
        010: 12 bits in legacy mode (not optimized power consumption)
        101: 14 bits
        110: 12 bits
        011: 10 bits
        111: 8 bits
    Others: Reserved, must not be used.
    Note: The software is allowed to write these bits only when ADSTART=0 and JADSTART=0 (which
    ensures that no conversion is ongoing).

    */
    ADC1 -> CFGR &= ~(0x07 << 2);
    
    /*
    Bits 31:28 LSHIFT[3:0]: Left shift factor
    This bitfield is set and cleared by software to define the left shifting applied to the final result with or
    without oversampling.
        0000: No left shift
        0001: Shift left 1-bit
        0010: Shift left 2-bits
        0011: Shift left 3-bits
        0100: Shift left 4-bits
        0101: Shift left 5-bits
        0110: Shift left 6-bits
        0111: Shift left 7-bits
        1000: Shift left 8-bits
        1001: Shift left 9-bits
        1010: Shift left 10-bits
        1011: Shift left 11-bits
        1100: Shift left 12-bits
        1101: Shift left 13-bits
        1110: Shift left 14-bits
        1111: Shift left 15-bits
    Note: The software is allowed to write this bit only when ADSTART=0 (which ensures that no
    conversion is ongoing).

    */
    ADC1 -> CFGR2 &= ~(0x0F << 28);
    
    /*
    
    Bits 25:16 OSVR[9:0]: Oversampling ratio
    This bitfield is set and cleared by software to define the oversampling ratio.
        0: 1x (no oversampling)
        1: 2x
        2: 3x
        ...
        1023: 1024x
    Note: The software is allowed to write this bit only when ADSTART=0 (which ensures that no
    conversion is ongoing).
    
    */
    ADC1 -> CFGR2 &= ~(0x03FF << 16);
    
    /*
    
    Bit 30 ADCALDIF: Differential mode for calibration
    This bit is set and cleared by software to configure the single-ended or differential inputs mode for
    the calibration.
        0: Writing ADCAL will launch a calibration in Single-ended inputs Mode.
        1: Writing ADCAL will launch a calibration in Differential inputs Mode.
    Note: The software is allowed to write this bit only when the ADC is disabled and is not calibrating
    (ADCAL=0, JADSTART=0, JADSTP=0, ADSTART=0, ADSTP=0, ADDIS=0 and ADEN=0).

    */
    ADC1 -> CR &= ~(0x01 << 30);
    
    /*
    
    Bit 16 ADCALLIN: Linearity calibration
    This bit is set and cleared by software to enable the Linearity calibration.
        0: Writing ADCAL will launch a calibration without the Linearity calibration.
        1: Writing ADCAL will launch a calibration with he Linearity calibration.
    Note: The software is allowed to write this bit only when the ADC is disabled and is not calibrating
    (ADCAL=0, JADSTART=0, JADSTP=0, ADSTART=0, ADSTP=0, ADDIS=0 and ADEN=0).

    */
    ADC1 -> CR |= 0x01 << 16;
    
    /*
    
    Bit 31 ADCAL: ADC calibration
    This bit is set by software to start the calibration of the ADC. Program first the bit ADCALDIF to
    determine if this calibration applies for single-ended or differential inputs mode.
    It is cleared by hardware after calibration is complete.
        0: Calibration complete
        1: Write 1 to calibrate the ADC. Read at 1 means that a calibration in progress.
    Note: The software is allowed to launch a calibration by setting ADCAL only when ADEN=0.
    The software is allowed to update the calibration factor by writing ADC_CALFACT only when
    ADEN=1 and ADSTART=0 and JADSTART=0 (ADC enabled and no conversion is ongoing)

    */
    ADC1 -> CR |= 0x01 << 31;
    
    while (ADC1 -> CR & (0x01 << 31))
        ;
    
    /*
    
    Bits 3:0 L[3:0]: Regular channel sequence length
    These bits are written by software to define the total number of conversions in the regular
    channel conversion sequence.
        0000: 1 conversion
        0001: 2 conversions
        ...
        1111: 16 conversions
    Note: The software is allowed to write these bits only when ADSTART=0 (which ensures that
    no regular conversion is ongoing).
    
    */
    ADC1 -> SQR1 &= ~(0x0F << 0);
    
    /*
    
    ADC sample time register 1 (ADC_SMPR1) 
    
    Bits 29:0 SMP[9:0][2:0]: Channel x sampling time selection (x = 0 to 9)
    These bits are written by software to select the sampling time individually for each channel.
    During sample cycles, the channel selection bits must remain unchanged.
        000: 1.5 ADC clock cycles
        001: 2.5 ADC clock cycles
        010: 8.5 ADC clock cycles
        011: 16.5 ADC clock cycles
        100: 32.5 ADC clock cycles
        101: 64.5 ADC clock cycles
        110: 387.5 ADC clock cycles
        111: 810.5 ADC clock cycles
    Note: The software is allowed to write these bits only when ADSTART=0 and JADSTART=0
    (which ensures that no conversion is ongoing).
    
    ADC sample time register 2 (ADC_SMPR2)
    
    Bits 29:0 SMP[19:10][2:0]: Channel x sampling time selection (x = 10 to 19)
    These bits are written by software to select the sampling time individually for each channel.
    During sampling cycles, the channel selection bits must remain unchanged.
        000: 1.5 ADC clock cycles
        001: 2.5 ADC clock cycles
        010: 8.5 ADC clock cycles
        011: 16.5 ADC clock cycles
        100: 32.5 ADC clock cycles
        101: 64.5 ADC clock cycles
        110: 387.5 ADC clock cycles
        111: 810.5 ADC clock cycles
    Note: The software is allowed to write these bits only when ADSTART=0 and JADSTART=0
    (which ensures that no conversion is ongoing).

    */
    ADC1 -> SMPR2 &= ~(0x07 << (3 * 9));
    ADC1 -> SMPR2 |= 0x07 << (3 * 9);
    
    /*
    
    Bit 0 ADEN: ADC enable control
    This bit is set by software to enable the ADC. The ADC will be effectively ready to operate once the
    flag ADRDY has been set.
    It is cleared by hardware when the ADC is disabled, after the execution of the ADDIS command.
        0: ADC is disabled (OFF state)
        1: Write 1 to enable the ADC.
    Note: The software is allowed to set ADEN only when all bits of ADC_CR registers are 0 (ADCAL=0,
    JADSTART=0, ADSTART=0, ADSTP=0, ADDIS=0 and ADEN=0) except for bit ADVREGEN
    which must be 1 (and the software must have wait for the startup time of the voltage regulator

    */
    ADC1 -> CR |= 0x01 << 0;
    
    /*
    
    Bit 0 ADRDY: ADC ready
    This bit is set by hardware after the ADC has been enabled (bit ADEN=1) and when the ADC
    reaches a state where it is ready to accept conversion requests.
    It is cleared by software writing 1 to it.
        0: ADC not yet ready to start conversion (or the flag event was already acknowledged and cleared
        by software)
        1: ADC is ready to start conversion
        
    */
    while ((ADC1 -> ISR & (0x01 << 0)) == 0)
        ;
}

uint16_t GetAnalogyDigitalConversion(uint8_t channel)
{
    /*
    Channel preselection register (ADC_PCSEL)
    For each channel selected through SQRx or JSQRx, the corresponding ADC_PCSEL bit
    must be previously configured.
    This ADC_PCSEL bit controls the analog switch integrated in the I/O level. The ADC input
    MUX selects the ADC input according to the SQRx and JSQRx with very high speed, the
    analog switch integrated in the IO cannot react as fast as ADC mux does. To avoid the delay
    on analog switch control on IO, it is necessary to pre select the input channels which will be
    selected in the SQRx, JSQRx.
    The selection is based on the VINP[i] of each ADC input. If ADC1 converts the
    ADC123_INP2(VINP[2]) as differential mode, ADC123_INP6(VINP[6]) also needs to be
    selected in ADC_PCSEL.
    Some I/Os are connected to several VINP[i] of the ADCx. The control inputs of the analog
    switch are ORed with the corresponding ADC_PCSEL register bits.
    
    Bits 19:0 PCSEL[19:0] :Channel x (VINP[i]) pre selection (x = 0 to 19)
    These bits are written by software to pre select the input channel at IO instance to be
    converted.
        0: Input Channel x (Vinp x) is not pre selected for conversion, the ADC conversion result with
        this channel shows wrong result.
        1: Input Channel x (Vinp x) is pre selected for conversion
    Note: The software is allowed to write these bits only when ADSTART=0 and JADSTART=0
    (which ensures that no conversion is ongoing).

    */
    ADC1 -> PCSEL |= 0x01 << channel;
    
    /*
    
    Bits 28:24 SQ4[4:0]: 4th conversion in regular sequence
    These bits are written by software with the channel number (0..19) assigned as the 4th in the
    regular conversion sequence.
    
    Bits 22:18 SQ3[4:0]: 3rd conversion in regular sequence
    These bits are written by software with the channel number (0..19) assigned as the 3rd in the
    regular conversion sequence.
    Note: The software is allowed to write these bits only when ADSTART=0 (which ensures that
    no regular conversion is ongoing).
    
    Bits 16:12 SQ2[4:0]: 2nd conversion in regular sequence
    These bits are written by software with the channel number (0..19) assigned as the 2nd in
    the regular conversion sequence.
    Note: The software is allowed to write these bits only when ADSTART=0 (which ensures that
    no regular conversion is ongoing).
    
    Bits 10:6 SQ1[4:0]: 1st conversion in regular sequence
    
    These bits are written by software with the channel number (0..19) assigned as the 1st in the
    regular conversion sequence.
    Note: The software is allowed to write these bits only when ADSTART=0 (which ensures that
    no regular conversion is ongoing).
    
    */
    ADC1 -> SQR1 &= ~(0x1F << 6 * 1);
    ADC1 -> SQR1 |= (channel << (6 * 1));
    
    /*
    
    Bit 2 ADSTART: ADC start of regular conversion
    This bit is set by software to start ADC conversion of regular channels. Depending on the
    configuration bits EXTEN, a conversion will start immediately (software trigger configuration) or once
    a regular hardware trigger event occurs (hardware trigger configuration).
    It is cleared by hardware:
    ¨C in single conversion mode (CONT=0, DISCEN=0) when software trigger is selected
    (EXTEN=0x0): at the assertion of the End of Regular Conversion Sequence (EOS) flag.
    ¨C In discontinuous conversion mode (CONT=0, DISCEN=1), when the software trigger is
    selected (EXTEN=0x0): at the end of conversion (EOC) flag.
    ¨C in all other cases: after the execution of the ADSTP command, at the same time that
    ADSTP is cleared by hardware.
        0: No ADC regular conversion is ongoing.
        1: Write 1 to start regular conversions. Read 1 means that the ADC is operating and eventually
    converting a regular channel.
    Note: The software is allowed to set ADSTART only when ADEN=1 and ADDIS=0 (ADC is enabled
    and there is no pending request to disable the ADC)
    In auto-injection mode (JAUTO=1), regular and auto-injected conversions are started by setting
    bit ADSTART (JADSTART must be kept cleared)
    */
    ADC1 -> CR |= 0x01 << 2;
    
    /*
    
    Bit 2 EOC: End of conversion flag
    This bit is set by hardware at the end of each regular conversion of a channel when a new data is
    available in the ADC_DR register. It is cleared by software writing 1 to it or by reading the ADC_DR
    register
        0: Regular channel conversion not complete (or the flag event was already acknowledged and
        cleared by software)
        1: Regular channel conversion complete

    */
    while ((ADC1 -> ISR & 0x01 << 2) == 0)
        ;
    
    return ADC1 -> DR;
}

uint16_t GetAverageAnalogyDigitalConversion(uint8_t channel, uint8_t times)
{
    uint32_t total = 0;
    
    for (uint8_t i = 0; i < times; i ++)
    {
        total += GetAnalogyDigitalConversion(channel);
        
        DelayMicroseconds(5);
    }
    
    return total / times;
}