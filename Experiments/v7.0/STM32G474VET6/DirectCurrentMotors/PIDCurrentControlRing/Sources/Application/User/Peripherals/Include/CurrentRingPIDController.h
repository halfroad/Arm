#ifndef __CURRENT_RING_PID_CONTROLLER_H
#define __CURRENT_RING_PID_CONTROLLER_H

#include <stm32g4xx.h>

void InitCurrentRingPIDController(void);

void Accelerate(void);
void Decelerate(void);
void Brake(void);

#endif  /*  #ifndef __CURRENT_RING_PID_CONTROLLER_H */