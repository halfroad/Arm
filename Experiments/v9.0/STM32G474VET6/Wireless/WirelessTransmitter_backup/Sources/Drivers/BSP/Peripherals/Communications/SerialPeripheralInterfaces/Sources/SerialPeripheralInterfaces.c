#include "../Include/SerialPeripheralInterfaces.h"

#define SPI                                                     SPI1
#define RCC_SPI_CLOCK_ENABLE()                                  do                                  \
                                                                {                                   \
                                                                    __HAL_RCC_SPI1_CLK_ENABLE();    \
                                                                }                                   \
                                                                while   (0)

#define MODE                                                    SPI_MODE_MASTER
#define DIRECTION                                               SPI_DIRECTION_2LINES
#define DATASIZE                                                SPI_DATASIZE_8BIT
#define POLARITY                                                SPI_POLARITY_LOW
#define PHASE                                                   SPI_PHASE_1EDGE
#define BAUD_RATE_PRESCALER                                     SPI_BAUDRATEPRESCALER_16
#define FIRSTBIT                                                SPI_FIRSTBIT_MSB
#define SLAVE_SELECT                                            SPI_NSS_SOFT

/*

#define RCC_PERIPH_CLOCK                                        RCC_PERIPHCLK_SPI1
#define SPI_CLOCK_SELECTION                                     Spi1ClockSelection
#define SPI_CLOCK_SOURCE                                        RCC_SPI1CLKSOURCE_PLL1Q

*/
                                                                
                                                                https://github.com/mokhwasomssi/stm32_hal_nrf24l01p/blob/main/nrf24l01p.c

/*

*/

/*  CS: PD10 */
#define SPI_NSS_GPIO_PORT                                       GPIOD
#define RCC_SPI_NSS_GPIO_PORT_ENABLE()                          do                                  \
                                                                {                                   \
                                                                    __HAL_RCC_GPIOD_CLK_ENABLE();   \
                                                                }                                   \
                                                                while   (0)

#define SPI_NSS_GPIO_PIN                                        GPIO_PIN_10
/*
    #define SPI_NSS_GPIO_PIN_ALTERNATE_FUNCTION                     GPIO_AF5_SPI1
*/

/*  MISO: PB4 */
#define SPI_MISO_GPIO_PORT                                      GPIOB
#define RCC_SPI_MISO_GPIO_PORT_ENABLE()                         do                                  \
                                                                {                                   \
                                                                    __HAL_RCC_GPIOB_CLK_ENABLE();   \
                                                                }                                   \
                                                                while   (0)

#define SPI_MISO_GPIO_PIN                                       GPIO_PIN_4
#define SPI_MISO_GPIO_PIN_ALTERNATE_FUNCTION                    GPIO_AF5_SPI1

/*  MOSI: PB5 */
#define SPI_MOSI_GPIO_PORT                                      GPIOB
#define RCC_SPI_MOSI_GPIO_PORT_ENABLE()                         do                                  \
                                                                {                                   \
                                                                    __HAL_RCC_GPIOB_CLK_ENABLE();   \
                                                                }                                   \
                                                                while   (0)

#define SPI_MOSI_GPIO_PIN                                       GPIO_PIN_5
#define SPI_MOSI_GPIO_PIN_ALTERNATE_FUNCTION                    GPIO_AF5_SPI1

/*  SCK: PB3 */
#define SPI_SCK_GPIO_PORT                                       GPIOB
#define RCC_SPI_SCK_GPIO_PORT_ENABLE()                          do                                  \
                                                                {                                   \
                                                                    __HAL_RCC_GPIOB_CLK_ENABLE();   \
                                                                }                                   \
                                                                while   (0)

#define SPI_SCK_GPIO_PIN                                        GPIO_PIN_3
#define SPI_SCK_GPIO_PIN_ALTERNATE_FUNCTION                     GPIO_AF5_SPI1
                                                                
#define BYTE_IF_EMPTY                                           0xFF
/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
extern void Error_Handler(void);

static void MspInitCallback(SPI_HandleTypeDef *hspi);

SPI_HandleTypeDef SPI_HandleType                    = { 0 };

