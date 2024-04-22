#include "./Timers/Include/Delay.h"

#include "../Include/InterIntegratedCircuits.h"

   /*  RCC Clock   */
#define GPIO_PORT_RCC_CLOCK_ENABLE(bus, bit, bitNumber)                     do                                                                                  \
                                                                            {                                                                                   \
                                                                                if ((RCC -> bus & (bit << bitNumber)) != (bit << bitNumber))                    \
                                                                                    RCC -> bus |= bit << bitNumber;                                             \
                                                                            }                                                                                   \
                                                                            while   (0)

#define DIVERT_GPIO_PORT_DIRECTION(port, bitNumber, direction)              do                                                                                  \
                                                                            {                                                                                   \
                                                                                if (PortDirectionInput == direction)                                            \
                                                                                {                                                                               \
                                                                                    port -> MODER &= ~(0x03 << bitNumber * 2);                                  \
                                                                                }                                                                               \
                                                                                else if (PortDirectionOutput == direction)                                      \
                                                                                {                                                                               \
                                                                                    port -> MODER &= ~(0x03 << bitNumber * 2);                                  \
                                                                                    port -> MODER |= PortDirectionOutput << bitNumber * 2;                      \
                                                                                }                                                                               \
                                                                            }                                                                                   \
                                                                            while   (0)

#define WRITE_GPIO_PORT(port, bitNumber, state)                             do                                                                                  \
                                                                            {                                                                                   \
                                                                                if (PortStateSet == state)                                                      \
                                                                                    port -> BSRR |= ENABLE_BIT_NUMBER << bitNumber;                             \
                                                                                else if (PortStateReset == state)                                               \
                                                                                    port -> BSRR |= ENABLE_BIT_NUMBER << (bitNumber + 0x10);                    \
                                                                            }                                                                                   \
                                                                            while   (0)
                                                                                
#define READ_GPIO_PORT(port, bitNumber)                                     port -> IDR >> bitNumber

#define ENABLE_BIT_NUMBER                                                   0x01    /*  Bit if set the bit field.   */

#define SCL_GPIO_PORT_BUS_BRIDGE                                            AHB2ENR
#define SCL_GPIO_PORT_BIT_FILED_POSITION                                    1       /*  PB5   */
#define SCL_GPIO_PORT_RCC_CLOCK_ENABLE()                                    GPIO_PORT_RCC_CLOCK_ENABLE(SCL_GPIO_PORT_BUS_BRIDGE, ENABLE_BIT_NUMBER, SCL_GPIO_PORT_BIT_FILED_POSITION);
#define SCL_GPIO_PORT                                                       GPIOB
#define SCL_GPIO_PIN_BIT_FILED_NUMBER                                       5

#define SDA_GPIO_PORT_BUS_BRIDGE                                            AHB2ENR
#define SDA_GPIO_PORT_BIT_FILED_POSITION                                    3       /*  PD9   */
#define SDA_GPIO_PORT_RCC_CLOCK_ENABLE()                                    GPIO_PORT_RCC_CLOCK_ENABLE(SDA_GPIO_PORT_BUS_BRIDGE, ENABLE_BIT_NUMBER, SDA_GPIO_PORT_BIT_FILED_POSITION);
#define SDA_GPIO_PORT                                                       GPIOD
#define SDA_GPIO_PIN_BIT_FILED_NUMBER                                       9

#ifdef USE_GPIO_AS_GROUND_PIN
/*
    GPIO as Ground Pin.
*/

#define GPPIO_RCC_CLOCK_REGISTER                        AHB2ENR
#define GPPIO_AS_GROUNG_PORT                            GPIOD
#define GPPIO_AS_GROUNG_PORT_BIT_FIELD_NUMBER           3
#define GPPIO_AS_GROUNG_PIN                             10
/*
    AHB2 peripheral clock enable register (RCC_AHB2ENR)
    
    Bit 3 GPIODEN: IO port D clock enable
    Set and cleared by software.
        0: IO port D clock disabled
        1: IO port D clock enabled
*/

