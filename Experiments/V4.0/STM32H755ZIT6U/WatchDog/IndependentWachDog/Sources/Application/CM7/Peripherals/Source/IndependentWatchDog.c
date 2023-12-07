#include "IndependentWatchDog.h"

/*

Prescaler: 0 - 7, only LSB 3 bits are significant.
Reload: 0 - 0xFFF.
Divider factor: 
Time(overflow) = 4 * 2^prescaler * reload / 32, with unit is milliseconds, maximum is 256.

*/
void InitIndependentWatchDog(uint8_t prescaler, uint16_t reload)
{
    /*
    
    Bits 15:0 KEY[15:0]: Key value (write only, read 0x0000)
    These bits must be written by software at regular intervals with the key value 0xAAAA,
    otherwise the watchdog generates a reset when the counter reaches 0.
    Writing the key value 0x5555 to enable access to the IWDG_PR, IWDG_RLR and
    IWDG_WINR registers (see Section 48.3.6: Register access protection)
    Writing the key value 0xCCCC starts the watchdog (except if the hardware watchdog option is
    selected)
    
    */
    IWDG1 -> KR = 0x5555;   /*  Writing the key value 0x5555 to enable access to the IWDG_PR, IWDG_RLR and
    IWDG_WINR registers. */
    /*
    
    Bits 2:0 PR[2:0]: Prescaler divider
    These bits are write access protected see Section 48.3.6: Register access protection. They are
    written by software to select the prescaler divider feeding the counter clock. PVU bit of the
    IWDG status register (IWDG_SR) must be reset in order to be able to change the prescaler
    divider.
        000: divider /4
        001: divider /8
        010: divider /16
        011: divider /32
        100: divider /64
        101: divider /128
        110: divider /256
        111: divider /256
    Note: Reading this register returns the prescaler value from the VDD voltage domain. This
    value may not be up to date/valid if a write operation to this register is ongoing. For this
    reason the value read from this register is valid only when the PVU bit in the IWDG
    status register (IWDG_SR) is reset.
        
    */
    IWDG1 -> PR = prescaler;
    
    /*
    
    Bits 11:0 RL[11:0]: Watchdog counter reload value
    These bits are write access protected see Register access protection. They are written by
    software to define the value to be loaded in the watchdog counter each time the value 0xAAAA
    is written in the IWDG key register (IWDG_KR). The watchdog counter counts down from this
    value. The timeout period is a function of this value and the clock prescaler. Refer to the
    datasheet for the timeout information.
    The RVU bit in the IWDG status register (IWDG_SR) must be reset to be able to change the
    reload value.
    Note: Reading this register returns the reload value from the VDD voltage domain. This value
    may not be up to date/valid if a write operation to this register is ongoing on it. For this
    reason the value read from this register is valid only when the RVU bit in the IWDG
    status register (IWDG_SR) is reset.
    
    */
    IWDG1 -> RLR = reload;
    
    /*
    
    These bits must be written by software at regular intervals with the key value 0xAAAA,
    otherwise the watchdog generates a reset when the counter reaches 0.
    
    */
    IWDG1 -> KR = 0xAAAA;
    
    /*
    
    Writing the key value 0xCCCC starts the watchdog.
    
    */
    IWDG1 -> KR = 0xCCCC;
}

void FeedIndependentWatchDog(void)
{
    /*
    
    These bits must be written by software at regular intervals with the key value 0xAAAA,
    otherwise the watchdog generates a reset when the counter reaches 0.
    
    */
    IWDG1 -> KR = 0xAAAA;
}