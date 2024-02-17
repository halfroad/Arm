#ifndef __PID_CONROLLER_H
#define __PID_CONROLLER_H

#include <stm32g4xx.h>

void InitPIDController(uint32_t prescaler, uint32_t period);

#endif  /* #ifndef __PID_CONROLLER_H */