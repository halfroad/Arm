#include "../Include/ZS040.h"

#define USART                                               USART2
#define USART_IRQN                                          USART2_IRQn
#define USART_IRQHANDLER                                    USART2_IRQHandler

#define RCC_USART_CLOCK_ENABLE()                            do                                      \
                                                            {                                       \
                                                                __HAL_RCC_USART2_CLK_ENABLE();      \
                                                            }                                       \
                                                            while   (0)                             \

#define USART_TX_GPIO_PORT                                  GPIOD
#define USART_TX_GPIO_PIN                                   GPIO_PIN_5
#define USART_TX_GPIO_PIN_ALTERNATE_FUNCTION                GPIO_AF7_USART2

#define RCC_USART_TX_GPIO_PORT_CLOCK_ENABLE()               do                                      \
                                                            {                                       \
                                                                __HAL_RCC_GPIOD_CLK_ENABLE();       \
                                                            }                                       \
                                                            while   (0)                             \
                                                                


#define USART_RX_GPIO_PORT                                  GPIOD
#define USART_RX_GPIO_PIN                                   GPIO_PIN_6
#define USART_RX_GPIO_PIN_ALTERNATE_FUNCTION                GPIO_AF7_USART2

#define RCC_USART_RX_GPIO_PORT_CLOCK_ENABLE()               do                                      \
                                                            {                                       \
                                                                __HAL_RCC_GPIOD_CLK_ENABLE();       \
                                                            }                                       \
                                                            while   (0)                             \

UART_HandleTypeDef blueToothUART_HandleType                 = { 0 };
CircularBufferTypeDef circularBufferType                    = { 0 };

static void (* OnBytesReceivedCallback)(void);
static void MspInitCallback(UART_HandleTypeDef *huart);

void InitSerialBlueTooth(uint32_t baudRate, void (* onBytesReceived)(void))
{
    blueToothUART_HandleType.Instance                       = USART;
    
    blueToothUART_HandleType.Init.BaudRate                  = baudRate;
    blueToothUART_HandleType.Init.WordLength                = UART_WORDLENGTH_8B;
    blueToothUART_HandleType.Init.StopBits                  = UART_STOPBITS_1;
    blueToothUART_HandleType.Init.Parity                    = UART_PARITY_NONE;
    blueToothUART_HandleType.Init.Mode                      = UART_MODE_TX_RX;
    
    blueToothUART_HandleType.MspInitCallback                = MspInitCallback;
    
    if (onBytesReceived)
        OnBytesReceivedCallback                             = onBytesReceived;
    
    HAL_UART_Init(&blueToothUART_HandleType);
}

static void MspInitCallback(UART_HandleTypeDef *huart)
{
    if (USART == huart -> Instance)
    {
        /*
        
        /STMicroelectronics/Nucleo Boards/Nucleo-H563ZI/User Manuals/um3115-stm32h5-nucleo144-board-mb1404-stmicroelectronics.pdf
        Table 23. Zio connector (CN9) pinout)
        
        
        Pin name                Signal name             STM32 pin               STM32 function
        
        D52                     USART_B_RX              PD6                     USART2
        D53                     USART_B_TX              PD5                     USART2
        
        MCUs/STM32H563ZI/Product Specifications/stm32h563zi.pdf
        Table 15. Alternate function AF0 to AF7(1) (continued)
        
        Port                    AF7
        
                                SDMMC1/SPI2/I2S2/SPI3/I2S3/SPI6/UART7/8/12/USART1/2/3/6/10/11
        PD5                     USART2_TX
        PD6                     USART2_RX
        
        */
        
        GPIO_InitTypeDef GPIO_InitType                      = { 0 };
        
        GPIO_InitType.Pin                                   = USART_TX_GPIO_PIN;
        GPIO_InitType.Mode                                  = GPIO_MODE_AF_PP;
        GPIO_InitType.Alternate                             = USART_TX_GPIO_PIN_ALTERNATE_FUNCTION;
        
        RCC_USART_TX_GPIO_PORT_CLOCK_ENABLE();
        HAL_GPIO_Init(USART_TX_GPIO_PORT, &GPIO_InitType);
        
        GPIO_InitType.Pin                                   = USART_RX_GPIO_PIN;
        GPIO_InitType.Alternate                             = USART_RX_GPIO_PIN_ALTERNATE_FUNCTION;
        
        RCC_USART_RX_GPIO_PORT_CLOCK_ENABLE();
        HAL_GPIO_Init(USART_RX_GPIO_PORT, &GPIO_InitType);
        
        RCC_USART_CLOCK_ENABLE();
        
        __HAL_UART_ENABLE_IT(huart, UART_IT_RXNE);
        
        HAL_NVIC_SetPriority(USART_IRQN, 0U, 0U);
        HAL_NVIC_EnableIRQ(USART_IRQN);
    }
}

HAL_StatusTypeDef Tranmist(uint8_t *byte, uint16_t length)
{
    return HAL_UART_Transmit(&blueToothUART_HandleType, byte, length, 0xFFFF);
}

void USART_IRQHANDLER(void)
{
    if (__HAL_UART_GET_FLAG(&blueToothUART_HandleType, UART_FLAG_RXNE) == SET)
    {
        uint8_t byte = 0;
        
        HAL_UART_Receive(&blueToothUART_HandleType, &byte, 1, 1000);
        __HAL_UART_CLEAR_FLAG(&blueToothUART_HandleType, UART_FLAG_RXNE);
        
        if (CircularBufferStateOK == CircularBufferWriteByte(byte, &circularBufferType))
        {
            if (OnBytesReceivedCallback)
                OnBytesReceivedCallback();
        }
    }
}