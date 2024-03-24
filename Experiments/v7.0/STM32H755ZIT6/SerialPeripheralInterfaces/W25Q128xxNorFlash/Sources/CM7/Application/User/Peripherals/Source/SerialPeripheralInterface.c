#include "../../../Core/Interrupts/Include/SystemInterrupts.h"

#include "../Include/SerialPeripheralInterface.h"

#define RCC_SPI_BUS                                                         APB2ENR
#define RCC_SPI_BUS_CLOCK_ENABLE                                            RCC_APB2ENR_SPI1EN

#define SPI                                                                 SPI1

/*  Configuration Register 1.  */
#define CONFIGURATION_REGISTER1                                             CFG1

#define DATA_SIZE_MASK                                                      SPI_CFG1_DSIZE_Msk
#define DATA_SIZE                                                           (SPI_CFG1_DSIZE_0 | SPI_CFG1_DSIZE_1 | SPI_CFG1_DSIZE_2)

#define CRC_ENABLE_MASK                                                     SPI_CFG1_CRCEN_Msk
#define CRC_ENABLE                                                          SPI_CFG1_CRCEN

#define CRC_SIZE_MSK                                                        SPI_CFG1_CRCSIZE_Msk
#define CRC_SIZE                                                            SPI_CFG1_CRCSIZE

#define MASTER_BAUD_RATE_MASK                                               SPI_CFG1_MBR_Msk
#define MASTER_BAUD_RATE                                                    SPI_CFG1_MBR_1

/*  Configuration Register 2.  */
#define CONFIGURATION_REGISTER2                                             CFG2

#define COMMUNICATION_MODE_MASK                                             SPI_CFG2_COMM_Msk
#define SERIAL_PROTOCOL_MASK                                                SPI_CFG2_SP_Msk
#define SPI_MASTER                                                          SPI_CFG2_MASTER
#define LSB_FRST_DATA_FRAME_FORMAT_MASK                                     SPI_CFG2_LSBFRST_Msk
#define LSB_FRST_DATA_FRAME_FORMAT                                          SPI_CFG2_LSBFRST
#define CLOCK_PHASE                                                         SPI_CFG2_CPHA
#define CLOCK_POLARITY                                                      SPI_CFG2_CPOL
#define SOFTWARE_MANAGEMENT_SLAVE_SELECT_MASK                               SPI_CFG2_SSM_Msk
#define SOFTWARE_MANAGEMENT_SLAVE_SELECT                                    SPI_CFG2_SSM
#define SLAVE_SELECT_OUTPUT_MANAGEMENT_IN_MASTER_MODE_MASK                  SPI_CFG2_SSOM_Msk
#define SLAVE_SELECT_OUTPUT_MANAGEMENT_IN_MASTER_MODE                       SPI_CFG2_SSOM
#define SLAVE_SELECT_OUTPUT_ENABLE_MASK                                     SPI_CFG2_SSOE_Msk
#define SLAVE_SELECT_OUTPUT_ENABLE                                          SPI_CFG2_SSOE

/*  Control Register 1.  */
#define CONTROL_REGISTER1                                                   CR1

#define INTERNAL_SLAVE_SELECT_MASK                                          SPI_CR1_SSI_Msk
#define INTERNAL_SLAVE_SELECT                                               SPI_CR1_SSI
#define SERIAL_PERIPHERAL_ENABLE                                            SPI_CR1_SPE
#define MASTER_TRANSFER_START                                               SPI_CR1_CSTART

#define INTERRUPTS_ENABLED                                                  0

/*  Interrupts Enablement Register.     */
#define INTERRUPTS_ENABLEMENT_REGISTER                                      IER

#define EOT_SUSP_TXC_INTERRUPT_ENABLE                                       (SPI_IER_RXPIE | SPI_IER_TXPIE | SPI_IER_EOTIE | SPI_IER_TXTFIE | SPI_IER_MODFIE)
#define SPI_IRQN                                                            SPI1_IRQn
#define SPI_IRQ_HANDLER                                                     SPI1_IRQHandler