#define GPIO_AS_GROUNG_PIN_RCC_CLOCK_ENABLE(clockRegister, portBitNumber)   do                                                                                  \
                                                                            {                                                                                   \
                                                                                RCC -> clockRegister |= 0x01 << portBitNumber;                                  \
                                                                            }                                                                                   \
                                                                            while   (0)
#endif  /*  #ifdef USE_GPIO_AS_GROUND_PIN   */

#define MAXIMUM_TRIAL_TIMES                                                 250

typedef enum
{
    PortDirectionInput  = 0x00,
    PortDirectionOutput = 0x01,
    
} PortDirections;

typedef enum
{
    PortStateReset  = 0x00,
    PortStateSet    = 0x01,
    
} PortStates;

typedef enum
{
    AcknowledgementStateOK       = 0x00,
    AcknowledgementStateError    = 0x01,
    
} AcknowledgementStates;

void IssueStartSignal(void);

void InitInterIntegratedCircuits(void)
{
    /*
    
    Bit 1 GPIOBEN: IO port B clock enable
    Set and cleared by software.
        0: IO port B clock disabled
        1: IO port B clock enabled
    
    */
    
    SDA_GPIO_PORT_RCC_CLOCK_ENABLE();
    
    /*
    
    Bits 31:0 MODE[15:0][1:0]: Port x configuration I/O pin y (y = 15 to 0)
    These bits are written by software to configure the I/O mode.
        00: Input mode
        01: General purpose output mode
        10: Alternate function mode
        11: Analog mode (reset state)
    
    */
    SDA_GPIO_PORT -> MODER                      &= ~(0x03 << SDA_GPIO_PIN_BIT_FILED_NUMBER * 2);
    SDA_GPIO_PORT -> MODER                      |= 0x01 << SDA_GPIO_PIN_BIT_FILED_NUMBER * 2;
    
    /*
    
    Bits 15:0 OT[15:0]: Port x configuration I/O pin y (y = 15 to 0)
    These bits are written by software to configure the I/O output type.
        0: Output push-pull (reset state)
        1: Output open-drain
        */
    SDA_GPIO_PORT -> OTYPER                     |= 0x01 << SDA_GPIO_PIN_BIT_FILED_NUMBER;

    /*
    
    Bits 31:0 PUPD[15:0][1:0]: Port x configuration I/O pin y (y = 15 to 0)
    These bits are written by software to configure the I/O pull-up or pull-down
        00: No pull-up, pull-down
        01: Pull-up
        10: Pull-down
        11: Reserved
    
    */
    SDA_GPIO_PORT -> PUPDR                      &= ~(0x03 << SDA_GPIO_PIN_BIT_FILED_NUMBER * 2);
    SDA_GPIO_PORT -> PUPDR                      |= 0x01 << SDA_GPIO_PIN_BIT_FILED_NUMBER * 2;
    
    SCL_GPIO_PORT_RCC_CLOCK_ENABLE(); 
    
    SCL_GPIO_PORT -> MODER                      &= ~(0x03 << SCL_GPIO_PIN_BIT_FILED_NUMBER * 2);
    SCL_GPIO_PORT -> MODER                      |= 0x01 << SCL_GPIO_PIN_BIT_FILED_NUMBER * 2;
    
    SCL_GPIO_PORT -> PUPDR                      &= ~(0x03 << SCL_GPIO_PIN_BIT_FILED_NUMBER * 2);
    SCL_GPIO_PORT -> PUPDR                      |= 0x01 << SCL_GPIO_PIN_BIT_FILED_NUMBER * 2;

    InitDelay();
}

void IssueStartSignal(void)
{
    /*  Set the direction of GPIO port to Output.
    DIVERT_GPIO_PORT_DIRECTION(SDA_GPIO_PORT, SDA_GPIO_PIN_BIT_FILED_NUMBER, PortDirectionOutput);
   */
    /*  Set the SDA and SCL.   */
    WRITE_GPIO_PORT(SDA_GPIO_PORT, SDA_GPIO_PIN_BIT_FILED_NUMBER, PortStateSet);
    WRITE_GPIO_PORT(SCL_GPIO_PORT, SCL_GPIO_PIN_BIT_FILED_NUMBER, PortStateSet);
    
    /*  Keep duration for t(SU): STA.    */
    DelayUs(4);
    
    /*  With SCL sets, reset the SDA to issue the Start signal.    */
    WRITE_GPIO_PORT(SDA_GPIO_PORT, SDA_GPIO_PIN_BIT_FILED_NUMBER, PortStateReset);
    
    /*  Keep duration for t(HD): STA.    */
    DelayUs(4);
    
    /*  Vise the SCL to allow the devices to prepare the bytes that write onto the SDA line.   */
    WRITE_GPIO_PORT(SCL_GPIO_PORT, SCL_GPIO_PIN_BIT_FILED_NUMBER, PortStateReset);
}

