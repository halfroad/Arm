#include "../Include/DigitalAnalogyConverter.h"

#define DAC                                                     DAC1
#define DAC_CHANNEL                                             DAC_CHANNEL_1

#define RCC_ADC_CLOCK_ENABLE()                                  do                                      \
                                                                {                                       \
                                                                    __HAL_RCC_DAC1_CLK_ENABLE();        \
                                                                }                                       \
                                                                while   (0)                             \
                                                                
#define GPIO_PORT_ADDITIONAL_FUNCTIONS_DAC                      GPIOA
#define GPIO_PIN_ADDITIONAL_FUNCTIONS_DAC                       GPIO_PIN_4
#define RCC_GPIO_PORT_ADDITIONAL_FUNCTIONS_DAC_CLOCK_ENABLE()   do                                      \
                                                                {                                       \
                                                                    __HAL_RCC_GPIOA_CLK_ENABLE();       \
                                                                }                                       \
                                                                while   (0)                             \

#define DAC_ALIGN                                               DAC_ALIGN_12B_R

extern void Error_Handler(void);

void ConfigureDigitalAnalogyConverterChannel(uint32_t channel);
static void MspInitCallback (DAC_HandleTypeDef *hdac);
static void ChannelConversionCompletionCallback (DAC_HandleTypeDef *hdac);

DAC_HandleTypeDef DAC_HandleType                = { 0 };


void InitDigitalAnalogyConverter(void)
{
    DAC_HandleType.Instance                     = DAC;
    DAC_HandleType.MspInitCallback              = MspInitCallback;
    
    DAC_HandleType.ConvCpltCallbackCh1          = ChannelConversionCompletionCallback;
    
    if (HAL_OK == HAL_DAC_Init(&DAC_HandleType))
        ConfigureDigitalAnalogyConverterChannel(DAC_CHANNEL);
    else
        Error_Handler();
}

void ConfigureDigitalAnalogyConverterChannel(uint32_t channel)
{
    DAC_ChannelConfTypeDef DAC_ChannelConfType  = { 0 };
    
    DAC_ChannelConfType.DAC_Trigger             = DAC_TRIGGER_NONE;
    DAC_ChannelConfType.DAC_OutputBuffer        = DAC_OUTPUTBUFFER_ENABLE;
    
    HAL_DAC_ConfigChannel(&DAC_HandleType, &DAC_ChannelConfType, channel);
    HAL_DAC_Start(&DAC_HandleType, DAC_CHANNEL);
}

static void MspInitCallback (DAC_HandleTypeDef *hdac)
{
    /*
    
    STMicroelectronics/MCUs/STM32H563ZI/Product Specifications/stm32h563zi.pdf
    Table 14. STM32H562xx and STM32H563xx pin/ball definition (continued)
    
    Pin name (function after reset)                             Additional functions
    
    PA4                                                         DAC1_OUT1
    PA5                                                         DAC1_OUT2
    
    */
    
    GPIO_InitTypeDef GPIO_InitType          = { 0 };
    
    GPIO_InitType.Pin                       = GPIO_PIN_ADDITIONAL_FUNCTIONS_DAC;
    GPIO_InitType.Mode                      = GPIO_MODE_ANALOG;
    
    RCC_ADC_CLOCK_ENABLE();
    HAL_GPIO_Init(GPIO_PORT_ADDITIONAL_FUNCTIONS_DAC, &GPIO_InitType);
    
    RCC_ADC_CLOCK_ENABLE();
}

static void ChannelConversionCompletionCallback (DAC_HandleTypeDef *hdac)
{
    
}

void AssignOutputVoltage(uint16_t voltage)
{
    double quantity = voltage;
    
    quantity /= 1000;
    quantity *= (4096 / 3.3);
    
    if (HAL_OK != HAL_DAC_SetValue(&DAC_HandleType, DAC_CHANNEL, DAC_ALIGN, quantity))
        Error_Handler();
}

uint16_t AcquireAssignedVoltage(void)
{
    return HAL_DAC_GetValue(&DAC_HandleType, DAC_CHANNEL);
}
