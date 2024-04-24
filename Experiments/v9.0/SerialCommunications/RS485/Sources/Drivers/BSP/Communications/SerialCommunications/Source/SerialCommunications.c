#include <stdio.h>
#include <string.h>

#include "../../../Middlewares/Third_Party/AlienTek/Malloc/Include/Malloc.h"

#include "../Include/SerialCommunications.h"

/*

AlienTek DMG474_V1.0.pdf
RS485 / ATK-MODULE Select

USART3_TX           PB10
USART3_RX           PB11

/STMicroelectronics/MCUs/STM32G474VET6/Product Specifications/stm32g474ve.pdf
Table 12. STM32G474xB/xC/xE pin definition (continued)

Pin name(function afterreset)                   Alternate functions

PB10                                            USART3_TX

Table 13. Alternate function (continued)



Port                                            AF7
                                                USART1/2/3/FDCAN/COMP7/5/6

Port B                  PB10                    USART3_TX
Port B                  PB11                    USART3_RX

*/

#define USART_GPIO_PORT                                 GPIOB
#define ENABLE_USART_GPIO_RCC_CLOCK()                   do                                      \
                                                        {                                       \
                                                            __HAL_RCC_GPIOB_CLK_ENABLE();       \
                                                        }                                       \
                                                        while   (0)

#define USART_TX_GPIO_PIN                               GPIO_PIN_10
#define USART_RX_GPIO_PIN                               GPIO_PIN_11

#define GPIO_ALTERNATE_FUNCTION_USARTX                  GPIO_AF7_USART3

#define ENABLE_USART_RCC_CLOCK()                        do { __HAL_RCC_USART3_CLK_ENABLE(); } while(0)

#define USARTX                                          USART3
#define USARTX_IRQN                                     USART3_IRQn
#define USARTX_IRQHANDLER                               USART3_IRQHandler

#define ENABLE_DMA_RCC_CLOCK()                          do { __HAL_RCC_DMA1_CLK_ENABLE(); } while(0)

#define DMA_STREAM_RECEIVE                              DMA1_Channel1
#define DMA_STREAM_RECEIVE_REQUEST                      DMA_REQUEST_USART3_RX
#define DMA_STREAM_RECEIVE_IRQN                         DMA1_Channel1_IRQn
#define DMA_STREAM_RECEIVE_IRQNHANDLER                  DMA1_Channel1_IRQHandler

#define DMA_STREAM_TRANSMIT                             DMA1_Channel2
#define DMA_STREAM_TRANSMIT_REQUEST                     DMA_REQUEST_USART3_TX
#define DMA_STREAM_TRANSMIT_IRQN                        DMA1_Channel2_IRQn
#define DMA_STREAM_TRANSMIT_IRQNHANDLER                 DMA1_Channel2_IRQHandler

#define MAXIMUM_RECEIVED_BUFFER_LENGTH                  100

#define USE_PRINTF_REDIRECT                             0

UART_HandleTypeDef UART_HandleType                                  = { 0 };

DMA_HandleTypeDef DMA_HandleTypeReceive                             = { 0 };
DMA_HandleTypeDef DMA_HandleTypeTransmit                            = { 0 };

uint8_t bytesBuffer[MAXIMUM_RECEIVED_BUFFER_LENGTH]         = { 0 };

extern void Error_Handler(void);
void *communicationProtocol;

static void (*OnBytesReceivedCallback)(void *protocol, uint8_t *bytes, uint16_t length);

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