void IssueStopSignal(void)
{
    /*  Set the direction of GPIO port to Output.
    DIVERT_GPIO_PORT_DIRECTION(SDA_GPIO_PORT, SDA_GPIO_PIN_BIT_FILED_NUMBER, PortDirectionOutput);
    */
    /*  Reset the SDA and SCL (SCL will be set later).   */
    WRITE_GPIO_PORT(SCL_GPIO_PORT, SCL_GPIO_PIN_BIT_FILED_NUMBER, PortStateReset);
    WRITE_GPIO_PORT(SDA_GPIO_PORT, SDA_GPIO_PIN_BIT_FILED_NUMBER, PortStateReset);
    
    DelayUs(4);
    
    WRITE_GPIO_PORT(SCL_GPIO_PORT, SCL_GPIO_PIN_BIT_FILED_NUMBER, PortStateSet);
    WRITE_GPIO_PORT(SDA_GPIO_PORT, SDA_GPIO_PIN_BIT_FILED_NUMBER, PortStateSet);
    
    /*  Keep duration for t(SU): STO.    */
    DelayUs(4);
}

void IssueAcknowledgeSignal(AcknowledgeRequirements acknowledgementRequired)
{
    /*  Reset the SCL, and get ready to acknowledge or non acknoledge the devices.   */
    WRITE_GPIO_PORT(SCL_GPIO_PORT, SCL_GPIO_PIN_BIT_FILED_NUMBER, PortStateReset);
    /*  
    DIVERT_GPIO_PORT_DIRECTION(SDA_GPIO_PORT, SDA_GPIO_PIN_BIT_FILED_NUMBER, PortDirectionOutput);
   */
    if (AcknowledgeRequired == acknowledgementRequired)
        WRITE_GPIO_PORT(SDA_GPIO_PORT, SDA_GPIO_PIN_BIT_FILED_NUMBER, PortStateReset);
    else
        WRITE_GPIO_PORT(SDA_GPIO_PORT, SDA_GPIO_PIN_BIT_FILED_NUMBER, PortStateSet);
    
    DelayUs(2);
    
    /*  Set the SCL, and allow the other devices to read the Acknowledgement or Non Acknowledgement just written by the MCU.   */
    WRITE_GPIO_PORT(SCL_GPIO_PORT, SCL_GPIO_PIN_BIT_FILED_NUMBER, PortStateSet);
    
    DelayUs(2);
    
    /*  Vise the SCL to allow the devices to prepare the bytes that write onto the SDA line.   */
    WRITE_GPIO_PORT(SCL_GPIO_PORT, SCL_GPIO_PIN_BIT_FILED_NUMBER, PortStateReset);
}

AcknowledgementStates PollForAcknowledgement(void)
{
    /*  Set the direction of GPIO port to Input so that the SDA is able to read the Acknowledgement or Non Acknowledgement from devices.
    DIVERT_GPIO_PORT_DIRECTION(SDA_GPIO_PORT, SDA_GPIO_PIN_BIT_FILED_NUMBER, PortDirectionInput);
    */
    WRITE_GPIO_PORT(SDA_GPIO_PORT, SDA_GPIO_PIN_BIT_FILED_NUMBER, PortStateSet);
    
    DelayUs(1);
    
    WRITE_GPIO_PORT(SCL_GPIO_PORT, SCL_GPIO_PIN_BIT_FILED_NUMBER, PortStateSet);
    
    DelayUs(1);
    
    uint8_t trials = 0;
    
    while (READ_GPIO_PORT(SDA_GPIO_PORT, SDA_GPIO_PIN_BIT_FILED_NUMBER) & PortStateSet)
    {
        trials ++;
        
        if (trials > MAXIMUM_TRIAL_TIMES)
        {
            IssueStopSignal();
            
            return AcknowledgementStateError;
        }
    }
    
    /*  Vise the SCL to allow the devices to prepare the bytes to write to the SDA line.   */
    WRITE_GPIO_PORT(SCL_GPIO_PORT, SCL_GPIO_PIN_BIT_FILED_NUMBER, PortStateReset);
    
    return AcknowledgementStateOK;
}

