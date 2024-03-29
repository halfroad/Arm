#include "SystemInterrupts.h"
#include "USART.h"

#define USART_RCC_CLOCK_ENABLE()                do  {   RCC -> APB1LENR |= 0x01 << 18;  }   while (0)
#define USART_IRQN                              USART3_IRQn

#define USART_RX_ENABLED                        1
#define USART_RX_MAXIMUM_LENGHT                 200

#define USART_IRQHANDLER                        USART3_IRQHandler

/*

Bit 15 - 0:

When the transmission is completed, the bits are set to
0x0A|0x0D|Bit#13|Bit#12|Bit#11|Bit#10|Bit#09|Bit#08|Bit#07|Bit#06|Bit#05|Bit#04|Bit#03|Bit#02|Bit#01|Bit#00

*/
uint16_t USART_RX_STATUS = 0;
uint8_t USART_RX_BUFFER[USART_RX_MAXIMUM_LENGHT];

void (*onReceivedHandler)(uint8_t *bufferr, uint8_t status);

void InitUSART(uint32_t APB1Clock, uint32_t baudRate, void(*onReceived)(uint8_t *buffer, uint8_t status))
{
    /*
    
    PD8 USART TX
    PD9 USART RX

    Bit 3 GPIODEN: GPIOD peripheral clock enable
    Set and reset by software.
        0: GPIOD peripheral clock disabled (default after reset)
        1: GPIOD peripheral clock enabled
    
    */
    RCC -> AHB4ENR |= 0x01 << 3;
    
    /*
    
    
    Bits 31:0 MODER[15:0][1:0]: Port x configuration I/O pin y (y = 15 to 0)
    These bits are written by software to configure the I/O mode.
        00: Input mode
        01: General purpose output mode
        10: Alternate function mode
        11: Analog mode (reset state)
    */
    
    GPIOD -> MODER &= ~((0x03 << 8 * 2) | (0x03 << 9 * 2));
    GPIOD -> MODER |= (0x02 << 8 * 2) | (0x02 << 9 * 2);
    
    /*
    
    Bits 15:0 OT[15:0]: Port x configuration I/O pin y (y = 15 to 0)
    These bits are written by software to configure the I/O output type.
        0: Output push-pull (reset state)
        1: Output open-drain
    */
    GPIOD -> OTYPER &= ~(0x01 << 8 | 0x01 << 9);
    
    /*
    
    Bits 31:0 OSPEEDR[15:0][1:0]: Port x configuration I/O pin y (y = 15 to 0)
    These bits are written by software to configure the I/O output speed.
        00: Low speed
        01: Medium speed
        10: High speed
        11: Very high speed
    Note: Refer to the product datasheets for the values of OSPEEDRy bits versus VDD range
    and external load.
        
    */
    
    GPIOD -> OSPEEDR &= ~((0x03 << 8 * 2) | (0x03 << 9 * 2));
    GPIOD -> OSPEEDR |= (0x02 << 8 * 2 | 0x02 << 9 * 2);
    
    /*
    
    Bits 31:0 PUPDR[15:0][1:0]: Port x configuration I/O pin y (y = 15 to 0)
    These bits are written by software to configure the I/O pull-up or pull-down
        00: No pull-up, pull-down
        01: Pull-up
        10: Pull-down
        11: Reserved
    
    */
    
    GPIOD -> PUPDR &= ~((0x03 << 8 * 2) | (0x03 << 9 * 2));
    GPIOD -> PUPDR |= (0x01 << 8 * 2 | 0x01 << 9 * 2);
    
    /*
    
    GPIOx_AFRL
    Bits 31:0 AFR[7:0][3:0]: Alternate function selection for port x I/O pin y (y = 7 to 0)
    These bits are written by software to configure alternate function I/Os.
        0000: AF0
        0001: AF1
        0010: AF2
        0011: AF3
        0100: AF4
        0101: AF5
        0110: AF6
        0111: AF7
        1000: AF8
        1001: AF9
        1010: AF10
        1011: AF11
        1100: AF12
        1101: AF13
        1110: AF14
        1111: AF15

    GPIOx_AFRH
    Bits 31:0 AFR[15:8][3:0]: Alternate function selection for port x I/O pin y (y = 15 to 8)
    These bits are written by software to configure alternate function I/Os.
        0000: AF0
        0001: AF1
        0010: AF2
        0011: AF3
        0100: AF4
        0101: AF5
        0110: AF6
        0111: AF7
        1000: AF8
        1001: AF9
        1010: AF10
        1011: AF11
        1100: AF12
        1101: AF13
        1110: AF14
        1111: AF15
        
        
        Product Specifications/stm32h755zi.pdf, Table 12. Port D alternate functions
        
        Port            AF7
        
        D   PD8         USART_TX
        D   PD9         USART_ RX
    */
    
    GPIOD -> AFR[1] &= ~((0x0F << 0 * 4) | (0x0F << 1 * 4));
    GPIOD -> AFR[1] |= (0x07 << 0 * 4 | 0x07 << 1 * 4);
    
    /*
    
    RCC APB1 clock register (RCC_APB1LENR)
    A peripheral can be allocated (enabled) by one or both CPUs. Please refer to
    Section 9.5.10: Peripheral allocation in order to get more information on peripheral
    allocation.
    
    
    Bit 18 USARTEN: USART Peripheral Clocks Enable
    Set and reset by software.
        0: USART peripheral clocks disable (default after reset)
        1: USART peripheral clocks enabled
    The peripheral clocks of the USART are: the kernel clock selected by USART234578SEL and
    provided to uart_ker_ck input, and the pclk1d2 bus interface clock.
    
    RCC -> APB1LENR |= 0x01 << 18;
    
    */
    
    USART_RCC_CLOCK_ENABLE();
    
    /*
    RCC domain 2 kernel clock configuration register (RCC_D2CCIP2R) 
    Bits 2:0 USART234578SEL[2:0]: USART2/3, UART4,5, 7/8 (APB1) kernel clock source selection
    Set and reset by software.
        000: rcc_pclk1 clock is selected as kernel clock (default after reset)
        001: pll2_q_ck clock is selected as kernel clock
        010: pll3_q_ck clock is selected as kernel clock
        011: hsi_ker_ck clock is selected as kernel clock
        100: csi_ker_ck clock is selected as kernel clock
        101: lse_ck clock is selected as kernel clock
    others: reserved, the kernel clock is disabled
    
    RCC -> D2CCIP2R &= ~(0x07 << 0);
    
    */
    
    /*
    
    USART baud rate register (USART_BRR)
    This register can only be written when the USART is disabled (UE=0). It may be
    automatically updated by hardware in auto baud rate detection mode.

    Bits 15:0 BRR[15:0]: USART baud rate
    BRR[15:4]
    BRR[15:4] = USARTDIV[15:4]
    BRR[3:0]
    When OVER8 = 0, BRR[3:0] = USARTDIV[3:0].
    When OVER8 = 1:
    BRR[2:0] = USARTDIV[3:0] shifted 1 bit to the right.
    BRR[3] must be kept cleared.
        
    */
    uint32_t division = APB1Clock * 1000000 / baudRate;
    
    /* Over8 = 0. */
    USART -> BRR = division;
    
    /* Reset all bits of CR1. */
    USART -> CR1 = 0;
    
    /*
    
    Bit 28 M1: Word length
    This bit must be used in conjunction with bit 12 (M0) to determine the word length. It is set or
    cleared by software.
        M[1:0] = ??00?．: 1 start bit, 8 Data bits, n Stop bit
        M[1:0] = ??01?．: 1 start bit, 9 Data bits, n Stop bit
        M[1:0] = ??10?．: 1 start bit, 7 Data bits, n Stop bit
    This bit can only be written when the USART is disabled (UE=0).
    Note: In 7-bits data length mode, the Smartcard mode, LIN master mode and Auto baud rate
    (0x7F and 0x55 frames detection) are not supported.

    */
    USART -> CR1 |= 0x00 << 28;
    
    /*
    
    Bit 12 M0: Word length
    This bit is used in conjunction with bit 28 (M1) to determine the word length. It is set or
    cleared by software (refer to bit 28 (M1)description).
    This bit can only be written when the USART is disabled (UE=0)
    
    */
    USART -> CR1 |= 0x00 << 12;
    
    /*
    
    Bit 15 OVER8: Oversampling mode
        0: Oversampling by 16
        1: Oversampling by 8
    This bit can only be written when the USART is disabled (UE=0).
    Note: In LIN, IrDA and Smartcard modes, this bit must be kept cleared.
    
    */
    USART -> CR1 &= ~(0x01 << 15);

    
    /*
    
    Bit 3 TE: Transmitter enable
    This bit enables the transmitter. It is set and cleared by software.
    0: Transmitter is disabled
    1: Transmitter is enabled
    Note: During transmission, a low pulse on the TE bit (??0?． followed by ??1?．) sends a preamble
    (idle line) after the current word, except in Smartcard mode. In order to generate an idle
    character, the TE must not be immediately written to ??1?．. To ensure the required duration,
    the software can poll the TEACK bit in the USART_ISR register.
    In Smartcard mode, when TE is set, there is a 1 bit-time delay before the transmission
    starts.
    
    */
    USART -> CR1 |=  0x01 << 3;

#if USART_RX_ENABLED

    /*
    
    Bit 2 RE: Receiver enable
    This bit enables the receiver. It is set and cleared by software.
    0: Receiver is disabled
    1: Receiver is enabled and begins searching for a start bit
    
    */
    USART -> CR1 |=  0x01 << 2;
    
    /*
    
    Bit 5 RXFNEIE: RXFIFO not empty interrupt enable
    This bit is set and cleared by software.
        0: Interrupt inhibited
        1: USART interrupt generated whenever ORE=1 or RXFNE=1 in the USART_ISR register
            
    */

    USART -> CR1 |= 0x01 << 5;
    
    /*
    
    Bit 7 DMAT: DMA enable transmitter
    This bit is set/reset by software
        1: DMA mode is enabled for transmission
        0: DMA mode is disabled for transmission
    
    */
    USART -> CR3 |= 0x01 << 7;
    
    /* Set the NVIC priorities. */
    SetNestedVectoredInterruptPriorty(2, 2, USART_IRQN);
    
#endif

    /*

    Bit 0 UE: USART enable
    When this bit is cleared, the USART prescalers and outputs are stopped immediately, and all
    current operations are discarded. The USART configuration is kept, but all the USART_ISR
    status flags are reset. This bit is set and cleared by software.
        0: USART prescaler and outputs disabled, low-power mode
        1: USART enabled
    Note: To enter low-power mode without generating errors on the line, the TE bit must be
    previously reset and the software must wait for the TC bit in the USART_ISR to be set
    before resetting the UE bit.
    The DMA requests are also reset when UE = 0 so the DMA channel must be disabled
    before resetting the UE bit.
    In Smartcard mode, (SCEN = 1), the SCLK is always available when CLKEN = 1,
    regardless of the UE bit value

    */

    USART -> CR1 |= 0x01 << 0;
    
    onReceivedHandler = onReceived;
}

