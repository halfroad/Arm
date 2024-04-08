#include "../Include/AdavancedTimer.h"

#define TIMER                                                       TIM8
#define RCC_TIMER_CLOCK_ENABLE()                                    do                                      \
                                                                    {                                       \
                                                                        __HAL_RCC_TIM8_CLK_ENABLE();        \
                                                                    }                                       \
                                                                    while   (0)
#define TIMER_OUTPUT_COMPARE_MODE                                   TIM_OCMODE_TOGGLE
#define TIMER_OUTPUT_COMPARE_POLARITY                               TIM_OCPOLARITY_HIGH
#define TIMER_OUTPUT_COMPARE_N_POLARITY                             TIM_OCNPOLARITY_LOW
#define TIMER_OUTPUT_COMPARE_CHANNLE_0                              TIM_CHANNEL_1
#define TIMER_OUTPUT_COMPARE_CHANNLE_1                              TIM_CHANNEL_2
#define TIMER_IRQN                                                  TIM8_CC_IRQn
#define TIMER_IRQ_HANDLER                                           TIM8_CC_IRQHandler

/*
STMicroelectronics/MCUs/STM32G474VET6/Product Specifications/stm32g474ve.pdf
Table 12. STM32G474xB/xC/xE pin definition (continued)

Pin name            Alternate functions
    PC6                 TIM8_CH1
    PC7                 TIM8_CH2

*/
#define CHANNEL_0_ALTERNATE_FUNCTION_GPIO_PORT                      GPIOC
#define RCC_CHANNEL_0_ALTERNATE_FUNCTION_GPIO_CLOCK_ENABLE()        do                                      \
                                                                    {                                       \
                                                                        __HAL_RCC_GPIOC_CLK_ENABLE();       \
                                                                    }                                       \
                                                                    while   (0)
#define CHANNEL_0_ALTERNATE_FUNCTION_GPIO_PIN                       GPIO_PIN_6
#define CHANNEL_0_ALTERNATE_FUNCTION_GPIO_PIN_ALTERNATE_FUNCTION    GPIO_AF4_TIM8
                                                                    
#define CHANNEL_1_ALTERNATE_FUNCTION_GPIO_PORT                      GPIOC
#define RCC_CHANNEL_1_ALTERNATE_FUNCTION_GPIO_CLOCK_ENABLE()        do                                      \
                                                                    {                                       \
                                                                        __HAL_RCC_GPIOC_CLK_ENABLE();       \
                                                                    }                                       \
                                                                    while   (0)
                                                                    
#define CHANNEL_1_ALTERNATE_FUNCTION_GPIO_PIN                       GPIO_PIN_7
#define CHANNEL_1_ALTERNATE_FUNCTION_GPIO_PIN_ALTERNATE_FUNCTION    GPIO_AF4_TIM8
                                                                    
TIM_HandleTypeDef TIM_HandleType = { 0 };

static void OC_MspInitCallback(TIM_HandleTypeDef *htim);

static void ConfigurePulseWidthModlationOutputChannel(uint32_t channel, uint16_t pulse);

void InitAdvancedTimer(uint32_t prescaler, uint16_t period, void (* advancedTimerOutputDelayElapsedHandler)(TIM_HandleTypeDef *htim))
{
    TIM_HandleType.Instance                 = TIMER;
    
    TIM_HandleType.Init.Prescaler           = prescaler;
    TIM_HandleType.Init.CounterMode         = TIM_COUNTERMODE_UP;
    TIM_HandleType.Init.Period              = period;
    TIM_HandleType.Init.ClockDivision       = TIM_CLOCKDIVISION_DIV1;
    TIM_HandleType.Init.RepetitionCounter   = 0;
    TIM_HandleType.Init.AutoReloadPreload   = TIM_AUTORELOAD_PRELOAD_DISABLE;
    
    TIM_HandleType.OC_MspInitCallback       = OC_MspInitCallback;
    
    HAL_TIM_OC_Init(&TIM_HandleType);

    ConfigurePulseWidthModlationOutputChannel(TIMER_OUTPUT_COMPARE_CHANNLE_0, 0);
    ConfigurePulseWidthModlationOutputChannel(TIMER_OUTPUT_COMPARE_CHANNLE_1, 0);
    
    HAL_TIM_RegisterCallback(&TIM_HandleType, HAL_TIM_OC_DELAY_ELAPSED_CB_ID, advancedTimerOutputDelayElapsedHandler);
    
    HAL_TIM_Base_Start(&TIM_HandleType);
}