void WriteByte(uint8_t byte, AcknowledgeRequirements pollForAcknowledgement)
{
    /*  Set the direction of GPIO port to Output so that the SDA is able to write the bytes.
    DIVERT_GPIO_PORT_DIRECTION(SDA_GPIO_PORT, SDA_GPIO_PIN_BIT_FILED_NUMBER, PortDirectionOutput);
    */
    /*  Set the SCL to prepare the byte.   */
    WRITE_GPIO_PORT(SCL_GPIO_PORT, SCL_GPIO_PIN_BIT_FILED_NUMBER, PortStateReset);
    
    uint8_t bit = 0x00;
    
    for (uint8_t i = 0; i < 8; i ++)
    {
        /*  Reset the bits rather than the bit number 7, then right shift the bits with 7 bit fields. Eventually, the bit number 0 will be the original bit number 7.
        
        e.g. byte                       = 0x8E,
        
        i                               = 0
        
        0x8E                            = 0b1000 1110
        byte & 0x80                     = 0b1000 0000
        
        0b1000 0000 >> 7                = 0b0000 0001   (Write starts from MSB)
        
        WRITE_GPIO_PORT(SDA_GPIO_PORT, SDA_GPIO_PIN_BIT_FILED_NUMBER, 1);
        
        byte <<= 1
        
        byte                            = (byte << 1)
        byte                            = 0b1000 1110 << 1
        byte                            = 0b0001 1100
        
        i = 1
        
        (byte & 0x80) >> 7
        (0b0001 1100 & 0x80) >> 7       = 0b0000 0000 >> 7 = 0b0000 000
        
        WRITE_GPIO_PORT(SDA_GPIO_PORT, SDA_GPIO_PIN_BIT_FILED_NUMBER, 0);
        
        byte                = 0b0001 1100 << 1
        byte                = 0b0011 1000
        
        i = 2
        
        (byte & 0x80) >> 7
        (0b0011 1000 & 0x80) >> 7       = 0b0000 0000 >> 7 = 0b0000 000
        
        WRITE_GPIO_PORT(SDA_GPIO_PORT, SDA_GPIO_PIN_BIT_FILED_NUMBER, 0);
        
        byte                = 0b0011 1000 << 1
        byte                = 0b0111 0000
        
        i = 3
        
        (byte & 0x80) >> 7
        (0b0111 0000 & 0x80) >> 7       = 0b0000 0000 >> 7 = 0b0000 000
        
        WRITE_GPIO_PORT(SDA_GPIO_PORT, SDA_GPIO_PIN_BIT_FILED_NUMBER, 0);
        
        byte                = 0b0111 0000 << 1
        byte                = 0b1110 0000
        
        i = 4
        
        (byte & 0x80) >> 7
        (0b1111 0000 & 0x80) >> 7       = 0b1000 0000 >> 7 = 0b1000 000
        
        WRITE_GPIO_PORT(SDA_GPIO_PORT, SDA_GPIO_PIN_BIT_FILED_NUMBER, 1);
        
        byte                = 0b1110 0000 << 1
        byte                = 0b1100 0000
        
        i = 5
        
        (byte & 0x80) >> 7
        (0b1100 00000 & 0x80) >> 7       = 0b1000 0000 >> 7 = 0b1000 000
        
        WRITE_GPIO_PORT(SDA_GPIO_PORT, SDA_GPIO_PIN_BIT_FILED_NUMBER, 1);
        
        byte                = 0b1100 0000 << 1
        byte                = 0b1000 0000
        
        i = 6
        
        (byte & 0x80) >> 7
        (0b1000 0000 & 0x80) >> 7       = 0b1000 0000 >> 7 = 0b1000 000
        
        WRITE_GPIO_PORT(SDA_GPIO_PORT, SDA_GPIO_PIN_BIT_FILED_NUMBER, 1);
        
        byte                = 0b1000 0000 << 1
        byte                = 0b0000 0000
        
        i = 7
        
        (byte & 0x80) >> 7
        (0b0000 0000 & 0x80) >> 7       = 0b0000 0000 >> 7 = 0b0000 000
        
        WRITE_GPIO_PORT(SDA_GPIO_PORT, SDA_GPIO_PIN_BIT_FILED_NUMBER, 0);
        
        byte                = 0b1000 0000 << 1
        byte                = 0b0000 0000
        
        */
        
        bit = (byte & 0x80) >> 7;

        WRITE_GPIO_PORT(SDA_GPIO_PORT, SDA_GPIO_PIN_BIT_FILED_NUMBER, bit);
        
        byte <<= 1;
        
        /*  Keep duration t(SU): DAT*/
        DelayUs(2);
        
        /*  Set the SCL to allow other devices to read the bit just written by SDA.   */
        WRITE_GPIO_PORT(SCL_GPIO_PORT, SCL_GPIO_PIN_BIT_FILED_NUMBER, PortStateSet);
        
        /*  Keep duration t(SU): DAT*/
        DelayUs(2);
        
        /*  Vise the SCL to allow the devices to prepare the bytes to write to the SDA line.   */
        WRITE_GPIO_PORT(SCL_GPIO_PORT, SCL_GPIO_PIN_BIT_FILED_NUMBER, PortStateReset);
        
        /*  Pull for acknowledgement from receiver. */
        DelayUs(2);
    }
    
    if (AcknowledgeRequired == pollForAcknowledgement)
    {
        if (AcknowledgementStateError == PollForAcknowledgement())
            IssueStopSignal();
    }
}

