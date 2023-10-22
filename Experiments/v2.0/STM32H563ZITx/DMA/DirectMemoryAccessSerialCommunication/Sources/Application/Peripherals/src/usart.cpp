#include "stm32h5xx_ll_rcc.h"
#include "stm32h5xx_ll_bus.h"
#include "stm32h5xx_ll_usart.h"


void usart_init(uint32_t prescaler, uint32_t baudRate)
{
    LL_USART_InitTypeDef usart_init_type = { 0 };
    
    /* Set the clock source for USART3. */
	LL_RCC_SetUSARTClockSource(LL_RCC_USART3_CLKSOURCE_PCLK1);
    /* Enable the clock. */
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART3);
    
    NVIC_SetPriority(USART3_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
    NVIC_EnableIRQ(USART3_IRQn);
    
    usart_init_type.PrescalerValue = prescaler;
    usart_init_type.BaudRate = baudRate;
    usart_init_type.DataWidth = LL_USART_DATAWIDTH_8B;
    usart_init_type.StopBits = LL_USART_STOPBITS_1;
    usart_init_type.Parity = LL_USART_PARITY_NONE;
    usart_init_type.TransferDirection = LL_USART_DIRECTION_TX_RX;
    usart_init_type.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
    usart_init_type.OverSampling = LL_USART_OVERSAMPLING_16;
    
    LL_USART_Init(USART3, &usart_init_type);
    
    LL_USART_SetTXFIFOThreshold(USART3, LL_USART_FIFOTHRESHOLD_1_8);
    LL_USART_SetRXFIFOThreshold(USART3, LL_USART_FIFOTHRESHOLD_1_8);
    
    LL_USART_DisableFIFO(USART3);
    
    LL_USART_ConfigAsyncMode(USART3);
    
    LL_USART_Enable(USART3);
    
}
