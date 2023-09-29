#include <stdint.h>

void init_basic_timer_delay(uint16_t prescaler, uint16_t period, void (*onTimeArrival)());

/* Config the NVIC */
void config_nvic(IRQn_Type irqn, uint32_t preemptionPriority, uint32_t subpriority);
	
void init_basic_timer_irq(uint16_t prescaler, uint16_t period);