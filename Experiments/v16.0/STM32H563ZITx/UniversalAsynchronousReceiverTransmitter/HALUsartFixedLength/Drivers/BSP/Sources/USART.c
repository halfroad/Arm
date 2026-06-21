#include <stm32h5xx.h>
#include <string.h>

#include "../Includes/USART.h"

/*

Virtual COM port (VCP): LPUSART1/USART3
The STM32H5 Nucleo-144 board offers the flexibility to connect the LPUSART1 or the USART3 interface to the 
STLINK-V3EC, or to the ST morpho and ARDUINO? Uno V3 connectors.
The selection is done by setting the related solder bridges (refer to Table 12 and Table 13 below).
By default, the serial interface USART3 (PD8/PD9) that supports the bootloader is connected and directly 
available as a Virtual COM port of a PC connected to the STLINK-V3EC USB Type-C? connector (CN1).

*/

/*

STM32\STMicroelectronics\MCUs\STM32H563ZI\Product Specifications\stm32h563zi.pdf

Table 14. STM32H562xx and STM32H563xx pin/ball definition (continued)

Pin name                    Alternate functions

PD8                         USART3_TX
PD9                         USART3_RX

*/

/*

STM32\STMicroelectronics\MCUs\STM32H563ZI\Product Specifications\stm32h563zi.pdf

Table 15. Alternate function AF0 to AF7(1) (continued)

Port                        AF7

PD8                         USART3_TX
PD9                         USART3_RX

*/

#define ENABLE_USART_TX_RX_GPIO_RCC_CLOCK()                         do                                              \
                                                                    {                                               \
                                                                        __HAL_RCC_GPIOD_CLK_ENABLE();               \
                                                                    }                                               \
                                                                    while (0)                                       \

#define DISABLE_USART_TX_RX_GPIO_RCC_CLOCK()                        do                                              \
                                                                    {                                               \
                                                                        __HAL_RCC_GPIOD_CLK_DISABLE();              \
                                                                    }                                               \
                                                                    while (0)                                       \

#define ENABLE_USART_TX_RX_RCC_CLOCK()                              do                                              \
                                                                    {                                               \
                                                                        __HAL_RCC_USART3_CLK_ENABLE();              \
                                                                    }                                               \
                                                                    while (0)                                       \
                                                                        
#define DISABLE_USART_TX_RX_RCC_CLOCK()                             do                                              \
                                                                    {                                               \
                                                                        __HAL_RCC_USART3_CLK_DISABLE();             \
                                                                    }                                               \
                                                                    while (0)                                       \

/*

https://community.st.com/stm32cubemx-mcus-29/usart3-not-available-to-use-in-cubemx-137945
This is the VCP , on USART3 on this board connected, so its already used - so it shows this to you.
If you wnat to use it, you have to cut the connections on the board and make your hardware then to the rx/tx PD8/9 .
Why not using it as intended for VCP (or debug printf(...) ) ?
see circuit of this board:

https://community.st.com/stm32-mcus-boards-and-hardware-tools-26/problem-of-using-usart3-on-stmh563zi-131961
Via ZIO connector, simply connect:

USART2_RX (PA3)  to USART3_TX (PD9)
USART2_TX (PD5) to USART3_TX (PD8)

Without disordering anything, you need to ensure that USART3 IOs are not configured to keep them as inputs.
Check the solder bridges mentioned in the documentation. Has anyone played with this board before you?

Is the VCP of the board visible on the host side (lsusb or device manager) ?

Do you mean that I should set USART3 to disabled. Then output the TX RX from USART2 and connect it to output
pin of USART3 (TX to TX RX to RX) than I can read the data from the USB-C of STLINK?

Yes and you need to disable the gpio pins of USART3 no matter it¡¯s is enabled or not. 


*/
#define USART_INSTANCE                                              USART3

#define USART_TX_RX_GPIO_PORT                                       GPIOD
#define USART_TX_GPIO_PIN                                           GPIO_PIN_8
#define USART_RX_GPIO_PIN                                           GPIO_PIN_9

#define GPIO_ALTERNATE_FUNCTION_USART                               GPIO_AF7_USART3
#define USART_IRQN                                                  USART3_IRQn
#define USART_IRQHANDLER                                            USART3_IRQHandler

#define RX_BUFFER_SIZE                                              6
                                                                    
USART_HandleTypeDef USART_HandleType            = { 0 };

uint8_t rx_buffer[RX_BUFFER_SIZE]               = { 0 };
uint8_t tx_buffer[RX_BUFFER_SIZE]               = { 0 };

