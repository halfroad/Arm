#include <math.h>

#include "../Include/CurrentVoltageTemperatureGatherer.h"

    /*
    
    /MCUs/STM32G474VET6/Reference Manuals/rm0440-stm32g4-series-advanced-armbased-32bit-mcus-stmicroelectronics.pdf
    Table 72. Interconnect 24
    
    PIN     Alternate Functions     Drive Board Functions   Independent     Description
    
    PB1     ADC3_IN1                PM2_VBUS                No              VBus Voltage sensor on PM2 Connector
    PE7     ADC3_IN4                PM2_AMPU Y              Yes             Phase U Current sensor on PM2 Connector
    PE9     ADC3_IN2                PM2_VTEMP               Yes             Motor Drive Board Temperature sensor on PM2 Connector
    
    */
    
#define DMAMUX_ENABLE()                                                         do  {    __HAL_RCC_DMAMUX1_CLK_ENABLE();    }   while (0)
#define DMA_CLOCK_ENABLE()                                                      do  {    __HAL_RCC_DMA1_CLK_ENABLE();    } while (0)
/*  /MCUs/STM32G474VET6/Reference Manuals/rm0440-stm32g4-series-advanced-armbased-32bit-mcus-stmicroelectronics.pdf 
13.3.2 DMAMUX mapping, Table 91. DMAMUX: assignment of multiplexer inputs to resources.    */
#define ADC_DMA_CHANNEL                                                         DMA1_Channel1
#define ADC_DMA_REQUEST                                                         DMA_REQUEST_ADC3
#define ADC_DMA_CHANNEL_IRQN                                                    DMA1_Channel1_IRQn
#define ADC_DMA_CHANNEL_IRQHANDLER                                              DMA1_Channel1_IRQHandler

#define SENSORS_ADC                                                             ADC3
#define SENSORS_ADC_RCC_CLOCK_ENABLE()                                          do  {   __HAL_RCC_ADC345_CLK_ENABLE();  }   while(0)

#define VBUS_VOLTAGE_SENSOR_GPIO_PORT                                           GPIOB
#define VBUS_VOLTAGE_SENSOR_GPIO_RCC_CLOCK_ENABLE()                             do  {   __HAL_RCC_GPIOB_CLK_ENABLE();  }   while(0)
#define VBUS_VOLTAGE_SENSOR_GPIO_PIN                                            GPIO_PIN_1
#define VBUS_VOLTAGE_SENSOR_ADC_CHANNEL                                         ADC_CHANNEL_1

#define PHASE_U_CURRENT_TEMPERATURE_SENSORS_GPIO_PORT                           GPIOE
#define PHASE_U_CURRENT_TEMPERATURE_SENSORS_GPIO_RCC_CLOCK_ENABLE()             do  {   __HAL_RCC_GPIOE_CLK_ENABLE();  }   while(0)
#define PHASE_U_CURRENT_SENSOR_GPIO_PIN                                         GPIO_PIN_7
#define PHASE_U_CURREN_SENSOR_ADC_CHANNEL                                       ADC_CHANNEL_4

#define TEMPERATURE_SENSOR_GPIO_PIN                                             GPIO_PIN_9
#define TEMPERATURE_SENSOR_ADC_CHANNEL                                          ADC_CHANNEL_2

#define ADC_CHANNELS_NUMBER                                                     3
#define ADC_GATHER_TIMES_PER_CHANNEL                                            1000

#define ADC_TO_VOLTAGE                                                          (float)(3.3f * 25 / 4096)

/*
    I = £¨ADC_Now - Initial_ADC£©* £¨3.3 / 4.096 / 0.12£©
 */
#define ADC_TO_CURRENT                                                          (float)(3.3f / 4.096 / 0.12f)
    
DMA_HandleTypeDef DMA_HandleType = { 0 };
ADC_HandleTypeDef ADC_HandleType = { 0 };

uint16_t gatheredConversions[ADC_CHANNELS_NUMBER * ADC_GATHER_TIMES_PER_CHANNEL] = { 0 };

const float Rp = 10000.0f;
const float Ka = 273.15f;
const float T2 = 273.15f + 25.0f;
const float Bx = 3380.0f;

extern void Error_Handler(void);

static void (* conversionCompletedCallback)(float voltage, float current, float temperature);
    
static void InitAnalogyDigitalConverter(void);
static void InitDirectMemoryAccess(void);
    
static void MspInitCallback(ADC_HandleTypeDef *hadc);
static void ConversionsCompletionCallback(ADC_HandleTypeDef *hadc);

static void ConfigureAnalogyDigitalConverterChannel(uint32_t channel, uint32_t rank, uint32_t samplingTime);
static void ComputeAnalogyDigitalConversions(void);
static float AcquireTemperature(uint16_t conversion);

