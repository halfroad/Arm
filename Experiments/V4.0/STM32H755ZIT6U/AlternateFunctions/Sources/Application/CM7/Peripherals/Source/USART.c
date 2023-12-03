#include "SystemInterrupts.h"
#include "USART.h"


#define USART3_RX_ENABLED               1
#define USART3_RX_MAXIMUM_LENGHT        200

/*

Bit 15 - 0:

When the transmission is completed, the bits are set to
0x0A|0x0D|Bit#13|Bit#12|Bit#11|Bit#10|Bit#09|Bit#08|Bit#07|Bit#06|Bit#05|Bit#04|Bit#03|Bit#02|Bit#01|Bit#00

*/
uint16_t USART3_RX_STATUS = 0;
uint8_t USART3_RX_BUFFER[USART3_RX_MAXIMUM_LENGHT];

void (*onReceivedHandler)(uint8_t *bufferr, uint8_t status);

void InitUSART(uint32_t APB1Clock, uint32_t baudRate, void(*onReceived)(uint8_t *buffer, uint8_t status))
{
    /*
    
    PD8 USART3 TX
    PD9 USART3 RX

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
    
    GPIOD -> MODER &= ~((0x03 << 8 *2) | (0x03 << 9 *2));
    GPIOD -> MODER |= (0x02 << 8 * 2) | (0x02 << 9 * 2);
    
    /*
    
    Bits 15:0 OT[15:0]: Port x configuration I/O pin y (y = 15 to 0)
    These bits are written by software to configure the I/O output type.
        0: Output push-pull (reset state)
        1: Output open-drain
    */
    GPIOD -> OTYPER |= (0x00 << 8 | 0x00 << 9);
    
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
    GPIOD -> PUPDR |= (0x01 << 8 * 2| 0x01 << 9 * 2);
    
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
        
        D   PD8         USART3_TX
        D   PD9         USART3_ RX
    */
    
    GPIOD -> AFR[1] &= ~((0x0F << 0 * 4) | (0x0F << 1 * 4));
    GPIOD -> AFR[1] |= (0x07 << 0 * 4 | 0x07 << 1 * 4);
    
    /*
    
    RCC APB1 clock register (RCC_APB1LENR)
    A peripheral can be allocated (enabled) by one or both CPUs. Please refer to
    Section 9.5.10: Peripheral allocation in order to get more information on peripheral
    allocation.
    
    
    Bit 18 USART3EN: USART3 Peripheral Clocks Enable
    Set and reset by software.
        0: USART3 peripheral clocks disable (default after reset)
        1: USART3 peripheral clocks enabled
    The peripheral clocks of the USART3 are: the kernel clock selected by USART234578SEL and
    provided to uart_ker_ck input, and the pclk1d2 bus interface clock.
    
    */
    RCC -> APB1LENR |= 0x01 << 18;
    
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
    uint32_t division = (APB1Clock * 1e6 + baudRate / 2) / baudRate;
    
    /* Over8 = 0. */
    USART3 -> BRR = division;
    USART3 -> CR1 = 0;
    
    /*
    
    Bit 28 M1: Word length
    This bit must be used in conjunction with bit 12 (M0) to determine the word length. It is set or
    cleared by software.
        M[1:0] = ¡®00¡¯: 1 start bit, 8 Data bits, n Stop bit
        M[1:0] = ¡®01¡¯: 1 start bit, 9 Data bits, n Stop bit
        M[1:0] = ¡®10¡¯: 1 start bit, 7 Data bits, n Stop bit
    This bit can only be written when the USART is disabled (UE=0).
    Note: In 7-bits data length mode, the Smartcard mode, LIN master mode and Auto baud rate
    (0x7F and 0x55 frames detection) are not supported.

    */
    USART3 -> CR1 |= 0x00 << 28;
    
    /*
    
    Bit 15 OVER8: Oversampling mode
        0: Oversampling by 16
        1: Oversampling by 8
    This bit can only be written when the USART is disabled (UE=0).
    Note: In LIN, IrDA and Smartcard modes, this bit must be kept cleared.
    
    */
    USART3 -> CR1 |= 0x00 << 15;
    
    /*
    
    Bit 12 M0: Word length
    This bit is used in conjunction with bit 28 (M1) to determine the word length. It is set or
    cleared by software (refer to bit 28 (M1)description).
    This bit can only be written when the USART is disabled (UE=0)
    
    */
    USART3 -> CR1 |= 0x00 << 12;
    
    /*
    
    Bit 3 TE: Transmitter enable
    This bit enables the transmitter. It is set and cleared by software.
    0: Transmitter is disabled
    1: Transmitter is enabled
    Note: During transmission, a low pulse on the TE bit (¡®0¡¯ followed by ¡®1¡¯) sends a preamble
    (idle line) after the current word, except in Smartcard mode. In order to generate an idle
    character, the TE must not be immediately written to ¡®1¡¯. To ensure the required duration,
    the software can poll the TEACK bit in the USART_ISR register.
    In Smartcard mode, when TE is set, there is a 1 bit-time delay before the transmission
    starts.
    
    */
    USART3 -> CR1 |=  0x01 << 3;

#if USART3_RX_ENABLED

    /*
    
    Bit 5 RXFNEIE: RXFIFO not empty interrupt enable
    This bit is set and cleared by software.
        0: Interrupt inhibited
        1: USART interrupt generated whenever ORE=1 or RXFNE=1 in the USART_ISR register
            
    */

    USART3 -> CR1 |= 0x01 << 5;
    
    /*
    
    Bit 2 RE: Receiver enable
    This bit enables the receiver. It is set and cleared by software.
    0: Receiver is disabled
    1: Receiver is enabled and begins searching for a start bit
    
    */
    USART3 -> CR1 |=  0x01 << 2;
    
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

    USART3 -> CR1 |= 0x01 << 0;
    
    SetNestedVectoredInterruptPriorty(2, 2, USART3_IRQn);
    
    onReceivedHandler = onReceived;
}

