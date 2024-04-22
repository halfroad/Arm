#include "../Include/SerialPeripheralInterfaces.h"

#define SPI                                                     SPI1
#define RCC_SPI_CLOCK_ENABLE()                                  __HAL_RCC_SPI1_CLK_ENABLE();
#define SPI_MODE                                                SPI_MODE_MASTER

/*  GPIOs   */

/*  Slave Select   */
#define SS_GPIO_PORT                                            GPIOB
#define RCC_SS_GPIO_PORT_CLOCK_ENABLE()                         do                                      \
                                                                {                                       \
                                                                    __HAL_RCC_GPIOB_CLK_ENABLE();       \
                                                                }                                       \
                                                                while(0)
#define SS_GPIO_PIN                                             GPIO_PIN_8

/*  Clock   */
#define CLK_GPIO_PORT                                           GPIOB
#define RCC_CLK_GPIO_PORT_CLOCK_ENABLE()                        do                                      \
                                                                {                                       \
                                                                    __HAL_RCC_GPIOB_CLK_ENABLE();       \
                                                                }                                       \
                                                                while(0)
#define CLK_GPIO_PIN                                            GPIO_PIN_3
#define CLK_GPIO_PIN_ALTERNATE_FUNCTION                         GPIO_AF5_SPI1

/*  Master Output, Slave Input   */
#define MOSI_GPIO_PORT                                          GPIOB
#define RCC_MOSI_GPIO_PORT_CLOCK_ENABLE()                       do                                      \
                                                                {                                       \
                                                                    __HAL_RCC_GPIOB_CLK_ENABLE();       \
                                                                }                                       \
                                                                while(0)
#define MOSI_GPIO_PIN                                           GPIO_PIN_4
#define MOSI_GPIO_PIN_ALTERNATE_FUNCTION                        GPIO_AF5_SPI1

/*  Master Input, Slave Output   */
#define MISO_GPIO_PORT                                          GPIOB
#define RCC_MISO_GPIO_PORT_CLOCK_ENABLE()                       do                                      \
                                                                {                                       \
                                                                    __HAL_RCC_GPIOB_CLK_ENABLE();       \
                                                                }                                       \
                                                                while(0)
#define MISO_GPIO_PIN                                           GPIO_PIN_5
#define MISO_GPIO_PIN_ALTERNATE_FUNCTION                        GPIO_AF5_SPI1

SPI_HandleTypeDef SPI_HandleType;

void MspInitCallback(SPI_HandleTypeDef *hspi);
uint8_t Transceive(uint8_t transmitByte);

void InitSerialPeripheralInterface(void)
{
    SPI_HandleType.Instance                 = SPI;
    
    SPI_HandleType.Init.Mode                = SPI_MODE;
    SPI_HandleType.Init.Direction           = SPI_DIRECTION_2LINES;
    SPI_HandleType.Init.DataSize            = SPI_DATASIZE_8BIT;
    SPI_HandleType.Init.CLKPolarity         = SPI_POLARITY_HIGH;
    SPI_HandleType.Init.CLKPhase            = SPI_PHASE_2EDGE;
    SPI_HandleType.Init.NSS                 = SPI_NSS_SOFT;
    SPI_HandleType.Init.BaudRatePrescaler   = SPI_BAUDRATEPRESCALER_16;
    SPI_HandleType.Init.FirstBit            = SPI_FIRSTBIT_MSB;
    SPI_HandleType.Init.TIMode              = SPI_TIMODE_DISABLE;
    SPI_HandleType.Init.CRCCalculation      = SPI_CRCCALCULATION_DISABLE;
    
    SPI_HandleType.MspInitCallback          = MspInitCallback;
    
    HAL_SPI_Init(&SPI_HandleType);
    
    __HAL_SPI_ENABLE(&SPI_HandleType);
}

void MspInitCallback(SPI_HandleTypeDef *hspi)
{
    if (SPI == hspi -> Instance)
    {
        GPIO_InitTypeDef GPIO_InitType      = { 0 };
        
        GPIO_InitType.Pin                   = SS_GPIO_PIN;
        GPIO_InitType.Mode                  = GPIO_MODE_OUTPUT_PP;
        GPIO_InitType.Pull                  = GPIO_PULLUP;
        GPIO_InitType.Speed                 = GPIO_SPEED_HIGH;
        
        RCC_SS_GPIO_PORT_CLOCK_ENABLE();
        HAL_GPIO_Init(SS_GPIO_PORT, &GPIO_InitType);
        
        GPIO_InitType.Pin                   = CLK_GPIO_PIN;
        GPIO_InitType.Mode                  = GPIO_MODE_AF_PP;
        GPIO_InitType.Alternate             = CLK_GPIO_PIN_ALTERNATE_FUNCTION;
        
        RCC_CLK_GPIO_PORT_CLOCK_ENABLE();
        HAL_GPIO_Init(CLK_GPIO_PORT, &GPIO_InitType);
        
                
        GPIO_InitType.Pin                   = MOSI_GPIO_PIN;
        GPIO_InitType.Alternate             = MOSI_GPIO_PIN_ALTERNATE_FUNCTION;
        
        RCC_MOSI_GPIO_PORT_CLOCK_ENABLE();
        HAL_GPIO_Init(MOSI_GPIO_PORT, &GPIO_InitType);
        
        GPIO_InitType.Pin                   = MISO_GPIO_PIN;
        GPIO_InitType.Alternate             = MISO_GPIO_PIN_ALTERNATE_FUNCTION;
        
        RCC_MISO_GPIO_PORT_CLOCK_ENABLE();
        HAL_GPIO_Init(MISO_GPIO_PORT, &GPIO_InitType);
        
        RCC_SPI_CLOCK_ENABLE();
    }
}


void ConfigureBaudRate(uint8_t prescaler)
{
    /*
    
    #define SPI_CR1_BR_0                (0x1UL << SPI_CR1_BR_Pos)
    #define SPI_CR1_BR_1                (0x2UL << SPI_CR1_BR_Pos)
    #define SPI_CR1_BR_2                (0x4UL << SPI_CR1_BR_Pos)

    */
    assert_param(IS_BAUDRATE_PRESCALER(prescaler));
    
    __HAL_SPI_DISABLE(&SPI_HandleType);
    
    SPI_HandleType.Instance -> CR1 &= ~SPI_CR1_BR_Msk;
    SPI_HandleType.Instance -> CR1 |= prescaler;
}

void SelectSlave(SlaveSelection selection)
{
    switch (selection)
    {
        case SlaveNotSelected:
            HAL_GPIO_WritePin(SS_GPIO_PORT, SS_GPIO_PIN, GPIO_PIN_SET);
        break;
        
        case SlaveSelected:
            HAL_GPIO_WritePin(SS_GPIO_PORT, SS_GPIO_PIN, GPIO_PIN_RESET);
        break;
    }
}

uint8_t Transceive(uint8_t transmitByte)
{
    uint8_t receiveByte;
    
    HAL_SPI_TransmitReceive(&SPI_HandleType, &transmitByte, &receiveByte, 1, HAL_MAX_DELAY);
    
    return receiveByte;
}

void Transmit(uint8_t *bytes, uint16_t size)
{
    HAL_SPI_Transmit(&SPI_HandleType, bytes, size, HAL_MAX_DELAY);
}
