#ifndef __BREAK_DEAD_TIME_COMPLEMENTARY_OUTPUTS_H

#define __BREAK_DEAD_TIME_COMPLEMENTARY_OUTPUTS_H

#include <stm32g4xx.h>

void InitBreakDeadTimeComplementaryOutputs(uint16_t prescaler, uint16_t autoReload);

void SetBreakDeadTimeCompare(uint8_t deadTime, uint16_t compare);

#endif