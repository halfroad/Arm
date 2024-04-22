#include "SystemInterrupts.h"

void ConfigNestedVectoredInterruptsPriortyGroup(uint8_t groupNumber)
{
    /*
    
    NVIC features
    
    Both CPU1 (Cortex?-M7) and CPU2 (Cortex?-M4) cores have their own nested vector
    interrupt controller (respectively NVIC1 and NVIC2). The NVIC includes the following
    features:
    - up to 150 maskable interrupt channels for STM32H7xxx (not including the 16 interrupt
    lines of Cortex?-M7 / Cortex?-M4 with FPU)
    - 16 programmable priority levels (4 bits of interrupt priority are used)

    AIRCR[10£º8] sets NVIC priority group.
    */
    
    
    /*
    
    i.e. groupNumber = 0x02 = 0b0000 0010;
    
    uint32_t group = ~groupNumber & 0x07 = 1111 1101 & 0b0000 0111 = 0b 0000 0101;
    group <<= 8 = 0b 0000 0101 << 8 = 0b 0000 0000, 0000 0000, 0000 0101, 0000 0000
    
    uint32_t aircr = SCB -> AIRCR;
   
    aircr &= 0x0000F8FF = aircr & 0b 0000 0000, 0000 0000, 1111 1000, 1111 111
    
    aircr |= 0x05FA0000
    aircr |= group
    
    SCB -> AIRCR = aircr
    
    */
    
    /*

    __NVIC_SetPriorityGrouping(NVIC_PRIORTY_GROUP);
    
    */
    
    uint32_t group = ~groupNumber & 0x07UL;
    
    group <<= 8;
    
    uint32_t aircr = SCB -> AIRCR;
    
    aircr &= 0x0000F8FF;
    aircr |= 0x05FA0000;
    aircr |= group;
    
    SCB -> AIRCR = aircr;
}

void SetNestedVectoredInterruptPriorty(uint8_t preemptionPriority, uint8_t subPriority, uint8_t irqn)
{   
    uint32_t preemption = preemptionPriority << (4 - NVIC_PRIORTY_GROUP);
    uint32_t sub = subPriority & (0x0F >> NVIC_PRIORTY_GROUP);
    
    uint32_t priority = preemption | sub;
    
    priority &= 0x0F;
    priority <<= 4;
    
    /*  __IOM uint32_t ISER[8U];               !< Offset: 0x000 (R/W)  Interrupt Set Enable Register */
    NVIC -> ISER[irqn / 32] = 0x01 << (irqn % 32);
    /*  __IOM uint8_t  IP[240U];               !< Offset: 0x300 (R/W)  Interrupt Priority Register (8Bit wide) */
    NVIC -> IP[irqn] |= priority;
}