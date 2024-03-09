#include "../Include/InterIntegratedCircuit.h"

#define I2C_SCL_GPIO_PORT                                                           GPIOF
#define I2C_SCL_GPIO_PORT_BIT_NUMBER                                                5
#define I2C_SCL_GPIO_PIN_BIT_NUMBER                                                 14
                                                                                    
#define I2C_SDA_GPIO_PORT                                                           GPIOF
#define I2C_SDA_GPIO_PORT_BIT_NUMBER                                                5
#define I2C_SDA_GPIO_PIN_BIT_NUMBER                                                 15

#define RCC_I2C_GPIO_PORT_CLOCK_ENABLE(bitNumber)                                   do                                                                                      \
                                                                                    {                                                                                       \
                                                                                        if (!(RCC -> AHB4ENR & (0x01 << bitNumber)))                                        \
                                                                                            RCC -> AHB4ENR |= 0x01 << bitNumber;                                            \
                                                                                    }                                                                                       \
                                                                                    while (0)

                                                                                    /*
#define I2C_GPIO_PULL(port, bitNumber, pull)                                        do                                                                                      \
                                                                                    {                                                                                       \
                                                                                        if (pull == PullDown)                                                               \
                                                                                            port -> ODR &= ~(0x01 << bitNumber);                                            \
                                                                                        else                                                                                \
                                                                                            port -> ODR |= 0x01 << bitNumber;                                               \
                                                                                    }                                                                                       \
                                                                                    while (0)
                                                                                    
*/
                                                                                    
#define I2C_GPIO_PULL(port, bitNumber, pull)                                        do                                                                                      \
                                                                                    {                                                                                       \
                                                                                        if (pull == PullDown)                                                               \
                                                                                            port -> BSRR |= 0x01 << (bitNumber + 16);                                         \
                                                                                        else                                                                                \
                                                                                            port -> BSRR |= 0x01 << bitNumber;                                              \
                                                                                    }                                                                                       \
                                                                                    while (0)
                                                                                        
#define I2C_SDA_DIVERT(direction)                                                   do                                                                                      \
                                                                                    {                                                                                       \
                                                                                        if (direction == PortDirectionOutput)                                               \
                                                                                        {                                                                                   \
                                                                                            I2C_SDA_GPIO_PORT -> MODER &= ~(0x03 << (I2C_SDA_GPIO_PIN_BIT_NUMBER * 2));     \
                                                                                            I2C_SDA_GPIO_PORT -> MODER |= 0x01 << (I2C_SDA_GPIO_PIN_BIT_NUMBER * 2);        \
                                                                                        }                                                                                   \
                                                                                        else if (direction == PortDirectionInput)                                           \
                                                                                                I2C_SDA_GPIO_PORT -> MODER &= ~(0x03 << I2C_SDA_GPIO_PIN_BIT_NUMBER * 2);   \
                                                                                    }                                                                                       \
                                                                                    while (0)
                                                                                        
#define READ_BITS(port, bitNumber)                                                  port -> IDR >> bitNumber
                                                                                    
#define MAXIMUM_RETRIALS                                                            250

/*

When SCL is Low, the SDA shall prepare the data to be transmitted.
When SCL is High, the SDA shall read the data from bus.
                                                                                    
Most cases, the pull down/up actions are performed by MCU (Master)

 */
typedef enum
{
    PullDown,
    PullUp,
    
} Pulls;

typedef enum
{
    PortDirectionInput,
    PortDirectionOutput,
    
} PortDirections;

