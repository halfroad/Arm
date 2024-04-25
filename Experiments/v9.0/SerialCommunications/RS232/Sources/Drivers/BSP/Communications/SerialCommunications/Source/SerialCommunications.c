#include <stdio.h>
#include <string.h>

#include "../../../Middlewares/Third_Party/AlienTek/Malloc/Include/Malloc.h"

#include "../Include/SerialCommunications.h"

#define USART                                                               USART2
#define USART_IRQN                                                          USART2_IRQn
#define USART_IRQHANDLER                                                    USART2_IRQHandler

#define RCC_USART_CLOCK_ENABLE()                                            do                                                  \
                                                                            {                                                   \
                                                                                __HAL_RCC_USART2_CLK_ENABLE();                  \
                                                                            }                                                   \
                                                                            while   (0)

#define USART_TX_GPIO_PORT                                                  GPIOD
#define RCC_USART_TX_GPIO_PORT_CLOCK_ENABLE()                               do                                                  \
                                                                            {                                                   \
                                                                                __HAL_RCC_GPIOD_CLK_ENABLE();                   \
                                                                            }                                                   \
                                                                            while   (0)
#define USART_TX_GPIO_PIN                                                   GPIO_PIN_5
#define USART_TX_GPIO_ALTERNATE_FUNCTION                                    GPIO_AF7_USART2

#define USART_RX_GPIO_PORT                                                  GPIOD
#define RCC_USART_RX_GPIO_PORT_CLOCK_ENABLE()                               do                                                  \
                                                                            {                                                   \
                                                                                __HAL_RCC_GPIOD_CLK_ENABLE();                   \
                                                                            }                                                   \
                                                                            while   (0)
#define USART_RX_GPIO_PIN                                                   GPIO_PIN_6
#define USART_RX_GPIO_ALTERNATE_FUNCTION                                    GPIO_AF7_USART2

#define RCC_DMA_DMAMUX_CLOCK_ENABLE()                                       do                                                  \
                                                                            {                                                   \
                                                                                __HAL_RCC_DMAMUX1_CLK_ENABLE();                 \
                                                                                __HAL_RCC_DMA1_CLK_ENABLE();                    \
                                                                            }                                                   \
                                                                            while   (0)

#define USART_TX_DMA_CHANNEL                                                DMA1_Channel1
#define USART_TX_DMA_REQUEST                                                DMA_REQUEST_USART2_TX
#define USART_TX_DMA_CHANNEL_IRQN                                           DMA1_Channel1_IRQn
#define USART_TX_DMA_CHANNEL_IRQHANDLER                                     DMA1_Channel1_IRQHandler
                                                                            
#define USART_RX_DMA_CHANNEL                                                DMA1_Channel2
#define USART_RX_DMA_REQUEST                                                DMA_REQUEST_USART2_RX
#define USART_RX_DMA_CHANNEL_IRQN                                           DMA1_Channel2_IRQn
#define USART_RX_DMA_CHANNEL_IRQHANDLER                                     DMA1_Channel2_IRQHandler

#define USART_MAXIMUM_RECEIVED_BUFFER_LENGTH                                256


/******************************************************************************************/
/* 加入以下代码, 支持printf函数, 而不需要选择use MicroLIB */

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
    while ((USARTX -> ISR & 0x40) == 0);              /* 等待上一个字符发送完成 */

    USARTX -> TDR = (uint8_t)ch;                      /* 将要发送的字符 ch 写入到TDR寄存器 */
    
    return ch;
}

#endif

UART_HandleTypeDef UART_HandleType;

DMA_HandleTypeDef DMA_HandleTypeTransmit;
DMA_HandleTypeDef DMA_HandleTypeReceive;

uint8_t bytesBuffer[USART_MAXIMUM_RECEIVED_BUFFER_LENGTH];
    
extern void Error_Handler(void);

static void (* OnNewBytesReceivedCallback)(void *protocol, uint8_t *bytes, uint16_t length);

