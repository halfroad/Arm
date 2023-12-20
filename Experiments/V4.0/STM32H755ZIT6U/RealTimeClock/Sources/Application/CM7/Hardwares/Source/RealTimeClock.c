#include "Delay.h"
#include "SystemInterrupts.h"
#include "RealTimeClock.h"

static uint8_t InitRealTimeClockMode(void)
{
    uint32_t retrials = 0XFFFFF;
    
    if (RTC -> ISR &(0x01 << 6))
        return 0;
    
    RTC -> ISR |= 0x01 << 7;
    
    while(retrials &&((RTC -> ISR & (0x01 << 6)) == 0x00))
        retrials--;
    
    if (retrials == 0)
        return 1;
    else
        return 0;
}

static uint8_t RealTimeClockSynchronize(void)
{
    uint32_t retrials = 0xFFFFF;
    
    RTC -> WPR = 0xCA;
    RTC -> WPR = 0x53;
    
    /*
    
    Bit 5 RSF: Registers synchronization flag
    This bit is set by hardware each time the calendar registers are copied into the shadow
    registers (RTC_SSRx, RTC_TRx and RTC_DRx). This bit is cleared by hardware in
    initialization mode, while a shift operation is pending (SHPF=1), or when in bypass shadow
    register mode (BYPSHAD=1). This bit can also be cleared by software.
    It is cleared either by software or by hardware in initialization mode.
        0: Calendar shadow registers not yet synchronized
        1: Calendar shadow registers synchronized
    
    */
    RTC -> ISR &= ~(1 << 5);
    
    while (retrials && ((RTC -> ISR &(0x01 << 5)) == 0x00))
        retrials --;
    
    if(retrials == 0)
        return 1;
    
    RTC -> WPR = 0xFF;
    
    return 0;
}

static void RealTimeClockWriteBackup(uint8_t key, uint32_t value)
{
    uint32_t bits = RTC_BASE + 0x50 + key * 4;
    
    (* (uint32_t *) bits) = value; 
}

static uint32_t RealTimeClockReadBackup(uint8_t key)
{
    uint32_t bits = RTC_BASE + 0x50 + key * 4;
    
    return (* (uint32_t *) bits);
}

static uint8_t DEC2BCD(uint8_t decimals)
{
    uint8_t BCDs = 0; 
    
    while(decimals >= 10)
    {
        BCDs ++;
        decimals-=10;
    } 
    
    return ((uint8_t)(BCDs << 4) | decimals);
}

static uint8_t BCD2DEC(uint8_t BCDs)
{
    uint8_t decimals = (BCDs >> 4) * 10;
    
    return (decimals + (BCDs & 0x0F));
}

uint8_t RealTimeClockSetTime(uint8_t hour, uint8_t minute, uint8_t second, uint8_t ampm)
{
    /*
        
    Bits 7:0 KEY[7:0]: Write protection key
    This byte is written by software.
    Reading this byte always returns 0x00.
    Refer to RTC register write protection for a description of how to unlock RTC register write
    protection.
    
    RTC register write protection
    After system reset, the RTC registers are protected against parasitic write access by
    clearing the DBP bit in the PWR_CR1 register (refer to the power control section). DBP bit
    must be set in order to enable RTC registers write access.
    After Backup domain reset, all the RTC registers are write-protected. Writing to the RTC
    registers is enabled by writing a key into the Write Protection register, RTC_WPR.
    The following steps are required to unlock the write protection on all the RTC registers
    except for RTC_TAMPCR, RTC_BKPxR, RTC_OR and RTC_ISR[13:8].
    1. Write ¡®0xCA¡¯ into the RTC_WPR register.
    2. Write ¡®0x53¡¯ into the RTC_WPR register.
    Writing a wrong key reactivates the write protection.
    The protection mechanism is not affected by system reset.

    */
    RTC -> WPR = 0xCA;
    RTC -> WPR = 0x53;
    
    if (InitRealTimeClockMode())
        return 1;
    
    /*
    
    RTC time register (RTC_TR)
    The RTC_TR is the calendar time shadow register. This register must be written in
    initialization mode only. Refer to Calendar initialization and configuration on page 2087 and
    Reading the calendar on page 2088.
    This register is write protected. The write access procedure is described in RTC register
    write protection on page 2086.
    
    Bit 22 PM: AM/PM notation
        0: AM or 24-hour format
        1: PM
    
    Bits 21:20 HT[1:0]: Hour tens in BCD format
    
    Bits 19:16 HU[3:0]: Hour units in BCD format
    
    Bit 15 Reserved, must be kept at reset value.
    
    Bits 14:12 MNT[2:0]: Minute tens in BCD format
    
    Bits 11:8 MNU[3:0]: Minute units in BCD format
    
    Bit 7 Reserved, must be kept at reset value.
    
    Bits 6:4 ST[2:0]: Second tens in BCD format
    
    Bits 3:0 SU[3:0]: Second units in BCD format
    
    */
    uint32_t bits = ((ampm & 0x01) << 22) | (DEC2BCD(hour) << 16) | (DEC2BCD(minute) << 8) | (DEC2BCD(second) << 0);
    
    RTC -> TR = bits;
    
    /*
    
    Bit 7 INIT: Initialization mode
        0: Free running mode
        1: Initialization mode used to program time and date register (RTC_TR and RTC_DR), and
    prescaler register (RTC_PRER). Counters are stopped and start counting from the new
    value when INIT is reset.
    
    */
    RTC -> ISR &= ~(0x01 << 7);
    
    return 0;
}

