#include "../Include/AdvancedTimer.h"

#define TIMER                                                   TIM8

#define RCC_TIMER_CLOCK_ENABLE()                                do                                  \
                                                                {                                   \
                                                                    __HAL_RCC_TIM8_CLK_ENABLE();    \
                                                                }                                   \
                                                                while   (0)                         \

#define COUNTER_MODE                                            TIM_COUNTERMODE_UP
#define OUTPUT_COMPARE_MODE                                     TIM_OCMODE_PWM2

#define OUTPUT_COMPARE_POLARITY                                 TIM_OCPOLARITY_HIGH
#define OUTPUT_COMPARE_COMPLEMENTARY_POLARITY                   TIM_OCNPOLARITY_HIGH

#define OUTPUT_COMPARE_IDLE_STATE                               TIM_OCIDLESTATE_RESET
#define OUTPUT_COMPARE_COMPLEMENTARY_IDLE_STATE                 TIM_OCNIDLESTATE_RESET

#define CHANNEL_1                                               TIM_CHANNEL_1
#define CHANNEL_2                                               TIM_CHANNEL_2

    /*
    
    /MCUs/STM32G474VET6/Product Specifications/stm32g474ve.pdf
    Table 13. Alternate function (continued)
    
    Port                            AF4
                                    I2C1/2/3/4/TIM1/8/16/17
    PC6                             TIM8_CH1
    PC7                             TIM8_CH2
    
    */
#define CHANNEL_1_ALTERNATE_FUNCTION_GPIO_PORT                  GPIOC
#define RCC_CHANNEL_1_ALTERNATE_FUNCTION_GPIO_CLOCK_ENABLE()    do                                  \
                                                                {                                   \
                                                                    __HAL_RCC_GPIOC_CLK_ENABLE();   \
                                                                }                                   \
                                                                while   (0)                         \

#define CHANNEL_1_ALTERNATE_FUNCTION_GPIO_PIN                   GPIO_PIN_6
#define CHANNEL_1_ALTERNATE_FUNCTION                            GPIO_AF4_TIM8

#define CHANNEL_2_ALTERNATE_FUNCTION_GPIO_PORT                  GPIOC
#define RCC_CHANNEL_2_ALTERNATE_FUNCTION_GPIO_CLOCK_ENABLE()    do                                  \
                                                                {                                   \
                                                                    __HAL_RCC_GPIOC_CLK_ENABLE();   \
                                                                }                                   \
                                                                while   (0)                         \

#define CHANNEL_2_ALTERNATE_FUNCTION_GPIO_PIN                   GPIO_PIN_7
#define CHANNEL_2_ALTERNATE_FUNCTION                            GPIO_AF4_TIM8

#define DIRECTION_1_GPIO_PORT                                   GPIOF
#define RCC_DIRECTION_1_GPIO_CLOCK_ENABLE()                     do                                  \
                                                                {                                   \
                                                                    __HAL_RCC_GPIOF_CLK_ENABLE();   \
                                                                }                                   \
                                                                while   (0)                         \

#define DIRECTION_1_GPIO_PIN                                    GPIO_PIN_2

#define DIRECTION_2_GPIO_PORT                                   GPIOE
#define RCC_DIRECTION_2_GPIO_CLOCK_ENABLE()                     do                                  \
                                                                {                                   \
                                                                    __HAL_RCC_GPIOE_CLK_ENABLE();   \
                                                                }                                   \
                                                                while   (0)                         \

#define DIRECTION_2_GPIO_PIN                                    GPIO_PIN_5

#define ENABLEMENT_1_GPIO_PORT                                  GPIOF
#define RCC_ENABLEMENT_1_GPIO_CLOCK_ENABLE()                    do                                  \
                                                                {                                   \
                                                                    __HAL_RCC_GPIOF_CLK_ENABLE();   \
                                                                }                                   \
                                                                while   (0)                         \

#define ENABLEMENT_1_GPIO_PIN                                   GPIO_PIN_9

#define ENABLEMENT_2_GPIO_PORT                                  GPIOE
#define RCC_ENABLEMENT_2_GPIO_CLOCK_ENABLE()                    do                                  \
                                                                {                                   \
                                                                    __HAL_RCC_GPIOE_CLK_ENABLE();   \
                                                                }                                   \
                                                                while   (0)                         \

#define ENABLEMENT_2_GPIO_PIN                                   GPIO_PIN_6


TIM_HandleTypeDef TIM_HandleType;

void PWM_MspInitCallback(TIM_HandleTypeDef *htim);
void ConfigurePulseWidthModulationChannels(uint16_t pulse);

