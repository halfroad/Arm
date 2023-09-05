#include "stm32f4xx_dma.h"
#include "stm32f4xx_usart.h"

#include "GPIO.h"
#include "USART.h"
#include "DirectMemoryAccess.h"

uint8_t sendBuffer[] = {0xAA};

void init_usart_dma(void)
{
	init_gpio_ports();
	init_usart_ports(115200);
	
	/* Enable RCC Clock for DMA Controller 2.*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	
	while (DMA_GetCmdStatus(DMA2_Stream7) != DISABLE)
		;
	
	/* Initialize the DMA Settings. */
	DMA_InitTypeDef dma_init_struct;
	
	/* Select Channel 4. */
	dma_init_struct.DMA_Channel = DMA_Channel_4;
	/* The Peripheral Base Address of DMA. */
	dma_init_struct.DMA_PeripheralBaseAddr = (uint32_t) &USART1 -> DR;
	/* Badse Address of Memory 0. */
	dma_init_struct.DMA_Memory0BaseAddr = (uint32_t) sendBuffer;
	/* Direction of the DMA, Memory to Peripheral. */
	dma_init_struct.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	/* No increment for Peripheral. */
	dma_init_struct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	/* Enable the increment for Memory. */
	dma_init_struct.DMA_MemoryInc = DMA_MemoryInc_Enable;
	/* Data size of Peripheral set to byte: 8 bits. */
	dma_init_struct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	/* Data size of Memory also set to byte: 8 bits. */
	dma_init_struct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	/* Normal mode. */
	dma_init_struct.DMA_Mode = DMA_Mode_Normal;
	/* Medium priority. */
	dma_init_struct.DMA_Priority = DMA_Priority_Medium;
	/* No FIFO. */
	dma_init_struct.DMA_FIFOMode = DMA_FIFOMode_Disable;
	/* Level 4 (16 words. )*/
	dma_init_struct.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	/* 4 bursts with 4 paces, FIFO level 1/4, single transmit.*/
	dma_init_struct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	/* 4 bursts with 4 paces, FIFO level 1/4, single transmit.*/
	dma_init_struct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	
	/* Initialize DMA Stream. */
	DMA_Init(DMA2_Stream7, &dma_init_struct);
	
	/* Enable the Transmit on USART1. */
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
}

void usart_transmit(uint16_t bytes)
{
	/* Disable DMA Stream. */
	DMA_Cmd(DMA2_Stream7, DISABLE);
	
	/* Ensure the DMA can be configured. */
	while (DMA_GetCmdStatus(DMA2_Stream7) != DISABLE)
		;
	
	/* Set the number of data would be transmitted. */
	DMA_SetCurrDataCounter(DMA2_Stream7, bytes);
	
	/* Enable the DMA tranmission. */
	DMA_Cmd(DMA2_Stream7, ENABLE);
	
	/* Wait for the completion of tranmission of DMA2 Stream 7. */
	while (DMA_GetFlagStatus(DMA2_Stream7, DMA_FLAG_TCIF7) == RESET)
		;
	
	DMA_ClearFlag(DMA2_Stream7, DMA_FLAG_TCIF7);
}