#include <stdint.h>

#define ACKNOWLEDGEMENT		0
#define	NO_ACKNOWLEDGEMENT	1

void i2c_init(void);

void i2c_generate_start_conditions(void);
void i2c_generate_stopt_conditions(void);

uint8_t i2c_send(uint8_t byte, uint8_t acknowledgement);
uint8_t i2c_read();