#include <stdio.h>

#include "../Include/SerialCommunications.h"

#define USART_TX_GPIO_PORT                                          GPIOD

#define ENABLE_USART_TX_GPIO_RCC_CLOCK()                            do                                      \
                                                                    {                                       \
                                                                        __HAL_RCC_GPIOD_CLK_ENABLE();       \
                                                                    }                                       \
                                                                    while   (0)                             \

#define USART_TX_GPIO_PIN                                           GPIO_PIN_8
#define USART_TX_GPIO_ALTERNATE_FUNCTION_USART                      GPIO_AF7_USART3

#define USART_RX_GPIO_PORT                                          GPIOD

#define ENABLE_USART_RX_GPIO_RCC_CLOCK()                            do                                      \
                                                                    {                                       \
                                                                        __HAL_RCC_GPIOD_CLK_ENABLE();       \
                                                                    }                                       \
                                                                    while   (0)                             \

#define USART_RX_GPIO_PIN                                           GPIO_PIN_9
#define USART_RX_GPIO_ALTERNATE_FUNCTION_USART                      GPIO_AF7_USART3

#define ENABLE_USART_RCC_CLOCK()                                    do                                      \
                                                                    {                                       \
                                                                        __HAL_RCC_USART3_CLK_ENABLE();      \
                                                                    }                                       \
                                                                    while   (0)                             \

#define USART                                                       USART3
#define USART_IRQN                                                  USART3_IRQn
#define USART_IRQHANDLER                                            USART3_IRQHandler

#define USART_ENABLE_RX

/******************************************************************************************/
/* 加入以下代码, 支持printf函数, 而不需要选择use MicroLIB */

#define USE_PRINTF_REDIRECT     1

#if USE_PRINTF_REDIRECT

#if (__ARMCC_VERSION >= 6010050)                    /* 使用AC6编译器时 */
__asm(".global __use_no_semihosting\n\t");          /* 声明不使用半主机模式 */
__asm(".global __ARM_use_no_argv \n\t");            /* AC6下需要声明main函数为无参数格式，否则部分例程可能出现半主机模式 */

#else
/* 使用AC5编译器时, 要在这里定义__FILE 和 不使用半主机模式 */
#pragma import(__use_no_semihosting)

struct __FILE
{
    int handle;
    /* Whatever you require here. If the only file you are using is */
    /* standard output using printf() for debugging, no file handling */
    /* is required. */
};

#endif

/* 不使用半主机模式，至少需要重定义_ttywrch\_sys_exit\_sys_command_string函数,以同时兼容AC6和AC5模式 */
int _ttywrch(int ch)
{
    ch = ch;
    return ch;
}

/* 定义_sys_exit()以避免使用半主机模式 */
void _sys_exit(int x)
{
    x = x;
}

char *_sys_command_string(char *cmd, int len)
{
    return NULL;
}

/* FILE 在 stdio.h里面定义. */
FILE __stdout;

/* 重定义fputc函数, printf函数最终会通过调用fputc输出字符串到串口 */
int fputc(int ch, FILE *f)
{
    while ((USART -> ISR & 0x40) == 0);              /* 等待上一个字符发送完成 */

    USART -> TDR = (uint8_t)ch;                      /* 将要发送的字符 ch 写入到TDR寄存器 */
    
    return ch;
}

#endif

UART_HandleTypeDef UART_HandleType          = { 0 };
uint8_t bytes[1]                            = { 0 };
RingBufferTypeDef ringBuffer                = { 0 };

void *communicationProtocol;
static void (*OnByteReceived)(void *protocol);

void InitSerialCommunications(uint32_t baudRate, void *protocol, void onByteReceivedHandler(void *protocol))
{
    UART_HandleType.Instance = USART;
    
    UART_HandleType.Init.BaudRate           = baudRate;
    UART_HandleType.Init.WordLength         = UART_WORDLENGTH_8B;
    UART_HandleType.Init.StopBits           = UART_STOPBITS_1;
    UART_HandleType.Init.Parity             = UART_PARITY_NONE;
    UART_HandleType.Init.HwFlowCtl          = UART_HWCONTROL_NONE;
    UART_HandleType.Init.Mode               = UART_MODE_TX_RX;
    
    communicationProtocol = protocol;
    
    if (onByteReceivedHandler)
        OnByteReceived                      = onByteReceivedHandler;
    
    HAL_UART_Init(&UART_HandleType);
}

void HAL_UART_MspInit(UART_HandleTypeDef *husart)
{
    if(husart -> Instance == USART)
    {
        ENABLE_USART_RCC_CLOCK();
            
        GPIO_InitTypeDef GPIO_InitType      = { 0 };
        
        GPIO_InitType.Pin                   = USART_TX_GPIO_PIN;
        GPIO_InitType.Mode                  = GPIO_MODE_AF_PP;
        GPIO_InitType.Pull                  = GPIO_PULLUP;
        GPIO_InitType.Speed                 = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitType.Alternate             = USART_TX_GPIO_ALTERNATE_FUNCTION_USART;
                
        ENABLE_USART_TX_GPIO_RCC_CLOCK();
        HAL_GPIO_Init(USART_TX_GPIO_PORT, &GPIO_InitType);
                
        GPIO_InitType.Pin                   = USART_RX_GPIO_PIN;
        GPIO_InitType.Alternate             = USART_RX_GPIO_ALTERNATE_FUNCTION_USART;
                
        ENABLE_USART_RX_GPIO_RCC_CLOCK();
        HAL_GPIO_Init(USART_RX_GPIO_PORT, &GPIO_InitType);
        
#ifdef USART_ENABLE_RX
        
        __HAL_UART_ENABLE_IT(husart, UART_IT_RXNE);
        
        HAL_NVIC_EnableIRQ(USART_IRQN);
        HAL_NVIC_SetPriority(USART_IRQN, 0U, 0U);
#endif
        
    }
}

HAL_StatusTypeDef Tranmist(uint8_t *byte, uint16_t length)
{
    return HAL_UART_Transmit(&UART_HandleType, byte, length, 0xFFFF);
}

void USART_IRQHANDLER(void)
{    
    HAL_UART_IRQHandler(&UART_HandleType);
    HAL_UART_Receive_IT(&UART_HandleType, bytes, sizeof(bytes));
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart -> Instance == USART)
    {
        uint8_t byte = *((huart -> pRxBuffPtr) - 1);
        
        RingBufferWriteByte(byte, &ringBuffer);
        
        if (OnByteReceived)
            OnByteReceived(communicationProtocol);
    }
}