#include <stm32h7xx.h>

#include "../Include/InterIntegratedCircuitSound.h"

#define I2S                                     SPI1


I2S_HandleTypeDef I2S_HandleType                = { 0 };

static void MspInitCallback(I2S_HandleTypeDef *hi2s);

void InitIInterIntegratedCircuitSound(uint32_t dataFormat, uint32_t audioFreq, uint32_t clockPolarity, uint32_t firstBit)
{
    I2S_HandleType.Instance                     = I2S;
    
    I2S_HandleType.Init.Mode                    = I2S_MODE_MASTER_FULLDUPLEX;
    I2S_HandleType.Init.Standard                = I2S_STANDARD_PHILIPS;
    I2S_HandleType.Init.DataFormat              = dataFormat;
    I2S_HandleType.Init.MCLKOutput              = I2S_MCLKOUTPUT_DISABLE;
    I2S_HandleType.Init.AudioFreq               = audioFreq;
    I2S_HandleType.Init.CPOL                    = clockPolarity;
    I2S_HandleType.Init.FirstBit                = firstBit;
    I2S_HandleType.Init.WSInversion             = I2S_WS_INVERSION_DISABLE;
    I2S_HandleType.Init.Data24BitAlignment      = I2S_DATA_24BIT_ALIGNMENT_RIGHT;
    I2S_HandleType.Init.MasterKeepIOState       = I2S_MASTER_KEEP_IO_STATE_DISABLE;
    
    I2S_HandleType.MspInitCallback              = MspInitCallback;
    
    HAL_I2S_Init(&I2S_HandleType);
}

void InitIDefaultInterIntegratedCircuitSound(void)
{
    InitIInterIntegratedCircuitSound(I2S_DATAFORMAT_16B, I2S_AUDIOFREQ_192K, I2S_CPOL_LOW, I2S_FIRSTBIT_MSB);
}

static void MspInitCallback(I2S_HandleTypeDef *hi2s)
{
    if (I2S == hi2s -> Instance)
    {
        /*
        
        /STM32H7xx_DFP/3.1.1/Documentation/DS12919.pdf
        
        Table 7. STM32H755xI pin/ball definition (continued)
        
        Pin name                                Alternate functions
        
        PA4                                     SPI1_NSS/I2S1_WS,
        PA5                                     SPI1_SCK/I2S1_CK,
        PA6                                     SPI1_MISO/I2S1_SDI,
        PA7                                     SPI1_MOSI/I2S1_SDO,
        
        PA15(JTDI)                              SPI1_NSS/I2S1_WS,
        PB3(JTDO/TRACESWO)                      SPI1_SCK/I2S1_CK,
        PB4(NJTRST)                             SPI1_MISO/I2S1_SDI,
        PB5                                     SPI1_MOSI/I2S1_SDO,
        PC4                                     I2S1_MCK,
        
        PG10                                    SPI1_NSS/I2S1_WS,
        PG11                                    SPI1_SCK/I2S1_CK,
        PD7                                     SPI1_MOSI/I2S1_SDO,
        PG9                                     SPI1_MISO/I2S1_SDI,
        
        Table 8. Port A alternate functions
        
        Port                                    AF5
        
        PA4                                     SPI1_NSS/I2S1_WS
        PA5                                     SPI1_SCK/I2S1_CK
        PA6                                     SPI1_MISO/I2S1_SDI
        PA7                                     SPI1_MOSI/I2S1_SDO
        
        PA15                                    SPI1_NSS/I2S1_WS
        PB3                                     SPI1_SCK/I2S1_CK
        PB4                                     SPI1_MISO/I2S1_SDI
        PB5                                     SPI1_MOSI/I2S1_SDO
        
        PC4                                     I2S1_MCK
        PD7                                     SPI1_MOSI/I2S1_SDO
        PD8                                     SPI1_MISO/I2S1_SDI
        
        PG10                                    SPI1_NSS/I2S1_WS
        PG11                                    SPI1_SCK/I2S1_CK
        
                
        Table 7. STM32H755xI pin/ball definition (continued)
        
        PB6                                     I2C1_SCL
        PB7                                     I2C1_SDA
        
        PB8                                     I2C1_SCL
        PB9                                     I2C1_SDA
        
        Table 9. Port B alternate functions
        
        Port                                    AF4
        
        PB6                                     I2C1_SCL
        PB7                                     I2C1_SDA
        
        PB8                                     I2C1_SCL
        PB9                                     I2C1_SDA
        
        WM8978_LRCK     <------------------->   SPI_NSS/I2S_WS:     PA4
        WM8978_SCLK     <------------------->   SPI_SCK/I2S_CK:     PA5
        WM8978_SDOUT    <------------------->   SPI_MISO/I2Sext_SD: PA6
        WM8978_SDIN     <------------------->   SPI_MOSI/I2S_SD:    PA7
        
        WM8978_MCLK     <------------------->   I2S_MCLK:           PC4
        
        WM8978_SCL      <------------------->   I2C_SCL:            PA2
        WM8978_SDA      <------------------->   I2C_SDA:            PA3
        
        */
        
        
    }
}