void InitInterIntegratedCircuit(void)
{
    /*
    
    GPIO port mode register (GPIOx_MODER)
(x =A to K)
    
    RM0399 - STM32H745755 and STM32H747757 advanced Arm?-based 32-bit MCUs.pdf
    
    Table 7. Register boundary addresses(1)
    
    Boundary address            Peripheral      Bus         Register map
    0x58020800 - 0x58020BFF     GPIOC           AHB4(D3)    Section 12.4: GPIO registers
    
    RCC AHB4 clock register (RCC_AHB4ENR)
    
    Bit 2 GPIOCEN: GPIOC peripheral clock enable
    Set and reset by software.
        0: GPIOC peripheral clock disabled (default after reset)
        1: GPIOC peripheral clock enabled
    
    */
    
    RCC_I2C_GPIO_PORT_CLOCK_ENABLE(I2C_SCL_GPIO_PORT_BIT_NUMBER);
   
   /*
   
   eg. PC8 resembles SCL on I2C, PC9 resembles SDA on I2C.
   
   */
   
   /*
   
    Bits 31:0 MODER[15:0][1:0]: Port x configuration I/O pin y (y = 15 to 0)
    These bits are written by software to configure the I/O mode.
        00: Input mode
        01: General purpose output mode
        10: Alternate function mode
        11: Analog mode (reset state
   
   */
   I2C_SCL_GPIO_PORT -> MODER &= ~(0x03 << I2C_SCL_GPIO_PIN_BIT_NUMBER * 2);
   I2C_SCL_GPIO_PORT -> MODER |= 0x01 << I2C_SCL_GPIO_PIN_BIT_NUMBER * 2;
   
   /*
   
    Bits 15:0 OT[15:0]: Port x configuration I/O pin y (y = 15 to 0)
    These bits are written by software to configure the I/O output type.
        0: Output push-pull (reset state)
        1: Output open-drain
   
   */
   
   I2C_SCL_GPIO_PORT -> OTYPER &= ~(0x01 << I2C_SCL_GPIO_PIN_BIT_NUMBER);
   
   /*
   
   Bits 31:0 PUPDR[15:0][1:0]: Port x configuration I/O pin y (y = 15 to 0)
    These bits are written by software to configure the I/O pull-up or pull-down
        00: No pull-up, pull-down
        01: Pull-up
        10: Pull-down
        11: Reserved
   
   */
   
   I2C_SCL_GPIO_PORT -> PUPDR &= ~(0x03 << I2C_SCL_GPIO_PIN_BIT_NUMBER * 2);
   I2C_SCL_GPIO_PORT -> PUPDR |= 0x02 << I2C_SCL_GPIO_PIN_BIT_NUMBER * 2;
   
   /*
   
   Bits 31:0 OSPEEDR[15:0][1:0]: Port x configuration I/O pin y (y = 15 to 0)
    These bits are written by software to configure the I/O output speed.
        00: Low speed
        01: Medium speed
        10: High speed
        11: Very high speed
    Note: Refer to the product datasheets for the values of OSPEEDRy bits versus VDD range
    and external load

   */
   I2C_SCL_GPIO_PORT -> OSPEEDR &= ~(0x03 << I2C_SCL_GPIO_PIN_BIT_NUMBER * 2);
   I2C_SCL_GPIO_PORT -> OSPEEDR |= 0x01 << I2C_SCL_GPIO_PIN_BIT_NUMBER * 2;
   
   I2C_GPIO_PULL(I2C_SCL_GPIO_PORT, I2C_SCL_GPIO_PIN_BIT_NUMBER, PullUp);
   
   /*   RCC_I2C_GPIO_PORT_CLOCK_ENABLE(I2C_SDA_GPIO_PORT_BIT_NUMBER);   */
     
   I2C_SDA_GPIO_PORT -> MODER &= ~(0x03 << I2C_SDA_GPIO_PIN_BIT_NUMBER * 2);
   I2C_SDA_GPIO_PORT -> MODER |= 0x01 << I2C_SDA_GPIO_PIN_BIT_NUMBER * 2;
   
   I2C_SDA_GPIO_PORT -> OTYPER &= ~(0x01 << I2C_SDA_GPIO_PIN_BIT_NUMBER);
   
   I2C_SDA_GPIO_PORT -> PUPDR &= ~(0x03 << I2C_SDA_GPIO_PIN_BIT_NUMBER * 2);
   I2C_SDA_GPIO_PORT -> PUPDR |= 0x02 << I2C_SDA_GPIO_PIN_BIT_NUMBER * 2;
   
   I2C_SDA_GPIO_PORT -> OSPEEDR &= ~(0x03 << I2C_SDA_GPIO_PIN_BIT_NUMBER * 2);
   I2C_SDA_GPIO_PORT -> OSPEEDR |= 0x01 << I2C_SDA_GPIO_PIN_BIT_NUMBER * 2;
   
   I2C_GPIO_PULL(I2C_SDA_GPIO_PORT, I2C_SDA_GPIO_PIN_BIT_NUMBER, PullUp);
}

/*

Issue I2C Start Signal circumstances,

1. SLC keeps high for a stamina t(SU: STA).
2. SDA jumps from high to low, and keep the low state for the stamina t(HD: STA)

*/
void IssueStartSignal (void)
{
    /*  Change the mode of SDA to output. */
    I2C_SDA_DIVERT(PortDirectionOutput);
    
    /* MCU pulls up the SDA and SCL levels. */
    I2C_GPIO_PULL(I2C_SDA_GPIO_PORT, I2C_SDA_GPIO_PIN_BIT_NUMBER, PullUp);
    I2C_GPIO_PULL(I2C_SCL_GPIO_PORT, I2C_SCL_GPIO_PIN_BIT_NUMBER, PullUp);
    
    /* Keep the SCL to be Pull Up state for duration t(SU: STA). */
    DelayUs(4);
    
    I2C_SDA_GPIO_PORT -> BSRR;
    
    /* MCU pulls down the SDA level to issue the I2C start signal: Lower the SDA from high while the CLK is still high. */
    I2C_GPIO_PULL(I2C_SDA_GPIO_PORT, I2C_SDA_GPIO_PIN_BIT_NUMBER, PullDown);
    
    /* Keep the Pull Down state of SDA for duration t(HD: STA). */
    DelayUs(4);
    
    /* MCU clamps the bus by lowering the SCL to make sure the SDA on the bus can be written by Slaves. */
    I2C_GPIO_PULL(I2C_SCL_GPIO_PORT, I2C_SCL_GPIO_PIN_BIT_NUMBER, PullDown);
}

