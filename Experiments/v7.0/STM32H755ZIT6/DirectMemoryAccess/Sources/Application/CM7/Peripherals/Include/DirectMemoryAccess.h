#ifndef __DIRECT_MEMORY_ACCESS_H
#define __DIRECT_MEMORY_ACCESS_H

#include <stm32h7xx.h>

#define DMA                                         DMA2
#define DMA_STREAM_TYPE                             DMA2_Stream7

#define DMA_STREAM_IRQN                             DMA2_Stream7_IRQn
#define DMA_STREAM_IRQHANDLER                       DMA2_Stream7_IRQHandler

void InitDirectMemoryAccess(uint32_t sourceMemoryAddress, uint32_t destionationPeripheralAddress, uint8_t DMARequestMUXInput, uint16_t transmissionItemsNumber);
void StartConveyItems(DMA_Stream_TypeDef *DMA_Stream_Type, uint16_t numberOfTransmissionItems);
    
#endif  /*  #ifndef __DIRECT_MEMORY_ACCESS_H    */