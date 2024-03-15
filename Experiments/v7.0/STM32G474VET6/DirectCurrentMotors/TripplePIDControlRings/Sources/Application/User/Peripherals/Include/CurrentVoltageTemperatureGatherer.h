#ifndef __CURRENT_VOLTAGE_TEMPERATURE_GATHERER_H
#define __CURRENT_VOLTAGE_TEMPERATURE_GATHERER_H

#include <stm32g4xx.h>

void InitCurrentVoltageTemperatureGatherer(void (*onConversionCompletionHandler)(float voltage, float current, float temperature));
void StopAnalogyDigitalConverter(void);

#endif  /*  #ifndef __CURRENT_VOLTAGE_TEMPERATURE_GATHERER_H    */