void InitCurrentVoltageTemperatureGatherer(void (*onConversionCompletionHandler)(float voltage, float current, float temperature))
{
    InitDirectMemoryAccess();
    InitAnalogyDigitalConverter();
    
    if (onConversionCompletionHandler)
        conversionCompletedCallback = onConversionCompletionHandler;
}

static void InitDirectMemoryAccess(void)
{
    DMAMUX_ENABLE();
    DMA_CLOCK_ENABLE();
    
    DMA_HandleType.Instance = ADC_DMA_CHANNEL;
    
    DMA_HandleType.Init.Request = ADC_DMA_REQUEST;
    DMA_HandleType.Init.Direction = DMA_PERIPH_TO_MEMORY;
    DMA_HandleType.Init.PeriphInc = DMA_PINC_DISABLE;
    DMA_HandleType.Init.MemInc = DMA_MINC_ENABLE;
    DMA_HandleType.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    DMA_HandleType.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    DMA_HandleType.Init.Mode = DMA_CIRCULAR;
    DMA_HandleType.Init.Priority = DMA_PRIORITY_MEDIUM;
    /*  DMA_HandleType.Init.Priority = DMA_PRIORITY_HIGH;   */
    
    if (HAL_OK != HAL_DMA_Init(&DMA_HandleType))
        Error_Handler();
    
    HAL_NVIC_SetPriority(ADC_DMA_CHANNEL_IRQN, 2U, 2U);
    HAL_NVIC_EnableIRQ(ADC_DMA_CHANNEL_IRQN);
}

static void InitAnalogyDigitalConverter(void)
{
    ADC_HandleType.Instance = SENSORS_ADC;
    
    ADC_HandleType.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
    ADC_HandleType.Init.Resolution = ADC_RESOLUTION_12B;
    ADC_HandleType.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    /*  ADC_HandleType.Init.GainCompensation = 0;   */
    ADC_HandleType.Init.ScanConvMode = ADC_SCAN_ENABLE;
    /*  ADC_HandleType.Init.EOCSelection = ADC_EOC_SINGLE_CONV; */
    ADC_HandleType.Init.EOCSelection = ADC_EOC_SEQ_CONV;
    ADC_HandleType.Init.LowPowerAutoWait = DISABLE;
    ADC_HandleType.Init.ContinuousConvMode = ENABLE;
    ADC_HandleType.Init.NbrOfConversion = ADC_CHANNELS_NUMBER;
    ADC_HandleType.Init.DiscontinuousConvMode = DISABLE;
    ADC_HandleType.Init.NbrOfDiscConversion = 0;
    ADC_HandleType.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    ADC_HandleType.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIG_EDGE_NONE;
    ADC_HandleType.Init.DMAContinuousRequests = ENABLE;
    ADC_HandleType.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
    ADC_HandleType.Init.OversamplingMode = DISABLE;
    
    ADC_HandleType.MspInitCallback = MspInitCallback;
    //ADC_HandleType.ConvCpltCallback = ConversionsCompletionCallback;
    
    if (HAL_OK != HAL_ADC_Init(&ADC_HandleType))
        Error_Handler();
    
    HAL_ADC_RegisterCallback(&ADC_HandleType, HAL_ADC_CONVERSION_COMPLETE_CB_ID, ConversionsCompletionCallback);
    HAL_ADCEx_Calibration_Start(&ADC_HandleType, ADC_SINGLE_ENDED);

     __HAL_LINKDMA(&ADC_HandleType, DMA_Handle, DMA_HandleType);
    
    if (HAL_OK != HAL_ADC_Start_DMA(&ADC_HandleType, (uint32_t *)gatheredConversions, ADC_CHANNELS_NUMBER * ADC_GATHER_TIMES_PER_CHANNEL))
        Error_Handler();
}

void InitGPIOs(void)
{
    /* ADC GPIOs. */
        
    GPIO_InitTypeDef GPIO_InitType = { 0 };
    
    GPIO_InitType.Pin = VBUS_VOLTAGE_SENSOR_GPIO_PIN;
    GPIO_InitType.Mode = GPIO_MODE_ANALOG;
    GPIO_InitType.Pull = GPIO_NOPULL;
    
    VBUS_VOLTAGE_SENSOR_GPIO_RCC_CLOCK_ENABLE();
    HAL_GPIO_Init(VBUS_VOLTAGE_SENSOR_GPIO_PORT, &GPIO_InitType);
    
    GPIO_InitType.Pin = PHASE_U_CURRENT_SENSOR_GPIO_PIN | TEMPERATURE_SENSOR_GPIO_PIN;
    
    PHASE_U_CURRENT_TEMPERATURE_SENSORS_GPIO_RCC_CLOCK_ENABLE();
    HAL_GPIO_Init(PHASE_U_CURRENT_TEMPERATURE_SENSORS_GPIO_PORT, &GPIO_InitType);
}

