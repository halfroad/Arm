#include "Delay.h"
#include "ExtendedInterrupts.h"
#include "SystemInterrupts.h"
#include "Standby.h"

static void WaitForInterrupts(void)
{
    __ASM volatile("wfi");
}

static void SystemOnStandby(void)
{
    /*
    
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
    associated GPIO port pull configuration shall be set to the same value or to ¡®00¡¯.
    The Wakeup pin pull configuration is kept in Standby mode.
        00: No pull-up
        01: Pull-up
        10: Pull-down
        11: Reserved

    */
    PWR -> WKUPEPR &= ~(0x03 << 16);
    PWR -> WKUPEPR |= 0x02 << 16;
    
    PWR -> WKUPCR |= 0x3F << 0;
    PWR -> CPUCR |= 0x07 << 0;
    
    SCB -> SCR |= 0x01 << 2;
    
    WaitForInterrupts();
}

static void EnterStandby(void)
{    
    /*
    
    RCC AHB4 peripheral reset register (RCC_AHB4RSTR)
    
    Bit 10 GPIOKRST: GPIOK block reset
    Set and reset by software.
        0: does not reset the GPIOK block (default after reset)
        1: resets the GPIOK block
    
    Bit 9 GPIOJRST: GPIOJ block reset
    Set and reset by software.
        0: does not reset the GPIOJ block (default after reset)
        1: resets the GPIOJ block
        
    Bit 8 GPIOIRST: GPIOI block reset
    Set and reset by software.
        0: does not reset the GPIOI block (default after reset)
        1: resets the GPIOI block
    
    Bit 7 GPIOHRST: GPIOH block reset
    Set and reset by software.
        0: does not reset the GPIOH block (default after reset)
        1: resets the GPIOH block
    
    Bit 6 GPIOGRST: GPIOG block reset
    Set and reset by software.
        0: does not reset the GPIOG block (default after reset)
        1: resets the GPIOG block
    
    Bit 5 GPIOFRST: GPIOF block reset
    Set and reset by software.
        0: does not reset the GPIOF block (default after reset)
        1: resets the GPIOF block
    
    Bit 4 GPIOERST: GPIOE block reset
    Set and reset by software.
        0: does not reset the GPIOE block (default after reset)
        1: resets the GPIOE block
    
    Bit 3 GPIODRST: GPIOD block reset
    Set and reset by software.
        0: does not reset the GPIOD block (default after reset)
        1: resets the GPIOD block
    
    Bit 2 GPIOCRST: GPIOC block reset
    Set and reset by software.
        0: does not reset the GPIOC block (default after reset)
        1: resets the GPIOC block
    
    Bit 1 GPIOBRST: GPIOB block reset
    Set and reset by software.
        0: does not reset the GPIOB block (default after reset)
        1: resets the GPIOB block
    
    Bit 0 GPIOARST: GPIOA block reset
    Set and reset by software.
        0: does not reset the GPIOA block (default after reset)
        1: resets the GPIOA block
    
    Reset all GPIO ports excepts GPIOA.
    
    */
    RCC -> AHB4RSTR |= 0x07FE;
    
    /*
    
    Bits 15:0 IDR[15:0]: Port x input data I/O pin y (y = 15 to 0)
    These bits are read-only. They contain the input value of the corresponding I/O port.

    */
    while ((GPIOA -> IDR & (0x01 << 13)) == 0)
        ;
    
    /*
    
    Bit 0 GPIOARST: GPIOA block reset
    Set and reset by software.
        0: does not reset the GPIOA block (default after reset)
        1: resets the GPIOA block
    
    */
    RCC -> AHB4RSTR |= 0x01 << 0;
    
    /*
    
    Bit 8 DBP: Disable backup domain write protection
    In reset state, the RCC_BDCR register, the RTC registers (including the backup registers),
    BREN and MOEN bits in PWR_CR2 register, are protected against parasitic write access.
    This bit must be set to enable write access to these registers.
        0: Access to RTC, RTC Backup registers and backup SRAM disabled
        1: Access to RTC, RTC Backup registers and backup SRAM enabled
    
    */
    PWR -> CR1 |= 0x01 << 8;
    
    /*
    
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
    
    Bit 15 TSIE: Time-stamp interrupt enable
        0: Time-stamp Interrupt disable
        1: Time-stamp Interrupt enable
        
    Bit 14 WUTIE: Wakeup timer interrupt enable
        0: Wakeup timer interrupt disabled
        1: Wakeup timer interrupt enabled
        
    Bit 13 ALRBIE: Alarm B interrupt enable
        0: Alarm B Interrupt disable
        1: Alarm B Interrupt enable
        
    Bit 12 ALRAIE: Alarm A interrupt enable
        0: Alarm A interrupt disabled
        1: Alarm A interrupt enabled

    */
    uint32_t bits = RTC -> CR & (0x0F << 12);
    
    RTC -> CR &= ~(0x0F << 12);
    
    /*
    Bit 13 TAMP1F: RTC_TAMP1 detection flag
    This flag is set by hardware when a tamper detection event is detected on the RTC_TAMP1
    input.
    It is cleared by software writing 0
    
    Bit 12 TSOVF: Time-stamp overflow flag
    This flag is set by hardware when a time-stamp event occurs while TSF is already set.
    This flag is cleared by software by writing 0. It is recommended to check and then clear
    TSOVF only after clearing the TSF bit. Otherwise, an overflow might not be noticed if a timestamp event occurs immediately before the TSF bit is cleared.
    
    Bit 11 TSF: Time-stamp flag
    This flag is set by hardware when a time-stamp event occurs.
    This flag is cleared by software by writing 0. If ITSF flag is set, TSF must be cleared together
    with ITSF by writing 0 in both bits.

    Bit 10 WUTF: Wakeup timer flag
    This flag is set by hardware when the wakeup auto-reload counter reaches 0.
    This flag is cleared by software by writing 0.
    This flag must be cleared by software at least 1.5 RTCCLK periods before WUTF is set to 1
    again.
    
    Bit 9 ALRBF: Alarm B flag
    This flag is set by hardware when the time/date registers (RTC_TR and RTC_DR) match the
    Alarm B register (RTC_ALRMBR).
    This flag is cleared by software by writing 0.
    
    Bit 8 ALRAF: Alarm A flag
    This flag is set by hardware when the time/date registers (RTC_TR and RTC_DR) match the
    Alarm A register (RTC_ALRMAR).
    This flag is cleared by software by writing 0.
    
    */
    RTC -> ISR &= ~(0x3F << 8);
    
    RTC -> CR |= bits;
    
    RTC -> WPR = 0xFF;
    
    SystemOnStandby();
}

