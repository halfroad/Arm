#include <stdint.h>

#define ACKNOWLEDGEMENT		0
#define	NO_ACKNOWLEDGEMENT	1

void i2c_init(void);

void i2c_start(void);
void i2c_stop(void);

uint8_t i2c_send(uint8_t byte);
uint8_t i2c_read(uint8_t acknowledgement);