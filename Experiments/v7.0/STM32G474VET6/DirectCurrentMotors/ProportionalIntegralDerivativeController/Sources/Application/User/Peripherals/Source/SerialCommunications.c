#include <stm32g4xx.h>

#include "../Include/SerialCommunications.h"

#define USART_GPIO_PORT                                 GPIOB
#define ENABLE_USART_GPIO_RCC_CLOCK()                   do { __HAL_RCC_GPIOB_CLK_ENABLE(); } while(0)
#define USART_TX_GPIO_PIN                               GPIO_PIN_6
#define USART_RX_GPIO_PIN                               GPIO_PIN_7

#define ENABLE_USART_RCC_CLOCK()                        do { __HAL_RCC_USART1_CLK_ENABLE(); } while(0)

#define USART_IRQHANDLER                                USART1_IRQHandler

USART_HandleTypeDef USART_HandleType;

void InitSerialCommunications(uint32_t baudRate)
{
    ENABLE_USART_GPIO_RCC_CLOCK();
    
    GPIO_InitTypeDef GPIO_InitType = { 0 };
    
    GPIO_InitType.Pin = USART_TX_GPIO_PIN | USART_RX_GPIO_PIN;
    GPIO_InitType.Mode = GPIO_MODE_AF_PP;
    GPIO_InitType.Pull = GPIO_NOPULL;
    GPIO_InitType.Speed = GPIO_SPEED_FAST;
    GPIO_InitType.Alternate = GPIO_AF7_USART1;
    
    HAL_GPIO_Init(USART_GPIO_PORT, &GPIO_InitType);
    
    USART_HandleType.Instance = USART1;
    
    USART_HandleType.Init.BaudRate = baudRate;
    USART_HandleType.Init.WordLength = USART_WORDLENGTH_8B;
    USART_HandleType.Init.StopBits = USART_STOPBITS_1;
    USART_HandleType.Init.Parity = USART_PARITY_NONE;
    USART_HandleType.Init.Mode = USART_MODE_TX_RX;
    USART_HandleType.Init.CLKPolarity = USART_POLARITY_HIGH;
    
    HAL_USART_Init(&USART_HandleType);
}

void HandleHostCommands(uint8_t *byte)
{
    
}

void USART_IRQHANDLER(void)
{
    uint8_t byte;
    
    if (__HAL_USART_GET_FLAG(&USART_HandleType, USART_FLAG_RXNE) == SET)
    {
        HAL_USART_Receive(&USART_HandleType, &byte, 1, 1000);
        HandleHostCommands(&byte);
    }
}

