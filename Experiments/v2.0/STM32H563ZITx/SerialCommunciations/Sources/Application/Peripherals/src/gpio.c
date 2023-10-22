#include "stm32h5xx_ll_gpio.h"
#include "stm32h5xx_ll_bus.h"
#include "stm32h5xx_ll_utils.h"
#include "stm32h5xx_ll_usart.h"


#define USART3RX_TX_GPIO_Pin		LL_GPIO_PIN_8
#define USART3RX_RX_GPIO_Pin		LL_GPIO_PIN_9

/*

https://www.cnblogs.com/mftang2018/p/16617962.html
https://www.st.com/resource/en/reference_manual/rm0481-stm32h563h573-and-stm32h562-armbased-32bit-mcus-stmicroelectronics.pdf
https://moons.link/en/post-378/
https://community.st.com/t5/stm32-mcus-products/usart3-pb10-pb11-not-working-on-nucleo-g474re/m-p/177054
*/

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
void USART_GPIO_Init(void)
{
	LL_GPIO_InitTypeDef gpio_init_type = { 0 };
	/* USER CODE BEGIN MX_GPIO_Init_1 */
	/* USER CODE END MX_GPIO_Init_1 */

	/* GPIO Ports Clock Enable */
	LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOD);
    
    gpio_init_type.Pin = USART3RX_TX_GPIO_Pin | USART3RX_RX_GPIO_Pin;
    gpio_init_type.Mode = LL_GPIO_MODE_ALTERNATE;
    gpio_init_type.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    gpio_init_type.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    gpio_init_type.Pull = LL_GPIO_PULL_NO;
    gpio_init_type.Alternate = LL_GPIO_AF_7;
    
    LL_GPIO_Init(GPIOD, &gpio_init_type);

	/* USER CODE BEGIN MX_GPIO_Init_2 */
	/* USER CODE END MX_GPIO_Init_2 */
}

void USART_Init(uint32_t baudRate)
{
    LL_USART_InitTypeDef usart_init_type = { 0 };
    
    USART_GPIO_Init();
    
    /*GRP2 mneans group 2. */
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART3);
    
    //usart_init_type.PrescalerValue = LL_USART_PRESCALER_DIV1;
    usart_init_type.BaudRate = baudRate;
    usart_init_type.DataWidth = LL_USART_DATAWIDTH_8B;
    usart_init_type.StopBits = LL_USART_STOPBITS_1;
    usart_init_type.Parity = LL_USART_PARITY_NONE;
    usart_init_type.TransferDirection = LL_USART_DIRECTION_TX_RX;;
    usart_init_type.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
    usart_init_type.OverSampling = LL_USART_OVERSAMPLING_8;
    
    LL_USART_Init(USART3, &usart_init_type);
    
    LL_USART_ConfigAsyncMode(USART3);
    
    LL_USART_Enable(USART3);
}

void transmit(uint8_t *bytes)
{
    for (uint8_t i = 0; i < sizeof bytes; i ++)
    {
        LL_USART_TransmitData8(USART3, bytes[i]);
        
        while (!LL_USART_IsActiveFlag_TXE(USART3))
            ;
    }
}
