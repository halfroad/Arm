#include "./Communications/SerialPeripheralInterfaces/Include/SerialPeripheralInterfaces.h"

#include "../Include/Nrf24xx.h"

#ifdef STM32H563xx

/*  PD4 */
#define CHIP_ENABLE_GPIO_PORT                                           GPIOD
#define RCC_CHIP_ENABLE_GPIO_CLOCK_ENABLE()                             do                                  \
                                                                        {                                   \
                                                                            __HAL_RCC_GPIOD_CLK_ENABLE();   \
                                                                        }                                   \
                                                                        while   (0)

#define CHIP_ENABLE_GPIO_PIN                                            GPIO_PIN_4

/*  PB1 */
#define INTERRUPT_REQUEST_GPIO_PORT                                     GPIOB

#define RCC_INTERRUPT_REQUEST_GPIO_CLOCK_ENABLE()                       do                                  \
                                                                        {                                   \
                                                                            __HAL_RCC_GPIOB_CLK_ENABLE();   \
                                                                        }                                   \
                                                                        while   (0)

#define INTERRUPT_REQUEST_GPIO_PIN                                      GPIO_PIN_1
#define INTERRUPT_REQUEST_GPIO_IRQN                                     EXTI1_IRQn
#define INTERRUPT_REQUEST_GPIO_IRQHandler                               EXTI1_IRQHandler
                                                                        
#else
                                                                        
#ifdef STM32G474xx
                                                                        
/*  PD8 */
#define CHIP_ENABLE_GPIO_PORT                                           GPIOD
#define RCC_CHIP_ENABLE_GPIO_CLOCK_ENABLE()                             do                                  \
                                                                        {                                   \
                                                                            __HAL_RCC_GPIOD_CLK_ENABLE();   \
                                                                        }                                   \
                                                                        while   (0)

#define CHIP_ENABLE_GPIO_PIN                                            GPIO_PIN_8

/*  PD9 */
#define INTERRUPT_REQUEST_GPIO_PORT                                     GPIOD

#define RCC_INTERRUPT_REQUEST_GPIO_CLOCK_ENABLE()                       do                                  \
                                                                        {                                   \
                                                                            __HAL_RCC_GPIOD_CLK_ENABLE();   \
                                                                        }                                   \
                                                                        while   (0)

#define INTERRUPT_REQUEST_GPIO_PIN                                      GPIO_PIN_9
#define INTERRUPT_REQUEST_GPIO_IRQN                                     EXTI9_5_IRQn
#define INTERRUPT_REQUEST_GPIO_IRQHandler                               EXTI9_5_IRQHandler

#endif  /*  #ifdef STM32H563xx  */
#endif  /*  #ifdef STM32G474xx  */

/*

Table 20. Command set for the nRF24L01+ SPI
SPI Command Registers.

*/

/*

    Command name                Command word (binary)               # Data bytes                    Operation
    R_REGISTER                  000A AAAA                           1 to 5
                                                                    LSByte first                    Read command and status registers. AAAAA =
                                                                                                    5 bit Register Map Address.
                                                                                 

*/
#define READ_COMMAND_AND_STATUS_REGISTERS                               0x00

/*

    Command name                Command word (binary)               # Data bytes                    Operation
    W_REGISTER                  001A AAAA                           1 to 5 LSByte first
                                                                                                    Write command and status registers. AAAAA = 5
                                                                                                    bit Register Map Address
                                                                                                    Executable in power down or standby modes
                                                                                                    only.
*/
#define WRITE_COMMAND_AND_STATUS_REGISTERS                              0x20

/*

    Command name                Command word (binary)               # Data bytes                    Operation
    R_RX_PAYLOAD                0110 0001                           1 to 32
                                                                    LSByte first                    Read RX-payload: 1 ¨C 32 bytes. A read operation
                                                                                                    always starts at byte 0. Payload is deleted from
                                                                                                    FIFO after it is read. Used in RX mode.
*/
#define READ_RX_PAYLOAD                                                 0x61

/*

    Command name                Command word (binary)               # Data bytes                    Operation
    W_TX_PAYLOAD                1010 0000                           1 to 32
                                                                    LSByte first                    Write TX-payload: 1 ¨C 32 bytes. A write operation
                                                                                                    always starts at byte 0 used in TX payload.
*/
#define WRITE_TX_PAYLOAD                                                0xA0

