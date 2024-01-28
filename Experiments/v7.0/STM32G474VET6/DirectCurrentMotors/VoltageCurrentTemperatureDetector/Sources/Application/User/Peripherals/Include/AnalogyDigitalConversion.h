#ifndef __ANALOGY_DIGITAL_CONVERSION_H
#define __ANALOGY_DIGITAL_CONVERSION_H

#include <stm32g4xx.h>

#define ADC_CHANNELS_NUMBER                                         3
#define ADC_GATHER_TIMES_PER_CHANNEL                                1000

/*
    V_POWER = ADC *£¨3.3f * 25 / 4096£©
 */
#define ADC_TO_VOLTAGE              (float)(3.3f * 25 / 4096)

/*
    I = £¨ADC_Now - Initial_ADC£©* £¨3.3 / 4.096 / 0.12£©
 */
#define ADC_TO_CURRENT              (float)(3.3f / 4.096 / 0.12f)

void InitEmientSensors(void);
float AcquireTemperature(uint16_t conversion);

#endif