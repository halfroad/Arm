#include <stdint.h>

void init_usart_ports(uint32_t bautRate);

void transmit(uint8_t byte);
uint8_t receive(void);
