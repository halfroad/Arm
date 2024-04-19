#include "../Include/AdvancedTimer.h"

#define TIM                                                                         TIM8

#define RCC_TIM_CLOCK_ENABLE()                                                      do                                      \
                                                                                    {                                       \
                                                                                        __HAL_RCC_TIM8_CLK_ENABLE();        \
                                                                                    }                                       \
                                                                                    while   (0)

#define TIM_IRQN                                                                    TIM8_CC_IRQn
#define TIM_IRQHANDLER                                                              TIM8_CC_IRQHandler
                                                                                    
#define OUTPUT_COMPARE_CHANNEL_MODE                                                 TIM_OCMODE_TOGGLE
#define OUTPUT_COMPARE_CHANNEL_POLARITY                                             TIM_OCPOLARITY_HIGH
#define OUTPUT_COMPARE_CHANNEL_COMPLEMENTARY_POLARITY                               TIM_OCNPOLARITY_LOW
#define OUTPUT_COMPARE_CHANNEL_IDLE_STATE                                           TIM_OCIDLESTATE_RESET
#define OUTPUT_COMPARE_CHANNEL_COMPLEMENTARY_IDLE_STATE                             TIM_OCNIDLESTATE_RESET

#define OUTPUT_COMPARE_CHANNEL_0                                                    TIM_CHANNEL_1
#define OUTPUT_COMPARE_CHANNEL_1                                                    TIM_CHANNEL_2

/*

Pins layout of Stepper Motor Drive Board

Connector 1

Pin Number      GPIO        Alternate Functions     Functions on drive      Isolated?       Description

65              PC6         TIM8_CH1                ST1_PUL                 N               PWM 1 on Stepper Motor Drive Board.
66              PC7         TIM8_CH2                ST2_PUL                 N               PWM 2 on Stepper Motor Drive Board.

/STM32G474VET6/Product Specifications/stm32g474ve.pdf
Table 13. Alternate function (continued)

Port            AF4

                I2C1/2/3/4/TIM1/8/16/17
                
PC6             TIM8_CH1
PC7             TIM8_CH2

*/

#define OUTPUT_COMPARE_CHANNEL_0_ALTERNATE_FUNCTION_GPIO_PORT                       GPIOC
#define OUTPUT_COMPARE_CHANNEL_0_ALTERNATE_FUNCTION_GPIO_PIN                        GPIO_PIN_6
#define OUTPUT_COMPARE_CHANNEL_0_ALTERNATE_FUNCTION                                 GPIO_AF4_TIM8

#define RCC_OUTPUT_COMPARE_CHANNEL_0_ALTERNATE_FUNCTION_GPIO_PORT_CLOCK_ENABLE()    do                                      \
                                                                                    {                                       \
                                                                                        __HAL_RCC_GPIOC_CLK_ENABLE();       \
                                                                                    }                                       \
                                                                                    while   (0)
 
#define OUTPUT_COMPARE_CHANNEL_1_ALTERNATE_FUNCTION_GPIO_PORT                       GPIOC
#define OUTPUT_COMPARE_CHANNEL_1_ALTERNATE_FUNCTION_GPIO_PIN                        GPIO_PIN_7
#define OUTPUT_COMPARE_CHANNEL_1_ALTERNATE_FUNCTION                                 GPIO_AF4_TIM8

#define RCC_OUTPUT_COMPARE_CHANNEL_1_ALTERNATE_FUNCTION_GPIO_PORT_CLOCK_ENABLE()    do                                      \
                                                                                    {                                       \
                                                                                        __HAL_RCC_GPIOC_CLK_ENABLE();      \
                                                                                    }                                       \
                                                                                    while   (0)

static void OC_MspInitCallback(TIM_HandleTypeDef *htim);
static void ConfigureOutputChannel(TIM_HandleTypeDef *htim, uint16_t pulse, uint8_t channel);