/*

    Command name                Command word (binary)               # Data bytes                    Operation
    FLUSH_TX                    1110 0001                           0                               Flush TX FIFO, used in TX mode.
*/
#define FLUSH_TX_FIFO                                                   0xE1
/*

    Command name                Command word (binary)               # Data bytes                    Operation
    FLUSH_RX                    1110 0010                           0                               Flush RX FIFO, used in RX mode
                                                                                                    Should not be executed during transmission of
                                                                                                    acknowledge, that is, acknowledge package will
                                                                                                    not be completed.
*/
#define FLUSH_RX_FIFO                                                   0xE2
/*

    Command name                Command word (binary)               # Data bytes                    Operation
    REUSE_TX_PL                 1110 0011                           0                               Used for a PTX device
                                                                                                    Reuse last transmitted payload.
                                                                                                    TX payload reuse is active until
                                                                                                    W_TX_PAYLOAD or FLUSH TX is executed. TX
                                                                                                    payload reuse must not be activated or deacti-
                                                                                                    vated during package transmission.
*/
#define REUSE_LAST_RXTED_PAYLOAD                                        0xE3

/*

    Command name                Command word (binary)               # Data bytes                    Operation
    R_RX_PL_WID                 0110 0000                           1                               Read RX payload width for the top
                                                                                                    R_RX_PAYLOAD in the RX FIFO.
                                                                                                    Note: Flush RX FIFO if the read value is larger
                                                                                                    than 32 bytes.
*/
#define READ_RX_PAYLOAD_WIDTH_IN_RX_FIFO                                0x60

/*

    Command name                Command word (binary)               # Data bytes                    Operation
    W_ACK_PAYLOAD               1010 1PPP                           1 to 32
                                                                    LSByte first                    Used in RX mode.
                                                                                                    Write Payload to be transmitted together with
                                                                                                    ACK packet on PIPE PPP. (PPP valid in the
                                                                                                    range from 000 to 101). Maximum three ACK
                                                                                                    packet payloads can be pending. Payloads with
                                                                                                    same PPP are handled using first in - first out
                                                                                                    principle. Write payload: 1¨C 32 bytes. A write
                                                                                                    operation always starts at byte 0.
*/
#define WRITE_PAYLOAD_TO_RX_TOGETHER_WITH_ACK_PACKET_ON_PIPE      0xA8

/*

    Command name                Command word (binary)               # Data bytes                    Operation
    W_TX_PAYLOAD_NOACK          1011 0000                           1 to 32
                                                                    LSByte first                    Used in TX mode. Disables AUTOACK on this
                                                                                                    specific packet.
*/
#define DISABLES_AUTOACK_ON_SPECIFIC_PACKET                             0xB0

/*

    Command name                Command word (binary)               # Data bytes                    Operation
    NOP                         1111 1111                           0                               No Operation. Might be used to read the STATUS
                                                                                                    register.
*/
#define NOP                                                             0xFF

/*

nRF24L01+Single Chip 2.4GHz Transceiver Product Specification v1.0.PDF
Addresses 18 to 1B are reserved for test purposes, altering them makes the chip malfunction.

Table 28. Register map of nRF24L01+
                                                                        
*/

#define CONFIGURATION_REGISTER                                          0x00        /*  CONFIG      */

#define ENABLE_AUTO_ACKNOWLEDGMENT                                      0x01        /*  EN_AA       */
#define ENABLED_RX_ADDRESSES                                            0x02        /*  EN_RXADDR   */

#define SETUP_OF_ADDRESS_WIDTHS                                         0x03        /*  SETUP_AW    */
#define SETUP_OF_AUTOMATIC_RETRANSMISSION                               0x04        /*  SETUP_RETR  */

#define RF_CHANNEL                                                      0x05        /*  RF_CH       */
#define RF_SETUP_REGISTER                                               0x06        /*  RF_SETUP    */

#define STATUS_REGISTER                                                 0x07        /*  STATUS      */

#define RX_OBSERVE_REGISTER                                             0x08        /*  OBSERVE_TX  */

#define RXD_POWER_DETECTOR                                              0x09        /*  RPD         */

#define RX_ADDRESS_DATA_PIPE_0                                          0x0A        /*  RX_ADDR_P0  */
#define RX_ADDRESS_DATA_PIPE_1                                          0x0B        /*  RX_ADDR_P1  */
#define RX_ADDRESS_DATA_PIPE_2                                          0x0C        /*  RX_ADDR_P2  */
#define RX_ADDRESS_DATA_PIPE_3                                          0x0D        /*  RX_ADDR_P3  */
#define RX_ADDRESS_DATA_PIPE_4                                          0x0E        /*  RX_ADDR_P4  */
#define RX_ADDRESS_DATA_PIPE_5                                          0x0F        /*  RX_ADDR_P5  */

