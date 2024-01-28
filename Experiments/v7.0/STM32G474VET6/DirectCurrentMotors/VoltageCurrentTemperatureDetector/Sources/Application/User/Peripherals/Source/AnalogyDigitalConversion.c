#include <math.h>

#include "../Include/AnalogyDigitalConversion.h"

/*

Table 12. STM32G474xB/xC/xE pin definition (continued)

Pin name        Alternate unctions          Functions on ATK Motor Drive Board

PB1             ADC3_IN1 / ADC1_IN12        PM2_VBUS, Voltage Sensor of DC with/o Motor Connector 2.
PE7             ADC3_IN4                    PM2_AMPU, U Phase Current Sensor of DC with/o Motor Connector 2.
PE8(Not Used)   ADC345_IN6                  PM2_AMPV, V Phase Current Sensor of DC with/o Motor Connector 2.
PE9             ADC3_IN2                    PM2_VTEMP, Temperature Sensor of DC with/o Motor Connector 2.
PE10(Not Used)  ADC345_IN14                 PM2_AMPW, W Phase Current Sensor of DC with/o Motor Connector 2.

*/

/* ADCx settings. */
#define ADC_INSTANCE                                                ADC3
#define ADC_INSTANCE_CLOCK_ENABLE()                                 do { __HAL_RCC_ADC345_CLK_ENABLE(); } while (0)

/* PM2_VBUS, Voltage Sensor of DC with/o Motor Connector 2. */
#define VBUS_VOLTAGE_SENSOR_GPIO_PORT                               GPIOB
#define VBUS_VOLTAGE_SENSOR_GPIO_PIN                                GPIO_PIN_1
#define VBUS_VOLTAGE_SENSOR_GPIO_CLOCK_ENABLE()                     do { __HAL_RCC_GPIOB_CLK_ENABLE(); } while (0)
#define VBUS_VOLTAGE_SENSOR_ADC_CHANNEL                             ADC_CHANNEL_1

/* U Phase Current Sensor of DC with/o Motor Connector 2.. */
#define PHASE_U_CURRENT_SENSOR_GPIO_PORT                            GPIOE
#define PHASE_U_CURRENT_SENSOR_GPIO_PIN                             GPIO_PIN_7
#define PHASE_U_CURRENT_SENSOR_GPIO_CLOCK_ENABLE()                  do { __HAL_RCC_GPIOE_CLK_ENABLE(); } while (0)
#define PHASE_U_CURRENT_SENSOR_ADC_CHANNEL                          ADC_CHANNEL_4

/* PM2_VTEMP, Temperature Sensor of DC with/o Motor Connector 2. */
/* #define TEMPERATURE_SENSOR_GPIO_PORT                                GPIOE */
#define TEMPERATURE_SENSOR_GPIO_PIN                                 GPIO_PIN_9
/* #define TEMPERATURE_SENSOR_GPIO_CLOCK_ENABLE()                      do { __HAL_RCC_GPIOE_CLK_ENABLE(); } while(0) */
#define TEMPERATURE_SENSOR_ADC_CHANNEL                              ADC_CHANNEL_2

#define DMA_CLOCK_ENABLE()                                          do { __HAL_RCC_DMA1_CLK_ENABLE(); } while (0)

#define DMA_INSTANCE                                                DMA1
#define DMA_CHANNEL                                                 DMA1_Channel1
#define DMA_REQUEST                                                 DMA_REQUEST_ADC3
#define DMA_CHANNEL_IRQN                                            DMA1_Channel1_IRQn

#define DMA_Channel_IRQHandler                                      DMA1_Channel1_IRQHandler

ADC_HandleTypeDef ADC3_HandleType = { 0 };
DMA_HandleTypeDef DMA1_ADC3_HandleType = { 0 };

void (* analogyDigitalConversionCompletionHandler) (uint16_t *) ;

const float Rp = 10000.0f;
const float T2 = 273.15f + 25.0f;
const float Bx = 3380.0f;
const float Ka = 273.15f;

uint16_t gatheredConversions[ADC_CHANNELS_NUMBER * ADC_GATHER_TIMES_PER_CHANNEL] = { 0 };
uint16_t averageConversions[ADC_CHANNELS_NUMBER];

static void InitDirectMemoryAccess(void);
static void InitAnalogyDigitalConversion(void);
void ConfigAnalogyDigitalConversionChannel(ADC_HandleTypeDef *ADC_HandleType, uint32_t channel, uint32_t rank, uint32_t samplingTime);

static void ComputeAnalogyDigitalConversions(uint16_t *averageConversions);

extern void Error_Handler(void);