/*  Control Register 2.  */
#define CONTROL_REGISTER2                                                   CR2

#define NUMBER_OF_DATA_AT_CURRENT_TRANSFER_MASK                             SPI_CR2_TSIZE_Msk
#define NUMBER_OF_DATA_AT_CURRENT_TRANSFER                                  SPI_CR2_TSIZE

#define GPIO_PORT_BUS                                                       AHB4ENR

/*  SCK */
#define SCK_RCC_GPIO_PORT_BUS_ENABLE                                        RCC_AHB4ENR_GPIOAEN
#define SCK_GPIO_PORT                                                       GPIOA
#define SCK_GPIO_PORT_BUS_BIT_NUMBER                                        0

#define SCK_GPIO_MODER_MODE_MASK                                            GPIO_MODER_MODE5_Msk
#define SCK_GPIO_MODER_MODE                                                 GPIO_MODER_MODE5_1

#define SCK_GPIO_OUTPUT_SPEED_MASK                                          GPIO_OSPEEDR_OSPEED5_Msk;
#define SCK_GPIO_OUTPUT_SPEED                                               GPIO_OSPEEDR_OSPEED5_1

#define SCK_GPIO_PULL_UP_DOWN_MASK                                          GPIO_PUPDR_PUPD5_Msk;
#define SCK_GPIO_PULL_UP_DOWN                                               GPIO_PUPDR_PUPD5_0

#define SCK_GPIO_ALTERNATE_FUNCTIOM_SELECTOR_MASK                           GPIO_AFRL_AFSEL5_Msk;
#define SCK_GPIO_ALTERNATE_FUNCTIOM_SELECTOR                                (GPIO_AFRL_AFSEL5_0 | GPIO_AFRL_AFSEL5_2)

#define SCK_GPIO_PIN_ALTERNATE_FUNCTION_REGISTERS_ARRAY_INDEX               0

/*  MISO */
#define MISO_RCC_GPIO_PORT_BUS_ENABLE                                       RCC_AHB4ENR_GPIOAEN
#define MISO_GPIO_PORT                                                      GPIOA
#define MISO_GPIO_PORT_BUS_BIT_NUMBER                                       0

#define MISO_GPIO_MODER_MODE_MASK                                           GPIO_MODER_MODE6_Msk
#define MISO_GPIO_MODER_MODE                                                GPIO_MODER_MODE6_1

#define MISO_GPIO_OUTPUT_SPEED_MASK                                         GPIO_OSPEEDR_OSPEED6_Msk;
#define MISO_GPIO_OUTPUT_SPEED                                              GPIO_OSPEEDR_OSPEED6_1

#define MISO_GPIO_PULL_UP_DOWN_MASK                                         GPIO_PUPDR_PUPD6_Msk;
#define MISO_GPIO_PULL_UP_DOWN                                              GPIO_PUPDR_PUPD6_0

#define MISO_GPIO_ALTERNATE_FUNCTIOM_SELECTOR_MASK                          GPIO_AFRL_AFSEL6_Msk;
#define MISO_GPIO_ALTERNATE_FUNCTIOM_SELECTOR                               (GPIO_AFRL_AFSEL6_0 | GPIO_AFRL_AFSEL6_2)

#define MISO_GPIO_PIN_ALTERNATE_FUNCTION_REGISTERS_ARRAY_INDEX              0

/*  MOSI */
#define MOSI_RCC_GPIO_PORT_BUS_ENABLE                                       RCC_AHB4ENR_GPIOAEN
#define MOSI_GPIO_PORT                                                      GPIOA
#define MOSI_GPIO_PORT_BUS_BIT_NUMBER                                       0