void SendByte(uint8_t byte)
{
     /*
        
    Bits 8:0 TDR[8:0]: Transmit data value
    Contains the data character to be transmitted.
    The USART_TDR register provides the parallel interface between the internal bus and the
    output shift register (see Figure 611).
    When transmitting with the parity enabled (PCE bit set to 1 in the USART_CR1 register),
    the value written in the MSB (bit 7 or bit 8 depending on the data length) has no effect
    because it is replaced by the parity.
    Note: This register must be written only when TXE/TXFNF=1.
        
    */
    
    USART -> TDR = byte;
    
    /*
    
    Bit 6 TC: Transmission complete
    This bit indicates that the last data written in the USART_TDR has been transmitted out of
    the shift register.
    It is set by hardware when the transmission of a frame containing data is complete and
    when TXFE is set.
    An interrupt is generated if TCIE=1 in the USART_CR1 register.
    TC bit is is cleared by software, by writing 1 to the TCCF in the USART_ICR register or by a
    write to the USART_TDR register.
        0: Transmission is not complete
        1: Transmission is complete
    Note: If TE bit is reset and no transmission is on going, the TC bit is immediately set.
    
    */
    while ((USART -> ISR & (0x01 << 6)) == 0)
        ;
    
    /*
    
    Bit 6 TCCF: Transmission complete clear flag
    Writing 1 to this bit clears the TC flag in the USART_ISR register
    
    */
    
    USART -> ICR |= 0x01 << 6;
}

