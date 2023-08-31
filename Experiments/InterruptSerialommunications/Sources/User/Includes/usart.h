#include "stm32f4xx.h"

void configure_nvic(IRQn_Type irqn, uint32_t preemptPriority, uint32_t subpriority);
void init_usart_ports(uint32_t baudRate);
void send(uint8_t byte);