#define MOSI_GPIO_MODER_MODE_MASK                                           GPIO_MODER_MODE7_Msk
#define MOSI_GPIO_MODER_MODE                                                GPIO_MODER_MODE7_1

#define MOSI_GPIO_OUTPUT_SPEED_MASK                                         GPIO_OSPEEDR_OSPEED7_Msk;
#define MOSI_GPIO_OUTPUT_SPEED                                              GPIO_OSPEEDR_OSPEED7_1

#define MOSI_GPIO_PULL_UP_DOWN_MASK                                         GPIO_PUPDR_PUPD7_Msk;
#define MOSI_GPIO_PULL_UP_DOWN                                              GPIO_PUPDR_PUPD7_0

#define MOSI_GPIO_ALTERNATE_FUNCTIOM_SELECTOR_MASK                          GPIO_AFRL_AFSEL7_Msk;
#define MOSI_GPIO_ALTERNATE_FUNCTIOM_SELECTOR                               (GPIO_AFRL_AFSEL7_0 | GPIO_AFRL_AFSEL7_2)

#define MOSI_GPIO_PIN_ALTERNATE_FUNCTION_REGISTERS_ARRAY_INDEX              0

/*  NSS */
#define NSS_RCC_GPIO_PORT_BUS_ENABLE                                        RCC_AHB4ENR_GPIOBEN
#define NSS_GPIO_PORT                                                       GPIOB
#define NSS_GPIO_PORT_BUS_BIT_NUMBER                                        1

#define NSS_GPIO_MODER_MODE_MASK                                            GPIO_MODER_MODE13_Msk
#define NSS_GPIO_MODER_MODE                                                 GPIO_MODER_MODE13_0

#define NSS_GPIO_OUTPUT_SPEED_MASK                                          GPIO_OSPEEDR_OSPEED13_Msk;
#define NSS_GPIO_OUTPUT_SPEED                                               GPIO_OSPEEDR_OSPEED13_1

#define NSS_GPIO_PULL_UP_DOWN_MASK                                          GPIO_PUPDR_PUPD13_Msk;
#define NSS_GPIO_PULL_UP_DOWN                                               GPIO_PUPDR_PUPD13_0

#define NSS_GPIO_BIT_SET                                                    GPIO_BSRR_BS13;
#define NSS_GPIO_BIT_RESET                                                  GPIO_BSRR_BR13

typedef enum
{
    SlaveSelected,
    SlaveUnselected,
    
} SlaveSelection;

