#include "stm32f4xx_tim.h"

void init_timer(TIM_TypeDef *tim, uint32_t RCC_APB1Periph, uint32_t prescaler, uint32_t period);