void StopAnalogyDigitalConverter(void)
{
    HAL_ADC_Stop_DMA(&ADC_HandleType);
}

static void MspInitCallback(ADC_HandleTypeDef *hadc)
{
    if (SENSORS_ADC == hadc -> Instance)
    {        
        /*  RCC Peripheral Clock selection. */
        
        RCC_PeriphCLKInitTypeDef RCC_PeriphCLKInitType = { 0 };
        
        RCC_PeriphCLKInitType.PeriphClockSelection = RCC_PERIPHCLK_ADC345;
        RCC_PeriphCLKInitType.Adc345ClockSelection = RCC_ADC345CLKSOURCE_SYSCLK;
        
        if (HAL_OK != HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitType))
            Error_Handler();
        
        SENSORS_ADC_RCC_CLOCK_ENABLE();

        InitGPIOs();
        
        ConfigureAnalogyDigitalConverterChannel(VBUS_VOLTAGE_SENSOR_ADC_CHANNEL, ADC_REGULAR_RANK_1, ADC_SAMPLETIME_640CYCLES_5);
        ConfigureAnalogyDigitalConverterChannel(PHASE_U_CURREN_SENSOR_ADC_CHANNEL, ADC_REGULAR_RANK_2, ADC_SAMPLETIME_640CYCLES_5);
        ConfigureAnalogyDigitalConverterChannel(TEMPERATURE_SENSOR_ADC_CHANNEL, ADC_REGULAR_RANK_3, ADC_SAMPLETIME_640CYCLES_5);
    }
}

void DMA1_Channel1_IRQHandler(void)
{
    HAL_DMA_IRQHandler(&DMA_HandleType);
}

static void ConversionsCompletionCallback(ADC_HandleTypeDef *hadc)
//void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    if (SENSORS_ADC == hadc -> Instance)
    {
        if (HAL_OK != HAL_ADC_Stop_DMA(hadc))
            Error_Handler();
        
        ComputeAnalogyDigitalConversions();
        
        if (HAL_OK != HAL_ADC_Start_DMA(hadc, (uint32_t *)gatheredConversions, ADC_CHANNELS_NUMBER * ADC_GATHER_TIMES_PER_CHANNEL))
            Error_Handler();
    }
}

static void ConfigureAnalogyDigitalConverterChannel(uint32_t channel, uint32_t rank, uint32_t samplingTime)
{
    ADC_ChannelConfTypeDef ADC_ChannelConfType = { 0 };
    
    ADC_ChannelConfType.Channel = channel;
    ADC_ChannelConfType.Rank = rank;
    ADC_ChannelConfType.SamplingTime = samplingTime;
    ADC_ChannelConfType.SingleDiff = ADC_SINGLE_ENDED;
    ADC_ChannelConfType.OffsetNumber = ADC_OFFSET_NONE;
    ADC_ChannelConfType.Offset = 0;
    
    if (HAL_OK != HAL_ADC_ConfigChannel(&ADC_HandleType, &ADC_ChannelConfType))
        Error_Handler();
}

float AcquireTemperature(uint16_t conversion)
{
    float Rt = 3.3f * 4700.0f / (conversion * 3.3f / 4096.0f) - 4700.0f;
    float temperarture = Rt / Rp;
    
    temperarture = log(temperarture);
    temperarture /= Bx;
    temperarture += 1.0f / T2;
    temperarture = 1.0f / temperarture;
    temperarture -= Ka;
    
    return (uint8_t)temperarture;
}

static void ComputeAnalogyDigitalConversions(void)
{  
    static int32_t totals[ADC_CHANNELS_NUMBER] = {0.0f, 0.0f, 0.0f};
    static int32_t initialCurrentADC = 0;
    
    for (uint16_t i = 0; i < ADC_GATHER_TIMES_PER_CHANNEL; i ++)
    {
        totals [0] += gatheredConversions [0 + i * ADC_CHANNELS_NUMBER];
        totals [1] += gatheredConversions [1 + i * ADC_CHANNELS_NUMBER];
        totals [2] += gatheredConversions [2 + i * ADC_CHANNELS_NUMBER];
    }
    
    if (initialCurrentADC == 0)
        initialCurrentADC = totals [1] / ADC_GATHER_TIMES_PER_CHANNEL;

    totals[0] /= ADC_GATHER_TIMES_PER_CHANNEL;
    totals[1] /= ADC_GATHER_TIMES_PER_CHANNEL;
    totals[2] /= ADC_GATHER_TIMES_PER_CHANNEL;
    
    if (conversionCompletedCallback)
        conversionCompletedCallback(ADC_TO_VOLTAGE * totals [0], (totals[1] - initialCurrentADC) * ADC_TO_CURRENT, AcquireTemperature(totals[2]));
}
        