static void InitGPIOs(void);
void IssueSelectSlaveSignal(SlaveSelection selection);
/*

MCUs/STM32H755ZIT6/Reference manuals/RM0399 - STM32H745755 and STM32H747757 advanced Arm?-based 32-bit MCUs.pdf

Table 7. Register boundary addresses(1) (continued)

Boundary address                    Peripheral              Bus
0x40013000 - 0x400133FF             SPI1 / I2S1             APB2
(D2)

*/
void InitSerialPeripheralInterface(void)
{
    InitGPIOs();
    /*
    
    Bit 12 SPI1EN: SPI1 Peripheral Clocks Enable
    Set and reset by software.
        0: SPI1 peripheral clocks disabled (default after reset)
        1: SPI1 peripheral clocks enabled:
    The peripheral clocks of the SPI1 are: the kernel clock selected by I2S123SRC and provided to
    ker_ck input, and the rcc_pclk2 bus interface clock.
    
    */
    RCC -> RCC_SPI_BUS |= RCC_SPI_BUS_CLOCK_ENABLE;
    
    /*
    
    Bits 4:0 DSIZE[4:0]: number of bits in at single SPI data frame
        00000: not used
        00001: not used
        00010: not used
        00011: 4-bits
        00100: 5-bits
        00101: 6-bits
        00110: 7-bits
        00111: 8-bits
        .....
        11101: 30-bits
        11110: 31-bits
        11111: 32-bits
    Note: The most significant bit at DSIZE bit field is reserved at the peripheral instances where
    data size is limited to 16-bit.
    */
    SPI -> CONFIGURATION_REGISTER1 &= ~DATA_SIZE_MASK; 
    SPI -> CONFIGURATION_REGISTER1 |= DATA_SIZE;
    
    
    /*
    
    Bit 22 CRCEN: hardware CRC computation enable
        0: CRC calculation disabled
        1: CRC calculation Enabled
    Bit 21 Reserved, must be kept at reset value.
    
    SPI -> CONFIGURATION_REGISTER1 &= ~CRC_ENABLE_MASK;
    */
    
    /*
    
    Bits 20:16 CRCSIZE[4:0]: length of CRC frame to be transacted and compared
    Most significant bits are taken into account from polynomial calculation when CRC result is
    transacted or compared. The length of the polynomial is not affected by this setting.
        00000: reserved
        00001: reserved
        00010: reserved
        00011: 4-bits
        00100: 5-bits
        00101: 6-bits
        00110: 7-bits
        00111: 8-bits
        .....
        11101: 30-bits
        11110: 31-bits
        11111: 32-bits
    The value must be equal or multiply of DSIZE length
    Note: If crc calculation is disabled by CRCEN=0, the CRCSIZE field must be kept at its default
    setting.
    Note: The most significant bit at CRCSIZE bit field is reserved at the peripheral instances
    where the data size is limited to 16-bit
    
    
    SPI -> CONFIGURATION_REGISTER1 &= ~CRC_SIZE_MSK;
    SPI -> CONFIGURATION_REGISTER1 |= CRC_SIZE;
    
    */
    
    /*
    
    Bits 30:28 MBR[2:0]: master baud rate
        000: SPI master clock/2
        001: SPI master clock/4
        010: SPI master clock/8
        011: SPI master clock/16
        100: SPI master clock/32
        101: SPI master clock/64
        110: SPI master clock/128
        111: SPI master clock/256
        
    */
    SPI -> CONFIGURATION_REGISTER1 &= ~ MASTER_BAUD_RATE_MASK; 
    SPI -> CONFIGURATION_REGISTER1 |= MASTER_BAUD_RATE;
    
    SPI -> CONFIGURATION_REGISTER1 |= SPI_CR1_HDDIR;
    
    /*
    
    Bits 18:17 COMM[1:0]: SPI communication mode
        00: full-duplex
        01: simplex transmitter
        10: simplex receiver
        11: half-duplex
    
    */
    SPI -> CONFIGURATION_REGISTER2 &= ~COMMUNICATION_MODE_MASK;
    
    /*
    
    its 21:19 SP[2:0]: Serial protocol
        000: SPI Motorola
        001: SPI TI
    others: Reserved, must not be used
    
    */
    SPI -> CONFIGURATION_REGISTER2 &= ~SERIAL_PROTOCOL_MASK;
    
    /*
    
    Bit 23 LSBFRST: data frame format
        0: MSB transmitted first
        1: LSB transmitted first
    Note: This bit can be also used in PCM and I2S modes
    
    SPI -> CONFIGURATION_REGISTER2 &= ~LSB_FRST_DATA_FRAME_FORMAT;
    
    */
    
    /*
    
    Bit 24 CPHA: clock phase
        0: the first clock transition is the first data capture edge
        1: the second clock transition is the first data capture edge
    
    */
    SPI -> CONFIGURATION_REGISTER2 |= CLOCK_PHASE;
     
    /*
    
    Bit 25 CPOL: clock polarity
        0: SCK signal is at 0 when idle
        1: SCK signal is at 1 when idle
    
    */
    SPI -> CONFIGURATION_REGISTER2 |= CLOCK_POLARITY;
       
    
    /*
    
    Bit 26 SSM: software management of SS (Slave Select) signal input
        0: SS input value is determined by the SS PAD
        1: SS input value is determined by the SSI bit
    SS signal input has to be managed by software (SSM=1, SSI=1) when SS output mode is
    enabled (SSOE=1) at master mode.
    
    Use Hardware Slave Select:
    SPI -> CONFIGURATION_REGISTER2 &= ~SOFTWARE_MANAGEMENT_SLAVE_SELECT_MASK;
    */
    
    /*  Use Hardware Slave Select:  */
    SPI -> CONFIGURATION_REGISTER2 |= SOFTWARE_MANAGEMENT_SLAVE_SELECT;
    
    /*
        
    Bit 30 SSOM: SS output management in master mode
    This bit is used in master mode when SSOE is enabled. It allows to configure SS output
    between two consecutive data transfers.
        0: SS is kept at active level till data transfer is completed, it becomes inactive with EOT flag
        1: SPI data frames are interleaved with SS non active pulses when MIDI[3:0]>1
    */
    SPI -> CONFIGURATION_REGISTER2 &= ~SLAVE_SELECT_OUTPUT_MANAGEMENT_IN_MASTER_MODE_MASK;
    /*  SPI -> CONFIGURATION_REGISTER2 |= SLAVE_SELECT_OUTPUT_MANAGEMENT_IN_MASTER_MODE;    */
    
    /*
    Bit 29 SSOE: SS output enable
    This bit is taken into account at master mode only
        0: SS output is disabled and the SPI can work in multi-master configuration
        1: SS output is enabled. The SPI cannot work in a multi-master environment. It forces the SS
    pin at inactive level after the transfer in according with SSOM, MIDI, MSSI, SSIOP bits setting
    
    https://community.st.com/t5/stm32-mcus-products/spi-configuration-bare-metal-stm32h743/td-p/409339
    
    */
    SPI -> CONFIGURATION_REGISTER2 &= ~SLAVE_SELECT_OUTPUT_ENABLE_MASK;
    SPI -> CONFIGURATION_REGISTER2 |= SLAVE_SELECT_OUTPUT_ENABLE;
    
    /*
    
    Bit 12 SSI: internal SS signal input level
    This bit has an effect only when the SSM bit is set. The value of this bit is forced onto the
    peripheral SS input and the I/O value of the SS pin is ignored
    
    */
    SPI -> CONTROL_REGISTER1 |= INTERNAL_SLAVE_SELECT;
    
    /*
    
    Bit 22 MASTER: SPI master
        0: SPI Slave
        1: SPI Master
        
        Master mode (line 19) also doesn't configure, register stays 0.
        
        in the "older" SPIs resulted from NSS (in 'H7 apparently renamed to SS, for some extra confusion to those attempting to migrate)
        being low (which it is if it's not configured to any pin nor as internal, a nice undocumented feature) forcing SPI into slave mode.
        You should see this in the status registers MODF bit being set immediately after the attempt to set the master bit.
        If you don't intend to use the hardware (N)SS pin, you ought to set both SSM and SSI (see footnote under Master and
        three independent slaves at star topology figure).
    */
    SPI -> CONFIGURATION_REGISTER2 |= SPI_MASTER;
    
#if INTERRUPTS_ENABLED

    SetNestedVectoredInterruptPriorty(SPI_IRQN, 2, 2);

    /*

    Bit 3 EOTIE: EOT, SUSP and TXC interrupt enable
        0: EOT/SUSP/TXC interrupt disabled
        1: EOT/SUSP/TXC interrupt enabled

    */

    SPI -> INTERRUPTS_ENABLEMENT_REGISTER |= EOT_SUSP_TXC_INTERRUPT_ENABLE;

#endif  /*  #if INTERRUPTS_ENABLED  */
    
    /*
    
    Bit 0 SPE: serial peripheral enable
    This bit is set by and cleared by software.
        0: serial peripheral disabled.
        1: serial peripheral enabled
    When SPE=1, SPI data transfer is enabled, Configuration registers and IOLOCK bit in
    CR1 are write protected. They can be changed only when SPE=0.
    When SPI=0 any SPI operation is stopped and disabled, internal state machine is reseted, all
    the FIFOs content is flushed, CRC calculation initialized, receive data register is read zero.
    SPE cannot be set when MODF error flag is active.
    
    */
    SPI -> CONTROL_REGISTER1 |= SERIAL_PERIPHERAL_ENABLE;
    
     /*
    
    Bit 9 CSTART: master transfer start
    This bit is set by software to start an SPI or I2S/PCM communication. In SPI mode, it is
    cleared by hardware when End Of Transfer (EOT) flag is set or when a transaction suspend
    request is accepted. In I2S/PCM mode, it is also cleared by hardware as described in the
    section stop sequence.
        0: master transfer is at idle
        1: master transfer is on-going or temporary suspended by automatic suspend
    In SPI mode, CSTART can be set only when SPE=1 and MASTER=1.
    In SPI mode, In case of master transmission is enabled, communication starts or continues
    only if any data is available in the transmission FIFO.
    In I2S/PCM mode, CSTART can be set when SPE = 1
    
    */
    SPI -> CONTROL_REGISTER1 |= MASTER_TRANSFER_START;
}

