#include "stm32h5xx.h"

#include "usart.h"

void init_usart()
{
    /* Enable RCC for USART1, Bit 14. 0b 00000000 00000000 10000000 00000000 */
    RCC -> APB2ENR &= ~(0x01 << 14);
    RCC -> APB2ENR |= (0x01 << 14);
    
    
    
   // USART1 ->
}