uint8_t RealTimeClockSetDate(uint8_t year, uint8_t month, uint8_t day, uint8_t weekday)
{
    
    RTC -> WPR = 0xCA;
    RTC -> WPR = 0x53;
    
    if (InitRealTimeClockMode())
        return 1;
    
    /*
    
    RTC date register (RTC_DR)
    The RTC_DR is the calendar date shadow register. This register must be written in
    initialization mode only. Refer to Calendar initialization and configuration on page 2087 and
    Reading the calendar on page 2088.
    This register is write protected. The write access procedure is described in RTC register
    write protection on page 2086.
    
    Bits 31:24 Reserved, must be kept at reset value.
    
    Bits 23:20 YT[3:0]: Year tens in BCD format
    
    Bits 19:16 YU[3:0]: Year units in BCD format
    
    Bits 15:13 WDU[2:0]: Week day units
        000: forbidden
        001: Monday
        ...
        111: Sunday
        
    Bit 12 MT: Month tens in BCD format
    
    Bits 11:8 MU[3:0]: Month units in BCD format
    
    Bits 7:6 Reserved, must be kept at reset value.
    
    Bits 5:4 DT[1:0]: Date tens in BCD format
    
    Bits 3:0 DU[3:0]: Date units in BCD format
    
    */
    uint32_t bits = ((weekday & 0x07) << 13) | (DEC2BCD(year) << 16) | (DEC2BCD(month) << 8) | (DEC2BCD(day) << 0);
    
    RTC -> DR = bits;
    
    /*
    
    Bit 7 INIT: Initialization mode
        0: Free running mode
        1: Initialization mode used to program time and date register (RTC_TR and RTC_DR), and
    prescaler register (RTC_PRER). Counters are stopped and start counting from the new
    value when INIT is reset.
    
    */
    RTC -> ISR &= ~(0x01 << 7);
    
    return 0;
}

void RealTimeClockGetTime(uint8_t *hour, uint8_t *minute, uint8_t *second, uint8_t *ampm)
{
    while (RealTimeClockSynchronize())
        ;
    
    /*
    
    Bit 22 PM: AM/PM notation
        0: AM or 24-hour format
        1: PM
    Bits 21:20 HT[1:0]: Hour tens in BCD format
    
    Bits 19:16 HU[3:0]: Hour units in BCD format
    
    Bit 15 Reserved, must be kept at reset value.
    
    Bits 14:12 MNT[2:0]: Minute tens in BCD format
    
    Bits 11:8 MNU[3:0]: Minute units in BCD format
    
    Bit 7 Reserved, must be kept at reset value.
    
    Bits 6:4 ST[2:0]: Second tens in BCD format
    
    Bits 3:0 SU[3:0]: Second units in BCD format
        
    */
    uint32_t bits = RTC -> TR;
    
    *ampm = bits >> 22;
    
    *hour = BCD2DEC((bits >> 16) & 0x3F);
    *minute = BCD2DEC((bits >> 8) & 0x7F);
    *second = BCD2DEC((bits >> 0) & 0x7F);
}