static void InitGPIOs(void)
{
    /*
    
    User Manuals/um2408-stm32h7-nucleo144-boards-mb1363-stmicroelectronics.pdf
    
    Table 17. NUCLEO-H745ZI-Q and NUCLEO-H755ZI-Q pin assignments (continued)
    
    Connector       Pin         Pin name        Signal name         STM32H7 pin         Function
    
    CN7             10          D13             SPI_A_SCK               PA5             SPI1_SCK
    CN7             12          D12             SPI_A_MISO              PA6             SPI1_MISO
    CN7             14          D11             SPI_A_MOSI              PA7             SPI1_MOSI
    CN7             16          D10             SPI_A_CS                PD14            SPI1_CS
    
    */
        
    if ((RCC -> GPIO_PORT_BUS & SCK_RCC_GPIO_PORT_BUS_ENABLE) != SCK_RCC_GPIO_PORT_BUS_ENABLE)
        RCC -> GPIO_PORT_BUS |= SCK_RCC_GPIO_PORT_BUS_ENABLE;
        
    /*
    
    Bits 31:0 MODER[15:0][1:0]: Port x configuration I/O pin y (y = 15 to 0)
    These bits are written by software to configure the I/O mode.
        00: Input mode
        01: General purpose output mode
        10: Alternate function mode
        11: Analog mode (reset state)
    */
    SCK_GPIO_PORT -> MODER &= ~SCK_GPIO_MODER_MODE_MASK;
    SCK_GPIO_PORT -> MODER |= SCK_GPIO_MODER_MODE;
    
    /*
    
    GPIO port output type register (GPIOx_OTYPER)
    
    Bits 15:0 OT[15:0]: Port x configuration I/O pin y (y = 15 to 0)
    These bits are written by software to configure the I/O output type.
        0: Output push-pull (reset state)
        1: Output open-drain
    
    */
    //SCK_GPIO_PORT -> OTYPER 
    
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
    SCK_GPIO_PORT -> OSPEEDR &= ~SCK_GPIO_OUTPUT_SPEED_MASK;
    SCK_GPIO_PORT -> OSPEEDR |= SCK_GPIO_OUTPUT_SPEED;
    
    /*
    
    Bits 31:0 PUPDR[15:0][1:0]: Port x configuration I/O pin y (y = 15 to 0)
    These bits are written by software to configure the I/O pull-up or pull-down
        00: No pull-up, pull-down
        01: Pull-up
        10: Pull-down
        11: Reserved
    
    */
    SCK_GPIO_PORT -> PUPDR &= ~SCK_GPIO_PULL_UP_DOWN_MASK;
    //SCK_GPIO_PORT -> PUPDR |= SCK_GPIO_PULL_UP_DOWN;
    
    /*
    
    GPIO alternate function low register (GPIOx_AFRL)
    
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
        
    GPIO alternate function high register (GPIOx_AFRH)
    
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
    */
    
    /*
    
    MCUs/STM32H755ZIT6/Product Specifications/stm32h755zi.pdf
    
    Table 9. Port A alternate functions
    
    Port        Pin name                AF5
    
    Port A      PA5                     SPI1_SCK
    Port A      PA6                     SPI1_MISO
    Port A      PA7                     SPI1_MOSI
    
    */
    SCK_GPIO_PORT -> AFR[SCK_GPIO_PIN_ALTERNATE_FUNCTION_REGISTERS_ARRAY_INDEX] &= ~SCK_GPIO_ALTERNATE_FUNCTIOM_SELECTOR_MASK;
    SCK_GPIO_PORT -> AFR[SCK_GPIO_PIN_ALTERNATE_FUNCTION_REGISTERS_ARRAY_INDEX] |= SCK_GPIO_ALTERNATE_FUNCTIOM_SELECTOR;
    
    /*  MISO    */
    
    if ((RCC -> GPIO_PORT_BUS & MISO_RCC_GPIO_PORT_BUS_ENABLE) != MISO_RCC_GPIO_PORT_BUS_ENABLE)
        RCC -> GPIO_PORT_BUS |= MISO_RCC_GPIO_PORT_BUS_ENABLE;
    
    MISO_GPIO_PORT -> MODER &= ~MISO_GPIO_MODER_MODE_MASK;
    MISO_GPIO_PORT -> MODER |= MISO_GPIO_MODER_MODE;
    
    MISO_GPIO_PORT -> OSPEEDR &= ~MISO_GPIO_OUTPUT_SPEED_MASK;
    MISO_GPIO_PORT -> OSPEEDR |= MISO_GPIO_OUTPUT_SPEED;
    
    MISO_GPIO_PORT -> PUPDR &= ~MISO_GPIO_PULL_UP_DOWN_MASK;
    //MISO_GPIO_PORT -> PUPDR |= MISO_GPIO_PULL_UP_DOWN;
    
    MISO_GPIO_PORT -> AFR[MISO_GPIO_PIN_ALTERNATE_FUNCTION_REGISTERS_ARRAY_INDEX] &= ~MISO_GPIO_ALTERNATE_FUNCTIOM_SELECTOR_MASK;
    MISO_GPIO_PORT -> AFR[MISO_GPIO_PIN_ALTERNATE_FUNCTION_REGISTERS_ARRAY_INDEX] |= MISO_GPIO_ALTERNATE_FUNCTIOM_SELECTOR;

    /*  MOSI    */
    
    if ((RCC -> GPIO_PORT_BUS & MOSI_RCC_GPIO_PORT_BUS_ENABLE) != MOSI_RCC_GPIO_PORT_BUS_ENABLE)
        RCC -> GPIO_PORT_BUS |= MOSI_RCC_GPIO_PORT_BUS_ENABLE;
    
    MOSI_GPIO_PORT -> MODER &= ~MOSI_GPIO_MODER_MODE_MASK;
    MOSI_GPIO_PORT -> MODER |= MOSI_GPIO_MODER_MODE;
    
    MOSI_GPIO_PORT -> OSPEEDR &= ~MOSI_GPIO_OUTPUT_SPEED_MASK;
    MOSI_GPIO_PORT -> OSPEEDR |= MOSI_GPIO_OUTPUT_SPEED;
    
    MOSI_GPIO_PORT -> PUPDR &= ~MOSI_GPIO_PULL_UP_DOWN_MASK;
   // MOSI_GPIO_PORT -> PUPDR |= MOSI_GPIO_PULL_UP_DOWN;
    
    MOSI_GPIO_PORT -> AFR[MOSI_GPIO_PIN_ALTERNATE_FUNCTION_REGISTERS_ARRAY_INDEX] &= ~MOSI_GPIO_ALTERNATE_FUNCTIOM_SELECTOR_MASK;
    MOSI_GPIO_PORT -> AFR[MOSI_GPIO_PIN_ALTERNATE_FUNCTION_REGISTERS_ARRAY_INDEX] |= MOSI_GPIO_ALTERNATE_FUNCTIOM_SELECTOR;
    
    /*  NSS    */
    
    if ((RCC -> GPIO_PORT_BUS & NSS_RCC_GPIO_PORT_BUS_ENABLE) != NSS_RCC_GPIO_PORT_BUS_ENABLE)
        RCC -> GPIO_PORT_BUS |= NSS_RCC_GPIO_PORT_BUS_ENABLE;
    
    NSS_GPIO_PORT -> MODER &= ~NSS_GPIO_MODER_MODE_MASK;
    NSS_GPIO_PORT -> MODER |= NSS_GPIO_MODER_MODE;
    
    NSS_GPIO_PORT -> OSPEEDR &= ~NSS_GPIO_OUTPUT_SPEED_MASK;
    NSS_GPIO_PORT -> OSPEEDR |= NSS_GPIO_OUTPUT_SPEED;
    
    NSS_GPIO_PORT -> PUPDR &= ~NSS_GPIO_PULL_UP_DOWN_MASK;
   // NSS_GPIO_PORT -> PUPDR |= NSS_GPIO_PULL_UP_DOWN;
    
    /*
    https://controllerstech.com/spi-using-registers-in-stm32/
    
    */
}

