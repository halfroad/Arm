#ifndef __VELOCITY_ENCODER_H
#define __VELOCITY_ENCODER_H

#include <stm32g4xx.h>

#include "../../../Middlewares/Third_Party/UpperHostCommunications/Include/UpperHostCommunications.h"


#define ENCODER_AUTO_RELOAD                     0xFFFF
#define ENCODER_PEAK_PULSE_WIDTH_MODULAION      8200


typedef struct
{
    int32_t lastCounterNumber;
    int32_t counterNumber;
    float delta;
    
} VelocityEncoderTypeDef;

void InitVelocityEncoder(uint32_t prescaler, uint32_t autoReload);
void InitCalculatorTimer(uint32_t prescaler, uint32_t autoReload, void (* onPIDComposedHandler)(float newPulseWidthModulation, int16_t velocity, PIDTypeDef *PIDType));

#endif  /* #ifndef __VELOCITY_ENCODER_H */