/*

Issue I2C Start Signal circumstances,

1. SLC keeps high.
2.1. SDA jumps from low to high,
2.2. and keep the high state for the stamina t(SU: STA).

*/
void IssueStopSignal(void)
{
    /*  Change the mode of SDA to output. */
    I2C_SDA_DIVERT(PortDirectionOutput);
    
    /* MCU pulls down the SCL and SDA levels. */
    I2C_GPIO_PULL(I2C_SCL_GPIO_PORT, I2C_SCL_GPIO_PIN_BIT_NUMBER, PullDown);
    I2C_GPIO_PULL(I2C_SDA_GPIO_PORT, I2C_SDA_GPIO_PIN_BIT_NUMBER, PullDown);
    
    DelayUs(4);
    
    /* 1. SLC keeps high. */
    I2C_GPIO_PULL(I2C_SCL_GPIO_PORT, I2C_SCL_GPIO_PIN_BIT_NUMBER, PullUp);
    
    /* 2.1. SDA jumps from low to high, */
    I2C_GPIO_PULL(I2C_SDA_GPIO_PORT, I2C_SDA_GPIO_PIN_BIT_NUMBER, PullUp);
    
    /* 2.2. and keep the high state for a stamina t(SU: STA), and the SCL and SDA on the bus are restored to idle state,and ready for next Start signal. */
    DelayUs(4);
}

/*

Bits giver waits for the acknowledgement from Slave (Peer of bits receiver), circumstances,

*/
uint8_t PollAcknowledgement(void)
{
    /*  Divert the direction of SDA to Input, and ready to receive the acknowledgement from Slaves. */
    I2C_SDA_DIVERT(PortDirectionInput);
    
    /*  MCU pulls up the SDA in order to receive the low level signal from Slaves. */
    I2C_GPIO_PULL(I2C_SDA_GPIO_PORT, I2C_SDA_GPIO_PIN_BIT_NUMBER, PullUp);
    /* Keep stamina for 1 us. */
    DelayUs(1);
    
    /* MCU pulls up the level of SCL on the bus so that the Slaves could issue the signal of acknowledgement via SDA. */
    I2C_GPIO_PULL(I2C_SCL_GPIO_PORT, I2C_SCL_GPIO_PIN_BIT_NUMBER, PullUp);
    
    DelayUs(1);
    
    uint8_t retirals = 0;
    
    while (READ_BITS(I2C_SDA_GPIO_PORT, I2C_SDA_GPIO_PIN_BIT_NUMBER))
    {
        if (++ retirals > MAXIMUM_RETRIALS)
        {
            /* Maximum retrials reached, acknowledgement may be not required or slave is offline. */
            IssueStopSignal();
            
            return 1;
        }
    }
    
    /*  Lower the SCL to make sure the bus can be written by SDA. */
    I2C_GPIO_PULL(I2C_SCL_GPIO_PORT, I2C_SCL_GPIO_PIN_BIT_NUMBER, PullDown);
    
    return 0;
}

/*

Issue I2C Acknowledgement Signal circumstances,

1. Post 8 consecutive bits received by the peer, the peer acknowledges the bits giver.
2.1. The peer sends back with low level to the giver, means the peer acknowledges that the bits are well received. High level means transmission is done.
2.2. the bits giver might determine the next action depends on the acknowledgement.
2.2. When the peer is no acknowledged, means the bits giver is out of services.

*/

void IssueAcknowledgementSignal(uint8_t acknowledgementRequired)
{
     /*  Lower the SCL to make sure the bus can be written by SDA. */
    I2C_GPIO_PULL(I2C_SCL_GPIO_PORT, I2C_SCL_GPIO_PIN_BIT_NUMBER, PullDown);
    
    /*  Divert the direction of SDA to output. */
    I2C_SDA_DIVERT(PortDirectionOutput);

    if (acknowledgementRequired)
        /* Issue Low signal to Acknowledge the Slave. */
        I2C_GPIO_PULL(I2C_SDA_GPIO_PORT, I2C_SDA_GPIO_PIN_BIT_NUMBER, PullDown);
    else
        /* Issue High signal to Non-Acknowledge the Slave. */
        I2C_GPIO_PULL(I2C_SDA_GPIO_PORT, I2C_SDA_GPIO_PIN_BIT_NUMBER, PullUp);
    
    /*  Keep 2 microseconds. */
    DelayUs(2);
    
    /*  Pull up the SCL so that the Slave can read the Acknowledgement signal via SDA from bus. */
    I2C_GPIO_PULL(I2C_SCL_GPIO_PORT, I2C_SCL_GPIO_PIN_BIT_NUMBER, PullUp);
    /*  Keep 2 microseconds. */
    DelayUs(2);
    
    /*  Lower the SCL to make sure the bus can be written by SDA. */
    I2C_GPIO_PULL(I2C_SCL_GPIO_PORT, I2C_SCL_GPIO_PIN_BIT_NUMBER, PullDown);
}