#define TX_ADDRESS                                                      0x10        /*  TX_ADDR  */

#define NUMBER_OF_BYTES_IN_RX_PAYLOAD_IN_DATA_PIPE_0                    0x11        /*  RX_PW_P0  */
#define NUMBER_OF_BYTES_IN_RX_PAYLOAD_IN_DATA_PIPE_1                    0x12        /*  RX_PW_P1  */
#define NUMBER_OF_BYTES_IN_RX_PAYLOAD_IN_DATA_PIPE_2                    0x13        /*  RX_PW_P2  */
#define NUMBER_OF_BYTES_IN_RX_PAYLOAD_IN_DATA_PIPE_3                    0x14        /*  RX_PW_P3  */
#define NUMBER_OF_BYTES_IN_RX_PAYLOAD_IN_DATA_PIPE_4                    0x15        /*  RX_PW_P4  */
#define NUMBER_OF_BYTES_IN_RX_PAYLOAD_IN_DATA_PIPE_5                    0x16        /*  RX_PW_P5  */
#define NUMBER_OF_BYTES_IN_RX_PAYLOAD_IN_DATA_PIPE_5                    0x16        /*  RX_PW_P5  */

#define FIFO_STATUS                                                     0x17        /*  FIFO Status Register.  */

#define ENABLE_DYNAMIC_PAYLOAD_LENGTH                                   0x1C        /*  DYNPD  */

#define FEATURE_REGISTER                                                0x1D        /*  FEATURE  */

#define TX_MAXIMUM_RETRILS                                              0x10
#define TX_OK                                                           0x20
#define RX_OK                                                           0x40

typedef enum
{
    CRCLength0,
    CRCLength1,
    
} CRCLengths;

typedef enum
{
    TxOutputPower_18dBm,
    TxOutputPower_12dBm ,
    TxOutputPower_6dBm  ,
    TxOutputPower0dBm   ,

} TxOutputPowers;

const uint8_t TRANSMITTER_ADDRESS[ADDRESS_WIDTH]                     = { 0x4C, 0xE6, 0xC0, 0x36, 0x01 };
const uint8_t RECEIVER_ADDRESS[ADDRESS_WIDTH]                        = { 0x4C, 0xE6, 0xC0, 0x36, 0x02 };

static void ChipEnable(ChipEnablements enablement);

void InitNrf24xx()
{
    GPIO_InitTypeDef GPIO_InitType          = { 0 };
    
    GPIO_InitType.Pin                       = CHIP_ENABLE_GPIO_PIN;
    GPIO_InitType.Mode                      = GPIO_MODE_OUTPUT_PP;
    GPIO_InitType.Pull                      = GPIO_PULLUP;
    
    RCC_CHIP_ENABLE_GPIO_CLOCK_ENABLE();
    HAL_GPIO_Init(CHIP_ENABLE_GPIO_PORT, &GPIO_InitType);
    
    GPIO_InitType.Pin                       = INTERRUPT_REQUEST_GPIO_PIN;
    GPIO_InitType.Mode                      = GPIO_MODE_IT_FALLING;
    GPIO_InitType.Pull                      = GPIO_NOPULL;
    
    RCC_INTERRUPT_REQUEST_GPIO_CLOCK_ENABLE();
    HAL_GPIO_Init(INTERRUPT_REQUEST_GPIO_PORT, &GPIO_InitType);
    
    HAL_NVIC_SetPriority(INTERRUPT_REQUEST_GPIO_IRQN, 1U, 0U);
    HAL_NVIC_EnableIRQ(INTERRUPT_REQUEST_GPIO_IRQN);
    
    InitSerialPeripheralInterfaces();
    
    SelectSlave(SlaveDeselectedSignal);
    ChipEnable(ChipEnabled);
}

inline static void ChipEnable(ChipEnablements enablement)
{
    switch (enablement)
    {
        case ChipEnabled:
            /*  Reset the CE pin to enable the chip.    */
            HAL_GPIO_WritePin(CHIP_ENABLE_GPIO_PORT, CHIP_ENABLE_GPIO_PIN, GPIO_PIN_SET);
        
        break;
        
        case ChipDisabled:
            /*  Reset the CE pin to enable the chip.    */
            HAL_GPIO_WritePin(CHIP_ENABLE_GPIO_PORT, CHIP_ENABLE_GPIO_PIN, GPIO_PIN_RESET);
        
        break;
    }
}