void InitSerialCommunications(uint32_t baudRate, void *protocol, void (* newBytesReceivedHandler)(void *protocol, uint8_t *bytes, uint16_t length))
{
    UART_HandleType.Instance                        = USART;
    
    UART_HandleType.Init.BaudRate                   = baudRate;
    UART_HandleType.Init.WordLength                 = UART_WORDLENGTH_8B;
    UART_HandleType.Init.StopBits                   = UART_STOPBITS_1;
    UART_HandleType.Init.Parity                     = UART_PARITY_NONE;
    UART_HandleType.Init.Mode                       = UART_MODE_TX_RX;
    UART_HandleType.Init.HwFlowCtl                  = UART_HWCONTROL_NONE;
    UART_HandleType.Init.OverSampling               = UART_OVERSAMPLING_16;
    
    /*  UART_HandleType.MspInitCallback                 = MspInitCallback;  */

    if (HAL_OK == HAL_UART_Init(&UART_HandleType))
    {
        if (newBytesReceivedHandler)
            OnNewBytesReceivedCallback              = newBytesReceivedHandler;
        
        __HAL_UART_ENABLE_IT(&UART_HandleType, UART_IT_RXNE);
        __HAL_UART_ENABLE_IT(&UART_HandleType, UART_IT_IDLE);
        
        HAL_UARTEx_ReceiveToIdle_DMA(&UART_HandleType, bytesBuffer, USART_MAXIMUM_RECEIVED_BUFFER_LENGTH);
    }
    else
        Error_Handler();
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    RCC_USART_CLOCK_ENABLE();
    
    /*

    DMG474_V1.0.pdf
    HMI / RS232 Select,
    
    USART2_TX                           PD5
    USART2_RX                           PD6
    
    STMicroelectronics/MCUs/STM32G474VET6/Product Specifications/stm32g474ve.pdf
    Table 13. Alternate function (continued)
    
    Port                                AF7
                                        USART1/2/3/FDCAN/COMP7/5/6
    
    PD5                                 USART2_TX
    PD6                                 USART2_RX
    
    */
    
    GPIO_InitTypeDef GPIO_InitType                          = { 0 };
    
    GPIO_InitType.Pin                                       = USART_TX_GPIO_PIN;
    GPIO_InitType.Mode                                      = GPIO_MODE_AF_PP;
    GPIO_InitType.Alternate                                 = GPIO_AF7_USART2;
    
    RCC_USART_TX_GPIO_PORT_CLOCK_ENABLE();
    HAL_GPIO_Init(USART_TX_GPIO_PORT, &GPIO_InitType);
    
    GPIO_InitType.Pin                                       = USART_RX_GPIO_PIN;
    
    RCC_USART_RX_GPIO_PORT_CLOCK_ENABLE();
    HAL_GPIO_Init(USART_RX_GPIO_PORT, &GPIO_InitType);
    
    /*
    
    Table 1. STM32G4 Series memory density
    
    Memory density                          Category 3
    
    256 Kbytes                              STM32G474
    
    --------------------------------------------------------------------------------------------------
    
    Table 90. DMAMUX instantiation
    
                                            Feature                                             DMAMUX
    
    Number of DMAMUX output request channels    Category 2 devices(1)                           12
                                                Category 3 devices(1)                           16
                                                Category 4 devices(1)                           16
                                    Number of DMAMUX request generator channels                 4
                                    Number of DMAMUX request trigger inputs                     21
                                    Number of DMAMUX synchronization inputs                     21
                                    Number of DMAMUX peripheral request inputs                  115
    --------------------------------------------------------------------------------------------------

    DMAMUX mapping
    
    The mapping of resources to DMAMUX is hardwired.
    
    DMAMUX is used with DMA1 and DMA2:
    For category 3 and category 4 devices:
        1. DMAMUX channels 0 to 7 are connected to DMA1 channels 1 to 8
        2. DMAMUX channels 8 to 15 are connected to DMA2 channels 1 to 8
    --------------------------------------------------------------------------------------------------
    
    Table 91. DMAMUX: assignment of multiplexer inputs to resources

    DMA request MUX input                   Resource
    
    26                                      USART2_RX
    27                                      USART2_TX
    --------------------------------------------------------------------------------------------------
    */
    
    RCC_DMA_DMAMUX_CLOCK_ENABLE();
    
    DMA_HandleTypeTransmit.Instance                         = USART_TX_DMA_CHANNEL;
    
    DMA_HandleTypeTransmit.Init.Request                     = USART_TX_DMA_REQUEST;
    DMA_HandleTypeTransmit.Init.Direction                   = DMA_PERIPH_TO_MEMORY;
    DMA_HandleTypeTransmit.Init.PeriphInc                   = DMA_PINC_DISABLE;
    DMA_HandleTypeTransmit.Init.MemInc                      = DMA_MINC_ENABLE;
    DMA_HandleTypeTransmit.Init.PeriphDataAlignment         = DMA_PDATAALIGN_BYTE;
    DMA_HandleTypeTransmit.Init.MemDataAlignment            = DMA_MDATAALIGN_BYTE;
    DMA_HandleTypeTransmit.Init.Mode                        = DMA_NORMAL;
    DMA_HandleTypeTransmit.Init.Priority                    = DMA_PRIORITY_HIGH;
    
    if (HAL_OK == HAL_DMA_Init(&DMA_HandleTypeTransmit))
    {
        __HAL_LINKDMA(&UART_HandleType, hdmatx, DMA_HandleTypeTransmit);
        
        HAL_NVIC_SetPriority(USART_TX_DMA_CHANNEL_IRQN, 0U, 0U);
        HAL_NVIC_EnableIRQ(USART_TX_DMA_CHANNEL_IRQN);
    }
    else
        Error_Handler();
    
    DMA_HandleTypeReceive.Instance                          = USART_TX_DMA_CHANNEL;
    
    DMA_HandleTypeReceive.Init.Request                      = USART_TX_DMA_REQUEST;
    DMA_HandleTypeReceive.Init.Direction                    = DMA_PERIPH_TO_MEMORY;
    DMA_HandleTypeReceive.Init.PeriphInc                    = DMA_PINC_DISABLE;
    DMA_HandleTypeReceive.Init.MemInc                       = DMA_MINC_ENABLE;
    DMA_HandleTypeReceive.Init.PeriphDataAlignment          = DMA_PDATAALIGN_BYTE;
    DMA_HandleTypeReceive.Init.MemDataAlignment             = DMA_MDATAALIGN_BYTE;
    DMA_HandleTypeReceive.Init.Mode                         = DMA_NORMAL;
    DMA_HandleTypeReceive.Init.Priority                     = DMA_PRIORITY_HIGH;
    
    if (HAL_OK == HAL_DMA_Init(&DMA_HandleTypeReceive))
    {
         __HAL_LINKDMA(&UART_HandleType, hdmatx, DMA_HandleTypeTransmit);
        
        HAL_NVIC_SetPriority(USART_RX_DMA_CHANNEL_IRQN, 0U, 0U);
        HAL_NVIC_EnableIRQ(USART_RX_DMA_CHANNEL_IRQN);
    }
    else
        Error_Handler();
    
    HAL_NVIC_SetPriority(USART_IRQN, 0U, 0U);
    HAL_NVIC_EnableIRQ(USART_IRQN);
}