void RealTimeClockGetDate(uint8_t *year, uint8_t *month, uint8_t *day, uint8_t *weekday)
{
    /*
    
    Bits 23:20 YT[3:0]: Year tens in BCD format
    
    Bits 19:16 YU[3:0]: Year units in BCD format
    
    Bits 15:13 WDU[2:0]: Week day units
        000: forbidden
        001: Monday
        ...
        111: Sunday
    Bit 12 MT: Month tens in BCD format
    
    Bits 11:8 MU[3:0]: Month units in BCD format
    
    Bits 7:6 Reserved, must be kept at reset value.
    
    Bits 5:4 DT[1:0]: Date tens in BCD format
    
    Bits 3:0 DU[3:0]: Date units in BCD format
    
    */
    
    while (RealTimeClockSynchronize())
        ;
    
    uint32_t bits = RTC -> DR;
    
    *year = BCD2DEC((bits >> 16) & 0xFF);
    *weekday = BCD2DEC((bits >> 13) & 0x07);
    *month = BCD2DEC((bits >> 8) & 0x1F);
    *day = BCD2DEC((bits >> 0) & 0x3F);
}

void RealTimeClockSetAlarmA(uint8_t weekday, uint8_t hour, uint8_t minute, uint8_t second)
{
    /*
        
    Bits 7:0 KEY[7:0]: Write protection key
    This byte is written by software.
    Reading this byte always returns 0x00.
    Refer to RTC register write protection for a description of how to unlock RTC register write
    protection.
    
    RTC register write protection
    After system reset, the RTC registers are protected against parasitic write access by
    clearing the DBP bit in the PWR_CR1 register (refer to the power control section). DBP bit
    must be set in order to enable RTC registers write access.
    After Backup domain reset, all the RTC registers are write-protected. Writing to the RTC
    registers is enabled by writing a key into the Write Protection register, RTC_WPR.
    The following steps are required to unlock the write protection on all the RTC registers
    except for RTC_TAMPCR, RTC_BKPxR, RTC_OR and RTC_ISR[13:8].
    1. Write ¡®0xCA¡¯ into the RTC_WPR register.
    2. Write ¡®0x53¡¯ into the RTC_WPR register.
    Writing a wrong key reactivates the write protection.
    The protection mechanism is not affected by system reset.

    */
        
    RTC -> WPR = 0xCA;
    RTC -> WPR = 0x53;
    
    /*
    
    Bit 8 ALRAE: Alarm A enable
        0: Alarm A disabled
        1: Alarm A enabled
    
    */
    RTC -> CR &= ~(0x01 << 8);
    
    /*
    
    Bit 0 ALRAWF: Alarm A write flag
    This bit is set by hardware when Alarm A values can be changed, after the ALRAE bit has
    been set to 0 in RTC_CR.
    It is cleared by hardware in initialization mode.
        0: Alarm A update not allowed
        1: Alarm A update allowed
    
    */
    while ((RTC -> ISR & 0x01) == 0)
        ;
    
    /*
    
    RTC alarm A register (RTC_ALRMAR)
    This register can be written only when ALRAWF is set to 1 in RTC_ISR, or in initialization
    mode.
    This register is write protected. The write access procedure is described in RTC register
    write protection on page 2086.
    
    */
    
    uint32_t bits = 0;
    
    /*
    
    Bit 30 WDSEL: Week day selection
        0: DU[3:0] represents the date units
        1: DU[3:0] represents the week day. DT[1:0] is don¡¯t care.
        
    */
    bits |= 0x01 << 30;
    
    /*
    
    Bit 22 PM: AM/PM notation
        0: AM or 24-hour format
        1: PM
    
    */
    bits |= 0x00 << 22;
    
    /*
    
    Bits 27:24 DU[3:0]: Date units or day in BCD format.
    
    */
    bits = DEC2BCD(weekday) << 24;
    
    /*
    
    Bits 21:20 HT[1:0]: Hour tens in BCD format.
    Bits 19:16 HU[3:0]: Hour units in BCD format.

    */
    bits |= DEC2BCD(hour) << 16;
    
    /*
    
    Bits 14:12 MNT[2:0]: Minute tens in BCD format.
    Bits 11:8 MNU[3:0]: Minute units in BCD format.

    */
    bits |= DEC2BCD(minute) << 8;
    
    /*
    
    Bits 6:4 ST[2:0]: Second tens in BCD format.
    Bits 3:0 SU[3:0]: Second units in BCD format.

    */
    bits |= DEC2BCD(second) << 0;
    
    /**/
    RTC -> ALRMAR = bits;
    
    /*
    
    This register can be written only when ALRAE is reset in RTC_CR register, or in initialization
    mode.
    Bits 27:24 MASKSS[3:0]: Mask the most-significant bits starting at this bit
        0: No comparison on sub seconds for Alarm A. The alarm is set when the seconds unit is
        incremented (assuming that the rest of the fields match).
        1: SS[14:1] are don¡¯t care in Alarm A comparison. Only SS[0] is compared.
        2: SS[14:2] are don¡¯t care in Alarm A comparison. Only SS[1:0] are compared.
        3: SS[14:3] are don¡¯t care in Alarm A comparison. Only SS[2:0] are compared.
        ...
        12: SS[14:12] are don¡¯t care in Alarm A comparison. SS[11:0] are compared.
        13: SS[14:13] are don¡¯t care in Alarm A comparison. SS[12:0] are compared.
        14: SS[14] is don¡¯t care in Alarm A comparison. SS[13:0] are compared.
        15: All 15 SS bits are compared and must match to activate alarm.
    The overflow bits of the synchronous counter (bits 15) is never compared. This bit can be
    different from 0 only after a shift operation.
    
    Bits 14:0 SS[14:0]: Sub seconds value
    This value is compared with the contents of the synchronous prescaler counter to determine if
    Alarm A is to be activated. Only bits 0 up MASKSS-1 are compared.
    */
    RTC -> ALRMASSR = 0;
    
    /*
    
    Bit 12 ALRAIE: Alarm A interrupt enable
        0: Alarm A interrupt disabled
        1: Alarm A interrupt enabled

    */
    RTC -> CR |= 0x01 << 12;
    
    /*
    
    Bit 8 ALRAE: Alarm A enable
    0: Alarm A disabled
    1: Alarm A enabled

    */
    RTC -> CR |= 0x01 << 8;
    
    /*
        
    Bits 7:0 KEY[7:0]: Write protection key
    This byte is written by software.
    Reading this byte always returns 0x00.
    Refer to RTC register write protection for a description of how to unlock RTC register write
    protection.

    Writing a wrong key reactivates the write protection.
    The protection mechanism is not affected by system reset.

    */
    
    RTC -> WPR = 0xFF;
    
    /*
    
    Bit 8 ALRAF: Alarm A flag
    This flag is set by hardware when the time/date registers (RTC_TR and RTC_DR) match the
    Alarm A register (RTC_ALRMAR).
    This flag is cleared by software by writing 0.
*/
    RTC -> ISR &= ~(0x01 << 8);
    
    EXTI -> PR1 = 0x01 << 17;
    EXTI -> IMR1 |= 0x01 << 17;
    
    /*
    
    Bits 21:0 TRx: Rising trigger event configuration bit of Configurable Event input x.(1)
        0: Rising trigger disabled (for Event and Interrupt) for input line
        1: Rising trigger enabled (for Event and Interrupt) for input line
    1. The Configurable event inputs are edge triggered, no glitch must be generated on these inputs.
    If a rising edge on the Configurable event input occurs during writing of the register, the associated pending bit will not be
    set.
    Rising and falling edge triggers can be set for the same Configurable Event input. In this case, both edges generate a
    trigger.

    */
    EXTI -> RTSR1 |= 0x01 << 17;
    
    SetNestedVectoredInterruptPriorty(2, 2, RTC_Alarm_IRQn);
}

