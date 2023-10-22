#include <string.h>

#include "stm32h5xx_ll_gpio.h"
#include "stm32h5xx_ll_rcc.h"
#include "stm32h5xx_ll_bus.h"

void usart_gpio_port_init(void)
{
	LL_GPIO_InitTypeDef gpio_init_type 	= { 0 };
	
    /* Enable the clock. */
    LL_APB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOD);
	
	/* PD8: USART3_TX
	*/
	gpio_init_type.Pin					= LL_GPIO_PIN_8 | LL_GPIO_PIN_9;
	gpio_init_type.Mode					= LL_GPIO_MODE_ALTERNATE;
	gpio_init_type.Speed				= LL_GPIO_SPEED_MEDIUM;
	gpio_init_type.OutputType			= LL_GPIO_OUTPUT_PUSHPULL;
	gpio_init_type.Pull					= LL_GPIO_PULL_UP;
	/* AF_7: USART*/
	gpio_init_type.Alternate			= LL_GPIO_AF_7;
	
	LL_GPIO_Init(GPIOD, &gpio_init_type);
	
	/* PD9: USART3_RX
	*/
	memset(&gpio_init_type, 0, sizeof gpio_init_type);
}