HAL_StatusTypeDef TransmitDMA(uint8_t *bytes, uint16_t length)
{
    return HAL_UART_Transmit_DMA(&UART_HandleType, bytes, length);
}

HAL_StatusTypeDef ReceivedDMA(uint8_t *bytes,uint8_t length)
{
    return HAL_UARTEx_ReceiveToIdle_DMA(&UART_HandleType, bytes, length);
}


void USART_IRQHANDLER(void)
{
    if (SET == __HAL_UART_GET_FLAG(&UART_HandleType, UART_IT_IDLE))
    {
        __HAL_UART_CLEAR_IDLEFLAG(&UART_HandleType);
        HAL_UART_DMAStop(&UART_HandleType);
        
        if (OnNewBytesReceivedCallback)
        {
            static uint16_t residue, receivedBytesLength = 0;
        
            residue             = __HAL_DMA_GET_COUNTER(&DMA_HandleTypeReceive);
            receivedBytesLength = USART_MAXIMUM_RECEIVED_BUFFER_LENGTH - residue;
            
            if (receivedBytesLength)
            {
                uint8_t *receivedBytesBuffer = custom_malloc(SRAMIN, sizeof(uint8_t) * receivedBytesLength);
                
                custom_mem_copy(receivedBytesBuffer, bytesBuffer, receivedBytesLength);
                
                custom_free(SRAMIN, receivedBytesBuffer);
                custom_mem_set(bytesBuffer, 0x00, USART_MAXIMUM_RECEIVED_BUFFER_LENGTH);
            }
        }
        
        HAL_UARTEx_ReceiveToIdle_DMA(&UART_HandleType, bytesBuffer, USART_MAXIMUM_RECEIVED_BUFFER_LENGTH);
    }
    
    HAL_UART_IRQHandler(&UART_HandleType);
}

void USART_TX_DMA_CHANNEL_IRQHANDLER(void)
{
    HAL_DMA_IRQHandler(&DMA_HandleTypeTransmit);
}

void USART_RX_DMA_CHANNEL_IRQHANDLER(void)
{
    HAL_DMA_IRQHandler(&DMA_HandleTypeReceive);
}