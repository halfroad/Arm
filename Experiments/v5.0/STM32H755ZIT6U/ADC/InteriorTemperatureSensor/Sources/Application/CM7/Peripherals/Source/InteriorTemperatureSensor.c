#include "../Include/Delay.h"
#include "../Include/InteriorTemperatureSensor.h"

#define ADC3_TEMPREATURE_SENSOR_CHANNEL     18

void InitInteriorTemperatureSensor(void)
{
    /*
    
    Bit 24 ADC3EN: ADC3 Peripheral Clocks Enable
    Set and reset by software.
        0: ADC3 peripheral clocks disabled (default after reset)
        1: ADC3 peripheral clocks enabled
    The peripheral clocks of the ADC3 are: the kernel clock selected by ADCSEL and provided to
    adc_ker_ck_input, and the rcc_hclk4 bus interface clock.
        
    */
    RCC -> AHB4ENR |= 0x01 << 24;
    
    /*
    
    Bit 24 ADC3RST: ADC3 block reset
    Set and reset by software.
        0: does not reset the ADC3 block (default after reset)
        1: resets the ADC3 block
    
    */
    
    /*
    
    Reset the ADC3.
    
    */
    RCC -> AHB4RSTR |= 0x01 << 24;
    
    /*
    
    Reset the ADC3 completed.
    
    */
    RCC -> AHB4RSTR &= ~(0x01 << 24);
    
    /*
    
    RCC domain 3 kernel clock configuration register (RCC_D3CCIPR) .
    
    Bits 17:16 ADCSEL[1:0]: SAR ADC kernel clock source selection
    Set and reset by software.
        00: pll2_p_ck clock selected as kernel peripheral clock (default after reset)
        01: pll3_r_ck clock selected as kernel peripheral clock
        10: per_ck clock selected as kernel peripheral clock
    others: reserved, the kernel clock is disabled.
    
    */
    RCC -> D3CCIPR &= ~(0x03 << 16);
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
    
    ADC3_COMMON -> CCR &= ~(0x03 << 16);
    
    */
    
    /*
    
    ADC x common control register (ADCx_CCR) (x=1/2 or 3)
    
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
    
    ADC3_COMMON -> CCR &= ~(0x0F << 18);
    ADC3_COMMON -> CCR |= 0x01 << 18;
    
    /*
    
    Bit 23 TSEN: Temperature sensor voltage enable
    This bit is set and cleared by software to control VSENSE channel.
        0: Temperature sensor channel disabled
        1: Temperature sensor channel enabled
    Note: The software is allowed to write this bit only when the ADCs are disabled (ADCAL=0,
    JADSTART=0, ADSTART=0, ADSTP=0, ADDIS=0 and ADEN=0).
    
    */
    
    ADC3_COMMON -> CCR |= 0x01 << 23;
    
    /*
    Reser ADC3 -> CR.
    
    Bit 0 ADEN: ADC enable control
    This bit is set by software to enable the ADC. The ADC will be effectively ready to operate once the
    flag ADRDY has been set.
    It is cleared by hardware when the ADC is disabled, after the execution of the ADDIS command.
        0: ADC is disabled (OFF state)
        1: Write 1 to enable the ADC.
    Note: The software is allowed to set ADEN only when all bits of ADC_CR registers are 0 (ADCAL=0,
    JADSTART=0, ADSTART=0, ADSTP=0, ADDIS=0 and ADEN=0) except for bit ADVREGEN
    which must be 1 (and the software must have wait for the startup time of the voltage regulator)

    */
    ADC3 -> CR = 0x00;
    
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
    ADC3 -> CR |= 0x01 << 28;
    
    /* There is not any flag in ADC_ISR ,so just wait for a short while to ensure the completion of ADC voltage regulator enablement. */
    DelayMicroseconds(20);
    
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
    BOOST bitfield of the slave ADC
    
    */
    ADC3 -> CR &= ~(0x03 << 8);
    ADC3 -> CR |= 0x03 << 8;
    
    /*
    
    ADC configuration register (ADC_CFGR) 
    
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
    ADC3 -> CFGR &= ~(0x01 << 13);
    
    /*
    
    Bit 12 OVRMOD: Overrun Mode
    This bit is set and cleared by software and configure the way data overrun is managed.
        0: ADC_DR register is preserved with the old data when an overrun is detected.
        1: ADC_DR register is overwritten with the last conversion result when an overrun is detected.
    Note: The software is allowed to write this bit only when ADSTART=0 (which ensures that no regular
    conversion is ongoing).

    */
    ADC3 -> CFGR |= 0x01 << 12;
    
    /*
    
    Bits 11:10 EXTEN[1:0]: External trigger enable and polarity selection for regular channels
    These bits are set and cleared by software to select the external trigger polarity and enable the
    trigger of a regular group.
        00: Hardware trigger detection disabled (conversions can be launched by software)
        01: Hardware trigger detection on the rising edge
        10: Hardware trigger detection on the falling edge
        11: Hardware trigger detection on both the rising and falling edges
    Note: The software is allowed to write 
    
    */
    ADC3 -> CFGR &= ~(0x03 << 10);
    
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
    
    ADC3 -> CFGR &= ~(0x1F << 5);
    
    */
    
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
    
    ADC3 -> CFGR &= ~(0x07 << 2);
    
    /*
    
    ADC configuration register 2 (ADC_CFGR2)
    
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
    ADC3 -> CFGR2 &= ~(0x0F << 28);
    
    /*
    
    Bits 25:16 OSVR[9:0]: Oversampling ratio
    This bitfield is set and cleared by software to define the oversampling ratio.
        0: 1x (no oversampling)
        1: 2x
        2: 3x
        ...
        1023: 1024x
    Note: The software is allowed to 
    
    */
    ADC3 -> CFGR2 &= ~(0x03FF << 16);
    
    /*
    
    Bit 30 ADCALDIF: Differential mode for calibration
    This bit is set and cleared by software to configure the single-ended or differential inputs mode for
    the calibration.
        0: Writing ADCAL will launch a calibration in Single-ended inputs Mode.
        1: Writing ADCAL will launch a calibration in Differential inputs Mode.
    Note: The software is allowed to write this bit only when the ADC is disabled and is not calibrating
    (ADCAL=0, JADSTART=0, JADSTP=0, ADSTART=0, ADSTP=0, ADDIS=0 and ADEN=0).

    */
    ADC3 -> CR &= ~(0x01 << 30);
    
    /*
    
    Bit 16 ADCALLIN: Linearity calibration
    This bit is set and cleared by software to enable the Linearity calibration.
        0: Writing ADCAL will launch a calibration without the Linearity calibration.
        1: Writing ADCAL will launch a calibration with he Linearity calibration.
    Note: The software is allowed to write this bit only when the ADC is disabled and is not calibrating
    (ADCAL=0, JADSTART=0, JADSTP=0, ADSTART=0, ADSTP=0, ADDIS=0 and ADEN=0).
    
    */
    ADC3 -> CR |= 0x01 << 16;
    
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
    ADC3 -> CR |= 0x01 << 31;
    
    while (ADC3 -> CR & (0x01 << 31))
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
    ADC3 -> SQR1 &= ~(0x0F << 0);
    
    /*
    
    Bits 31:30 Reserved, must be kept at reset value.
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
    ADC3 -> SMPR2 &= ~(0x07 << (0x03 * 8));
    ADC3 -> SMPR2 |= 0x07 << (0x03 * 8);
    
    ADC3 -> CR |= 0x01 << 0;
}

uint16_t AcquireAnalogyDigitalConversion3(uint8_t channel)
{
    /*
    
    ADC channel preselection register (ADC_PCSEL) 
    
    Bits 19:0 PCSEL[19:0] :Channel x (VINP[i]) pre selection (x = 0 to 19)
    These bits are written by software to pre select the input channel at IO instance to be
    converted.
        0: Input Channel x (Vinp x) is not pre selected for conversion, the ADC conversion result with
        this channel shows wrong result.
        1: Input Channel x (Vinp x) is pre selected for conversion
    Note: The software is allowed to write these bits only when ADSTART=0 and JADSTART=0
    (which ensures that no conversion is ongoing).
    
    */
    ADC3 -> PCSEL |= 0x01 << channel;
    
    /*
    
    Bits 10:6 SQ1[4:0]: 1st conversion in regular sequence
    These bits are written by software with the channel number (0..19) assigned as the 1st in the
    regular conversion sequence.
    Note: The software is allowed to write these bits only when ADSTART=0 (which ensures that
    no regular conversion is ongoing).

    */
    ADC3 -> SQR1 &= ~(0x1F << (0x01 * 6));
    ADC3 -> SQR1 |= channel << (0x01 * 6);
    
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
    ADC3 -> CR |= 0x01 << 2;
    
    /*
    
    Bit 2 EOC: End of conversion flag
    This bit is set by hardware at the end of each regular conversion of a channel when a new data is
    available in the ADC_DR register. It is cleared by software writing 1 to it or by reading the ADC_DR
    register
        0: Regular channel conversion not complete (or the flag event was already acknowledged and
        cleared by software)
        1: Regular channel conversion complete
    
    */
    while (!(ADC3 -> ISR & 0x01 << 2))
        ;
    
    /*
    
    Bits 31:0 RDATA[31:0]: Regular Data converted
    These bits are read-only. They contain the conversion result from the last converted regular channel.
    The data are left- or right-aligned as described in Section 26.4.27: Data management
    
    */
    return ADC3 -> DR;
}

uint16_t AcquireAverageAnalogyDigitalConversion3(uint8_t channel, uint8_t times)
{
    uint32_t average = 0;
    
    for (uint8_t i = 0; i < times; i ++)
    {
        average += AcquireAnalogyDigitalConversion3(channel);
        
        DelayMilliseconds(5);
    }
    
    average /= times;
    
    return average;
}

uint16_t AcquireInteriorTemperature(void)
{    
    uint16_t ts_calibration1 = *(volatile uint16_t *)(0x1FF1E820);
    uint16_t ts_calibration2 = *(volatile uint16_t *)(0x1FF1E840);
    
    float proportionFactor = (float)(110.0 - 30.0) / (ts_calibration2 - ts_calibration1);
    uint16_t average = AcquireAverageAnalogyDigitalConversion3(ADC3_TEMPREATURE_SENSOR_CHANNEL, 10);
    
    float temperature = proportionFactor * (average - ts_calibration1) + 30.0;
    
    return temperature;
}