void TransmitByte(uint8_t byte)
{
    /*  Divert the direction of SDA to output. */
    I2C_SDA_DIVERT(PortDirectionOutput);
    
    /*  Lower the SCL to make sure the bus can be written by SDA. */
    I2C_GPIO_PULL(I2C_SCL_GPIO_PORT, I2C_SCL_GPIO_PIN_BIT_NUMBER, PullDown);
    
    for (uint8_t i = 0; i < 8; i ++)
    {
        /*
        
        e.g. byte = 0xAB = 0b1010 1011,
        
        when i = 0,
            (0xAB & 0x08) >> 7 = (0b1010 1011 & 0x1000 0000) >> 7 = 0b1000 0000 >> 7 = 0b0000 0001 =    bit #7 of 0xAB is 1.
            byte <<= 1, 0xAB << 1 = 0b1010 1011 << 1 = 0b0101 0110 = 0x56.
        
        when i = 1,
            (0x56 & 0x08) >> 7 = (0b0101 0110 & 0x1000 0000) >> 7 = 0b0000 0000 >> 7 = 0b0000 0000 =    bit #6 of 0xAB is 0.
            byte <<= 1, 0x56 << 1 = 0b0101 0110 << 1 = 0b1010 1100 = 0xAC.
        
        when i = 2,
                                                                                                        bit #5 of 0xAB is 1.
                                                                                                        bit #4 of 0xAB is 0.
                                                                                                        bit #3 of 0xAB is 1.
                                                                                                        bit #2 of 0xAB is 0.
                                                                                                        bit #2 of 0xAB is 1.
                                                                                                        bit #2 of 0xAB is 1.
        */
        I2C_GPIO_PULL(I2C_SDA_GPIO_PORT, I2C_SDA_GPIO_PIN_BIT_NUMBER, (byte & 0x80) >> 7);
        
        /* shift left  bit to reveal next head bit. */
        byte <<= 1;
        /*  Keep 2 microseconds. */
        DelayUs(2);
        
        /*  MCU pulls up the SCL so that the Slave can read the bit signal via SDA from bus. */
        I2C_GPIO_PULL(I2C_SCL_GPIO_PORT, I2C_SCL_GPIO_PIN_BIT_NUMBER, PullUp);
        
        /*  Keep 2 microseconds. */
        DelayUs(2);
        
        /*  MCU lowers the SCL to make sure the bus can be written by SDA on the Slave. */
        I2C_GPIO_PULL(I2C_SCL_GPIO_PORT, I2C_SCL_GPIO_PIN_BIT_NUMBER, PullDown);
        
        /*  Keep 2 microseconds. */
        DelayUs(2);
    }
}

uint8_t ReadByte(uint8_t acknowledgementRequired)
{
    /*  Divert the direction of SDA to input so that the SDA port can read the bits from the bus. */
    I2C_SDA_DIVERT(PortDirectionInput);
    
    uint8_t byte = 0b00000000;
    
    for (uint8_t i = 0; i < 8; i ++)
    {
        /*  MCU lowers the SCL to make sure the bus can be written by SDA on the Slave. */
        I2C_GPIO_PULL(I2C_SCL_GPIO_PORT, I2C_SCL_GPIO_PIN_BIT_NUMBER, PullDown);
        
        /*  Keep 2 microseconds, wait for the SDA to be wtitten by Slaves. */
        DelayUs(2);
        
        /*  MCU pulls up the SCL so that the MCU can read the SDA written by Slaves. */
        I2C_GPIO_PULL(I2C_SCL_GPIO_PORT, I2C_SCL_GPIO_PIN_BIT_NUMBER, PullUp);
        
        byte <<= 1;
        
        if (READ_BITS(I2C_SDA_GPIO_PORT, I2C_SDA_GPIO_PIN_BIT_NUMBER))
            byte ++;
        
        /*  Keep 1 microseconds. */
        DelayUs(1);
    }
    
    IssueAcknowledgementSignal(acknowledgementRequired ? 0x00 : 0x01);
    
    return byte;
}