__IO uint8_t rx_complete                        = 0;
__IO uint8_t tx_complete                        = 0;

extern void Error_Handler(void);

void InitUSART (uint32_t baudRate)
{    
    USART_HandleType.Instance                    = USART_INSTANCE;
    
    USART_HandleType.Init.BaudRate               = baudRate;
    USART_HandleType.Init.WordLength             = USART_WORDLENGTH_8B;
    USART_HandleType.Init.StopBits               = USART_STOPBITS_1;
    USART_HandleType.Init.Parity                 = USART_PARITY_NONE;
    USART_HandleType.Init.Mode                   = USART_MODE_TX_RX;
    USART_HandleType.Init.ClockPrescaler         = USART_PRESCALER_DIV1;
    
    ENABLE_USART_TX_RX_RCC_CLOCK ();

    if (HAL_OK != HAL_USART_Init(&USART_HandleType))
        Error_Handler();
    else
        HAL_USART_Receive_IT(&USART_HandleType, rx_buffer, RX_BUFFER_SIZE);
}

void HAL_USART_MspInit (USART_HandleTypeDef *hUSART)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(hUSART);
    
    if (hUSART -> Instance == USART_INSTANCE)
    {
        GPIO_InitTypeDef GPIO_InitType  = { 0 };
        
        GPIO_InitType.Pin               = USART_TX_GPIO_PIN | USART_RX_GPIO_PIN;
        GPIO_InitType.Mode              = GPIO_MODE_AF_PP;
        GPIO_InitType.Pull              = GPIO_PULLUP;
        GPIO_InitType.Alternate         = GPIO_ALTERNATE_FUNCTION_USART;
        
        ENABLE_USART_TX_RX_GPIO_RCC_CLOCK ();
        
        HAL_GPIO_Init (USART_TX_RX_GPIO_PORT, &GPIO_InitType);
        
        HAL_NVIC_SetPriority (USART_IRQN, 2, 2);
        HAL_NVIC_EnableIRQ (USART_IRQN);
        
        /*
        #define  USE_HAL_USART_REGISTER_CALLBACKS     0U    */ /* USART register callback disabled     */
        // HAL_USART_RegisterCallback(hUSART, HAL_USART_MSPINIT_CB_ID, USART_MspInit);
    }
}

/**
  * @brief DeInitialize the USART MSP.
  * @param hUSART USART handle.
  * @retval None
  */
void HAL_USART_MspDeInit (USART_HandleTypeDef *huart)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(huart);
    
    if (huart -> Instance == USART_INSTANCE)
    {
        DISABLE_USART_TX_RX_GPIO_RCC_CLOCK ();
        DISABLE_USART_TX_RX_RCC_CLOCK ();
    }
}

void USART_IRQHANDLER (void)
{
    HAL_USART_IRQHandler (&USART_HandleType);
}

/**
  * @brief  Rx Transfer completed callback.
  * @param hUSART USART handle.
  * @retval None
  */
void HAL_USART_RxCpltCallback (USART_HandleTypeDef *huart)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(huart);
    
    if (huart -> Instance == USART_INSTANCE)
    {
        rx_complete = 1;
    }
}

/**
  * @brief  Rx Transfer completed callback.
  * @param hUSART USART handle.
  * @retval None
  */
void HAL_USART_TxCpltCallback (USART_HandleTypeDef *huart)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED (huart);
    
    if (huart -> Instance == USART_INSTANCE)
    {
        tx_complete = 1;
    }
    
}

/**
  * @brief USART error callback.
  * @param hUSART USART handle.
  * @retval None
  */
void HAL_USART_ErrorCallback (USART_HandleTypeDef *huart)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(huart);
     // Restart reception on error
    HAL_USART_Receive_IT(huart, rx_buffer, RX_BUFFER_SIZE);
}

void USARTTransmitReceive (void)
{
    if (rx_complete)
    {
        rx_complete = 0;
        
        memcpy(tx_buffer, rx_buffer, RX_BUFFER_SIZE);
        
        /* Transmit the same byte (interrupt driven, non-blocking) */
        HAL_USART_Transmit_IT (&USART_HandleType, tx_buffer, RX_BUFFER_SIZE);
    }
    
    if (tx_complete)
    {
        tx_complete = 0;
        
        HAL_USART_Receive_IT (&USART_HandleType, rx_buffer, RX_BUFFER_SIZE);
    }
}
