#include "../Include/DigitalAnalogyConversion.h"

void InitDigitalAnalogyConversion(void)
{
    /*
    
    RCC APB1 clock register (RCC_APB1LENR)
    A peripheral can be allocated (enabled) by one or both CPUs. Please refer to
    Section 9.5.10: Peripheral allocation in order to get more information on peripheral
    allocation.
    
    Bit 29 DAC12EN: DAC1 and 2 peripheral clock enable
    Set and reset by software.
        0: DAC1 and 2 peripheral clock disable (default after reset)
        1: DAC1 and 2 peripheral clock enabled
        
    */
    RCC -> APB1LENR |= 0x01 << 29;
    
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
    GPIOA -> MODER |= 0x03 << (5 * 2);
    
    /*
    
    Bits 31:0 PUPDR[15:0][1:0]: Port x configuration I/O pin y (y = 15 to 0)
    These bits are written by software to configure the I/O pull-up or pull-down
        00: No pull-up, pull-down
        01: Pull-up
        10: Pull-down
        11: Reserved
    
    */
    GPIOA -> PUPDR &= ~(0x03 << (5 * 2));
    GPIOA -> PUPDR |= 0x01 << (5 * 2);
    
    /*
    
    Bits 20:16 OTRIM2[4:0]: DAC channel2 offset trimming value
    These bits are available only on dual-channel DACs. Refer to Section 27.3: DAC
    implementation.
    
    Bits 4:0 OTRIM1[4:0]: DAC channel1 offset trimming value
    
    */
    DAC1 -> CCR = 0;
    
    /*
    
    Bits 18:16 MODE2[2:0]: DAC channel2 mode
    These bits can be written only when the DAC is disabled and not in the calibration mode
    (when bit EN2=0 and bit CEN2 =0 in the DAC_CR register). If EN2=1 or CEN2 =1 the write
    operation is ignored.
    They can be set and cleared by software to select the DAC channel2 mode:
    每 DAC channel2 in Normal mode
        000: DAC channel2 is connected to external pin with Buffer enabled
        001: DAC channel2 is connected to external pin and to on chip peripherals with buffer
        enabled
        010: DAC channel2 is connected to external pin with buffer disabled
        011: DAC channel2 is connected to on chip peripherals with Buffer disabled
        每 DAC channel2 in Sample and hold mode
        100: DAC channel2 is connected to external pin with Buffer enabled
        101: DAC channel2 is connected to external pin and to on chip peripherals with Buffer
        enabled
        110: DAC channel2 is connected to external pin and to on chip peripherals with Buffer
        disabled
        111: DAC channel2 is connected to on chip peripherals with Buffer disabled
    Note: This register can be modified only when EN2=0.
    Refer to Section 27.3: DAC implementation for the availability of DAC channel2.
        
    Bits 2:0 MODE1[2:0]: DAC channel1 mode
    These bits can be written only when the DAC is disabled and not in the calibration mode
    (when bit EN1=0 and bit CEN1 =0 in the DAC_CR register). If EN1=1 or CEN1 =1 the write
    operation is ignored.
    They can be set and cleared by software to select the DAC channel1 mode:
    每 DAC channel1 in Normal mode
        000: DAC channel1 is connected to external pin with Buffer enabled
        001: DAC channel1 is connected to external pin and to on chip peripherals with Buffer
        enabled
        010: DAC channel1 is connected to external pin with Buffer disabled
        011: DAC channel1 is connected to on chip peripherals with Buffer disabled
        每 DAC channel1 in sample & hold mode
        100: DAC channel1 is connected to external pin with Buffer enabled
        101: DAC channel1 is connected to external pin and to on chip peripherals with Buffer
        enabled
        110: DAC channel1 is connected to external pin and to on chip peripherals with Buffer
        disabled
        111: DAC channel1 is connected to on chip peripherals with Buffer disabled
    Note: This register can be modified only when EN1=0.
        
    */
    DAC1 -> MCR &= ~(0x07 << 0);
    
    /*
    
    DAC control register (DAC_CR).
    
    */
    DAC1 -> CR = 0;
    
    /*
    
    Bit 1 TEN1: DAC channel1 trigger enable
    
    This bit is set and cleared by software to enable/disable DAC channel1 trigger.
        0: DAC channel1 trigger disabled and data written into the DAC_DHR1 register are
        transferred one dac_pclk clock cycle later to the DAC_DOR1 register
        1: DAC channel1 trigger enabled and data from the DAC_DHR1 register are transferred
    three dac_pclk clock cycles later to the DAC_DOR1 register
    
    Note: When software trigger is selected, the transfer from the DAC_DHR1 register to the
    DAC_DOR1 register takes only one dac_pclk clock cycle.
    
    */
    DAC1 -> CR &= ~(0x01 << 1);
    
    /*
    
    Bits 5:2 TSEL1[3:0]: DAC channel1 trigger selection.
    
    These bits select the external event used to trigger DAC channel1
        0000: SWTRIG1
        0001: dac_ch1_trig1
        0010: dac_ch1_trig2
        ...
        1111: dac_ch1_trig15
    Refer to the trigger selection tables in Section 27.4.2: DAC pins and internal signals for
    details on trigger configuration and mapping.
    
    Note: Only used if bit TEN1 = 1 (DAC channel1 trigger enabled).
    
    */
    DAC1 -> CR &= ~(0x01 << 2);
    
    /*
    
    Bits 7:6 WAVE1[1:0]: DAC channel1 noise/triangle wave generation enable
    These bits are set and cleared by software.
        00: wave generation disabled
        01: Noise wave generation enabled
        1x: Triangle wave generation enabled
    Only used if bit TEN1 = 1 (DAC channel1 trigger enabled).
    
    */
    DAC1 -> CR &= ~(0x03 << 6);
    
    /*
    
    Bit 12 DMAEN1: DAC channel1 DMA enable
    This bit is set and cleared by software.
        0: DAC channel1 DMA mode disabled
        1: DAC channel1 DMA mode enabled
    
    */
    DAC1 -> CR &= ~(0x01 << 12);
    
    /*
    
    Bit 14 CEN1: DAC channel1 calibration enable
    This bit is set and cleared by software to enable/disable DAC channel1 calibration, it can be
    written only if bit EN1=0 into DAC_CR (the calibration mode can be entered/exit only when
    the DAC channel is disabled) Otherwise, the write operation is ignored.
        0: DAC channel1 in Normal operating mode
        1: DAC channel1 in calibration mode
    
    */
    DAC1 -> CR &= ~(0x01 << 14);
    
    /*
    
    Bit 0 EN1: DAC channel1 enable
    This bit is set and cleared by software to enable/disable DAC channel1.
        0: DAC channel1 disabled
        1: DAC channel1 enabled
    
    */
    DAC1 -> CR |= 0x01 << 0;
    
    /*
    
    DAC channel1 12-bit right-aligned data holding register (DAC_DHR12R1).
    
    Bits 11:0 DACC1DHR[11:0]: DAC channel1 12-bit right-aligned data
    These bits are written by software. They specify 12-bit data for DAC channel1.
    
    */
    DAC1 -> DHR12R1 = 0;
}

void SetDigitalAnalogyConversion1Voltage(uint16_t voltage)
{
    float analogy = voltage / 1000.0;
    
    analogy = analogy * 4096 / 3.3;
    
    DAC1 -> DHR12R1 = analogy;
}