static void InitDMAChannels(void)
{
    __HAL_RCC_DMAMUX1_CLK_ENABLE();
    ENABLE_DMA_RCC_CLOCK();

    /* DMA interrupt init */
    DMA_HandleTypeReceive.Instance                  = DMA_STREAM_RECEIVE;
    
    DMA_HandleTypeReceive.Init.Request              = DMA_STREAM_RECEIVE_REQUEST;
    DMA_HandleTypeReceive.Init.Direction            = DMA_PERIPH_TO_MEMORY;
    DMA_HandleTypeReceive.Init.PeriphInc            = DMA_PINC_DISABLE;
    DMA_HandleTypeReceive.Init.MemInc               = DMA_MINC_ENABLE;
    DMA_HandleTypeReceive.Init.PeriphDataAlignment  = DMA_PDATAALIGN_BYTE;
    DMA_HandleTypeReceive.Init.PeriphDataAlignment  = DMA_MDATAALIGN_BYTE;
    DMA_HandleTypeReceive.Init.Mode                 = DMA_NORMAL;
    DMA_HandleTypeReceive.Init.Priority             = DMA_PRIORITY_MEDIUM;
    
    /*
    
    MCUs/STM32G474VET6/Reference Manuals/rm0440-stm32g4-series-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
    
    Table 1: STM32G4 Series memory density
    
    Memory density                                          Category 3
    256 Kbytes                                              STM32G474
    ------------------------------------------------------------------------------------------------------------
    
    Table 90. DMAMUX instantiation
    
    Number of DMAMUX output request channels                Category 3 devices
    ------------------------------------------------------------------------------------------------------------

    DMAMUX mapping
    
    The mapping of resources to DMAMUX is hardwired.
    DMAMUX is used with DMA1 and DMA2:
    
    For category 3 and category 4 devices:
    
    1. DMAMUX channels 0 to 7 are connected to DMA1 channels 1 to 8
    2. DMAMUX channels 8 to 15 are connected to DMA2 channels 1 to 8
    ------------------------------------------------------------------------------------------------------------

    Table 91. DMAMUX: assignment of multiplexer inputs to resources
    
    DMA request MUX input                                   Resource
    
    28                                                      USART3_RX
    29                                                      USART3_TX
    
    ------------------------------------------------------------------------------------------------------------

    */
    
    if (HAL_OK == HAL_DMA_Init(&DMA_HandleTypeReceive))
    {
        __HAL_LINKDMA(&UART_HandleType, hdmarx, DMA_HandleTypeReceive);
        
        /* DMA1_Channel4_IRQn interrupt configuration */
        HAL_NVIC_SetPriority(DMA_STREAM_RECEIVE_IRQN, 0U, 0U);
        HAL_NVIC_EnableIRQ(DMA_STREAM_RECEIVE_IRQN);
    }
    else
        Error_Handler();
    
    DMA_HandleTypeTransmit.Instance                 = DMA_STREAM_TRANSMIT;
    
    DMA_HandleTypeTransmit.Init.Request             = DMA_STREAM_TRANSMIT_REQUEST;
    DMA_HandleTypeTransmit.Init.Direction           = DMA_MEMORY_TO_PERIPH;
    DMA_HandleTypeTransmit.Init.PeriphInc           = DMA_PINC_DISABLE;
    DMA_HandleTypeTransmit.Init.MemInc              = DMA_MINC_ENABLE;
    DMA_HandleTypeTransmit.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    DMA_HandleTypeTransmit.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
    DMA_HandleTypeTransmit.Init.Mode                = DMA_NORMAL;
    DMA_HandleTypeTransmit.Init.Priority            = DMA_PRIORITY_MEDIUM;
    
    if (HAL_OK == HAL_DMA_Init(&DMA_HandleTypeTransmit))
    {
        __HAL_LINKDMA(&UART_HandleType, hdmatx, DMA_HandleTypeTransmit);
        
        /* DMA1_Channel5_IRQn interrupt configuration */
        HAL_NVIC_SetPriority(DMA_STREAM_TRANSMIT_IRQN, 0U, 0U);
        HAL_NVIC_EnableIRQ(DMA_STREAM_TRANSMIT_IRQN);
    }
    else
        Error_Handler();
}

void InitSerialCommunications(uint32_t baudRate, void *protocol, void onBytesReceivedHandler(void *protocol, uint8_t *bytes, uint16_t length))
{
    InitDMAChannels();
    
    UART_HandleType.Instance                = USARTX;
    
    UART_HandleType.Init.BaudRate           = baudRate;
    UART_HandleType.Init.WordLength         = UART_WORDLENGTH_8B;
    UART_HandleType.Init.StopBits           = UART_STOPBITS_1;
    UART_HandleType.Init.Parity             = UART_PARITY_NONE;
    UART_HandleType.Init.Mode               = UART_MODE_TX_RX;
    UART_HandleType.Init.HwFlowCtl          = UART_HWCONTROL_NONE;
    UART_HandleType.Init.OverSampling       = UART_OVERSAMPLING_16;
    
    communicationProtocol                   = protocol;
    
    if (HAL_OK == HAL_UART_Init(&UART_HandleType))
    {
        if (onBytesReceivedHandler)
            OnBytesReceivedCallback         = onBytesReceivedHandler;
        
        __HAL_UART_ENABLE_IT(&UART_HandleType, UART_IT_RXNE);
        __HAL_UART_ENABLE_IT(&UART_HandleType, UART_IT_IDLE);
        
        HAL_UARTEx_ReceiveToIdle_DMA(&UART_HandleType, bytesBuffer, MAXIMUM_RECEIVED_BUFFER_LENGTH);
    }
    else
        Error_Handler();
}

