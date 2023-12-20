#ifndef SYSTEM_INITIALIZER_H

#define SYSTEM_INITIALIZER_H

#include "stm32h7xx.h"


void InitSystem(uint32_t pll1_prescalerr_m, uint32_t pll1_vco_multiplication_factor_n, uint32_t pll1_division_factor_p, uint32_t pll1_division_factor_q, uint32_t pll1_division_factor_r);

void SoftResetSystem(void);

#endif

