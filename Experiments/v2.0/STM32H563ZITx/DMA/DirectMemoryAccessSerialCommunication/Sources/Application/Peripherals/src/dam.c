#include "stm32h5xx_ll_rcc.h"
#include "stm32h5xx_ll_bus.h"
#include "stm32h5xx_ll_dma.h"

void dam_init(uint8_t *buffer)
{
    LL_DMA_InitTypeDef dma_init_type = { 0 };
    
    LL_APB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPDMA1);
    
    NVIC_SetPriority(GPDMA1_Channel1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
    NVIC_EnableIRQ(GPDMA1_Channel1_IRQn);
    
    dma_init_type.SrcAddress = (uint32_t)buffer;
    dma_init_type.DestAddress = (uint32_t) & USART3 -> TDR;
    dma_init_type.Direction = LL_DMA_DIRECTION_MEMORY_TO_PERIPH;
    dma_init_type.BlkHWRequest = LL_DMA_HWREQUEST_SINGLEBURST;
}