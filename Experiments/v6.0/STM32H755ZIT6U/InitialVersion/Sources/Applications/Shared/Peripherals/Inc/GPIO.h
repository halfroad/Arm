#ifndef GPIO_H
#define GPIO_H

#define GREEN_LED_GPIO_PORT     GPIOB
#define GREEN_LED_GPIO_PIN      GPIO_PIN_0

#define YELLOW_LED_GPIO_PIN     GPIO_PIN_14

#define RED_LED_GPIO_PORT       GPIOE
#define RED_LED_GPIO_PIN        GPIO_PIN_1

void InitLEDs();

#endif