inline static uint8_t ReadRegister(uint8_t reg)
{
    uint8_t command = READ_COMMAND_AND_STATUS_REGISTERS | reg, status;
    
    SelectSlave(SlaveSelectedSignal);
    
    HAL_StatusTypeDef statusType = SpiTransceive(&command, &status, 1);
    
    if (HAL_OK == statusType)
    {        
        uint8_t value;
        
        statusType = SpiReceive(&value, 1);
        
        if (HAL_OK == statusType)
            return value;
    }
    
    SelectSlave(SlaveDeselectedSignal);
    
    return NOP;
}

inline static uint8_t WriteRegister(uint8_t reg, uint8_t value)
{
    uint8_t command = WRITE_COMMAND_AND_STATUS_REGISTERS | reg, status;
    
    SelectSlave(SlaveSelectedSignal);
    
    HAL_StatusTypeDef statusType = SpiTransceive(&reg, &status, 1);
    if (HAL_OK == statusType)
        statusType = SpiTransmit(&value, 1);
    
    SelectSlave(SlaveDeselectedSignal);
    
    return status;
}

inline static HAL_StatusTypeDef FlushRxFifo()
{
    uint8_t command = FLUSH_RX_FIFO, status;
    
    SelectSlave(SlaveSelectedSignal);
    
    HAL_StatusTypeDef statusType = SpiTransceive(&command, &status, 1);
    
    SelectSlave(SlaveDeselectedSignal);
    
    return statusType;
}

inline static HAL_StatusTypeDef FlushTxFifo()
{
    uint8_t command = FLUSH_TX_FIFO, status;
    
    SelectSlave(SlaveSelectedSignal);
    
    HAL_StatusTypeDef statusType = SpiTransceive(&command, &status, 1);
    
    SelectSlave(SlaveDeselectedSignal);
    
    return statusType;
}

inline static uint8_t CheckStatus(void)
{
    uint8_t command = NOP, status = NOP;
    
    SelectSlave(SlaveSelectedSignal);
    
    HAL_StatusTypeDef statusType = SpiTransceive(&command, &status, 1);
    
    if (HAL_OK != statusType)
        status = NOP;
    
    SelectSlave(SlaveDeselectedSignal);
    
    return status;
}

inline static uint8_t CheckFifoStatus(void)
{
    return ReadRegister(FIFO_STATUS);
}


inline static void AssignRxPayloadWidth(uint8_t width)
{
    WriteRegister(NUMBER_OF_BYTES_IN_RX_PAYLOAD_IN_DATA_PIPE_0, width);
}

inline static void ClearRxDataRegister(void)
{
    uint8_t status = CheckStatus();
    
    status |= 0x40;
    
    WriteRegister(STATUS_REGISTER, status);
}

inline static void ClearTxDataRegister(void)
{
    uint8_t status = CheckStatus();
    
    status |= 0x20;
    
    WriteRegister(STATUS_REGISTER, status);
}

inline static void ClearMaximumRetrialsRegister(void)
{
    uint8_t status = CheckStatus();
    
    status |= 0x10;
    
    WriteRegister(STATUS_REGISTER, status);
}

inline static void PowerUp(void)
{
    uint8_t configurationRegister = ReadRegister(CONFIGURATION_REGISTER);
    
    configurationRegister |= 0x01 << 1;
    
    WriteRegister(CONFIGURATION_REGISTER, configurationRegister);
}

inline static void PowerDown(void)
{
    uint8_t configurationRegister = ReadRegister(CONFIGURATION_REGISTER);
    
    configurationRegister &= 0xFD;
    
    WriteRegister(CONFIGURATION_REGISTER, configurationRegister);
}

inline static void ConfigureCRCLength(CRCLengths length)
{
    uint8_t configurationRegister = ReadRegister(CONFIGURATION_REGISTER);
    
    switch (length)
    {
        case CRCLength0:
            
            configurationRegister &= 0xFB;
        
            break;
        
        case CRCLength1:
            
            configurationRegister |= 0x01 << 2;
        
            break;
        
        default:
            break;
    }
    
    WriteRegister(CONFIGURATION_REGISTER, configurationRegister);
}