void USART3_IRQHandler(void)
{
    /*
    
    Bit 5 RXFNE: RXFIFO not empty
    RXFNE bit is set by hardware when the RXFIFO is not empty, meaning that data can be
    read from the USART_RDR register. Every read operation from the USART_RDR frees a
    location in the RXFIFO.
    RXFNE is cleared when the RXFIFO is empty. The RXFNE flag can also be cleared by
    writing 1 to the RXFRQ in the USART_RQR register.
    An interrupt is generated if RXFNEIE=1 in the USART_CR1 register.
        0: Data is not received
        1: Received data is ready to be read.
    
    */
    
    uint8_t byte;
    
    if (USART3 -> ISR & (0x01 << 5))
    {
        byte = USART3 -> RDR;
        
        /*The MSB number 15 is not set, the tranmission is not completed. */
        if ((USART3_RX_STATUS & 0x8000) == 0)
        {
            /* 0x0D is received. */
            if (USART3_RX_STATUS & 0x4000)
            {
                if (byte == 0x0d)
                {
                    USART3_RX_STATUS |= 0x8000;
                    
                    if (onReceivedHandler)
                        onReceivedHandler(USART3_RX_BUFFER, USART3_RX_STATUS);
                }
                else
                    USART3_RX_STATUS = 0;   /* Error occured. */
            }
            else
            {
                if (byte == 0x0D)
                    USART3_RX_STATUS |= 0x4000;
                else
                {
                    USART3_RX_BUFFER[USART3_RX_STATUS & 0x3FFF] = byte;
                    
                    USART3_RX_STATUS ++;
                    
                    if (USART3_RX_STATUS > USART3_RX_MAXIMUM_LENGHT - 1)
                        USART3_RX_STATUS = 0;
                }
            }
        }
    }
}