static void ConfigurePulseWidthModlationOutputChannel(uint32_t channel, uint16_t pulse)
{
    TIM_OC_InitTypeDef TIM_OC_InitType      = { 0 };
    
    TIM_OC_InitType.OCMode                  = TIMER_OUTPUT_COMPARE_MODE;
    TIM_OC_InitType.Pulse                   = pulse;
    TIM_OC_InitType.OCPolarity              = TIMER_OUTPUT_COMPARE_POLARITY;
    TIM_OC_InitType.OCNPolarity             = TIMER_OUTPUT_COMPARE_N_POLARITY;
    TIM_OC_InitType.OCFastMode              = TIM_OCFAST_DISABLE;
    TIM_OC_InitType.OCIdleState             = TIM_OCIDLESTATE_RESET;
    TIM_OC_InitType.OCNIdleState            = TIM_OCNIDLESTATE_RESET;
    
    HAL_TIM_OC_ConfigChannel(&TIM_HandleType, &TIM_OC_InitType, channel);
    
    __HAL_TIM_DISABLE_OCxPRELOAD(&TIM_HandleType, channel);
}

static void OC_MspInitCallback(TIM_HandleTypeDef *htim)
{
    if (TIMER == htim -> Instance)
    {
        RCC_TIMER_CLOCK_ENABLE();
    
        GPIO_InitTypeDef GPIO_InitType          = { 0 };
        
        GPIO_InitType.Pin                       = CHANNEL_0_ALTERNATE_FUNCTION_GPIO_PIN;
        
        /*
        
        STMicroelectronics/MCUs/STM32G474VET6/Product Specifications/stm32g474ve.pdf
        4.11 Alternate functions
        Table 13. Alternate function
        
        Port                AF4
        PC6                 TIM8_CH1
        PC7                 TIM8_CH2
        
        */
        
        GPIO_InitType.Pin                       = CHANNEL_0_ALTERNATE_FUNCTION_GPIO_PIN;
        GPIO_InitType.Mode                      = GPIO_MODE_AF_PP;
        GPIO_InitType.Alternate                 = CHANNEL_0_ALTERNATE_FUNCTION_GPIO_PIN_ALTERNATE_FUNCTION;
        
        RCC_CHANNEL_0_ALTERNATE_FUNCTION_GPIO_CLOCK_ENABLE();
        HAL_GPIO_Init(CHANNEL_0_ALTERNATE_FUNCTION_GPIO_PORT, &GPIO_InitType);
            
        GPIO_InitType.Pin                       = CHANNEL_1_ALTERNATE_FUNCTION_GPIO_PIN;
        GPIO_InitType.Alternate                 = CHANNEL_1_ALTERNATE_FUNCTION_GPIO_PIN_ALTERNATE_FUNCTION;
        
        RCC_CHANNEL_1_ALTERNATE_FUNCTION_GPIO_CLOCK_ENABLE();
        HAL_GPIO_Init(CHANNEL_0_ALTERNATE_FUNCTION_GPIO_PORT, &GPIO_InitType);
        
        HAL_NVIC_SetPriority(TIMER_IRQN, 2, 2);
        HAL_NVIC_EnableIRQ(TIMER_IRQN);
    }
}

void TIMER_IRQ_HANDLER(void)
{
    HAL_TIM_IRQHandler(&TIM_HandleType);
}

void StartPulseWidthModulation(PulseWidthModulationOutputCompareChannels channel)
{    
    switch (channel)
    {
        case PulseWidthModulationOutputCompareChannel0:
            HAL_TIM_OC_Start_IT(&TIM_HandleType, TIMER_OUTPUT_COMPARE_CHANNLE_0);
            break;
        
        case PulseWidthModulationOutputCompareChannel1:
            HAL_TIM_OC_Start_IT(&TIM_HandleType, TIMER_OUTPUT_COMPARE_CHANNLE_1);
            break;
        
        default:
            break;
    }
}

void StopPulseWidthModulation(PulseWidthModulationOutputCompareChannels channel)
{    
    switch (channel)
    {
        case PulseWidthModulationOutputCompareChannel0:
            HAL_TIM_OC_Stop_IT(&TIM_HandleType, TIMER_OUTPUT_COMPARE_CHANNLE_0);
            break;
        
        case PulseWidthModulationOutputCompareChannel1:
            HAL_TIM_OC_Stop_IT(&TIM_HandleType, TIMER_OUTPUT_COMPARE_CHANNLE_1);
            break;
        
        default:
            break;
    }
}

void AssignNewCompare(uint16_t increments)
{
    static uint16_t counter = 0;
    
    counter = __HAL_TIM_GET_COUNTER(&TIM_HandleType);
    
    switch (TIM_HandleType.Channel)
    {
        case HAL_TIM_ACTIVE_CHANNEL_1:
            __HAL_TIM_SET_COMPARE(&TIM_HandleType, TIMER_OUTPUT_COMPARE_CHANNLE_0, (counter + increments) % 0xFFFF);
            break;
        
        case HAL_TIM_ACTIVE_CHANNEL_2:
            __HAL_TIM_SET_COMPARE(&TIM_HandleType, TIMER_OUTPUT_COMPARE_CHANNLE_1, (counter + increments) % 0xFFFF);
            break;
        
        default:
            break;
    }
    
    
}