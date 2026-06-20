#include "../Include/USART.h"


#define ENABLE_USART_GPIO_RCC_CLOCK()           do                                  \
                                                {                                   \
                                                    __HAL_RCC_GPIOD_CLK_ENABLE();   \
                                                }                                   \
                                                while (0)                           \

#define ENABLE_USART_RCC_CLOCK()                do                                  \
                                                {                                   \
                                                    __HAL_RCC_USART3_CLK_ENABLE();  \
                                                }                                   \
                                                while (0)                           \
                                            
#define USART_GPIO_PORT                         GPIOD
#define USART_GPIO_TX_PIN                       GPIO_PIN_8
#define USART_GPIO_RX_PIN                       GPIO_PIN_9

#define USART_INSTANCE                          USART3
#define GPIO_ALTERNATE_FUNCTION                 GPIO_AF7_USART3

#define USART_IRQN                              USART3_IRQn
#define USART_IRQHANDLER                        USART3_IRQHandler

#define RX_TX_DEFAULT_TIMEOUT                   0xFFFF

extern void Error_Handler(void);

USART_HandleTypeDef USART_HandleType            = { 0 };

void InitilizeUniversalAsynchronousReceiverTransmitter (uint32_t baudRate)
{
    USART_HandleType.Instance           = USART_INSTANCE;
    
    USART_HandleType.Init.BaudRate      = baudRate;
    USART_HandleType.Init.WordLength    = USART_WORDLENGTH_8B;
    USART_HandleType.Init.StopBits      = USART_STOPBITS_1;
    USART_HandleType.Init.Parity        = USART_PARITY_NONE;
    USART_HandleType.Init.Mode          = USART_MODE_TX_RX;
    
    if (HAL_OK != HAL_USART_Init(&USART_HandleType))
        Error_Handler();
}

void HAL_USART_MspInit (USART_HandleTypeDef *husart)
{
    if (husart -> Instance == USART_INSTANCE)
    {
        RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
        
        PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART3;
        PeriphClkInitStruct.Usart2ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
        /*
        
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
        {
          Error_Handler();
        }
        */
    
        ENABLE_USART_GPIO_RCC_CLOCK ();
        ENABLE_USART_RCC_CLOCK();
        
        GPIO_InitTypeDef GPIO_InitType = { 0 };
        
        GPIO_InitType.Mode = GPIO_MODE_AF_PP;
        GPIO_InitType.Pin = USART_GPIO_TX_PIN | USART_GPIO_RX_PIN;
        /*
        
        Specifications/stm32h563zi.pdf
        Table 15. Alternate function AF0 to AF7(1) (continued)
        
        Port                AF7
        PD8                 USART3_TX
        PD9                 USART3_RX
        
        */
        GPIO_InitType.Alternate = GPIO_ALTERNATE_FUNCTION;
        
        HAL_GPIO_Init (USART_GPIO_PORT, &GPIO_InitType);
    }
}

HAL_StatusTypeDef Transmit (uint8_t *bytes, uint16_t size)
{
    return HAL_USART_Transmit (&USART_HandleType, bytes, size, HAL_MAX_DELAY);
}

HAL_StatusTypeDef Receive (uint8_t *byte)
{
    return HAL_USART_Receive(&USART_HandleType, byte, 1, HAL_MAX_DELAY);
}