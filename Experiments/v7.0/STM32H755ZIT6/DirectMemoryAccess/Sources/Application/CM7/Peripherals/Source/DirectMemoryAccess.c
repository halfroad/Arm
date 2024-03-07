#include "../../Interrupts/Include/SystemInterrupts.h"
#include "../Include/DirectMemoryAccess.h"

void ConfigDirectMemoryAccess(DMA_Stream_TypeDef *DMA_Stream_Type, uint8_t DMARequestMUXInput, uint32_t sourceMemoryAddress, uint32_t destionationPeripheralAddress, uint16_t transmissionItemsNumber);

void InitDirectMemoryAccess(uint32_t sourceMemoryAddress, uint32_t destionationPeripheralAddress, uint8_t DMARequestMUXInput, uint16_t transmissionItemsNumber)
{
    ConfigDirectMemoryAccess(DMA_STREAM_TYPE, DMARequestMUXInput,sourceMemoryAddress, destionationPeripheralAddress, transmissionItemsNumber);
}

void ConfigDirectMemoryAccess(DMA_Stream_TypeDef *DMA_Stream_Type, uint8_t DMARequestMUXInput, uint32_t sourceMemoryAddress, uint32_t destionationPeripheralAddress, uint16_t transmissionItemsNumber)
{
    DMA_TypeDef *DMA_Type;
    
    /*
    Check whether the stream is DMA1 or DMA2.
    0x40020400 - 0x400207FF
    DMA2
    AHB1(D2)
    
    */
    if ((uint32_t)DMA_Stream_Type > (uint32_t)DMA2)
    {
        DMA_Type = DMA2;
        
        /*
        
        Bit 1 DMA2EN: DMA2 clock enable
        Set and reset by software.
            0: DMA2 clock disabled (default after reset)
            1: DMA2 clock enabled
        
        */
        RCC -> AHB1ENR |= 0x01 << 1;
    }
    else
    {
        DMA_Type = DMA1;
        
        /*
        
        Bit 0 DMA1EN: DMA1 clock enable
        Set and reset by software.
            0: DMA1 clock disabled (default after reset)
            1: DMA1 clock enabled
        
        */
        RCC -> AHB1ENR |= 0x01 << 0;
    }
    
    /*
    
    Bit 0 EN: stream enable / flag stream ready when read low
    This bit is set and cleared by software.
        0: stream disabled
        1: stream enabled
    This bit may be cleared by hardware:
    每 on a DMA end of transfer (stream ready to be configured)
    每 if a transfer error occurs on the AHB master buses
    每 when the FIFO threshold on memory AHB port is not compatible with the size of the
    burst
    When this bit is read as 0, the software is allowed to program the configuration and FIFO bits
    registers. It is forbidden to write these registers when the EN bit is read as 1.
    Note: Before setting EN bit to 1 to start a new transfer, the event flags corresponding to the
    stream in DMA_LISR or DMA_HISR register must be cleared.
    
    */
    while (DMA_Stream_Type -> CR & (0x01 << 0))
        ;
    
    /*
    
    Get the number of stream.
    
    */
    uint8_t streamNumber = ((uint32_t)DMA_Stream_Type - (uint32_t)DMA_Type - 0x10) / 0x18;
    
    /*
    
    DMA low interrupt flag clear register (DMA_LIFCR),
    
        Bits 27, 21, 11, 5 CTCIF[3:0]: stream x clear transfer complete interrupt flag (x = 3 to 0)
            Writing 1 to this bit clears the corresponding TCIFx flag in the DMA_LISR register.
        Bits 26, 20, 10, 4 CHTIF[3:0]: stream x clear half transfer interrupt flag (x = 3 to 0)
            Writing 1 to this bit clears the corresponding HTIFx flag in the DMA_LISR register
        Bits 25, 19, 9, 3 CTEIF[3:0]: Stream x clear transfer error interrupt flag (x = 3 to 0)
            Writing 1 to this bit clears the corresponding TEIFx flag in the DMA_LISR register.
        Bits 24, 18, 8, 2 CDMEIF[3:0]: stream x clear direct mode error interrupt flag (x = 3 to 0)
            Writing 1 to this bit clears the corresponding DMEIFx flag in the DMA_LISR register.
        Bits 22, 16, 6, 0 CFEIF[3:0]: stream x clear FIFO error interrupt flag (x = 3 to 0)
            Writing 1 to this bit clears the corresponding CFEIFx flag in the DMA_LISR register
    
    
    DMA high interrupt flag clear register (DMA_HIFCR),
    
        Bits 27, 21, 11, 5 CTCIF[7:4]: stream x clear transfer complete interrupt flag (x = 7 to 4)
            Writing 1 to this bit clears the corresponding TCIFx flag in the DMA_HISR register.
        Bits 26, 20, 10, 4 CHTIF[7:4]: stream x clear half transfer interrupt flag (x = 7 to 4)
            Writing 1 to this bit clears the corresponding HTIFx flag in the DMA_HISR register.
        Bits 25, 19, 9, 3 CTEIF[7:4]: stream x clear transfer error interrupt flag (x = 7 to 4)
            Writing 1 to this bit clears the corresponding TEIFx flag in the DMA_HISR register.
        Bits 24, 18, 8, 2 CDMEIF[7:4]: stream x clear direct mode error interrupt flag (x = 7 to 4)
            Writing 1 to this bit clears the corresponding DMEIFx flag in the DMA_HISR register.
        Bits 22, 16, 6, 0 CFEIF[7:4]: stream x clear FIFO error interrupt flag (x = 7 to 4)
            Writing 1 to this bit clears the corresponding CFEIFx flag in the DMA_HISR register

    0x3D << 6 * (7 - 6) + 16 = 0b 0011 1101 << 22 = 0b 0000-1111 0100-0000 0000-0000 0000-0000, 1111 1101 corresponds to bits 22, 24, 25, 26, 27 for stream x = 7.
    
    */
    if (streamNumber >= 6)
        DMA_Type -> HIFCR |= 0x3D << (6 * (streamNumber - 6) + 16);
    else if (streamNumber >= 4)
        DMA_Type -> HIFCR |= 0x3D << (6 * (streamNumber - 4));
    else if (streamNumber >= 2)
        DMA_Type -> LIFCR |= 0x3D << (6 * (streamNumber - 2) + 16);
    else
        DMA_Type -> LIFCR |= 0x3D << 6 * streamNumber;
    
    if ((uint32_t) DMA_Stream_Type > (uint32_t)DMA2)
        streamNumber += 8;
    
    DMAMUX_Channel_TypeDef *DMAMUX_Channel_Type = (DMAMUX_Channel_TypeDef *)(DMAMUX1_BASE + streamNumber * 4);
    
    /*
    
    DMAMUX1 request line multiplexer channel x configuration register(DMAMUX1_CxCR)
    
    Bits 6:0 DMAREQ_ID[6:0]: DMA request identification
        Selects the input DMA request. See the DMAMUX table about assignments of multiplexer
        inputs to resources.
    
    Table 125. DMAMUX1: assignment of multiplexer inputs to resource.
    
    DMA request MUX input       Resource
    45                          usart3_rx_dma
    46                          usart3_tx_dma
    
    RM0399
    Table 105. DMAMUX1, DMA1 and DMA2 connections(1) (continued)
    
    */
    
    DMAMUX_Channel_Type -> CCR &= ~(0x7F << 0);
    DMAMUX_Channel_Type -> CCR |= DMARequestMUXInput;
    
    /*
    
    DMA stream x memory 0 address register (DMA_SxM0AR).
    
    Bits 31:0 M0A[31:0]: memory 0 address
    Base address of memory area 0 from/to which the data is read/written.
    These bits are write-protected. They can be written only if:
    每 the stream is disabled (EN = 0 in DMA_SxCR) or
    每 the stream is enabled (EN = 1 in DMA_SxCR) and CT = 1 in DMA_SxCR (in
    double-buffer mode).
    */
    DMA_Stream_Type -> M0AR = sourceMemoryAddress;
    
    /*
    
    DMA stream x peripheral address register (DMA_SxPAR).
    
    Bits 31:0 PAR[31:0]: peripheral address
    Base address of the peripheral data register from/to which the data is read/written.
    These bits are write-protected and can be written only when bit EN = 0 in DMA_SxCR.
    
    */
    DMA_Stream_Type -> PAR = destionationPeripheralAddress;
    
    /*
    
    DMA stream x number of data register (DMA_SxNDTR)
    
    Bits 15:0 NDT[15:0]: number of data items to transfer (0 up to 65535)
    This register can be written only when the stream is disabled. When the stream is enabled,
    this register is read-only, indicating the remaining data items to be transmitted. This register
    decrements after each DMA transfer.
    Once the transfer is completed, this register can either stay at zero (when the stream is in
    normal mode) or be reloaded automatically with the previously programmed value in the
    following cases:
    每 when the stream is configured in circular mode.
    每 when the stream is enabled again by setting EN bit to 1.
    If the value of this register is zero, no transaction can be served even if the stream is
    enabled.
    */
    DMA_Stream_Type -> NDTR &= ~(0xFFFF << 0);
    DMA_Stream_Type -> NDTR |= transmissionItemsNumber & 0xFFFF;
    
    /*
    
    DMA stream x configuration register (DMA_SxCR).
    
    Reset the configruations.
    
    */
    DMA_Stream_Type -> CR &= ~(0x01 << 0);

    /*
    
    Bit 2 TEIE: transfer error interrupt enable
    This bit is set and cleared by software.
        0: TE interrupt disabled
        1: TE interrupt enabled
    
    */
    DMA_Stream_Type -> CR |= 0x01 << 2;
    
    /*
    
    Bit 3 HTIE: half transfer interrupt enable
    This bit is set and cleared by software.
        0: HT interrupt disabled
        1: HT interrupt enabled

    */
    DMA_Stream_Type -> CR |= 0x01 << 3;
    
    /*
    
    Bit 4 TCIE: transfer complete interrupt enable
    This bit is set and cleared by software.
        0: TC interrupt disabled
        1: TC interrupt enabled
    
    */
    
    DMA_Stream_Type -> CR |= 0x01 << 4;
    
    SetNestedVectoredInterruptPriorty(2, 2, DMA_STREAM_IRQN);
    
    /*
    
    Bit 5 PFCTRL: peripheral flow controller
    This bit is set and cleared by software.
        0: DMA is the flow controller.
        1: The peripheral is the flow controller.
    This bit is protected and can be written only if EN = 0.
    When the memory-to-memory mode is selected (bits DIR[1:0]=10), then this bit is
    automatically forced to 0 by hardware.
    
    DMA_Stream_Type -> CR |= ~(0x01 << 5);
    */
    
    /*
    
    Bits 7:6 DIR[1:0]: data transfer direction
    These bits are set and cleared by software.
        00: peripheral-to-memory
        01: memory-to-peripheral
        10: memory-to-memory
        11: reserved
    These bits are protected and can be written only if EN = 0.
        
    */
    DMA_Stream_Type -> CR &= ~(0x03 << 6);
    DMA_Stream_Type -> CR |= 0x01 << 6;
    
    /*
    
    Bit 8 CIRC: circular mode
    This bit is set and cleared by software and can be cleared by hardware.
        0: circular mode disabled
        1: circular mode enabled
    When the peripheral is the flow controller (bit PFCTRL = 1) and the stream is enabled
    (EN = 1), then this bit is automatically forced by hardware to 0.
    It is automatically forced by hardware to 1 if the DBM bit is set, as soon as the stream is
    enabled (EN = 1).
    
    DMA_Stream_Type -> CR |= 0x01 << 8;
    
    */
    
    /*
    
    Bit 9 PINC: peripheral increment mode
    This bit is set and cleared by software.
        0: peripheral address pointer fixed
        1: peripheral address pointer incremented after each data transfer (increment done
    according to PSIZE)
    This bit is protected and can be written only if EN = 0.
    
    */
    DMA_Stream_Type -> CR &= ~(0x01 << 9);
    
    /*
    
    Bit 10 MINC: memory increment mode
    This bit is set and cleared by software.
        0: memory address pointer is fixed
        1: memory address pointer is incremented after each data transfer (increment is done
    according to MSIZE)
    This bit is protected and can be written only if EN = 0.
    
    */
    DMA_Stream_Type -> CR |= 0x01 << 10;
    
    /*
    
    Bits 12:11 PSIZE[1:0]: peripheral data size
    These bits are set and cleared by software.
        00: byte (8-bit)
        01: half-word (16-bit)
        10: word (32-bit)
        11: reserved
    These bits are protected and can be written only if EN = 0.
    
    */
    DMA_Stream_Type -> CR &= ~(0x03 << 11);
    
    /*
    
    Bits 14:13 MSIZE[1:0]: memory data size
    These bits are set and cleared by software.
        00: byte (8-bit)
        01: half-word (16-bit)
        10: word (32-bit)
        11: reserved
    These bits are protected and can be written only if EN = 0.
    In direct mode, MSIZE is forced by hardware to the same value as PSIZE as soon as
    EN = 1.
    
    */
    DMA_Stream_Type -> CR &= ~(0x03 << 13);
    
    /*
    
    Bits 17:16 PL[1:0]: priority level
    These bits are set and cleared by software.
        00: low
        01: medium
        10: high
        11: very high
    These bits are protected and can be written only if EN = 0.
    
    */
    DMA_Stream_Type -> CR &= ~(0x03 << 16);
    DMA_Stream_Type -> CR |= 0x01 << 16;
    
    /*
    
    Bit 20 TRBUFF: Enable the DMA to handle bufferable transfers.
        0: bufferable transfers not enabled
        1: bufferable transfers enabled
    Note: This bit must be set to 1 if the DMA stream manages UART/USART/LPUART transfers.
    
    */
    
    DMA_Stream_Type -> CR |= 0x01 << 20;
    
    /*
    
    Bits 22:21 PBURST[1:0]: peripheral burst transfer configuration
    These bits are set and cleared by software.
        00: single transfer
        01: INCR4 (incremental burst of 4 beats)
        10: INCR8 (incremental burst of 8 beats)
        11: INCR16 (incremental burst of 16 beats)
    These bits are protected and can be written only if EN = 0.
    In direct mode, these bits are forced to 0x0 by hardware.

    */
    DMA_Stream_Type -> CR &= ~(0x03 << 21);
    
    /*
    
    Bits 24:23 MBURST[1:0]: memory burst transfer configuration
    These bits are set and cleared by software.
        00: single transfer
        01: INCR4 (incremental burst of 4 beats)
        10: INCR8 (incremental burst of 8 beats)
        11: INCR16 (incremental burst of 16 beats)
    These bits are protected and can be written only if EN = 0.
    In direct mode, these bits are forced to 0x0 by hardware as soon as bit EN = 1.
    
    */
    DMA_Stream_Type -> CR &= ~(0x03 << 23);
    
    /*
    
    Bits 1:0 FTH[1:0]: FIFO threshold selection
    These bits are set and cleared by software.
        00: 1/4 full FIFO
        01: 1/2 full FIFO
        10: 3/4 full FIFO
        11: full FIFO
    These bits are not used in the direct mode when the DMIS = 0.
    These bits are protected and can be written only if EN = 0.
    */
    DMA_Stream_Type -> FCR |= 0x03 << 0;
    
    /*
    
    Bit 2 DMDIS: direct mode disable
    This bit is set and cleared by software. It can be set by hardware.
        0: direct mode enabled
        1: direct mode disabled
    This bit is protected and can be written only if EN = 0.
    This bit is set by hardware if the memory-to-memory mode is selected (DIR bit in
    DMA_SxCR are 10) and the EN = 1 in DMA_SxCR because the direct mode is not allowed in the memory-to-memory configuration.
    
    */
    DMA_Stream_Type -> FCR |=0x01 << 2;
    
    /*
    
    Bits 5:3 FS[2:0]: FIFO status.
    These bits are read-only.
        000: 0 < fifo_level < 1/4
        001: 1/4 ≒ fifo_level < 1/2
        010: 1/2 ≒ fifo_level < 3/4
        011: 3/4 ≒ fifo_level < full
        100: FIFO is empty
        101: FIFO is full
    others: no meaning
    These bits are not relevant in the direct mode (DMDIS = 0)
    
    */

    /*
    
    Bit 7 FEIE: FIFO error interrupt enable
    This bit is set and cleared by software.
        0: FE interrupt disabled
        1: FE interrupt enabled
        
    */
    DMA_Stream_Type -> FCR &= ~(0x01 << 7);
    
    
}

