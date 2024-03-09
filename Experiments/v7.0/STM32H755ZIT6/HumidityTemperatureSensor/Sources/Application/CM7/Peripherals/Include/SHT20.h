#ifndef __SHT20_H
#define __SHT20_H

#include <stm32h7xx.h>

void InitHumidityTemperatureSensor(void);

uint16_t AcquireTemperature(void);
uint16_t AcquireRelativeHumidity(void);

#endif  /*  #ifndef __SHT20_H   */