void USART_IRQHANDLER(void)
{
    /*
    
   Bit 5 RXNE: Read data register not empty
    RXNE bit is set by hardware when the content of the USART_RDR shift register has been
    transferred to the USART_RDR register. It is cleared by reading from the USART_RDR
    register. The RXNE flag can also be cleared by writing 1 to the RXFRQ in the USART_RQR
    register.
    An interrupt is generated if RXNEIE=1 in the USART_CR1 register.
        0: Data is not received
        1: Received data is ready to be read
    
    */
    
    uint8_t byte;
    
    if (USART -> ISR & (0x01 << 5))
    {
        byte = USART -> RDR;
        
        /*The MSB number 15 is not set, the tranmission is not completed. */
        if ((USART_RX_STATUS & 0x8000) == 0)
        {
            /* 0x0D is received. */
            if (USART_RX_STATUS & 0x4000)
            {
                if (byte == 0x0A)
                {
                    USART_RX_STATUS |= 0x8000;
                    
                    /*
                    
                    Bit 3 RXFRQ: Receive data flush request
                    Writing 1 to this bit empties the entire receive FIFO i.e. clears the bit RXFNE.
                    This enables to discard the received data without reading them, and avoid an overrun
                    condition.

                    */
                    USART -> RQR |= 0x01 << 3;
                    
                    if (onReceivedHandler)
                    {
                        onReceivedHandler(USART_RX_BUFFER, USART_RX_STATUS & 0x3FFFF);
                        
                        USART_RX_STATUS = 0;
                    }
                    
                    
                }
                else
                    USART_RX_STATUS = 0;   /* Error occured. */
            }
            else
            {
                if (byte == 0x0D)
                    USART_RX_STATUS |= 0x4000;
                else
                {
                    USART_RX_BUFFER[USART_RX_STATUS & 0x3FFF] = byte;
                    
                    USART_RX_STATUS ++;
                    
                    if (USART_RX_STATUS > USART_RX_MAXIMUM_LENGHT - 1)
                        USART_RX_STATUS = 0;
                }
            }
        }
    }
    
    /*
    
    Bit 3 ORE: Overrun error
    This bit is set by hardware when the data currently being received in the shift register is
    ready to be transferred into the USART_RDR register while RXNE=1. It is cleared by a
    software, writing 1 to the ORECF, in the USART_ICR register.
    An interrupt is generated if RXNEIE=1 or EIE = 1 in the USART_CR1 register.
        0: No overrun error
        1: Overrun error is detected
    
    Bit 3 ORECF: Overrun error clear flag
    Writing 1 to this bit clears the ORE flag in the USART_ISR register.
    
    */
    
    if (USART -> ISR & 0x01 << 3)
        USART -> ICR |= 0x01 << 3;
}