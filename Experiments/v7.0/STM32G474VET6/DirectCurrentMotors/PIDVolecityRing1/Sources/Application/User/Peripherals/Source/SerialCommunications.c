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
/* �������´���, ֧��printf����, ������Ҫѡ��use MicroLIB */

#if 1
#if (__ARMCC_VERSION >= 6010050)                    /* ʹ��AC6������ʱ */
__asm(".global __use_no_semihosting\n\t");          /* ������ʹ�ð�����ģʽ */
__asm(".global __ARM_use_no_argv \n\t");            /* AC6����Ҫ����main����Ϊ�޲�����ʽ�����򲿷����̿��ܳ��ְ�����ģʽ */

#else
/* ʹ��AC5������ʱ, Ҫ�����ﶨ��__FILE �� ��ʹ�ð�����ģʽ */
#pragma import(__use_no_semihosting)

struct __FILE
{
    int handle;
    /* Whatever you require here. If the only file you are using is */
    /* standard output using printf() for debugging, no file handling */
    /* is required. */
};

#endif

/* ��ʹ�ð�����ģʽ��������Ҫ�ض���_ttywrch\_sys_exit\_sys_command_string����,��ͬʱ����AC6��AC5ģʽ */
int _ttywrch(int ch)
{
    ch = ch;
    return ch;
}

/* ����_sys_exit()�Ա���ʹ�ð�����ģʽ */
void _sys_exit(int x)
{
    x = x;
}

char *_sys_command_string(char *cmd, int len)
{
    return NULL;
}

/* FILE �� stdio.h���涨��. */
FILE __stdout;

/* �ض���fputc����, printf�������ջ�ͨ������fputc����ַ��������� */
int fputc(int ch, FILE *f)
{
    while ((USARTX -> ISR & 0x40) == 0);              /* �ȴ���һ���ַ�������� */

    USARTX -> TDR = (uint8_t)ch;                      /* ��Ҫ���͵��ַ� ch д�뵽TDR�Ĵ��� */
    
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