void RealTimeClockPeriodicWakeup(uint8_t weekdaySelection, uint16_t autoReload)
{
    /*
        
    Bits 7:0 KEY[7:0]: Write protection key
    This byte is written by software.
    Reading this byte always returns 0x00.
    Refer to RTC register write protection for a description of how to unlock RTC register write
    protection.
    
    RTC register write protection
    After system reset, the RTC registers are protected against parasitic write access by
    clearing the DBP bit in the PWR_CR1 register (refer to the power control section). DBP bit
    must be set in order to enable RTC registers write access.
    After Backup domain reset, all the RTC registers are write-protected. Writing to the RTC
    registers is enabled by writing a key into the Write Protection register, RTC_WPR.
    The following steps are required to unlock the write protection on all the RTC registers
    except for RTC_TAMPCR, RTC_BKPxR, RTC_OR and RTC_ISR[13:8].
    1. Write ¡®0xCA¡¯ into the RTC_WPR register.
    2. Write ¡®0x53¡¯ into the RTC_WPR register.
    Writing a wrong key reactivates the write protection.
    The protection mechanism is not affected by system reset.

    */
    RTC -> WPR = 0xCA;
    RTC -> WPR = 0x53;
    
    /*
    
    Bit 14 WUTIE: Wakeup timer interrupt enable
        0: Wakeup timer interrupt disabled
        1: Wakeup timer interrupt enabled
    
    */
    RTC -> CR &= ~(0x01 << 10);
    
    /*
    
    Bit 10 WUTE: Wakeup timer enable
        0: Wakeup timer disabled
        1: Wakeup timer enabled
    Note: When the wakeup timer is disabled, wait for WUTWF=1 before enabling it again.

    */
    RTC -> CR &= ~(0x01 << 10);
    
    /*
        
    Bit 2 WUTWF: Wake-up timer write flag
    This bit is set by hardware up to 2 RTCCLK cycles after the WUTE bit has been set to 0 in
    RTC_CR, and is cleared up to 2 RTCCLK cycles after the WUTE bit has been set to 1. The
    wake-up timer values can be changed when WUTE bit is cleared and WUTWF is set.
        0: Wake-up timer configuration update not allowed
        1: Wake-up timer configuration update allowed
    */
    while ((RTC -> ISR & 0x04) == 0)
        ;
    
    /*
    
    Bits 2:0 WUCKSEL[2:0]: Wakeup clock selection
        000: RTC/16 clock is selected
        001: RTC/8 clock is selected
        010: RTC/4 clock is selected
        011: RTC/2 clock is selected
        10x: ck_spre (usually 1 Hz) clock is selected
        11x: ck_spre (usually 1 Hz) clock is selected and 216 is added to the WUT counter value
        (see note below)
    
    */
    RTC -> CR &= ~(0x07 << 0);
    RTC -> CR |= weekdaySelection & 0x07;
    
    /*
    
    Bits 15:0 WUT[15:0]: Wakeup auto-reload value bits
    When the wakeup timer is enabled (WUTE set to 1), the WUTF flag is set every (WUT[15:0]
    + 1) ck_wut cycles. The ck_wut period is selected through WUCKSEL[2:0] bits of the
    RTC_CR register
    When WUCKSEL[2] = 1, the wakeup timer becomes 17-bits and WUCKSEL[1] effectively
    becomes WUT[16] the most-significant bit to be reloaded into the timer.
    The first assertion of WUTF occurs (WUT+1) ck_wut cycles after WUTE is set. Setting
    WUT[15:0] to 0x0000 with WUCKSEL[2:0] =011 (RTCCLK/2) is forbidden. 

    */
    RTC -> WUTR = autoReload;
    
    /*
    
    Bit 10 WUTF: Wakeup timer flag
    This flag is set by hardware when the wakeup auto-reload counter reaches 0.
    This flag is cleared by software by writing 0.
    This flag must be cleared by software at least 1.5 RTCCLK periods before WUTF is set to 1
    again.

    */
    RTC -> ISR &= ~(0x01 << 10);
    
    /*
    
    Bit 14 WUTIE: Wakeup timer interrupt enable
        0: Wakeup timer interrupt disabled
        1: Wakeup timer interrupt enabled

    */
    RTC -> CR |= 0x01 << 14;
    /*
    
    Bit 10 WUTE: Wakeup timer enable
        0: Wakeup timer disabled
        1: Wakeup timer enabled
    Note: When the wakeup timer is disabled, wait for WUTWF=1 before enabling it again.

    */
    RTC -> CR |= 0x01 << 10;
    
    /*
        
    Bits 7:0 KEY[7:0]: Write protection key
    This byte is written by software.
    Reading this byte always returns 0x00.
    Refer to RTC register write protection for a description of how to unlock RTC register write
    protection.
    
    Writing a wrong key reactivates the write protection.
    The protection mechanism is not affected by system reset.

    */
    RTC -> WPR = 0xFF;
    
    EXTI -> PR1 = 0x01 << 19;
    EXTI -> IMR1 = 0x01 << 19;
    
    SetNestedVectoredInterruptPriorty(2, 3, RTC_WKUP_IRQn);
}