void InitAdvancedTimer(uint16_t prescaler, uint32_t period)
{
    TIM_HandleType.Instance                     = TIMER;
    
    TIM_HandleType.Init.Prescaler               = prescaler;
    TIM_HandleType.Init.CounterMode             = COUNTER_MODE;
    TIM_HandleType.Init.Period                  = period;
    TIM_HandleType.Init.ClockDivision           = TIM_CLOCKDIVISION_DIV1;
    TIM_HandleType.Init.RepetitionCounter       = 0;
    
   TIM_HandleType.PWM_MspInitCallback           = PWM_MspInitCallback;

    HAL_TIM_PWM_Init(&TIM_HandleType);
    
    ConfigurePulseWidthModulationChannels(period / 2);
}

void ConfigurePulseWidthModulationChannels(uint16_t pulse)
{
    TIM_OC_InitTypeDef TIM_OC_InitType          = { 0 };
    
    TIM_OC_InitType.OCMode                      = OUTPUT_COMPARE_MODE;
    TIM_OC_InitType.Pulse                       = pulse;
    TIM_OC_InitType.OCPolarity                  = OUTPUT_COMPARE_POLARITY;
    TIM_OC_InitType.OCNPolarity                 = OUTPUT_COMPARE_COMPLEMENTARY_POLARITY;
    TIM_OC_InitType.OCFastMode                  = TIM_OCFAST_DISABLE;
    TIM_OC_InitType.OCIdleState                 = OUTPUT_COMPARE_IDLE_STATE;
    TIM_OC_InitType.OCNIdleState                = OUTPUT_COMPARE_COMPLEMENTARY_IDLE_STATE;
    
    HAL_TIM_PWM_ConfigChannel(&TIM_HandleType, &TIM_OC_InitType, CHANNEL_1);
    HAL_TIM_PWM_ConfigChannel(&TIM_HandleType, &TIM_OC_InitType, CHANNEL_2);
}

void EnableGPIOs(void)
{
    GPIO_InitTypeDef GPIO_InitType              = { 0 };
    
    GPIO_InitType.Pin                           = CHANNEL_1_ALTERNATE_FUNCTION_GPIO_PIN;
    GPIO_InitType.Mode                          = GPIO_MODE_AF_PP;
    GPIO_InitType.Alternate                     = CHANNEL_1_ALTERNATE_FUNCTION;
    
    RCC_CHANNEL_1_ALTERNATE_FUNCTION_GPIO_CLOCK_ENABLE();
    HAL_GPIO_Init(CHANNEL_1_ALTERNATE_FUNCTION_GPIO_PORT, &GPIO_InitType);
    
    GPIO_InitType.Pin                           = CHANNEL_2_ALTERNATE_FUNCTION_GPIO_PIN;
    GPIO_InitType.Alternate                     = CHANNEL_2_ALTERNATE_FUNCTION;
    
    RCC_CHANNEL_2_ALTERNATE_FUNCTION_GPIO_CLOCK_ENABLE();
    HAL_GPIO_Init(CHANNEL_2_ALTERNATE_FUNCTION_GPIO_PORT, &GPIO_InitType);
    
    GPIO_InitType.Pin                           = DIRECTION_1_GPIO_PIN;
    GPIO_InitType.Mode                          = GPIO_MODE_OUTPUT_PP;
    
    RCC_DIRECTION_1_GPIO_CLOCK_ENABLE();
    HAL_GPIO_Init(DIRECTION_1_GPIO_PORT, &GPIO_InitType);
    
    GPIO_InitType.Pin                           = DIRECTION_2_GPIO_PIN;
    GPIO_InitType.Mode                          = GPIO_MODE_OUTPUT_PP;
    
    RCC_DIRECTION_2_GPIO_CLOCK_ENABLE();
    HAL_GPIO_Init(DIRECTION_2_GPIO_PORT, &GPIO_InitType);
    
    GPIO_InitType.Pin                           = ENABLEMENT_1_GPIO_PIN;
    GPIO_InitType.Mode                          = GPIO_MODE_OUTPUT_PP;
    
    RCC_ENABLEMENT_1_GPIO_CLOCK_ENABLE();
    HAL_GPIO_Init(ENABLEMENT_1_GPIO_PORT, &GPIO_InitType);
    
    GPIO_InitType.Pin                           = ENABLEMENT_2_GPIO_PIN;
    
    RCC_ENABLEMENT_2_GPIO_CLOCK_ENABLE();
    HAL_GPIO_Init(ENABLEMENT_2_GPIO_PORT, &GPIO_InitType);
}

 void PWM_MspInitCallback(TIM_HandleTypeDef *htim)
 {
     if (TIMER == htim -> Instance)
     {
         RCC_TIMER_CLOCK_ENABLE();
         
         EnableGPIOs();
     }
 }