inline static void SetAddressWidth(uint8_t width)
{
    WriteRegister(SETUP_OF_ADDRESS_WIDTHS, width - 2);
}

inline static void SetAutoRetransmissionsNumber(uint8_t number)
{
    uint8_t automaticRetransmission = ReadRegister(SETUP_OF_AUTOMATIC_RETRANSMISSION);
    
    automaticRetransmission |= 0x0F;
    automaticRetransmission |= number;
    
    WriteRegister(SETUP_OF_AUTOMATIC_RETRANSMISSION, automaticRetransmission);
}

inline static void SetAutoRetransmissionsDelay(uint16_t microseconds)
{
    uint8_t automaticRetransmission = ReadRegister(SETUP_OF_AUTOMATIC_RETRANSMISSION);
    
    automaticRetransmission |= 0x0F;
    automaticRetransmission |= (microseconds / 250 - 1) << 4;
    
    WriteRegister(SETUP_OF_AUTOMATIC_RETRANSMISSION, automaticRetransmission);
}

inline static void SetRadioFrequencyChannel(uint16_t mhz)
{
    uint16_t channel = mhz - 2400;
    
    WriteRegister(RF_CHANNEL, channel);
}

inline static void SetTxOutputPower(TxOutputPowers power)
{
    uint8_t RFSetupRegister = ReadRegister(RF_SETUP_REGISTER) & 0xF9;
    
    RFSetupRegister |= power << 1;
    
    WriteRegister(RF_SETUP_REGISTER, RFSetupRegister);
}

inline static void SetRadioFrequencyDataRate(RadioFrequencyDataRates rate)
{
    uint8_t RFSetupRegister = ReadRegister(RF_SETUP_REGISTER) & 0xD7;
    
    switch (rate)
    {
        case RadioFrequencyDataRate250kpbs:
            
            RFSetupRegister |= 0x01 << 5;
        
            break;
        
        case RadioFrequencyDataRate1Mpbs:
            break;
        
        case RadioFrequencyDataRate2Mbps:
            
            RFSetupRegister |= 0x01 << 3;
        
            break;
        
        default:
            break;
    }
    
    WriteRegister(RF_SETUP_REGISTER, RFSetupRegister);
}

inline static HAL_StatusTypeDef ReadRxFifo(uint8_t *payload)
{
    uint8_t command = READ_RX_PAYLOAD, status;
    
    SelectSlave(SlaveSelectedSignal);
    
    HAL_StatusTypeDef statusType = SpiTransceive(&command, &status, 1);
    
    if (HAL_OK == statusType)
        statusType = SpiReceive(payload, PAYLOAD_WIDTH);
    
    SelectSlave(SlaveDeselectedSignal);
    
    return statusType;
}

inline static HAL_StatusTypeDef WriteTxFifo(uint8_t *payload)
{
    uint8_t command = WRITE_TX_PAYLOAD, status;
    
    SelectSlave(SlaveSelectedSignal);
    
    HAL_StatusTypeDef statusType = SpiTransceive(&command, &status, 1);
    
    if (HAL_OK == statusType)
        statusType = SpiTransmit(payload, PAYLOAD_WIDTH);

    SelectSlave(SlaveDeselectedSignal);
    
    return statusType;
}

inline static void Reset(void)
{
    SelectSlave(SlaveDeselectedSignal);
    ChipEnable(ChipDisabled);
    
    WriteRegister(CONFIGURATION_REGISTER, 0x08);
    WriteRegister(ENABLE_AUTO_ACKNOWLEDGMENT, 0x3F);
    WriteRegister(ENABLED_RX_ADDRESSES, 0x3F);
    WriteRegister(SETUP_OF_ADDRESS_WIDTHS, 0x3F);
    WriteRegister(SETUP_OF_AUTOMATIC_RETRANSMISSION, 0x3F);
    
    WriteRegister(RF_CHANNEL, 0x03);
    WriteRegister(RF_SETUP_REGISTER, 0x07);
    WriteRegister(STATUS_REGISTER, 0x7E);
    
    
    WriteRegister(NUMBER_OF_BYTES_IN_RX_PAYLOAD_IN_DATA_PIPE_0, 0x00);
    WriteRegister(NUMBER_OF_BYTES_IN_RX_PAYLOAD_IN_DATA_PIPE_1, 0x00);
    WriteRegister(NUMBER_OF_BYTES_IN_RX_PAYLOAD_IN_DATA_PIPE_2, 0x00);
    WriteRegister(NUMBER_OF_BYTES_IN_RX_PAYLOAD_IN_DATA_PIPE_3, 0x00);
    WriteRegister(NUMBER_OF_BYTES_IN_RX_PAYLOAD_IN_DATA_PIPE_4, 0x00);
    WriteRegister(NUMBER_OF_BYTES_IN_RX_PAYLOAD_IN_DATA_PIPE_5, 0x00);
    
    WriteRegister(FIFO_STATUS  , 0x11);
    
    WriteRegister(ENABLE_DYNAMIC_PAYLOAD_LENGTH, 0x00);
    WriteRegister(FEATURE_REGISTER, 0x00);
    
    FlushRxFifo();
    FlushTxFifo();
}

