#include <stm32h5xx.h>
#include <string.h>

#include "../Includes/USART.h"

/*


https://github.com/joaomariolago/stm32h5-zenoh-lwip-demo/blob/master/firmware/Core/Src/usart.c

*/

#define USART_INSTANCE                                      USART3

#define RCC_USART_CLOCK_ENABLE()                            do                                          \
                                                            {                                           \
                                                                __HAL_RCC_USART3_CLK_ENABLE();          \
                                                            }                                           \
                                                            while (0)                                   \

#define RCC_USART_CLOCK_DISABLE()                           do                                          \
                                                            {                                           \
                                                                __HAL_RCC_USART3_CLK_DISABLE();         \
                                                            }                                           \
                                                            while (0)                                   \

/*

um3115-stm32h5-nucleo144-board-mb1404-stmicroelectronics.pdf

Virtual COM port (VCP): LPUART1/USART3
The STM32H5 Nucleo-144 board offers the flexibility to connect the LPUART1 or the USART3 interface to the 
STLINK-V3EC, or to the ST morpho and ARDUINO? Uno V3 connectors.
The selection is done by setting the related solder bridges (refer to Table 12 and Table 13 below).
By default, the serial interface USART3 (PD8/PD9) that supports the bootloader is connected and directly 
available as a Virtual COM port of a PC connected to the STLINK-V3EC USB Type-C? connector (CN1).

Table 12. USART3 connection

Pin name            Definition
PD8                 USART3 TX
PD9                 USART3 RX

Table 15. Alternate function AF0 to AF7(1) (continued)
Port                AF7
PD8                 USART3_TX
PD9                 USART3_TX

*/

#define USART_TX_RX_GPIO_PORT                               GPIOD
#define USART_TX_GPIO_PORT                                  GPIO_PIN_8
#define USART_RX_GPIO_PORT                                  GPIO_PIN_9
#define USART_GPIO_ALTERNATE_FUNCTION                       GPIO_AF7_USART3

#define RCC_USART_GPIO_CLOCK_ENABLE()                       do                                          \
                                                            {                                           \
                                                                __HAL_RCC_GPIOD_CLK_ENABLE();           \
                                                            }                                           \
                                                            while (0)                                   \

#define RCC_USART_GPIO_CLOCK_DISABLE()                      do                                          \
                                                            {                                           \
                                                                __HAL_RCC_GPIOD_CLK_DISABLE();          \
                                                            }                                           \
                                                            while (0)                                   \

#define TX_BUFFER_SIZE                                      128
#define RX_BUFFER_SIZE                                      128
                                                            
#define USART_IRQN                                          USART3_IRQn

#define USART_IRQHANDLER                                    void USART3_IRQHandler (void)
/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
extern void Error_Handler (void);

UART_HandleTypeDef UART_HandleType  = { 0 };

uint8_t tx_buffer[TX_BUFFER_SIZE]   = { 0 };
uint8_t rx_buffer[RX_BUFFER_SIZE]   = { 0 };

__IO uint16_t rx_length             = 0;

__IO uint8_t tx_complete            = 0;
__IO uint8_t rx_complete            = 0;

void InitUSART (uint32_t baudRate)
{
    UART_HandleType.Instance            = USART_INSTANCE;
    
    UART_HandleType.Init.BaudRate       = baudRate;
    UART_HandleType.Init.WordLength     = UART_WORDLENGTH_8B;
    UART_HandleType.Init.StopBits       = UART_STOPBITS_1;
    UART_HandleType.Init.Parity         = UART_PARITY_NONE;
    UART_HandleType.Init.Mode           = UART_MODE_TX_RX;
    UART_HandleType.Init.OverSampling   = UART_OVERSAMPLING_16;
    
    if (HAL_OK != HAL_UART_Init (&UART_HandleType))
        Error_Handler();
    
    if (HAL_OK != HAL_UARTEx_ReceiveToIdle_IT ( &UART_HandleType, rx_buffer, RX_BUFFER_SIZE))
        Error_Handler();
}

/**
  * @brief Initialize the UART MSP.
  * @param huart UART handle.
  * @retval None
  */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(huart);
    
    if (huart -> Instance == USART_INSTANCE)
    {
        GPIO_InitTypeDef GPIO_InitType  = { 0 };
        
        GPIO_InitType.Pin               = USART_TX_GPIO_PORT | USART_RX_GPIO_PORT;
        GPIO_InitType.Mode              = GPIO_MODE_AF_PP;
        GPIO_InitType.Pull              = GPIO_PULLUP;                              /*  Keep RX idle high   */
        GPIO_InitType.Alternate         = USART_GPIO_ALTERNATE_FUNCTION;
        
        RCC_USART_CLOCK_ENABLE();
        RCC_USART_GPIO_CLOCK_ENABLE();
        
        HAL_GPIO_Init (USART_TX_RX_GPIO_PORT, &GPIO_InitType);
        
        /*  NVIC configurations.    */
        HAL_NVIC_SetPriority (USART_IRQN, 2, 2);
        HAL_NVIC_EnableIRQ (USART_IRQN);
    }
}

/**
  * @brief DeInitialize the UART MSP.
  * @param huart UART handle.
  * @retval None
  */
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(huart);

    RCC_USART_CLOCK_DISABLE ();
    HAL_GPIO_DeInit (USART_TX_RX_GPIO_PORT, USART_TX_GPIO_PORT | USART_RX_GPIO_PORT);
    RCC_USART_GPIO_CLOCK_DISABLE ();
}

USART_IRQHANDLER
{
    HAL_UART_IRQHandler (&UART_HandleType);
}

/**
  * @brief  Reception Event Callback (Rx event notification called after use of advanced reception service).
  * @param  huart UART handle
  * @param  Size  Number of data available in application reception buffer (indicates a position in
  *               reception buffer until which, data are available)
  * @retval None
  */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(huart);
  UNUSED(Size);
    
    if (huart -> Instance == USART_INSTANCE)
    {
        if (Size)
        {
            rx_length = Size;
            rx_complete = 1;
        }
        else
        {
            /* Idle event but no data - reset reception */
            HAL_UARTEx_ReceiveToIdle_IT (huart, rx_buffer, RX_BUFFER_SIZE);
        }
    }
}

/**
  * @brief Tx Transfer completed callback.
  * @param huart UART handle.
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(huart);
    
    if (huart -> Instance == USART_INSTANCE)
        tx_complete = 1;
}

/**
  * @brief  UART error callback.
  * @param  huart UART handle.
  * @retval None
  */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    /* Prevent unused argument(s) compilation warning */
    UNUSED(huart);

    if (huart -> Instance == USART_INSTANCE)
    {
        /* On any error, restart reception */
        HAL_UARTEx_ReceiveToIdle_IT (huart, rx_buffer, RX_BUFFER_SIZE);
    }
}

void TransmitReceive (void)
{
    /* Reception completed */
    if (rx_complete)
    {
        rx_complete = 0;
        
        /* Copy received data to transmit buffer (echo) */
        memcpy (tx_buffer, rx_buffer, rx_length);
        
        /* Transmit the exact number of received bytes */
        HAL_UART_Transmit_IT ( &UART_HandleType, tx_buffer, rx_length);
    }
    
    if (tx_complete)
    {
        tx_complete = 0;
        
        /* Restart IDLE reception for the next package */
        HAL_UARTEx_ReceiveToIdle_IT (&UART_HandleType, rx_buffer, RX_BUFFER_SIZE);
    }
}