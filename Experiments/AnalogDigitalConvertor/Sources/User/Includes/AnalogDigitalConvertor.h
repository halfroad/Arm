#include <stdint.h>

void init_adc_port(uint32_t pin);
void init_adc_input(void);
uint16_t acquire_adc_result(void);