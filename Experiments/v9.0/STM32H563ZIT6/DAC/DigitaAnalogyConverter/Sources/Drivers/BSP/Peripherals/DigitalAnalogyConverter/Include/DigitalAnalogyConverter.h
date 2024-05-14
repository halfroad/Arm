#ifndef __DIGITAL_ANALOGY_CONVERTER_H
#define __DIGITAL_ANALOGY_CONVERTER_H

#include <stm32h5xx.h>

void InitDigitalAnalogyConverter(void);

void AssignOutputVoltage(uint16_t voltage);
uint16_t AcquireAssignedVoltage(void);

#endif  /*  #ifndef __DIGITAL_ANALOGY_CONVERTER_H   */