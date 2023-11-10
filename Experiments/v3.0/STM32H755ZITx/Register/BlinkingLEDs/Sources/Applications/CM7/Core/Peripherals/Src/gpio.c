#include "stm32h7xx_bus.h"

#include "gpio.h"

void init_gpios()
{
    GPIO_InitTypeDef gpio_init_type = { 0 };
    
    APB4_GRP1_EnableClock(AHB4_GRP1_PERIPH_GPIOB);
    GPIO_ResetOutputPin(GREEN_LED_GPIO_Port, GREEN_LED_Pin);
    
    gpio_init_type.Pin = GREEN_LED_Pin;
    gpio_init_type.Mode = GPIO_MODE_OUTPUT;
    gpio_init_type.Speed = GPIO_SPEED_FREQ_HIGH;
    gpio_init_type.OutputType = GPIO_OUTPUT_PUSHPULL;
    gpio_init_type.Pull = GPIO_PULL_NO;
    
    GPIO_Init(GREEN_LED_GPIO_Port, &gpio_init_type);
}
