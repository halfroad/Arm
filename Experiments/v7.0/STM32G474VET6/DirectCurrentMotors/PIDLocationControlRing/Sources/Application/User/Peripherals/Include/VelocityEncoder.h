#ifndef __VELOCITY_ENCODER_H
#define __VELOCITY_ENCODER_H

#include <stm32g4xx.h>

#define ENCODER_AUTO_RELOAD                     0xFFFF
#define ENCODER_PEAK_PULSE_WIDTH_MODULAION      4200

void InitVelocityEncoder(uint32_t prescaler, uint32_t autoReload);

#endif  /* #ifndef __VELOCITY_ENCODER_H */