void Transceive(uint8_t bitsNumber, uint32_t bytes)
{
    IssueSelectSlaveSignal(SlaveSelected);
    
    /*
    
    Bits 31:0 TXDR[31:0]: transmit data register
    The register serves as an interface with TxFIFO. A write to it accesses TxFIFO.
    Note: data is always right-aligned. Unused bits are ignored when writing to the register, and
    read as zero when the register is read.
    Note: DR can be accessed byte-wise (8-bit access): in this case only one data-byte is written
    by single access.
    halfword-wise (16 bit access) in this case 2 data-bytes or 1 halfword-data can be
    written by single access.
    word-wise (32 bit access). In this case 4 data-bytes or 2 halfword-data or word-data
    can be written by single access
    
    */
    
    while ((SPI -> SR & SPI_SR_TXP) == 0)
        ;
        
    if (bitsNumber > 15)
        *((__IO uint32_t *)&SPI -> TXDR) = bytes;
    else if (bitsNumber > 7)
        *((__IO uint16_t *)&SPI -> TXDR) = bytes;
    else
        *((__IO uint8_t *)&SPI -> TXDR) = bytes;
    
    /*
    
    https://community.st.com/t5/stm32-mcus-products/spi-not-sending/td-p/394883
    https://www.mikrocontroller.net/attachment/545366/H750_SPI1.c
http://m.eeworld.com.cn/bbs_thread-1118514-1-1.html
    */
    while ((SPI -> SR & SPI_SR_TXC) == 0)
        ;
    
    SPI -> IFCR = SPI_IFCR_EOTC | SPI_IFCR_TXTFC;
    
    IssueSelectSlaveSignal(SlaveUnselected);
}

void IssueSelectSlaveSignal(SlaveSelection selection)
{
    switch (selection)
    {
        case SlaveSelected:
            NSS_GPIO_PORT -> BSRR |= NSS_GPIO_BIT_RESET;
        break;
        
        case SlaveUnselected:
            NSS_GPIO_PORT -> BSRR |= NSS_GPIO_BIT_SET;
        break;
    }
}

void SPI_IRQ_HANDLER(void)
{
    uint8_t i = 0;
    
    i ++;
}