static uint8_t CheckWakeUp(void)
{
    /*
    
    Bits 31:16 BR[15:0]: Port x reset I/O pin y (y = 15 to 0)
    These bits are write-only. A read to these bits returns the value 0x0000.
        0: No action on the corresponding ODRx bit
        1: Resets the corresponding ODRx bit
    Note: If both BSx and BRx are set, BSx has priority.
    
    Bits 15:0 BS[15:0]: Port x set I/O pin y (y = 15 to 0)
    These bits are write-only. A read to these bits returns the value 0x0000.
        0: No action on the corresponding ODRx bit
        1: Sets the corresponding ODRx bit
    
    */
    GPIOB -> BSRR = 0x01 << 0;
    
    uint8_t duration = 0, releasedNumber = 0;
    
    while(1)
    {
        if ((GPIOC -> IDR & 0x2000) == 0x2000)
        {
            duration ++;
            releasedNumber = 0;
        }
        else
        {
            releasedNumber ++;
            
            if (releasedNumber > 3)
            {
                GPIOB -> BSRR = 0x01 << 16;
                
                return 1;
            }
        }
        
        DelayMilliseconds(30);
        
        if (duration >= 100)
        {
            GPIOB -> BSRR = 0x01 << 0;
            
            return 0;
        }
    }
}

void EXTI15_10_IRQHandler(void)
{
    /*
    Bits 21:0 PRx: Configurable event inputs x Pending bit
        0: No trigger request occurred
        1: selected trigger request occurred
    This bit is set when the selected edge event arrives on the external interrupt line. This bit is
    cleared by writing a 1 into the bit or by changing the sensitivity of the edge detector.
    */
    
    EXTI_D1 -> PR1 = 0x01 << 0;
    
    if (CheckWakeUp())
        EnterStandby();
}

void InitWakeUp(void)
{
    /*
    
    Bit 2 GPIOCEN: GPIOC peripheral clock enable
    Set and reset by software.
        0: GPIOC peripheral clock disabled (default after reset)
        1: GPIOC peripheral clock enabled
    
    */
    RCC -> AHB4ENR |= 0x01 << 2;
    
    /*
    
    Bits 31:0 MODER[15:0][1:0]: Port x configuration I/O pin y (y = 15 to 0)
    These bits are written by software to configure the I/O mode.
        00: Input mode
        01: General purpose output mode
        10: Alternate function mode
        11: Analog mode (reset state)
    
    */
    GPIOC -> MODER &= ~(0x03 << 13 * 2);
    
    /*
    
    Bits 31:0 PUPDR[15:0][1:0]: Port x configuration I/O pin y (y = 15 to 0)
    These bits are written by software to configure the I/O pull-up or pull-down
        00: No pull-up, pull-down
        01: Pull-up
        10: Pull-down
        11: Reserved

    */
    GPIOC -> PUPDR &= ~(0x03 << 13 * 2);
    GPIOC -> PUPDR |= 0x02 << 13 * 2;
    
    if (CheckWakeUp() == 1)
    {
        EnterStandby();
    }
    
    ConfigureExternalInterrupt(0x02, 13, 0x00);
    SetNestedVectoredInterruptPriorty(2, 2, EXTI15_10_IRQn);
}
