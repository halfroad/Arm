#include "stm32h5xx_ll_gpio.h"
#include "stm32h5xx_ll_bus.h"
#include "stm32h5xx_ll_utils.h"


#define GREEN_LED_GPIO_Pin		LL_GPIO_PIN_0
#define GREEN_LED_GPIO_Port		GPIOB

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
void GPIO_Init(void)
{
	LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	/* USER CODE BEGIN MX_GPIO_Init_1 */
	/* USER CODE END MX_GPIO_Init_1 */

	/* GPIO Ports Clock Enable */
	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOH);
	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);

	/**/
	LL_GPIO_ResetOutputPin(GREEN_LED_GPIO_Port, GREEN_LED_GPIO_Pin);

	/**/
	GPIO_InitStruct.Pin = GREEN_LED_GPIO_Pin;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;

	LL_GPIO_Init(GREEN_LED_GPIO_Port, &GPIO_InitStruct);

	/* USER CODE BEGIN MX_GPIO_Init_2 */
	/* USER CODE END MX_GPIO_Init_2 */
}

void flash_led(void)
{
	LL_GPIO_TogglePin(GREEN_LED_GPIO_Port, GREEN_LED_GPIO_Pin);
	
	LL_mDelay(100);
}