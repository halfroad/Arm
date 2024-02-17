#ifndef __VELOCITY_ENCODER_H
#define __VELOCITY_ENCODER_H

#include <stm32g4xx.h>

typedef struct
{
    uint32_t previousCounterNumber;
    uint32_t counterNumber;
    float delta;
    
} VelocityEncoder_TypeDef;

void InitVelocityEncoder(uint32_t prescaler, uint32_t autoReload);
void InitCalculatorTimer(uint32_t prescaler, uint32_t autoReload);


#endif  /* #ifndef __VELOCITY_ENCODER_H */