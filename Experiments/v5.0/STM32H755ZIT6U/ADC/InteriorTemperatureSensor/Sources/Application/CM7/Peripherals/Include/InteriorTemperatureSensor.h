#ifndef __INTERIOR_TEMPERATURE_SENSOR_H
#define __INTERIOR_TEMPERATURE_SENSOR_H

#include <stm32h7xx.h>

void InitInteriorTemperatureSensor(void);
uint16_t AcquireInteriorTemperature(void);

#endif