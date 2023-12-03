#include "SystemInitializer.h"
#include "SystemInterrupts.h"

#define MAXIMUM_RETRIALS    0x7FFF

void EnableCaches()
{
    SCB_EnableICache();
    SCB_EnableDCache();
}

static uint8_t SetSystemClock(uint32_t pll1_prescalerr_m, uint32_t pll1_vco_multiplication_factor_n, uint32_t pll1_division_factor_p, uint32_t pll1_division_factor_q, uint32_t pll1_division_factor_r)
{
    uint16_t retrials = 0;
    uint8_t status = 0;
    
    /*
    
    Bit 2 SCUEN: Supply configuration update enable
        This bit is read-only:
        0: Supply configuration update locked.
        1: Single write enabled to Supply configuration (LDOEN and BYPASS)
    
    Yet, this bit not available on STM32H755xxx. But for the boards that support
    this bit, it is not allowed to change due to the bit is read-only.
    
    Bit 1 LDOEN(1): Low drop-out regulator enable
        0: Low drop-out regulator disabled.
        1: Low drop-out regulator enabled (default)
    Bit 0 BYPASS(1): Power management unit bypass
        0: Power management unit normal operation.
        1: Power management unit bypassed, voltage monitoring still active.
    */
   // PWR -> CR3 &= ~(0x01 << 0);
   
    MODIFY_REG(PWR -> CR3, (PWR_CR3_SMPSLEVEL | PWR_CR3_SMPSEXTHP | PWR_CR3_SMPSEN | PWR_CR3_LDOEN | PWR_CR3_BYPASS), PWR_CR3_SMPSEN);

   
   /*
   
    Bits 15:14 VOS: Voltage scaling selection according to performance
    These bits control the VCORE voltage level and allow to obtains the best trade-off between
    power consumption and performance:
    每 When increasing the performance, the voltage scaling shall be changed before increasing
    the system frequency.
    每 When decreasing performance, the system frequency shall first be decreased before
    changing the voltage scaling.
        00: Reserved (Scale 3 selected).
        01: Scale 3 (default)
        10: Scale 2
        11: Scale 1
        
        Scale 1 for best performance for FLASH.

   */
   PWR -> D3CR &= ~(0x03 << 14);
   PWR -> D3CR |= 0x03 << 14;
   
   /*
   
   Bit 13 VOSRDY: VOS Ready bit for VCORE voltage scaling output selection.
   This bit is set to 1 by hardware when Bypass mode is selected in PWR control register 3
    (PWR_CR3).
        0: Not ready, voltage level below VOS selected level.
        1: Ready, voltage level at or above VOS selected level
    
    Wiat till the Power is stable.

   */
   
   while ((PWR -> D3CR & (0x01 << 13)) == 0)
       ;
   
#ifdef USE_HSE
   
    /*
   
   Bit 16 HSEON: HSE clock enable
    Set and cleared by software.
    Cleared by hardware to stop the HSE when entering Stop or Standby mode.
    This bit cannot be cleared if the HSE is used directly (via SW mux) as system clock or if the HSE is
    selected as reference clock for PLL1 with PLL1 enabled (PLL1ON bit set to ＆1＊).
        0: HSE is OFF (default after reset)
        1: HSE is ON
                   
   */
   RCC -> CR |= (0x01 << 16);
   
   /*
   
   Bit 17 HSERDY: HSE clock ready flag
    Set by hardware to indicate that the HSE oscillator is stable.
        0: HSE clock is not ready (default after reset)
        1: HSE clock is ready

   */
   while ((RCC -> CR & (0x01 << 17)) == 0 && retrials < MAXIMUM_RETRIALS)
       retrials ++;
       
#else
   
    /*
   
   Bit 0 HSION: High Speed Internal clock enable
    Set and cleared by software.
    Set by hardware to force the HSI to ON when the product leaves Stop mode, if STOPWUCK = ＆0＊ or
    STOPKERWUCK = ＆0＊.
    Set by hardware to force the HSI to ON when the product leaves Standby mode or in case of a
    failure of the HSE which is used as the system clock source.
    This bit cannot be cleared if the HSI is used directly (via SW mux) as system clock or if the HSI is
    selected as reference clock for PLL1 with PLL1 enabled (PLL1ON bit set to ＆1＊).
        0: HSI is OFF
        1: HSI is ON (default after reset)
               
   */
   RCC -> CR |= (0x01 << 0);
   
   /*
   
   Bit 2 HSIRDY: HSI clock ready flag
    Set by hardware to indicate that the HSI oscillator is stable.
    0: HSI clock is not ready (default after reset)
    1: HSI clock is ready

   */
   while ((RCC -> CR & (0x01 << 2)) == 0 && retrials < MAXIMUM_RETRIALS)
       retrials ++;

#endif
   
   if (retrials == MAXIMUM_RETRIALS)
       status = 1;
   else
   {
       /*
       
       Bits 1:0 PLLSRC[1:0]: DIVMx and PLLs clock source selection
        Set and reset by software to select the PLL clock source.
        These bits can be written only when all PLLs are disabled.
        In order to save power, when no PLL is used, the value of PLLSRC must be set to ＆11＊.
            00: HSI selected as PLL clock (hsi_ck) (default after reset)
            01: CSI selected as PLL clock (csi_ck)
            10: HSE selected as PLL clock (hse_ck)
            11: No clock send to DIVMx divider and PLLs
       
       */
       
#ifdef USE_HSE
       RCC -> PLLCKSELR &= ~(0x03 << 0);
       RCC -> PLLCKSELR |= 0x02 << 0;
#else
       RCC -> PLLCKSELR &= ~(0x03 << 0);
#endif
       
       /*
       
       Bits 9:4 DIVM1[5:0]: Prescaler for PLL1
        Set and cleared by software to configure the prescaler of the PLL1.
        The hardware does not allow any modification of this prescaler when PLL1 is enabled (PLL1ON =
        ＆1＊).
        In order to save power when PLL1 is not used, the value of DIVM1 must be set to ＊0＊.
            000000: prescaler disabled
            000001: division by 1 (bypass)
            000010: division by 2
            000011: division by 3
            ...
            100000: division by 32 (default after reset)
            ...
            111111: division by 63

       */
       RCC -> PLLCKSELR &= ~(0x3F << 4);
       RCC -> PLLCKSELR |= (pll1_prescalerr_m << 4);
       
       /*
       
       Bits 8:0 DIVN1[8:0]: Multiplication factor for PLL1 VCO
        Set and reset by software to control the multiplication factor of the VCO.
        These bits can be written only when the PLL is disabled (PLL1ON = ＆0＊ and PLL1RDY = ＆0＊).
        0x003: DIVN1 = 4
        0x004: DIVN1 = 5
        0x005: DIVN1 = 6
        ...
        0x080: DIVN1 = 129 (default after reset)
        ...
        0x1FF: DIVN1 = 512
        Others: wrong configurations
        Caution: The software has to set correctly these bits to insure that the VCO output frequency is
        between its valid frequency range, which is:
        每 192 to 836 MHz if PLL1VCOSEL = ＆0＊
        每 150 to 420 MHz if PLL1VCOSEL = ＆1＊
        每
        VCO output frequency = Fref1_ck x DIVN1, when fractional value 0 has been loaded into FRACN1,
        with:
        每 DIVN1 between 4 and 512
        每 The input frequency Fref1_ck between 1MHz and 16 MHz

       */
       RCC -> PLL1DIVR &= ~(0x1FF << 0);
       RCC -> PLL1DIVR |= (pll1_vco_multiplication_factor_n - 1) << 0;
       
       /*
       
       Bits 15:9 DIVP1[6:0]: PLL1 DIVP division factor
        Set and reset by software to control the frequency of the pll1_p_ck clock.
        These bits can be written only when the PLL1 is disabled (PLL1ON = ＆0＊ and PLL1RDY = ＆0＊).
        Note that odd division factors are not allowed.
            0000000: pll1_p_ck = vco1_ck
            0000001: pll1_p_ck = vco1_ck / 2 (default after reset)
            0000010: Not allowed
            0000011: pll1_p_ck = vco1_ck / 4
            ...
            1111111: pll1_p_ck = vco1_ck / 128
       
       */
       RCC -> PLL1DIVR &= ~(0x7F << 9);
       RCC -> PLL1DIVR |= (pll1_division_factor_p - 1) << 9;
       
       /*
       
       Bits 22:16 DIVQ1[6:0]: PLL1 DIVQ division factor
        Set and reset by software to control the frequency of the pll1_q_ck clock.
        These bits can be written only when the PLL1 is disabled (PLL1ON = ＆0＊ and PLL1RDY = ＆0＊).
            0000000: pll1_q_ck = vco1_ck
            0000001: pll1_q_ck = vco1_ck / 2 (default after reset)
            0000010: pll1_q_ck = vco1_ck / 3
            0000011: pll1_q_ck = vco1_ck / 4
            ...
            1111111: pll1_q_ck = vco1_ck / 128
       
       */
       
       RCC -> PLL1DIVR &= ~(0x7F << 16);
       RCC -> PLL1DIVR |= (pll1_division_factor_q - 1) << 16;
       
       /*
       
       Bits 30:24 DIVR1[6:0]: PLL1 DIVR division factor
        Set and reset by software to control the frequency of the pll1_r_ck clock.
        These bits can be written only when the PLL1 is disabled (PLL1ON = ＆0＊ and PLL1RDY = ＆0＊).
            0000000: pll1_r_ck = vco1_ck
            0000001: pll1_r_ck = vco1_ck / 2 (default after reset)
            0000010: pll1_r_ck = vco1_ck / 3
            0000011: pll1_r_ck = vco1_ck / 4
            ...
            1111111: pll1_r_ck = vco1_ck / 128

       */
       
       RCC -> PLL1DIVR &= ~(0x7F << 24);
       RCC -> PLL1DIVR |= (pll1_division_factor_r - 1) << 24;
       
       /*
       
       Bits 3:2 PLL1RGE[1:0]: PLL1 input frequency range
        Set and reset by software to select the proper reference frequency range used for PLL1.
        This bit must be written before enabling the PLL1.
            00: The PLL1 input (ref1_ck) clock range frequency is between 1 and 2 MHz (default after reset)
            01: The PLL1 input (ref1_ck) clock range frequency is between 2 and 4 MHz
            10: The PLL1 input (ref1_ck) clock range frequency is between 4 and 8 MHz
            11: The PLL1 input (ref1_ck) clock range frequency is between 8 and 16 MHz
            
            ref1_ck / pll1_prescalerr_m, update this value accordingly once upon the pll1_prescalerr_m gets changed.
       */
       
       RCC -> PLLCFGR &= ~(0x03 << 2);
       RCC -> PLLCFGR |= 0x02 << 2;
       
       /*
       
       Bit 1 PLL1VCOSEL: PLL1 VCO selection
        Set and reset by software to select the proper VCO frequency range used for PLL1.
        These bits must be written before enabling the PLL1.
        0: Wide VCO range: 192 to 836 MHz (default after reset)
        1: Medium VCO range: 150 to 420 MHz

       */
       RCC -> PLLCFGR &= ~(0x01 << 1);
       
       /*
       
       Bit 17 DIVQ1EN: PLL1 DIVQ divider output enable
        Set and reset by software to enable the pll1_q_ck output of the PLL1.
        In order to save power, when the pll1_q_ck output of the PLL1 is not used, the pll1_q_ck must be
        disabled.
        This bit can be written only when the PLL1 is disabled (PLL1ON = ＆0＊ and PLL1RDY = ＆0＊).
            0: pll1_q_ck output is disabled
            1: pll1_q_ck output is enabled (default after reset)
            
       Bit 16 DIVP1EN: PLL1 DIVP divider output enable
        
        Set and reset by software to enable the pll1_p_ck output of the PLL1.
        This bit can be written only when the PLL1 is disabled (PLL1ON = ＆0＊ and PLL1RDY = ＆0＊).
        In order to save power, when the pll1_p_ck output of the PLL1 is not used, the pll1_p_ck must be
        disabled.
            0: pll1_p_ck output is disabled
            1: pll1_p_ck output is enabled (default after reset)

       */
       RCC -> PLLCFGR |= 0x03 << 16;
       
       /*
       
       Bit 24 PLL1ON: PLL1 enable
        Set and cleared by software to enable PLL1.
        Cleared by hardware when entering Stop or Standby mode. Note that the hardware prevents writing
        this bit to ＊0＊, if the PLL1 output is used as the system clock.
            0: PLL1 OFF (default after reset)
            1: PLL1 ON

       */
       
       RCC -> CR |= 0x01 << 24;
       
       /*
       
       Bit 25 PLL1RDY: PLL1 clock ready flag
        Set by hardware to indicate that the PLL1 is locked.
            0: PLL1 unlocked (default after reset)
            1: PLL1 locked

       */
       while ((RCC -> CR & (0x01 << 25)) == 0)
           ;
       
       /*
       
       Bits 17:12 DIVM2[5:0]: Prescaler for PLL2
        Set and cleared by software to configure the prescaler of the PLL2.
        The hardware does not allow any modification of this prescaler when PLL2 is enabled (PLL2ON =
        ＆1＊).
        In order to save power when PLL2 is not used, the value of DIVM2 must be set to ＊0＊.
            000000: prescaler disabled
            000001: division by 1 (bypass)
            000010: division by 2
            000011: division by 3
            ...
            100000: division by 32 (default after reset)
            ...
            111111: division by 63
       
       */
       RCC -> PLLCKSELR &= ~(0x3F << 12);
       RCC -> PLLCKSELR |= (25 << 12);
       
       /*
       
       Bits 8:0 DIVN2[8:0]: Multiplication factor for PLL2 VCO
        Set and reset by software to control the multiplication factor of the VCO.
        These bits can be written only when the PLL is disabled (PLL2ON = ＆0＊ and PLL2RDY = ＆0＊).
        Caution: The software has to set correctly these bits to insure that the VCO output frequency is
        between its valid frequency range, which is:
        每 192 to 836 MHz if PLL2VCOSEL = ＆0＊
        每 150 to 420 MHz if PLL2VCOSEL = ＆1＊
        VCO output frequency = Fref2_ck x DIVN2, when fractional value 0 has been loaded into FRACN2,
        with
        每 DIVN2 between 4 and 512
        每 The input frequency Fref2_ck between 1MHz and 16MHz
            0x003: DIVN2 = 4
            0x004: DIVN2 = 5
            0x005: DIVN2 = 6
            ...
            0x080: DIVN2 = 129 (default after reset)
            ...
            0x1FF: DIVN2 = 512
            Others: wrong configurations
       
       */
       
       RCC -> PLL2DIVR &= ~(0x1FF << 0);
       RCC -> PLL2DIVR |= (440 - 1) << 0;
       
       /*
       
       Bits 30:24 DIVR2[6:0]: PLL2 DIVR division factor
        Set and reset by software to control the frequency of the pll2_r_ck clock.
        These bits can be written only when the PLL2 is disabled (PLL2ON = ＆0＊ and PLL2RDY = ＆0＊).
            0000000: pll2_r_ck = vco2_ck
            0000001: pll2_r_ck = vco2_ck / 2 (default after reset)
            0000010: pll2_r_ck = vco2_ck / 3
            0000011: pll2_r_ck = vco2_ck / 4
            ...
            1111111: pll2_r_ck = vco2_ck / 128
       
       */
       RCC -> PLL2DIVR &= ~(0x7F << 24);
       RCC -> PLL2DIVR |= (2 - 1) << 24;
       
       /*
       
       Bits 7:6 PLL2RGE[1:0]: PLL2 input frequency range
        Set and reset by software to select the proper reference frequency range used for PLL2.
        These bits must be written before enabling the PLL2.
            00: The PLL2 input (ref2_ck) clock range frequency is between 1 and 2 MHz (default after reset)
            01: The PLL2 input (ref2_ck) clock range frequency is between 2 and 4 MHz
            10: The PLL2 input (ref2_ck) clock range frequency is between 4 and 8 MHz
            11: The PLL2 input (ref2_ck) clock range frequency is between 8 and 16 MHz
       
       */
       RCC -> PLLCFGR &= ~(0x03 << 6);
       
       /*
       
       Bit 5 PLL2VCOSEL: PLL2 VCO selection
        Set and reset by software to select the proper VCO frequency range used for PLL2.
        This bit must be written before enabling the PLL2.
            0: Wide VCO range:192 to 836 MHz (default after reset)
            1: Medium VCO range:150 to 420 MHz
       
       */
       
       RCC -> PLLCFGR &= ~(0x01 << 5);
       
       /*
       
       Bit 21 DIVR2EN: PLL2 DIVR divider output enable
        Set and reset by software to enable the pll2_r_ck output of the PLL2.
        To save power, DIVR3EN and DIVR3 bits must be set to ＊0＊ when the pll3_r_ck is not used.
        This bit can be written only when the PLL2 is disabled (PLL2ON = ＆0＊ and PLL2RDY = ＆0＊).
            0: pll2_r_ck output is disabled
            1: pll2_r_ck output is enabled (default after reset)
       
       */
       RCC -> PLLCFGR |= 0x01 << 21;
       
       /*
       
       Bits 1:0 FMCSEL[1:0]: FMC kernel clock source selection
        00: rcc_hclk3 clock selected as kernel peripheral clock (default after reset)
        01: pll1_q_ck clock selected as kernel peripheral clock
        10: pll2_r_ck clock selected as kernel peripheral clock
        11: per_ck clock selected as kernel peripheral clock

       */
       RCC -> D1CCIPR &= ~(0x03 << 0);
       RCC -> D1CCIPR |= 0x02 << 0;
       
       /*
       
       Bit 26 PLL2ON: PLL2 enable
        Set and cleared by software to enable PLL2.
        Cleared by hardware when entering Stop or Standby mode.
            0: PLL2 OFF (default after reset)
            1: PLL2 ON

       */
       RCC -> CR |= 0x01 << 26;
       
       /*
       
       Bit 27 PLL2RDY: PLL2 clock ready flag
        Set by hardware to indicate that the PLL2 is locked.
            0: PLL2 unlocked (default after reset)
            1: PLL2 locked

       */
       while ((RCC -> CR & (0x01 << 27)) == 0)
           ;
       
       /*
       
       Bits 3:0 HPRE[3:0]: D1 domain AHB prescaler
        Set and reset by software to control the division factor of rcc_hclk3 and rcc_aclk. Changing this
        division ratio has an impact on the frequency of all bus matrix clocks and CPU2 clock.
            0xxx: rcc_hclk3 = sys_d1cpre_ck (default after reset)
            1000: rcc_hclk3 = sys_d1cpre_ck / 2
            1001: rcc_hclk3 = sys_d1cpre_ck / 4
            1010: rcc_hclk3 = sys_d1cpre_ck / 8
            1011: rcc_hclk3 = sys_d1cpre_ck / 16
            1100: rcc_hclk3 = sys_d1cpre_ck / 64
            1101: rcc_hclk3 = sys_d1cpre_ck / 128
            1110: rcc_hclk3 = sys_d1cpre_ck / 256
            1111: rcc_hclk3 = sys_d1cpre_ck / 512
        Note: The clocks are divided by the new prescaler factor from1 to 16 periods of the slowest APB
        clock among rcc_pclk[4:1] after HPRE update.
        Note: Note also that rcc_hclk3 = rcc_aclk.
       
       */
       RCC -> D1CFGR &= ~(0x0F << 0);
       RCC -> D1CFGR |= 0x08 << 0;
       
       /*
       
       Bits 11:8 D1CPRE[3:0]: D1 domain Core prescaler
        Set and reset by software to control D1 domain CPU clock division factor.
        Changing this division ratio has an impact on the frequency of CPU1 clock, all bus matrix clocks
        and CPU2 clock.
        The clocks are divided by the new prescaler factor. This factor ranges from 1 to 16 periods of the
        slowest APB clock among rcc_pclk[4:1] after D1CPRE update. The application can check if the
        new division factor is taken into account by reading back this register.
        0xxx: sys_ck not divided (default after reset)
            1000: sys_ck divided by 2
            1001: sys_ck divided by 4
            1010: sys_ck divided by 8
            1011: sys_ck divided by 16
            1100: sys_ck divided by 64
            1101: sys_ck divided by 128
            1110: sys_ck divided by 256
            1111: sys_ck divided by 512
       
       */
       
       RCC -> D1CFGR &= ~(0x0F << 8);
       RCC -> D1CFGR |= 0x08 << 8;
       
       /*
       
       Bits 2:0 SW[2:0]: System clock switch
        Set and reset by software to select system clock source (sys_ck).
        Set by hardware in order to:
        每 force the selection of the HSI or CSI (depending on STOPWUCK selection) when leaving a
        system Stop mode
        每 force the selection of the HSI in case of failure of the HSE when used directly or indirectly
        as system clock.
            000: HSI selected as system clock (hsi_ck) (default after reset)
            001: CSI selected as system clock (csi_ck)
            010: HSE selected as system clock (hse_ck)
            011: PLL1 selected as system clock (pll1_p_ck)
            others: Reserved
       
       */
       RCC -> CFGR &= ~(0x07 << 0);
       RCC -> CFGR |= 0x03 << 0;
       
       while (1)
       {
           /*
           
           Bits 5:3 SWS[2:0]: System clock switch status
            Set and reset by hardware to indicate which clock source is used as system clock.
                000: HSI used as system clock (hsi_ck) (default after reset)
                001: CSI used as system clock (csi_ck)
                010: HSE used as system clock (hse_ck)
                011: PLL1 used as system clock (pll1_p_ck)
                others: Reserved
           
           0x07 << 3 == 0b0000 0111 << 3 = 0b0011 1000
           
           */
           retrials = (RCC -> CFGR & (0x07 << 3)) >> 3;
           
           if (retrials == 3)
               break;
       }
       
       /*
       
       Bits 3:0 LATENCY: Read latency
        These bits are used to control the number of wait states used during read operations on both
        non-volatile memory banks. The application software has to program them to the correct
        value depending on the embedded Flash memory interface frequency and voltage
        conditions.
            0000: zero wait state used to read a word from non-volatile memory
            0001: one wait state used to read a word from non-volatile memory
            0010: two wait states used to read a word from non-volatile memory
            ...
            0100: four wait states used to read a word from non-volatile memory
        Note: No check is performed by hardware to verify that the configuration is correct
               
       */
       
       /*
       FLASH -> ACR &= ~(0x0F << 0);
       FLASH -> ACR |= 0x02 << 0;
       
       
       Bits 5:4 WRHIGHFREQ: Flash signal delay
        These bits are used to control the delay between non-volatile memory signals during
        programming operations. The application software has to program them to the correct value
        depending on the embedded Flash memory interface frequency. Please refer to Table 16 for
        details.
        
        
        Number of wait states
        (LATENCY)
        Programming
        delay
        (WRHIGHFREQ)
        AXI Interface clock frequency vs VCORE range
        VOS3 range
        0.95 V - 1.05 V
        VOS2 range
        1.05 V - 1.15 V
        VOS1 range
        1.15 V - 1.26 V
        VOS0 range
        1.26 V - 1.40 V
        0 WS (1
        FLASH clock
        cycle)
        00 [0;45 MHz] [0;55 MHz] [0;70 MHz] [0;70 MHz]
        1 WS (2
        FLASH clock
        cycles)
        01 ]45 MHz;90 MHz] ]55 MHz;110 MHz] ]70 MHz;140 MHz] ]70 MHz;140 MHz]
        2 WS (3
        FLASH clock
        cycles)
        01 ]90 MHz;135 MHz] ]110 MHz;165 MHz] ]140 MHz;185 MHz] ]140 MHz;185 MHz]
        10 - - ]185 MHz;210 MHz] ]185 MHz;210 MHz]
        3 WS (4
        FLASH clock
        cycles)
        10 ]135 MHz;180 MHz] ]165 MHz;225 MHz] ]210 MHz;225 MHz] ]210 MHz;225 MHz]
        4 WS (5
        FLASH clock
        cycles)
        10 ]180 MHz;225 MHz] 225 MHz - ]225 MHz;240 MHz]


        Note: No check is performed by hardware to verify that the configuration is correct.
               
       */
       
       /*
       
       FLASH -> ACR &= (0x03 << 4);
       FLASH -> ACR |= 0x02 << 4;
       
       
       Bits 6:4 D1PPRE[2:0]: D1 domain APB3 prescaler
        Set and reset by software to control the division factor of rcc_pclk3.
        The clock is divided by the new prescaler factor from 1 to 16 cycles of rcc_hclk3 after D1PPRE
        write.
        0xx: rcc_pclk3 = rcc_hclk3 (default after reset)
            100: rcc_pclk3 = rcc_hclk3 / 2
            101: rcc_pclk3 = rcc_hclk3 / 4
            110: rcc_pclk3 = rcc_hclk3 / 8
            111: rcc_pclk3 = rcc_hclk3 / 16 
       
       */
       
       RCC -> D1CFGR &= ~(0x07 << 4);
       RCC -> D1CFGR |= 0x04 << 4;
       
       /*
       
       Bits 6:4 D2PPRE1[2:0]: D2 domain APB1 prescaler
        Set and reset by software to control D2 domain APB1 clock division factor.
        The clock is divided by the new prescaler factor from 1 to 16 cycles of rcc_hclk1 after D2PPRE1
        write.
        0xx: rcc_pclk1 = rcc_hclk1 (default after reset)
            100: rcc_pclk1 = rcc_hclk1 / 2
            101: rcc_pclk1 = rcc_hclk1 / 4
            110: rcc_pclk1 = rcc_hclk1 / 8
            111: rcc_pclk1 = rcc_hclk1 / 16
       
       */
       RCC -> D2CFGR &= ~(0x07 << 4);
       RCC -> D2CFGR |= 0x04 << 4;
       
       /*
       
       Bits 10:8 D2PPRE2[2:0]: D2 domain APB2 prescaler
        Set and reset by software to control D2 domain APB2 clock division factor.
        The clock is divided by the new prescaler factor from 1 to 16 cycles of rcc_hclk1 after D2PPRE2
        write.
        0xx: rcc_pclk2 = rcc_hclk1 (default after reset)
            100: rcc_pclk2 = rcc_hclk1 / 2
            101: rcc_pclk2 = rcc_hclk1 / 4
            110: rcc_pclk2 = rcc_hclk1 / 8
            111: rcc_pclk2 = rcc_hclk1 / 16
       
       */
       RCC -> D2CFGR &= ~(0x07 << 8);
       RCC -> D2CFGR |= 0x04 << 8;
       
       /*
       
       Bits 6:4 D3PPRE[2:0]: D3 domain APB4 prescaler
        Set and reset by software to control D3 domain APB4 clock division factor.
        The clock is divided by the new prescaler factor from 1 to 16 cycles of rcc_hclk4 after D3PPRE
        write.
        0xx: rcc_pclk4 = rcc_hclk4 (default after reset)
            100: rcc_pclk4 = rcc_hclk4 / 2
            101: rcc_pclk4 = rcc_hclk4 / 4
            110: rcc_pclk4 = rcc_hclk4 / 8
            111: rcc_pclk4 = rcc_hclk4 / 16
       
       */
       RCC -> D3CFGR &= ~(0x07 << 4);
       RCC -> D3CFGR |= (0x04 << 4);
       
       /*
       
       Bit 7 CSION: CSI clock enable
        Set and reset by software to enable/disable CSI clock for system and/or peripheral.
        Set by hardware to force the CSI to ON when the system leaves Stop mode, if STOPWUCK = ＆1＊ or
        STOPKERWUCK = ＆1＊.
        This bit cannot be cleared if the CSI is used directly (via SW mux) as system clock or if the CSI is
        selected as reference clock for PLL1 with PLL1 enabled (PLL1ON bit set to ＆1＊).
            0: CSI is OFF (default after reset)
            1: CSI is ON
       
       */
       RCC -> CR |= 0x01 << 7;
       
       /*
       
       Bit 1 SYSCFGEN: SYSCFG peripheral clock enable
        Set and reset by software.
            0: SYSCFG peripheral clock disabled (default after reset)
            1: SYSCFG peripheral clock enabled

       
       */
       RCC -> APB4ENR |= 0x01 << 1;
       
       /*
       
       Bit 0 EN: Enable
        This bit enables the I/O compensation cell.
            0: I/O compensation cell disabled
            1: I/O compensation cell enabled
       
       */
       SYSCFG -> CCCSR |= 0x01 << 0;
   }
   
   return status;
}