TIM_HandleTypeDef TIM_HandleType            = { 0 };
TIM_OC_InitTypeDef TIM_OC_InitType          = { 0 };

void InitAdvancedTimer(uint16_t prescaler, uint16_t period, void (* onOutputCompareDelayElapsedCallback)(TIM_HandleTypeDef *htim))
{
    TIM_HandleType.Instance                 = TIM;
    
    TIM_HandleType.Init.Prescaler           = prescaler;
    TIM_HandleType.Init.CounterMode         = TIM_COUNTERMODE_UP;
    TIM_HandleType.Init.Period              = period;
    TIM_HandleType.Init.ClockDivision       = TIM_CLOCKDIVISION_DIV1;
    
    TIM_HandleType.OC_MspInitCallback       = OC_MspInitCallback;
    //TIM_HandleType.OC_DelayElapsedCallback  = OC_DelayElapsedCallback;
    
    HAL_TIM_OC_Init(&TIM_HandleType);
    
    if (onOutputCompareDelayElapsedCallback)
        HAL_TIM_RegisterCallback(&TIM_HandleType, HAL_TIM_OC_DELAY_ELAPSED_CB_ID, onOutputCompareDelayElapsedCallback);
    
    ConfigureOutputChannel(&TIM_HandleType, 0, OUTPUT_COMPARE_CHANNEL_0);
    ConfigureOutputChannel(&TIM_HandleType, 0, OUTPUT_COMPARE_CHANNEL_1);
    
    HAL_TIM_Base_Start(&TIM_HandleType);
}

static void ConfigureOutputChannel(TIM_HandleTypeDef *htim, uint16_t pulse, uint8_t channel)
{
    TIM_OC_InitType.OCMode                  = OUTPUT_COMPARE_CHANNEL_MODE;
    TIM_OC_InitType.Pulse                   = pulse;
    TIM_OC_InitType.OCPolarity              = OUTPUT_COMPARE_CHANNEL_POLARITY;
    TIM_OC_InitType.OCNPolarity             = OUTPUT_COMPARE_CHANNEL_COMPLEMENTARY_POLARITY;
    TIM_OC_InitType.OCFastMode              = TIM_OCFAST_DISABLE;
    TIM_OC_InitType.OCIdleState             = OUTPUT_COMPARE_CHANNEL_IDLE_STATE;
    TIM_OC_InitType.OCNIdleState            = OUTPUT_COMPARE_CHANNEL_COMPLEMENTARY_IDLE_STATE;
    
    HAL_TIM_OC_ConfigChannel(htim, &TIM_OC_InitType, channel);
    
    __HAL_TIM_DISABLE_OCxPRELOAD(&TIM_HandleType, channel);
}

static void OC_MspInitCallback(TIM_HandleTypeDef *htim)
{
    if (htim -> Instance == TIM)
    {
        RCC_TIM_CLOCK_ENABLE();
        
        GPIO_InitTypeDef GPIO_InitType      = { 0 };
        
        GPIO_InitType.Pin                   = OUTPUT_COMPARE_CHANNEL_0_ALTERNATE_FUNCTION_GPIO_PIN | OUTPUT_COMPARE_CHANNEL_1_ALTERNATE_FUNCTION_GPIO_PIN;
        GPIO_InitType.Mode                  = GPIO_MODE_AF_PP;
        GPIO_InitType.Pull                  = GPIO_PULLUP;
        GPIO_InitType.Speed                 = GPIO_SPEED_HIGH;
        GPIO_InitType.Alternate             = OUTPUT_COMPARE_CHANNEL_0_ALTERNATE_FUNCTION;
        
        RCC_OUTPUT_COMPARE_CHANNEL_0_ALTERNATE_FUNCTION_GPIO_PORT_CLOCK_ENABLE();
        HAL_GPIO_Init(OUTPUT_COMPARE_CHANNEL_0_ALTERNATE_FUNCTION_GPIO_PORT, &GPIO_InitType);
        
        HAL_NVIC_SetPriority(TIM_IRQN, 2, 3);
        HAL_NVIC_EnableIRQ(TIM_IRQN);
    }
}