void StartConveyItems(DMA_Stream_TypeDef *DMA_Stream_Type, uint16_t numberOfItems)
{
    /*
    
    Bit 0 EN: stream enable / flag stream ready when read low
    This bit is set and cleared by software.
        0: stream disabled
        1: stream enabled
    This bit may be cleared by hardware:
    每 on a DMA end of transfer (stream ready to be configured)
    每 if a transfer error occurs on the AHB master buses
    每 when the FIFO threshold on memory AHB port is not compatible with the size of the
    burst
    When this bit is read as 0, the software is allowed to program the configuration and FIFO bits
    registers. It is forbidden to write these registers when the EN bit is read as 1.
    Note: Before setting EN bit to 1 to start a new transfer, the event flags corresponding to the
    stream in DMA_LISR or DMA_HISR register must be cleared.
        
    
    Disable the DMA.
    
    */
    DMA_Stream_Type -> CR &= ~(0x01 << 0);
    
    while (DMA_Stream_Type -> CR & 0x01)
        ;
    
    /*
    
    DMA stream x number of data register (DMA_SxNDTR)
    
    Bits 15:0 NDT[15:0]: number of data items to transfer (0 up to 65535)
    This register can be written only when the stream is disabled. When the stream is enabled,
    this register is read-only, indicating the remaining data items to be transmitted. This register
    decrements after each DMA transfer.
    Once the transfer is completed, this register can either stay at zero (when the stream is in
    normal mode) or be reloaded automatically with the previously programmed value in the
    following cases:
        每 when the stream is configured in circular mode.
        每 when the stream is enabled again by setting EN bit to 1.
    If the value of this register is zero, no transaction can be served even if the stream is
    enabled.
        
    */
    
    DMA_Stream_Type -> NDTR = numberOfItems;
    
    /*  Enable the DMA. */
    DMA_Stream_Type -> CR |= 0x01 << 0;
}