void HAL_UART_MspInit(UART_HandleTypeDef *husart)
{
    if(husart ->Instance == USARTX)
    {
        ENABLE_USART_RCC_CLOCK();
        ENABLE_USART_GPIO_RCC_CLOCK();
    
        GPIO_InitTypeDef GPIO_InitType                  = { 0 };
        
        GPIO_InitType.Pin                               = USART_TX_GPIO_PIN | USART_RX_GPIO_PIN;
        GPIO_InitType.Mode                              = GPIO_MODE_AF_PP;
        GPIO_InitType.Pull                              = GPIO_PULLUP;
        GPIO_InitType.Speed                             = GPIO_SPEED_FAST;
        GPIO_InitType.Alternate                         = GPIO_ALTERNATE_FUNCTION_USARTX;
              
        HAL_GPIO_Init(USART_GPIO_PORT, &GPIO_InitType);
        
        /*
        
        https://blog.csdn.net/as480133937/article/details/105013368
        
        */
        
        HAL_NVIC_SetPriority(USARTX_IRQN, 0U, 0U);
        HAL_NVIC_EnableIRQ(USARTX_IRQN);
    }
}

HAL_StatusTypeDef TransmitDMA(uint8_t *bytes, uint16_t length)
{
    return HAL_UART_Transmit_DMA(&UART_HandleType, bytes, length);
}

HAL_StatusTypeDef ReceivedDMA(uint8_t *bytes,uint8_t length)
{
    return HAL_UARTEx_ReceiveToIdle_DMA(&UART_HandleType, bytes, length);
}

void USARTX_IRQHANDLER(void)
{
    if (SET == __HAL_UART_GET_FLAG(&UART_HandleType, UART_FLAG_IDLE))
    {
        __HAL_UART_CLEAR_IDLEFLAG(&UART_HandleType);
        
        HAL_UART_DMAStop(&UART_HandleType);
        
        static uint16_t residue                 = 0;
        static uint16_t receivedBytesLength     = 0;
        
        residue                                 = __HAL_DMA_GET_COUNTER(&DMA_HandleTypeReceive);
        receivedBytesLength                     = MAXIMUM_RECEIVED_BUFFER_LENGTH - residue;
        
        if (OnBytesReceivedCallback)
        {
            if (receivedBytesLength > 0)
            {
                uint8_t *receivedBytesBuffer = custom_malloc(SRAMIN, sizeof(uint8_t) * receivedBytesLength);

                memcpy(receivedBytesBuffer, bytesBuffer, receivedBytesLength);
                
                OnBytesReceivedCallback(communicationProtocol, receivedBytesBuffer, receivedBytesLength);
                
                custom_free(SRAMIN, receivedBytesBuffer);
                
                memset(bytesBuffer, 0, MAXIMUM_RECEIVED_BUFFER_LENGTH);
            }
        }
        
        HAL_UARTEx_ReceiveToIdle_DMA(&UART_HandleType, bytesBuffer, MAXIMUM_RECEIVED_BUFFER_LENGTH);
    }
    
    HAL_UART_IRQHandler(&UART_HandleType);
}

/**
  * @brief This function handles DMA1 channel4 global interrupt.
  */
void DMA_STREAM_RECEIVE_IRQNHANDLER(void)
{
  HAL_DMA_IRQHandler(&DMA_HandleTypeReceive);
}

/**
  * @brief This function handles DMA1 channel5 global interrupt.
  */
void DMA_STREAM_TRANSMIT_IRQNHANDLER(void)
{
  HAL_DMA_IRQHandler(&DMA_HandleTypeTransmit);
}