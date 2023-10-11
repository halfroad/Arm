#include "stm32f4xx_usart.h"

void usart_ports_init(uint32_t baudRate);

void send_byte(uint16_t byte);
void send_bytes(uint8_t *bytes, uint16_t length);

uint16_t receive_byte(void);
void receive_bytes(uint8_t *bytes, uint16_t length);