static void SetNVICVectorTable(uint32_t baseAddress, uint32_t offset)
{
    /* Structure type to access the System Control Block (SCB). */
    SCB -> VTOR = baseAddress | (offset & (uint32_t) 0xFFFFFE00);
}

static void InitSystemClock(uint32_t pll1_prescalerr_m, uint32_t pll1_vco_multiplication_factor_n, uint32_t pll1_division_factor_p, uint32_t pll1_division_factor_q)
{
    /*
    
    Bit 0 HSION: High Speed Internal clock enable
        Set and cleared by software.
        Set by hardware to force the HSI to ON when the product leaves Stop mode, if STOPWUCK = ＆0＊ or
        STOPKERWUCK = ＆0＊.
        Set by hardware to force the HSI to ON when the product leaves Standby mode or in case of a
        failure of the HSE which is used as the system clock source.
        This bit cannot be cleared if the HSI is used directly (via SW mux) as system clock or if the HSI is
        selected as reference clock for PLL1 with PLL1 enabled (PLL1ON bit set to ＆1＊).
            0: HSI is OFF
            1: HSI is ON (default after reset)
    
    */
    RCC -> CR = 0x00000001;
    
    /* Reset CFGR, D1CFGR, D2CFGR and D3CFGR. */
    RCC -> CFGR = 0x00000000;
    RCC -> D1CFGR = 0x00000000;
    RCC -> D2CFGR = 0x00000000;
    RCC -> D3CFGR = 0x00000000;
    
    /* Reset PLLCKSELR. */
    RCC -> PLLCKSELR = 0x00000000;
    /* Reset PLLCFGR. */
    RCC -> PLLCFGR = 0x00000000;
    
    /*
    
    RCC clock source interrupt enable register (RCC_CIER)
        0: interrupt disabled (default after reset)
        1: interrupt enabled
    */
    RCC -> CIER = 0x00000000;
    
    /*!< AXI interconnect - TARG 7 issuing functionality modification register,              Address offset: 0x8108          */
    GPV -> AXI_TARG7_FN_MOD |= 0x00000001;
    
    SetSystemClock(pll1_prescalerr_m, pll1_vco_multiplication_factor_n, pll1_division_factor_p, pll1_division_factor_q, 2);
    
    EnableCaches();
    
#ifdef VECTOR_TABLE_ON_RAM

    SetNVICVectorTable(D1_AXISRAM_BASE, 0x00);

#else

    SetNVICVectorTable(FLASH_BANK1_BASE, 0x00);
    
#endif

}