void InitEmientSensors(void)
{
    InitDirectMemoryAccess();
    InitAnalogyDigitalConversion();
    
    HAL_ADCEx_Calibration_Start(&ADC3_HandleType, ADC_SINGLE_ENDED);
    
    ConfigAnalogyDigitalConversionChannel(&ADC3_HandleType, VBUS_VOLTAGE_SENSOR_ADC_CHANNEL, ADC_REGULAR_RANK_1, ADC_SAMPLETIME_640CYCLES_5);
    ConfigAnalogyDigitalConversionChannel(&ADC3_HandleType, PHASE_U_CURRENT_SENSOR_ADC_CHANNEL, ADC_REGULAR_RANK_2, ADC_SAMPLETIME_640CYCLES_5);
    ConfigAnalogyDigitalConversionChannel(&ADC3_HandleType, TEMPERATURE_SENSOR_ADC_CHANNEL, ADC_REGULAR_RANK_3, ADC_SAMPLETIME_640CYCLES_5);

    __HAL_LINKDMA(&ADC3_HandleType, DMA_Handle, DMA1_ADC3_HandleType);
    
    /* ADC3 interrupt Init */
    HAL_NVIC_SetPriority(DMA_CHANNEL_IRQN, 3, 3);
    HAL_NVIC_EnableIRQ(DMA_CHANNEL_IRQN);
    
    HAL_ADC_Start_DMA(&ADC3_HandleType, (uint32_t *)gatheredConversions, ADC_CHANNELS_NUMBER * ADC_GATHER_TIMES_PER_CHANNEL);

}

static void InitDirectMemoryAccess(void)
{
    __HAL_RCC_DMAMUX1_CLK_ENABLE();
    DMA_CLOCK_ENABLE();
    
    /* ADC3 DMA Init */
    DMA1_ADC3_HandleType.Instance = DMA_CHANNEL;
    
    DMA1_ADC3_HandleType.Init.Request = DMA_REQUEST;
    DMA1_ADC3_HandleType.Init.Direction = DMA_PERIPH_TO_MEMORY;
    DMA1_ADC3_HandleType.Init.PeriphInc= DMA_PINC_DISABLE;
    DMA1_ADC3_HandleType.Init.MemInc = DMA_MINC_ENABLE;
    DMA1_ADC3_HandleType.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    DMA1_ADC3_HandleType.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    DMA1_ADC3_HandleType.Init.Mode = DMA_CIRCULAR;
    DMA1_ADC3_HandleType.Init.Priority = DMA_PRIORITY_MEDIUM;
    
    if (HAL_DMA_Init(&DMA1_ADC3_HandleType) != HAL_OK)
        Error_Handler();
}

/* void InitAnalogyDigitalConversion(void (* analogyDigitalConversionCompletion)(uint16_t *conversions)) */
void InitAnalogyDigitalConversion(void)
{
    /* ADC3 Init */
    /* ADC3_HandleTypeDef ADC3_HandleType = { 0 }; */
    
    ADC3_HandleType.Instance = ADC_INSTANCE;
    
    ADC3_HandleType.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;  /* 170MHz / 4. */
    ADC3_HandleType.Init.Resolution = ADC_RESOLUTION_12B;
    ADC3_HandleType.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    ADC3_HandleType.Init.GainCompensation = 0x00U;
    ADC3_HandleType.Init.ScanConvMode = ADC_SCAN_ENABLE;
    //ADC3_HandleType.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
    ADC3_HandleType.Init.EOCSelection = ADC_EOC_SEQ_CONV;
    ADC3_HandleType.Init.LowPowerAutoWait = DISABLE;
    ADC3_HandleType.Init.ContinuousConvMode = ENABLE;
    // ADC3_HandleType.Init.ContinuousConvMode = DISABLE;
    ADC3_HandleType.Init.NbrOfConversion = ADC_CHANNELS_NUMBER;
    ADC3_HandleType.Init.DiscontinuousConvMode = DISABLE;
    ADC3_HandleType.Init.NbrOfDiscConversion = 0;
    ADC3_HandleType.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    ADC3_HandleType.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    ADC3_HandleType.Init.DMAContinuousRequests = ENABLE;
    ADC3_HandleType.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
    ADC3_HandleType.Init.OversamplingMode = DISABLE;
    
    /* ADC3_HandleType.MspInitCallback = 
    
    analogyDigitalConversionCompletionHandler = analogyDigitalConversionCompletion;
    
    */
    if (HAL_ADC_Init(&ADC3_HandleType) != HAL_OK)
        Error_Handler();
}

void ConfigAnalogyDigitalConversionChannel(ADC_HandleTypeDef *ADC_HandleType, uint32_t channel, uint32_t rank, uint32_t samplingTime)
{
    ADC_ChannelConfTypeDef ADC_ChannelConfType = { 0 };
    
    ADC_ChannelConfType.Channel = channel;
    ADC_ChannelConfType.Rank = rank;
    ADC_ChannelConfType.SamplingTime = samplingTime;
    ADC_ChannelConfType.SingleDiff = ADC_SINGLE_ENDED;
    ADC_ChannelConfType.OffsetNumber = ADC_OFFSET_NONE;
    ADC_ChannelConfType.Offset = 0;
    
    if (HAL_ADC_ConfigChannel(ADC_HandleType, &ADC_ChannelConfType) != HAL_OK)
        Error_Handler();
    
}

