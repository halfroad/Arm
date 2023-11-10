#ifndef GPIO_H
#define GPIO_H

#include "stm32h7xx_gpio.h"

#define GREEN_LED_GPIO_Port     GPIOB
#define GREEN_LED_Pin           GPIO_PIN_0

void init_gpios();


#endif