void InitSystem(uint32_t pll1_prescalerr_m, uint32_t pll1_vco_multiplication_factor_n, uint32_t pll1_division_factor_p, uint32_t pll1_division_factor_q, uint32_t pll1_division_factor_r)
{
    SetSystemClock(pll1_prescalerr_m, pll1_vco_multiplication_factor_n, pll1_division_factor_p, pll1_division_factor_q, pll1_division_factor_r);
    SystemCoreClockUpdate();
    
    EnableCaches();
    
    ConfigNestedVectoredInterruptsPriortyGroup(NVIC_PRIORTY_GROUP);
}

static void SoftResetSystem(void)
{
    /*
    
    This reset is generated by software
    when writing SYSRESETREQ bit
    located into AIRCR register of the
    Cortex-M7 core.
    每 Same scope as pwr_bor_rst reset.
            
    */
    SCB -> AIRCR = 0X05FA0000 | (uint32_t) 0x04;
}

static void EnterLowPowerConsumption(void)
{
    /*
    
    The following operating modes are available for the different system blocks (see Table 33):
     CPU subsystem modes:
    每 CRun
        CPU and CPU subsystem peripheral allocated via RCC PERxEN bits are clocked.
        每 CSleep:
    The CPU clocks is stalled and the CPU subsystem allocated peripheral(s) clock
    operate according to RCC PERxLPEN.
    每 CStop:
    CPU and CPU subsystem peripheral clocks are stalled.
     D1 domain and D2 domain modes:
    每 DRun
    The domain bus matrix is clocked:
    - The domain CPU subsystem(a) is in CRun or CSleep mode,
    or
    RM0399 Rev 3 295/3528
    RM0399 Power control (PWR)
    338
    - the other domain CPU subsystem(a) having an allocated peripheral in the domain
    is in CRun or CSleep mode.
    每 DStop
    The domain bus matrix clock is stalled:
    - The domain CPU subsystem is in CStop mode
    and
    - The other domain CPU subsystem has no peripheral allocated in the domain.
    or the other domain CPU subsystem having an allocated peripheral in the domain
    is also in CStop mode
    and
    - At least one PDDS_Dn(b) bit for the domain select DStop.
    每 DStandby
    The domain is powered down:
    - The domain CPU subsystem is in CStop mode
    and
    - The other domain CPU subsystem has no peripheral allocated in the domain
    or the other domain CPU subsystem having an allocated peripheral in the domain
    is also in CStop mode
    and
    - All PDDS_Dn(b) bits for the domain select DStandby mode.
     System /D3 domain modes
    每 Run/Run*
    The system clock and D3 domain bus matrix clock are running:
    - A CPU subsystem is in CRun or CSleep mode
    or
    - A wakeup signal is active. (i.e. System D3 autonomous mode)
    The Run* mode is entered after a POR reset and a wakeup from Standby. In Run*
    mode, the performance is limited and the system supply configuration shall be
    programmed in PWR control register 3 (PWR_CR3). The system enters Run
    mode only when the ACTVOSRDY bit in PWR control status register 1
    (PWR_CSR1) is set to 1.
    每 Stop
    The system clock and D3 domain bus matrix clock is stalled:
    - both CPU subsystems are in CStop mode.
    and
    - all wakeup signals are inactive.
    and
    - At least one PDDS_Dn(b) bit for any domain select Stop mode.
    每 Standby
    The system is powered down:
    - both CPU subsystems are in CStop mode
    and
    - all wakeup signals are inactive.
    
    */
    
    /*!< PWR wakeup enable and polarity register, Address offset: 0x28 
    
    Bits 5:0 WKUPENn+1: Enable Wakeup Pin WKUPn+1
    Each bit is set and cleared by software.
        0: An event on WKUPn+1 pin does not wakeup the system from Standby mode.
        1: A rising or falling edge on WKUPn+1 pin wakes-up the system from Standby mode.
   
    Note: An additional wakeup event is detected if WKUPn+1 pin is enabled (by setting the
    WKUPENn+1 bit) when WKUPn+1 pin level is already high when WKUPPn+1 selects rising
    edge, or low when WKUPPn+1 selects falling edge.
    
    */
    
    PWR -> WKUPEPR &= ~(0x01 << 0);
    PWR -> WKUPEPR |= 0x01 << 0;
    
    /*
    
    Bits 13:8 WKUPPn-7: Wakeup pin polarity bit for WKUPn-7
    These bits define the polarity used for event detection on WKUPn-7 external wakeup pin.
        0: Detection on high level (rising edge)
        1: Detection on low level (falling edge)
    
    */
    PWR -> WKUPEPR &= ~(0x01 << 8);
    
    /*
    
    Bits 27:16 WKUPPUPD[truncate(n/2)-7): Wakeup pin pull configuration for WKUP(truncate(n/2)-7)
    These bits define the I/O pad pull configuration used when WKUPEN(truncate(n/2)-7) = 1. The
    associated GPIO port pull configuration shall be set to the same value or to ＆00＊.
    The Wakeup pin pull configuration is kept in Standby mode.
        00: No pull-up
        01: Pull-up
        10: Pull-down
        11: Reserved
        
    */
    PWR -> WKUPEPR &= (0x03 << 16);
    PWR -> WKUPEPR |= 0x02 << 16;
    
    /*
    
    Bits 5:0 WKUPCn+1: Clear Wakeup pin flag for WKUPn+1.
    These bits are always read as 0.
        0: No effect
        1: Writing 1 clears the WKUPFn+1 Wakeup pin flag (bit is cleared to 0 by hardware)
    
    */
    PWR -> WKUPCR |= 0x3F << 0;
    
    /*!< Offset: 0x010 (R/W)  System Control Register.
    
    The CSleep mode is entered according to Section 7.7.3: Entering low-power modes, when
    the SLEEPDEEP bit in the Cortex?-M System Control register is cleared. 

    
    */
    SCB -> SCR |= 0x01 << 2;
}

static void WaitForInterrupts(void)
{
    __ASM volatile("wfi");
}

/* Disable all interupts except FAULTS and NMI. */
static void DisableInterrupts(void)
{
    __ASM volatile("cpsid i");
}

/* Enable all interrupts. */
static void EnableInterrupts(void)
{
    __ASM volatile("cpsie i");
}

/* Set the top address of stack. 

Not available in Compiler 6.x, only in 5.x.

__asm__ void MSR_MSP(uint32_t address)
{
    MSR MSP, r0
    BX r14
}
*/

static void MSR_MSP(uint32_t address)
{
    __ASM("MSR MSP, r0");
    __ASM("BX r0");
}