/**
  * @brief  Initialize the ADC MSP.
  * @param hadc ADC handle
  * @retval None
  */
void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
{
  /* Prevent unused argument(s) compilation warning
  UNUSED(hadc);
    */
  /* NOTE : This function should not be modified. When the callback is needed,
            function HAL_ADC_MspInit must be implemented in the user file.
   */
       
    if (hadc -> Instance == ADC_INSTANCE)
    {
        GPIO_InitTypeDef GPIO_InitType = { 0 };
        RCC_PeriphCLKInitTypeDef RCC_PeriphCLKInitType = { 0 };
        
        /* Set the clock source of ADCx. */
        RCC_PeriphCLKInitType.PeriphClockSelection = RCC_PERIPHCLK_ADC345;
        RCC_PeriphCLKInitType.Adc345ClockSelection = RCC_ADC345CLKSOURCE_SYSCLK;
        
        if (HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitType) != HAL_OK)
            Error_Handler();
        
        /* Enable ADCx Clock. */
        ADC_INSTANCE_CLOCK_ENABLE();
        
        /* Enable GPIO clock for Voltage Sensor. */
        VBUS_VOLTAGE_SENSOR_GPIO_CLOCK_ENABLE();
        /* Enable GPIO clock for Current Sensor. */
        PHASE_U_CURRENT_SENSOR_GPIO_CLOCK_ENABLE();
        
        /**ADC3 GPIO Configuration
        PB1     ------> ADC3_IN1
        PE7     ------> ADC3_IN4
        PE9     ------> ADC3_IN2
        */
        
        GPIO_InitType.Pin = VBUS_VOLTAGE_SENSOR_GPIO_PIN;
        GPIO_InitType.Mode = GPIO_MODE_ANALOG;
        GPIO_InitType.Pull = GPIO_NOPULL;
        
        HAL_GPIO_Init(VBUS_VOLTAGE_SENSOR_GPIO_PORT, &GPIO_InitType);
        
        GPIO_InitType.Pin = PHASE_U_CURRENT_SENSOR_GPIO_PIN | TEMPERATURE_SENSOR_GPIO_PIN;
        
        HAL_GPIO_Init(PHASE_U_CURRENT_SENSOR_GPIO_PORT, &GPIO_InitType);
    }
}

/**
  * @brief This function handles DMA1 channel1 global interrupt.
  */
void DMA_Channel_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel1_IRQn 0 */

  /* USER CODE END DMA1_Channel1_IRQn 0 */
  HAL_DMA_IRQHandler(&DMA1_ADC3_HandleType);
  /* USER CODE BEGIN DMA1_Channel1_IRQn 1 */

  /* USER CODE END DMA1_Channel1_IRQn 1 */
}

/**
  * @brief  Conversion complete callback in non-blocking mode.
  * @param hadc ADC handle
  * @retval None
  */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
  /* Prevent unused argument(s) compilation warning 
  UNUSED(hadc);
    */
  /* NOTE : This function should not be modified. When the callback is needed,
            function HAL_ADC_ConvCpltCallback must be implemented in the user file.
   */
    
    if (hadc -> Instance == ADC_INSTANCE)
    {
        HAL_ADC_Stop_DMA(&ADC3_HandleType);
        
        /*
        if (analogyDigitalConversionCompletionHandler)
            analogyDigitalConversionCompletionHandler(averageConversions);
        */
        ComputeAnalogyDigitalConversions(averageConversions);
        
        HAL_ADC_Start_DMA(&ADC3_HandleType, (uint32_t *)gatheredConversions, ADC_CHANNELS_NUMBER * ADC_GATHER_TIMES_PER_CHANNEL);
    }
}

static void ComputeAnalogyDigitalConversions(uint16_t *conversions)
{
    uint32_t averages [3] = { 0, 0, 0 };
    
    for (uint16_t i = 0; i < ADC_GATHER_TIMES_PER_CHANNEL; i ++)
    {
        averages[0] += gatheredConversions[0 + i * ADC_CHANNELS_NUMBER];    /* Gathered voltages. */
        averages[1] += gatheredConversions[1 + i * ADC_CHANNELS_NUMBER];    /* Gathered temperatures. */
        averages[2] += gatheredConversions[2 + i * ADC_CHANNELS_NUMBER];    /* Gathered currents. */
    }
    
    averages[0] /= ADC_CHANNELS_NUMBER;
    averages[1] /= ADC_CHANNELS_NUMBER;
    averages[2] /= ADC_CHANNELS_NUMBER;
    
    conversions[0] = averages[0];
    conversions[1] = averages[1];
    conversions[2] = averages[2];
}

float AcquireTemperature(uint16_t conversion)
{
    float Rt = 3.3f * 4700.0f / (conversion * 3.3f / 4096.0f) - 4700.0f;
    float temperature = Rt / Rp;
    
    temperature = log(temperature);
    temperature /= Bx;
    temperature += 1.0f / T2;
    temperature = 1.0f / temperature;
    temperature -= Ka;
    
    return temperature;
}