void InitSerialPeripheralInterfaces(void)
{
    SPI_HandleType.Instance                         = SPI;
    
    SPI_HandleType.Init.Mode                        = MODE;
    SPI_HandleType.Init.Direction                   = DIRECTION;
    SPI_HandleType.Init.DataSize                    = DATASIZE;
    SPI_HandleType.Init.CLKPolarity                 = POLARITY;
    SPI_HandleType.Init.CLKPhase                    = PHASE;
    SPI_HandleType.Init.NSS                         = SLAVE_SELECT;
    SPI_HandleType.Init.BaudRatePrescaler           = BAUD_RATE_PRESCALER;
    SPI_HandleType.Init.FirstBit                    = FIRSTBIT;
    SPI_HandleType.Init.TIMode                      = SPI_TIMODE_DISABLE;
    SPI_HandleType.Init.CRCCalculation              = SPI_CRCCALCULATION_DISABLE;
    SPI_HandleType.Init.CRCPolynomial               = 0x07;
    SPI_HandleType.Init.CRCLength                   = SPI_CRC_LENGTH_DATASIZE;
    SPI_HandleType.Init.NSSPMode                    = SPI_NSS_PULSE_DISABLE;
    
    /*
    SPI_HandleType.Init.MasterKeepIOState           = SPI_MASTER_KEEP_IO_STATE_ENABLE;
    */
    
    SPI_HandleType.MspInitCallback                  = MspInitCallback;
    
    if (HAL_OK != HAL_SPI_Init(&SPI_HandleType))
        Error_Handler();
}

static void MspInitCallback(SPI_HandleTypeDef *hspi)
{
    if (SPI == hspi -> Instance)
    {
        RCC_SPI_CLOCK_ENABLE();
        
        /*
        
        RCC_PeriphCLKInitTypeDef RCC_PeriphCLKInitType  = { 0 };
        
        RCC_PeriphCLKInitType.PeriphClockSelection      = RCC_PERIPH_CLOCK;
        RCC_PeriphCLKInitType.SPI_CLOCK_SELECTION       = SPI_CLOCK_SOURCE;

        HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitType);
        
        */
        
        GPIO_InitTypeDef GPIO_InitType                  = { 0 };
        
        GPIO_InitType.Pin                               = SPI_NSS_GPIO_PIN;
        GPIO_InitType.Mode                              = GPIO_MODE_OUTPUT_PP;
        GPIO_InitType.Speed                             = GPIO_SPEED_FREQ_VERY_HIGH;
        /*
            GPIO_InitType.Alternate                         = SPI_NSS_GPIO_PIN_ALTERNATE_FUNCTION;
        */
        
        RCC_SPI_NSS_GPIO_PORT_ENABLE();
        HAL_GPIO_Init(SPI_NSS_GPIO_PORT, &GPIO_InitType);
        
        GPIO_InitType.Pin                               = SPI_MISO_GPIO_PIN;
        GPIO_InitType.Mode                              = GPIO_MODE_AF_PP;
        GPIO_InitType.Alternate                         = SPI_MISO_GPIO_PIN_ALTERNATE_FUNCTION;
        
        RCC_SPI_MISO_GPIO_PORT_ENABLE();
        HAL_GPIO_Init(SPI_MISO_GPIO_PORT, &GPIO_InitType);
        
        GPIO_InitType.Pin                               = SPI_MOSI_GPIO_PIN;
        GPIO_InitType.Alternate                         = SPI_MOSI_GPIO_PIN_ALTERNATE_FUNCTION;
        
        RCC_SPI_MOSI_GPIO_PORT_ENABLE();
        HAL_GPIO_Init(SPI_MOSI_GPIO_PORT, &GPIO_InitType);
        
        GPIO_InitType.Pin                               = SPI_SCK_GPIO_PIN;
        GPIO_InitType.Alternate                         = SPI_SCK_GPIO_PIN_ALTERNATE_FUNCTION;
        
        RCC_SPI_SCK_GPIO_PORT_ENABLE();
        HAL_GPIO_Init(SPI_SCK_GPIO_PORT, &GPIO_InitType);
    }
}

void SelectSlave(SlaveSelectionSignals signal)
{
    switch (signal)
    {
        case SlaveSelectedSignal:
            
            HAL_GPIO_WritePin(SPI_NSS_GPIO_PORT, SPI_NSS_GPIO_PIN, GPIO_PIN_RESET);
        
            break;
        
        case SlaveDeselectedSignal:
            
            HAL_GPIO_WritePin(SPI_NSS_GPIO_PORT, SPI_NSS_GPIO_PIN, GPIO_PIN_SET);
        
            break;
        
        default:
            break;
    }
}

HAL_StatusTypeDef SpiTransmit(uint8_t *bytes, uint8_t size)
{
    return HAL_SPI_Transmit(&SPI_HandleType, bytes, size, HAL_MAX_DELAY & 0x000000FFU);
}

HAL_StatusTypeDef SpiReceive(uint8_t *bytes,  uint8_t size)
{
    return HAL_SPI_Receive(&SPI_HandleType, bytes, size, HAL_MAX_DELAY & 0x000000FFU);
}

HAL_StatusTypeDef SpiTransceive(uint8_t *transmitBytes, uint8_t *receivedBytes,  uint8_t size)
{
    return HAL_SPI_TransmitReceive(&SPI_HandleType, transmitBytes, receivedBytes, size, HAL_MAX_DELAY & 0x000000FFU);
}