void TIM_IRQHANDLER(void)
{
    HAL_TIM_IRQHandler(&TIM_HandleType);
}

void StartOutputCompare(OutputCompareChannels channel)
{
    switch (channel)
    {
        case OutputCompareChannel0:
        {
            if (TIM_OC_InitType.OCMode == TIM_OCMODE_PWM1 ||
                TIM_OC_InitType.OCMode == TIM_OCMODE_PWM2)
            {
                HAL_TIM_PWM_Start(&TIM_HandleType, OUTPUT_COMPARE_CHANNEL_0);
            }
            else if (TIM_OC_InitType.OCMode == TIM_OCMODE_TOGGLE)
            {
                HAL_TIM_OC_Start_IT(&TIM_HandleType, OUTPUT_COMPARE_CHANNEL_0);
            }
        }
        
            break;
        
        case OutputCompareChannel1:
        {
            
            if (TIM_OC_InitType.OCMode == TIM_OCMODE_PWM1 ||
                TIM_OC_InitType.OCMode == TIM_OCMODE_PWM2)
            {
                HAL_TIM_PWM_Start(&TIM_HandleType, OUTPUT_COMPARE_CHANNEL_1);
            }
            else if (TIM_OC_InitType.OCMode == TIM_OCMODE_TOGGLE)
            {
                HAL_TIM_OC_Start_IT(&TIM_HandleType, OUTPUT_COMPARE_CHANNEL_1);
            }
        }
            break;
        
        default:
            break;
    }
}

void StopOutputCompare(OutputCompareChannels channel)
{
    switch (channel)
    {
        case OutputCompareChannel0:
        {
            if (TIM_OC_InitType.OCMode == TIM_OCMODE_PWM1 ||
                TIM_OC_InitType.OCMode == TIM_OCMODE_PWM2)
            {
                HAL_TIM_PWM_Stop(&TIM_HandleType, OUTPUT_COMPARE_CHANNEL_0);
            }
            else if (TIM_OC_InitType.OCMode == TIM_OCMODE_TOGGLE)
            {
                HAL_TIM_OC_Stop_IT(&TIM_HandleType, OUTPUT_COMPARE_CHANNEL_0);
            }
        }
        
            break;
        
        case OutputCompareChannel1:
            
            if (TIM_OC_InitType.OCMode == TIM_OCMODE_PWM1 ||
                TIM_OC_InitType.OCMode == TIM_OCMODE_PWM2)
            {
                HAL_TIM_PWM_Stop(&TIM_HandleType, OUTPUT_COMPARE_CHANNEL_1);
            }
            else if (TIM_OC_InitType.OCMode == TIM_OCMODE_TOGGLE)
            {
                HAL_TIM_OC_Stop_IT(&TIM_HandleType, OUTPUT_COMPARE_CHANNEL_1);
            }
        
            break;
        
        default:
            break;
    }
}

void ResetCounter(void)
{
    __HAL_TIM_SET_COUNTER(&TIM_HandleType, 0);
}

void AssignNewCompare(OutputCompareChannels channel, uint16_t increments)
{
    static uint16_t counter = 0;
    
    counter = __HAL_TIM_GetCounter(&TIM_HandleType);
    
    switch (channel)
    {
        case OutputCompareChannel0:

            __HAL_TIM_SET_COMPARE(&TIM_HandleType, OUTPUT_COMPARE_CHANNEL_0, (counter + increments) & 0xFFFF);
        
        break;
        
        case OutputCompareChannel1:
            
            __HAL_TIM_SET_COMPARE(&TIM_HandleType, OUTPUT_COMPARE_CHANNEL_1, (counter + increments)  & 0xFFFF);
        
        break;
        
        default:
            break;
    }
}