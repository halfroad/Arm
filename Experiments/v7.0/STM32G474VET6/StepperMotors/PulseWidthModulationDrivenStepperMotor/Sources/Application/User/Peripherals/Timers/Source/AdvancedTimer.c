#include "../Include/AdvancedTimer.h"

#define TIMER                                                       TIM8
#define RCC_TIMER_CLOCK_ENABLE()                                    do  {   __HAL_RCC_TIM8_CLK_ENABLE();    }   while(0)
#define STEPPER_MOTOR_OUTPUT_COMPARE_CHANNEL_TIM_MODE               TIM_OCMODE_PWM2

#define STEPPER_MOTOR1_TIM_OUTPUT_COMPARE_CHANNEL                   TIM_CHANNEL_1
#define STEPPER_MOTOR2_TIM_OUTPUT_COMPARE_CHANNEL                   TIM_CHANNEL_2

/*

STM32G474VET6/Product Specifications/stm32g474ve.pdf
Table 13. Alternate function (continued)
Port                        AF4

PC6                         TIM8_CH1
PC7                         TIM8_CH2

*/
#define STEPPER_MOTOR_TIM_ALTERNATE_FUNCTION                        GPIO_AF4_TIM8

#define STEPPER_MOTOR1_TIM_PULSE_GPIO_PORT                          GPIOC
#define STEPPER_MOTOR1_TIM_PULSE_GPIO_PORT_ENABLE()                 __HAL_RCC_GPIOC_CLK_ENABLE()
#define STEPPER_MOTOR1_TIM_PULSE_GPIO_PIN                           GPIO_PIN_6

#define STEPPER_MOTOR2_TIM_PULSE_GPIO_PORT                          GPIOC
#define STEPPER_MOTOR2_TIM_PULSE_GPIO_PORT_ENABLE()                 __HAL_RCC_GPIOC_CLK_ENABLE()
#define STEPPER_MOTOR2_TIM_PULSE_GPIO_PIN                           GPIO_PIN_7

TIM_HandleTypeDef TIM_HandleType                                    = { 0 };

static void PWM_MspInitCallback(TIM_HandleTypeDef *htim);
static void ConfigurePulseWidthModulationChannel(uint32_t channel, uint16_t pulse);

void InitAdvancedTimer(uint32_t prescaler, uint16_t period)
{
    TIM_HandleType.Instance                                         = TIM8;
    
    TIM_HandleType.Init.Prescaler                                   = prescaler;
    TIM_HandleType.Init.CounterMode                                 = TIM_COUNTERMODE_UP;
    TIM_HandleType.Init.Period                                      = period;
    TIM_HandleType.Init.ClockDivision                               = TIM_CLOCKDIVISION_DIV1;
    TIM_HandleType.Init.RepetitionCounter                           = 0;
    TIM_HandleType.Init.AutoReloadPreload                           = TIM_AUTORELOAD_PRELOAD_DISABLE;
    
    TIM_HandleType.PWM_MspInitCallback                              = PWM_MspInitCallback;
    
    HAL_TIM_PWM_Init(&TIM_HandleType);
    
    ConfigurePulseWidthModulationChannel(STEPPER_MOTOR1_TIM_OUTPUT_COMPARE_CHANNEL, period / 2);
    ConfigurePulseWidthModulationChannel(STEPPER_MOTOR2_TIM_OUTPUT_COMPARE_CHANNEL, period / 2);
}

static void ConfigurePulseWidthModulationChannel(uint32_t channel, uint16_t pulse)
{
    TIM_OC_InitTypeDef TIM_OC_InitType                              = { 0 };
    
    TIM_OC_InitType.OCMode                                          = STEPPER_MOTOR_OUTPUT_COMPARE_CHANNEL_TIM_MODE;
    TIM_OC_InitType.Pulse                                           = pulse;
    
    TIM_OC_InitType.OCPolarity                                      = TIM_OCPOLARITY_HIGH;
    TIM_OC_InitType.OCNPolarity                                     = TIM_OCNPOLARITY_HIGH;
    
    TIM_OC_InitType.OCIdleState                                     = TIM_OCIDLESTATE_RESET;
    TIM_OC_InitType.OCNIdleState                                    = TIM_OCNIDLESTATE_RESET;
    
    TIM_OC_InitType.OCFastMode                                      = TIM_OCFAST_DISABLE;
    
    /*  Initializes the TIM PWM  channels according to the specified
  *         parameters in the TIM_OC_InitTypeDef.   */
    HAL_TIM_PWM_ConfigChannel(&TIM_HandleType, &TIM_OC_InitType, channel);
}

static void PWM_MspInitCallback(TIM_HandleTypeDef *htim)
{
    if (htim -> Instance == TIMER)
    {
        RCC_TIMER_CLOCK_ENABLE();
        
        GPIO_InitTypeDef GPIO_InitType                              = { 0 };
        
        GPIO_InitType.Pin                                           = STEPPER_MOTOR1_TIM_PULSE_GPIO_PIN;
        GPIO_InitType.Mode                                          = GPIO_MODE_AF_PP;
        GPIO_InitType.Pull                                          = GPIO_NOPULL;
        GPIO_InitType.Alternate                                     = GPIO_AF4_TIM8;
        
        STEPPER_MOTOR1_TIM_PULSE_GPIO_PORT_ENABLE();
        HAL_GPIO_Init(STEPPER_MOTOR1_TIM_PULSE_GPIO_PORT, &GPIO_InitType);
        
        GPIO_InitType.Pin                                           = STEPPER_MOTOR2_TIM_PULSE_GPIO_PIN;
        
        STEPPER_MOTOR2_TIM_PULSE_GPIO_PORT_ENABLE();
        HAL_GPIO_Init(STEPPER_MOTOR2_TIM_PULSE_GPIO_PORT, &GPIO_InitType);
    }
}

void StartPulseWidthModulation(StepperMotorNumbers stepperMotorNumber)
{
    switch (stepperMotorNumber)
    {
        case StepperMotorNumber0:
            HAL_TIM_PWM_Start(&TIM_HandleType, STEPPER_MOTOR1_TIM_OUTPUT_COMPARE_CHANNEL);
        break;
        
        case StepperMotorNumber1:
            HAL_TIM_PWM_Start(&TIM_HandleType, STEPPER_MOTOR2_TIM_OUTPUT_COMPARE_CHANNEL);
        break;
        
        default:
            break;
    }
}

void StopPulseWidthModulation(StepperMotorNumbers stepperMotorNumber)
{
    switch (stepperMotorNumber)
    {
        case StepperMotorNumber0:
            HAL_TIM_PWM_Stop(&TIM_HandleType, STEPPER_MOTOR1_TIM_OUTPUT_COMPARE_CHANNEL);
        break;
        
        case StepperMotorNumber1:
            HAL_TIM_PWM_Stop(&TIM_HandleType, STEPPER_MOTOR2_TIM_OUTPUT_COMPARE_CHANNEL);
        break;
        
        default:
            break;
    }
}