uint8_t InitRealTimeClock(void)
{
    /*
    
    Bit 8 DBP: Disable backup domain write protection
    In reset state, the RCC_BDCR register, the RTC registers (including the backup registers),
    BREN and MOEN bits in PWR_CR2 register, are protected against parasitic write access.
    This bit must be set to enable write access to these registers.
        0: Access to RTC, RTC Backup registers and backup SRAM disabled
        1: Access to RTC, RTC Backup registers and backup SRAM enabled
        
    */
    PWR -> CR1 = 0x01 << 8;
    
    uint16_t retrials = 0, subSeconds = 0, backupFlags = RealTimeClockReadBackup(0);
    
    if (backupFlags == 0x5050)
    {
        retrials = 10;
        
        /*
        
        Bits 15:0 SS[15:0]: Sub second value
        SS[15:0] is the value in the synchronous prescaler counter. The fraction of a second is given by
        the formula below:
        Second fraction = (PREDIV_S - SS) / (PREDIV_S + 1)
        Note: SS can be larger than PREDIV_S only after a shift operation. In that case, the correct
        time/date is one second less than as indicated by RTC_TR/RTC_DR.
        
        */
        subSeconds = RTC -> SSR;
        
        while (retrials)
        {
            DelayMicroseconds(10);
            
            if (subSeconds == RTC -> SSR)
                retrials --;
            else
                break;
        }
        
        /* LSE is down, clear the configurations and wait for the next. */
        if (retrials == 0)
        {
            //RealTimeClockWriteBackup(0, 0xFFFF);
            
            /*
            
            Bit 16 BDRST: Backup domain software reset
            Set and reset by software.
                0: Reset not activated (default after backup domain reset)
                1: Resets the entire VSW domain
            
            */
            RCC -> BDCR |= 0x01 << 16;
            
            DelayMilliseconds(10);
            
            /*
            
            Reset is done.
            
            */
            RCC -> BDCR &= 0 << 16;
        }
    }
    else
    {
        /*
        
        Bit 0 LSION: LSI oscillator enable
        Set and reset by software.
            0: LSI is OFF (default after reset)
            1: LSI is ON
        
        */
        RCC -> CSR |= 0x01 << 0;
        
        /*
        
        Bit 1 LSIRDY: LSI oscillator ready
        Set and reset by hardware to indicate when the Low Speed Internal RC oscillator is stable.
        This bit needs 3 cycles of lsi_ck clock to fall down after LSION has been set to ¡¯0¡¯.
        This bit can be set even when LSION is not enabled if there is a request for LSI clock by the Clock
        Security System on LSE or by the Low Speed Watchdog or by the RTC.
            0: LSI clock is not ready (default after reset)
            1: LSI clock is ready
        
        */
        while (!(RCC -> CSR & 0x02))
            ;
        
        /*
        
        Bit 0 LSEON: LSE oscillator enabled
        Set and reset by software.
            0: LSE oscillator OFF (default after backup domain reset)
            1: LSE oscillator ON
        
        */
        /*
        
        Try to enable LSE.
        
        */
        RCC -> BDCR |= 0x01 << 0;
        
        /*
        
        Bit 1 LSERDY: LSE oscillator ready
        Set and reset by hardware to indicate when the LSE is stable. This bit needs 6 cycles of lse_ck
        clock to fall down after LSEON has been set to ¡¯0¡¯.
            0: LSE oscillator not ready (default after backup domain reset)
            1: LSE oscillator ready
        */
        while (retrials && ((RCC -> BDCR & 0x02) == 0))
        {
            retrials --;
            
            DelayMilliseconds(5);
        }
        
        /*
        
        RCC Backup domain control register (RCC_BDCR) 
        
        */
        uint32_t bits = RCC -> BDCR;
        
        /*
        
        Bits 9:8 RTCSEL[1:0]:RTC clock source selection
        Set by software to select the clock source for the RTC. These bits can be written only one time
        (except in case of failure detection on LSE). These bits must be written before LSECSSON is
        enabled. The BDRST bit can be used to reset them, then it can be written one time again.
        If HSE is selected as RTC clock: this clock is lost when the system is in Stop mode or in case of a
        pin reset (NRST).
            00: No clock (default after backup domain reset)
            01: LSE clock used as RTC clock
            10: LSI clock used as RTC clock
            11: HSE clock divided by RTCPRE value is used as RTC clock
        
        */
        bits &= ~(0x03 << 8);
        
        if (retrials == 0)
            bits |= 0x01 << 9;  /* Failed to enable LSE, try to enable the LSI. */
        else
            bits |= 0x01 << 8;  /* LSE Clock as the RTC source. */
        
        /*
        
        Bit 15 RTCEN: RTC clock enable
        Set and reset by software.
            0: rtc_ck clock is disabled (default after backup domain reset)
            1: rtc_ck clock enabled
        
        */
        bits |= 0x01 << 15;
        
        RCC -> BDCR = bits;
        
        /*
        
        Bits 7:0 KEY[7:0]: Write protection key
        This byte is written by software.
        Reading this byte always returns 0x00.
        Refer to RTC register write protection for a description of how to unlock RTC register write
        protection.
        
        RTC register write protection
        After system reset, the RTC registers are protected against parasitic write access by
        clearing the DBP bit in the PWR_CR1 register (refer to the power control section). DBP bit
        must be set in order to enable RTC registers write access.
        After Backup domain reset, all the RTC registers are write-protected. Writing to the RTC
        registers is enabled by writing a key into the Write Protection register, RTC_WPR.
        The following steps are required to unlock the write protection on all the RTC registers
        except for RTC_TAMPCR, RTC_BKPxR, RTC_OR and RTC_ISR[13:8].
        1. Write ¡®0xCA¡¯ into the RTC_WPR register.
        2. Write ¡®0x53¡¯ into the RTC_WPR register.
        Writing a wrong key reactivates the write protection.
        The protection mechanism is not affected by system reset.

        */
        RTC -> WPR = 0xCA;
        RTC -> WPR = 0x53;
        
        /*
        
        RTC control register (RTC_CR)
        
        */
        RTC -> CR = 0;
        
        if (InitRealTimeClockMode())
        {
            /*
            
            Bit 16 BDRST: Backup domain software reset
            Set and reset by software.
                0: Reset not activated (default after backup domain reset)
                1: Resets the entire VSW domain
            
            */
            RCC -> BDCR |= 0x01 << 16;
            
            DelayMicroseconds(10);
            
            /* Reset is done. */
            RCC -> BDCR = 0x00 << 16;
            
            /* Enter RTC initial mode. */
            return 2;
        }
        
        /*
        RTC prescaler register (RTC_PRER)
        
        This register must be written in initialization mode only. The initialization must be performed
        in two separate write accesses. Refer to Calendar initialization and configuration on
        page 2087.
        This register is write protected. The write access procedure is described in RTC register
        write protection on page 2086.
        
        Bits 22:16 PREDIV_A[6:0]: Asynchronous prescaler factor
        This is the asynchronous division factor:
        ck_apre frequency = RTCCLK frequency/(PREDIV_A+1)
        Bit 15 Reserved, must be kept at reset value.
        
        Bits 14:0 PREDIV_S[14:0]: Synchronous prescaler factor
        This is the synchronous division factor:
        ck_spre frequency = ck_apre frequency/(PREDIV_S+1)
        
        */
        /* Bits 14:0 PREDIV_S[14:0]: Synchronous prescaler factor. */
        RTC -> PRER = 0xFF;
        /* Bits 22:16 PREDIV_A[6:0]: Asynchronous prescaler factor. */
        RTC -> PRER |= 0x7F << 16;
        
        /*
        
        Bit 6 FMT: Hour format
            0: 24 hour/day format
            1: AM/PM hour format

        */
        RTC -> CR &= ~(0x01 << 6);
        
        /*
        
        Bit 7 INIT: Initialization mode
            0: Free running mode
            1: Initialization mode used to program time and date register (RTC_TR and RTC_DR), and
        prescaler register (RTC_PRER). Counters are stopped and start counting from the new
        value when INIT is reset.

        */
        RTC -> ISR &= ~(0x01 << 7);
        
        /*
        
        Writing a wrong key reactivates the write protection.
        The protection mechanism is not affected by system reset.
        
        */
        RTC -> WPR = 0xFF;
        
        if (backupFlags != 0x5051)
        {
            RealTimeClockSetTime(15, 12, 10, 0);
            RealTimeClockSetDate(23, 12, 19, 2);
            RealTimeClockSetAlarmA(7, 0, 0, 10);
        }
        
        if (retrials == 0)
            RealTimeClockWriteBackup(0, 0x5051);
        else
            RealTimeClockWriteBackup(0, 0x5050);
    }
    
    //RealTimeSetWakeup(4, 0);
    
    return 0;
}