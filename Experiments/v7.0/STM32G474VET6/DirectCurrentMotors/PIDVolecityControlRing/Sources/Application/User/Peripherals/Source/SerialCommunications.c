#include <stdio.h>

#include "../Include/SerialCommunications.h"

#define USART_GPIO_PORT                                 GPIOB
#define ENABLE_USART_GPIO_RCC_CLOCK()                   do { __HAL_RCC_GPIOB_CLK_ENABLE(); } while(0)
#define USART_TX_GPIO_PIN                               GPIO_PIN_6
#define USART_RX_GPIO_PIN                               GPIO_PIN_7

#define GPIO_ALTERNATE_FUNCTION_USARTX                  GPIO_AF7_USART1

#define ENABLE_USART_RCC_CLOCK()                        do { __HAL_RCC_USART1_CLK_ENABLE(); } while(0)

#define USARTX                                          USART1
#define USARTX_IRQN                                     USART1_IRQn
#define USARTX_IRQHANDLER                               USART1_IRQHandler
#define USARTX_ENABLE_RX

/******************************************************************************************/
/* 加入以下代码, 支持printf函数, 而不需要选择use MicroLIB */

#if 1
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
    while ((USARTX -> ISR & 0x40) == 0);              /* 等待上一个字符发送完成 */

    USARTX -> TDR = (uint8_t)ch;                      /* 将要发送的字符 ch 写入到TDR寄存器 */
    
    return ch;
}

#endif

UART_HandleTypeDef UART_HandleType;

static void (*OnByteReceived)(uint8_t *byte);

void InitSerialCommunications(uint32_t baudRate, void onByteReceivedHandler(uint8_t *byte))
{
    UART_HandleType.Instance = USARTX;
    
    UART_HandleType.Init.BaudRate = baudRate;
    UART_HandleType.Init.WordLength = UART_WORDLENGTH_8B;
    UART_HandleType.Init.StopBits = UART_STOPBITS_1;
    UART_HandleType.Init.Parity = UART_PARITY_NONE;
    UART_HandleType.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    UART_HandleType.Init.Mode = UART_MODE_TX_RX;
    
    if (onByteReceivedHandler)
        OnByteReceived = onByteReceivedHandler;
    
    HAL_UART_Init(&UART_HandleType);
}

void HAL_UART_MspInit(UART_HandleTypeDef *husart)
{
    if(husart ->Instance == USARTX)
    {
        ENABLE_USART_RCC_CLOCK();
        ENABLE_USART_GPIO_RCC_CLOCK();
    
        GPIO_InitTypeDef GPIO_InitType = { 0 };
        
        GPIO_InitType.Pin = USART_TX_GPIO_PIN | USART_RX_GPIO_PIN;
        GPIO_InitType.Mode = GPIO_MODE_AF_PP;
        GPIO_InitType.Pull = GPIO_PULLUP;
        GPIO_InitType.Speed = GPIO_SPEED_FAST;
        GPIO_InitType.Alternate = GPIO_ALTERNATE_FUNCTION_USARTX;
              
        HAL_GPIO_Init(USART_GPIO_PORT, &GPIO_InitType);
        
        __HAL_UART_DISABLE_IT(husart, UART_IT_TC);
        
#ifdef USARTX_ENABLE_RX
        
        __HAL_UART_ENABLE_IT(husart, UART_IT_RXNE);
        
        HAL_NVIC_EnableIRQ(USARTX_IRQN);
        HAL_NVIC_SetPriority(USARTX_IRQN, 3, 0);
#endif
        
    }
}

HAL_StatusTypeDef tranmist(uint8_t *byte, uint16_t length)
{
    return HAL_UART_Transmit(&UART_HandleType, byte, length, 0xFFFF);
}

void USARTX_IRQHANDLER(void)
{
    uint8_t byte;
    
    if (__HAL_UART_GET_FLAG(&UART_HandleType, UART_FLAG_RXNE) == SET)
    {
        HAL_UART_Receive(&UART_HandleType, &byte, 1, 1000);
        __HAL_UART_CLEAR_FLAG(&UART_HandleType, UART_FLAG_RXNE);
        
        if (OnByteReceived)
            OnByteReceived(&byte);
    }
}