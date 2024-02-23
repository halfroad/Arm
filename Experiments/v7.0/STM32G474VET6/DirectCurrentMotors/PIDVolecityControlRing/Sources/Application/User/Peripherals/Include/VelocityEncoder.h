#ifndef __VELOCITY_ENCODER_H
#define __VELOCITY_ENCODER_H

#include <stm32g4xx.h>

#define ENCODER_AUTO_RELOAD                             0xFFFF

typedef struct
{
    int32_t previousCounterNumber;
    int32_t counterNumber;
    float delta;
    
} VelocityEncoderTypeDef;

void InitVelocityEncoder(uint32_t prescaler, uint32_t autoReload);
void InitCalculatorTimer(uint32_t prescaler, uint32_t autoReload);


#endif  /* #ifndef __VELOCITY_ENCODER_H */