uint8_t ReadByte(AcknowledgeRequirements acknowledgeTransmitterNeeded)
{
    /*  Divert the direction of SDA to Input to make the SDA to be possible to read the byte.
    DIVERT_GPIO_PORT_DIRECTION(SDA_GPIO_PORT, SDA_GPIO_PIN_BIT_FILED_NUMBER, PortDirectionInput);
   */
    uint8_t byte = 0x00;
    
    for (uint8_t i = 0; i < 8; i ++)
    {
        /*  Reset the SCL to allow the other devices to write the byte onto the SDA line.   */
        WRITE_GPIO_PORT(SCL_GPIO_PORT, SCL_GPIO_PIN_BIT_FILED_NUMBER, PortStateReset);
        
        /*  Wait for the write t be completed.  */
        DelayUs(2);
        
        /*  Set the SCL to allow the MCU to read byte from the SDA line.   */
        WRITE_GPIO_PORT(SCL_GPIO_PORT, SCL_GPIO_PIN_BIT_FILED_NUMBER, PortStateReset);
        
        /*
        
        e.g. the Transmitter wrote a byte 0x8E onto the SDA,
        
        Bits on SDA = 0x8E = 0b1000 1111
        
        byte = 0x00 = 0b0000 0000
        
        i = 0,
        
        byte <<= 1
        byte = byte << 1;
        byte = 0x00 << 1
        byte = 0b0000 0000 << 1
        
        byte = 0b0000 0000
        
        Bits on SDA = 0x8E = 0b1000 111[1]
        READ_GPIO_PORT(SDA_GPIO_PORT, SDA_GPIO_PIN_BIT_FILED_NUMBER) = 1    (Read from MSB)
        
        byte ++
        byte = 0b0000 0000 + 0b0000 0001
        byte = 0b0000 000[1]
        
        i = 1,
        
        byte = 0b0000 000[1] << 1
        byte = 0b0000 00[1]0
        
        Bits on SDA = 0x8E = 0b1[0]00 1111
        READ_GPIO_PORT(SDA_GPIO_PORT, SDA_GPIO_PIN_BIT_FILED_NUMBER) = 0    (Read from MSB)
        
        byte = 0b0000 00[1]0
        
        i = 2,
        
        byte = 0b0000 00[1]0 << 1
        byte = 0b0000 0[1]00
        
        Bits on SDA = 0x8E = 0b1000 1[1]11
        READ_GPIO_PORT(SDA_GPIO_PORT, SDA_GPIO_PIN_BIT_FILED_NUMBER) = 0    (Read from MSB)
        
        byte = 0b0000 0[1]00
        
        i = 3,
        
        byte = 0b0000 0[1]00 << 1
        byte = 0b0000 [1]000
        
        Bits on SDA = 0x8E = 0b100[0] 1111
        READ_GPIO_PORT(SDA_GPIO_PORT, SDA_GPIO_PIN_BIT_FILED_NUMBER) = 0    (Read from MSB)
        
        byte = 0b0000 [1]000
        
        i = 4,
        
        byte = byte = 0b0000 [1]000 << 1
        byte = 0b000[1] 0000
        
        Bits on SDA = 0x8E = 0b1000 [1]111
        READ_GPIO_PORT(SDA_GPIO_PORT, SDA_GPIO_PIN_BIT_FILED_NUMBER) = 1    (Read from MSB)
        
        byte ++
        byte = 0b000[1] 0000 + 0b0000 0001
        byte = 0b000[1] 000[1]
        
        i = 5,
        
        byte = 0b000[1] 000[1] << 1
        byte = 0b00[1]0 00[1]0
        
        Bits on SDA = 0x8E = 0b1000 1[1]11
        READ_GPIO_PORT(SDA_GPIO_PORT, SDA_GPIO_PIN_BIT_FILED_NUMBER) = 1    (Read from MSB)
        
        byte ++
        byte = 0b00[1]0 00[1]0 + 0b0000 0001
        byte = 0b00[1]0 00[1][1]
        
        i = 6,
        
        byte = 0b00[1]0 00[1][1] << 1
        byte = 0b0[1]00 0[1][1]0
        
        Bits on SDA = 0x8E = 0b1000 11[1]1
        READ_GPIO_PORT(SDA_GPIO_PORT, SDA_GPIO_PIN_BIT_FILED_NUMBER) = 1    (Read from MSB)
        
        byte ++
        byte = 0b0[1]00 0[1][1]0 + 0b0000 0001
        byte = 0b0[1]00 0[1][1][1]
        
        i = 7,
        
        byte = 0b0[1]00 0[1][1][1] << 1
        byte = 0b[1]000 [1][1][1]0
        
        Bits on SDA = 0x8E = 0b1000 111[1]
        READ_GPIO_PORT(SDA_GPIO_PORT, SDA_GPIO_PIN_BIT_FILED_NUMBER) = 1    (Read from MSB)
        
        byte ++
        byte = 0b[1]000 [1][1][1]0 + 0b0000 0001
        byte = 0b[1]000 [1][1][1][1]
        
        */
        
        byte <<= 1;
        
        if (READ_GPIO_PORT(SDA_GPIO_PORT, SDA_GPIO_PIN_BIT_FILED_NUMBER))
            byte ++;
        
        DelayUs(1);
    }
    
    IssueAcknowledgeSignal(acknowledgeTransmitterNeeded);
    
    return byte;
}

#ifdef USE_GPIO_AS_GROUND_PIN

void PullDownAsGround(void)
{
    GPIO_AS_GROUNG_PIN_RCC_CLOCK_ENABLE(GPPIO_RCC_CLOCK_REGISTER, GPPIO_AS_GROUNG_PORT_BIT_FIELD_NUMBER);
    
    GPPIO_AS_GROUNG_PORT -> MODER &= ~(0x03 << GPPIO_AS_GROUNG_PIN * 2);
    GPPIO_AS_GROUNG_PORT -> MODER |= 0x01 << GPPIO_AS_GROUNG_PIN * 2;
    
    GPPIO_AS_GROUNG_PORT -> PUPDR &= ~(0x03 << GPPIO_AS_GROUNG_PIN * 2);
    GPPIO_AS_GROUNG_PORT -> PUPDR |= 0x02 << GPPIO_AS_GROUNG_PIN * 2;
}

#endif   /* #ifdef USE_GPIO_AS_GROUND_PIN   */