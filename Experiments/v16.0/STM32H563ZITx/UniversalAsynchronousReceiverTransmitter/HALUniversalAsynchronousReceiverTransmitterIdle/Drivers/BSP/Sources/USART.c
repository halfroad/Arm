#include <stm32h5xx.h>

#include "../Includes/USART.h"

/*

Virtual COM port (VCP): LPUART1/USART3
The STM32H5 Nucleo-144 board offers the flexibility to connect the LPUART1 or the USART3 interface to the 
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

#define ENABLE_USART_TX_GPIO_RCC_CLOCK()                            do                                              \
                                                                    {                                               \
                                                                        __HAL_RCC_GPIOD_CLK_ENABLE();               \
                                                                    }                                               \
                                                                    while (0)                                       \

#define DISABLE_USART_TX_GPIO_RCC_CLOCK()                           do                                              \
                                                                    {                                               \
                                                                        __HAL_RCC_GPIOD_CLK_DISABLE();              \
                                                                    }                                               \
                                                                    while (0)                                       \

#define ENABLE_USART_TX_RCC_CLOCK()                                 do                                              \
                                                                    {                                               \
                                                                        __HAL_RCC_USART3_CLK_ENABLE();              \
                                                                    }                                               \
                                                                    while (0)                                       \
                                                                        
#define DISABLE_USART_TX_RCC_CLOCK()                                do                                              \
                                                                    {                                               \
                                                                        __HAL_RCC_USART3_CLK_DISABLE();             \
                                                                    }                                               \
                                                                    while (0)                                       \

#define USART_INSTANCE                                              USART3

#define USART_TX_RX_GPIO_PORT                                       GPIOD
#define USART_TX_GPIO_PIN                                           GPIO_PIN_8
#define USART_RX_GPIO_PIN                                           GPIO_PIN_9
#define GPIO_ALTERNATE_FUNCTION_USART                               GPIO_AF7_USART3

#define RX_BUFFER_SIZE                                              1
                                                                    
USART_HandleTypeDef USART_HandleType        = { 0 };

uint8_t rx_buffer[RX_BUFFER_SIZE]           = { 0 };
__IO uint8_t rx_complete                    = 0;

extern void Error_Handler(void);

void InitUSART(uint32_t baudRate)
{    
    USART_HandleType.Instance               = USART_INSTANCE;
    
    USART_HandleType.Init.BaudRate          = baudRate;
    USART_HandleType.Init.WordLength        = USART_WORDLENGTH_8B;
    USART_HandleType.Init.StopBits          = USART_STOPBITS_1;
    USART_HandleType.Init.Parity            = USART_PARITY_NONE;
    USART_HandleType.Init.Mode              = USART_MODE_TX_RX;

    if (HAL_OK != HAL_USART_Init(&USART_HandleType))
        Error_Handler();
}

void HAL_USART_MspInit(USART_HandleTypeDef *husart)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(husart);
    
    if (husart -> Instance == USART_INSTANCE)
    {
        GPIO_InitTypeDef GPIO_InitType  = { 0 };
        
        GPIO_InitType.Pin               = USART_TX_GPIO_PIN | USART_RX_GPIO_PIN;
        GPIO_InitType.Mode              = GPIO_MODE_AF_PP;
        GPIO_InitType.Alternate         = GPIO_ALTERNATE_FUNCTION_USART;
        
        ENABLE_USART_TX_GPIO_RCC_CLOCK ();
        HAL_GPIO_Init (USART_TX_RX_GPIO_PORT, &GPIO_InitType);
        
        ENABLE_USART_TX_RCC_CLOCK ();
        
        HAL_NVIC_SetPriority (USART3_IRQn, 2, 2);
        HAL_NVIC_EnableIRQ (USART3_IRQn);
        
        /*
        #define  USE_HAL_USART_REGISTER_CALLBACKS     0U    */ /* USART register callback disabled     */
        // HAL_USART_RegisterCallback(husart, HAL_USART_MSPINIT_CB_ID, USART_MspInit);
    }
}

/**
  * @brief DeInitialize the USART MSP.
  * @param husart USART handle.
  * @retval None
  */
void HAL_USART_MspDeInit(USART_HandleTypeDef *husart)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(husart);
    
    if (husart -> Instance == USART_INSTANCE)
    {
        DISABLE_USART_TX_GPIO_RCC_CLOCK ();
        DISABLE_USART_TX_RCC_CLOCK();
    }
}

void USART3_IRQHandler(void)
{
    HAL_USART_IRQHandler (&USART_HandleType);
}

/**
  * @brief  Rx Transfer completed callback.
  * @param husart USART handle.
  * @retval None
  */
void HAL_USART_RxCpltCallback(USART_HandleTypeDef *husart)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(husart);
    
    if (husart -> Instance == USART3)
    {
        rx_complete = 1;
    }
}

/**
  * @brief  Rx Transfer completed callback.
  * @param husart USART handle.
  * @retval None
  */
void HAL_USART_TxCpltCallback(USART_HandleTypeDef *husart)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(husart);
    
    HAL_USART_Receive_IT (husart, rx_buffer, RX_BUFFER_SIZE);
    
}

void TransmitReceive (void)
{
    if (rx_complete)
    {
        rx_complete = 0;
        
        /* Transmit the same byte (interrupt driven, non-blocking) */
        HAL_USART_Transmit_IT (&USART_HandleType, rx_buffer, RX_BUFFER_SIZE);
    }
}