inline static HAL_StatusTypeDef SetRxAddress(void)
{
    uint8_t command = WRITE_COMMAND_AND_STATUS_REGISTERS | RX_ADDRESS_DATA_PIPE_0, status;
    
    SelectSlave(SlaveSelectedSignal);
    
    HAL_StatusTypeDef statusType = SpiTransceive(&command, &status, 1);
    
    if (HAL_OK == statusType)
        statusType = SpiTransmit((uint8_t *)RECEIVER_ADDRESS, ADDRESS_WIDTH);

    SelectSlave(SlaveDeselectedSignal);
    
    return statusType; 
}

inline static HAL_StatusTypeDef SetTxAddress(void)
{
    uint8_t command = WRITE_COMMAND_AND_STATUS_REGISTERS | TX_ADDRESS, status;
    
    SelectSlave(SlaveSelectedSignal);
    
    HAL_StatusTypeDef statusType = SpiTransceive(&command, &status, 1);
    
    if (HAL_OK == statusType)
        statusType = SpiTransmit((uint8_t *)TRANSMITTER_ADDRESS, ADDRESS_WIDTH);

    SelectSlave(SlaveDeselectedSignal);
    
    return statusType; 
}

inline static void SwitchToRxMode(void)
{
    uint8_t configurationRegister = ReadRegister(CONFIGURATION_REGISTER);
    
    configurationRegister|= 0x01 << 0;
    
    WriteRegister(CONFIGURATION_REGISTER, configurationRegister);
}

inline static void SwitchToTxMode(void)
{
    uint8_t configurationRegister = ReadRegister(CONFIGURATION_REGISTER);
    
    configurationRegister &= 0xFE;
    
    WriteRegister(CONFIGURATION_REGISTER, configurationRegister);
}

HAL_StatusTypeDef Nrf24xxTransmit(uint8_t *payload)
{
    return WriteTxFifo(payload);
}

HAL_StatusTypeDef Nrf24xxReceive(uint8_t *payload)
{
    HAL_StatusTypeDef statusType = ReadRxFifo(payload);
    
    ClearRxDataRegister();
    
    return statusType;
}

void Nrf24xxRxInit(uint16_t channel, RadioFrequencyDataRates rate)
{
    Reset();
    
    InitNrf24xx();
    
    SwitchToRxMode();
    
    SetRxAddress();
    
    PowerUp();
    
    AssignRxPayloadWidth(PAYLOAD_WIDTH);
    
    SetRadioFrequencyChannel(channel);
    SetRadioFrequencyDataRate(rate);
    SetTxOutputPower(TxOutputPower0dBm);
    
    ConfigureCRCLength(CRCLength0);
    
    SetAddressWidth(ADDRESS_WIDTH);
    
    SetAutoRetransmissionsNumber(TX_MAXIMUM_RETRILS);
    SetAutoRetransmissionsDelay(250);
    
    ChipEnable(ChipEnabled);
}

void Nrf24xxTxInit(uint16_t channel, RadioFrequencyDataRates rate)
{
    InitNrf24xx();
    
    Reset();
    
    SwitchToTxMode();
    
    SetTxAddress();
    SetRxAddress();
    
    PowerUp();

    SetRadioFrequencyChannel(channel);
    SetRadioFrequencyDataRate(rate);
    SetTxOutputPower(TxOutputPower0dBm);
    
    ConfigureCRCLength(CRCLength0);
    
    SetAddressWidth(ADDRESS_WIDTH);
    
    SetAutoRetransmissionsNumber(TX_MAXIMUM_RETRILS);
    SetAutoRetransmissionsDelay(250);
    
    ChipEnable(ChipEnabled);
}

void INTERRUPT_REQUEST_GPIO_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(INTERRUPT_REQUEST_GPIO_PIN);
}