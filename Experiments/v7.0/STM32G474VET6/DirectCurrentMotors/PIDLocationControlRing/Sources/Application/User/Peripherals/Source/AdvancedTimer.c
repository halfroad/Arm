#include "../Include/AdvancedTimer.h"

#define TIMER_CHANNEL_GPIO_CLOCK_ENABLE()                                               do { __HAL_RCC_GPIOC_CLK_ENABLE(); } while(0)

#define TIMER_CHANNEL_GPIO_PORT                                                         GPIOC
#define TIMER_CHANNEL_GPIO_PIN                                                          GPIO_PIN_6
#define TIMER_CHANNEL_GPIO_PORT_ALTERNATE_FUNCTION                                      GPIO_AF4_TIM8

#define TIMER_COMPLEMENTARY_CHANNEL_GPIO_CLOCK_ENABLE()                                 do { __HAL_RCC_GPIOC_CLK_ENABLE(); } while(0)

#define TIMER_COMPLEMENTARY_CHANNEL_GPIO_PORT                                           GPIOC
#define TIMER_COMPLEMENTARY_CHANNEL_GPIO_PIN                                            GPIO_PIN_10
// #define TIMER_CIMPLEMENTARY_CHANNEL_GPIO_PORT_ALTERNATE_FUNCTION                        GPIO_AF4_TIM8_COMP1

#define ADVANCE_TIMER_INSTANCE                                                          TIM8
#define ADVANCE_TIMER_CHANNEL                                                           TIM_CHANNEL_1
#define TIMER_CLOCK_ENABLE()                                                            do { __HAL_RCC_TIM8_CLK_ENABLE(); } while(0)

TIM_HandleTypeDef TIM8_Handle;

void InitAdvancedTimer(uint16_t prescaler, uint16_t autoReload,uint16_t defaultCompare)
{
    /* Initialize the Adavanced Timer TIM1.
    TIM8_HandleTypeDef TIM8_Handle = { 0 };
    */
    
    TIMER_CLOCK_ENABLE();
    
    TIM8_Handle.Instance = ADVANCE_TIMER_INSTANCE;
    
    /* TIM8_Handle.Init.Prescaler = __HAL_TIM_CALC_PSC(SystemCoreClock, 1000); */
    TIM8_Handle.Init.Prescaler = prescaler;
    TIM8_Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
    TIM8_Handle.Init.Period = autoReload;
    TIM8_Handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    TIM8_Handle.Init.RepetitionCounter = 0;
    TIM8_Handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    
    HAL_TIM_PWM_Init(&TIM8_Handle);
    
    TIM_OC_InitTypeDef TIM_OC_InitType = { 0 };
    
    /* Configure the output of Pulse Width Modulation.*/
    TIM_OC_InitType.OCMode = TIM_OCMODE_PWM1;
    TIM_OC_InitType.Pulse = defaultCompare;
    TIM_OC_InitType.OCPolarity = TIM_OCPOLARITY_LOW;
    TIM_OC_InitType.OCNPolarity = TIM_OCNPOLARITY_LOW;
    TIM_OC_InitType.OCFastMode = TIM_OCFAST_ENABLE;
    TIM_OC_InitType.OCIdleState = TIM_OCIDLESTATE_RESET;
    TIM_OC_InitType.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    
    HAL_TIM_PWM_ConfigChannel(&TIM8_Handle, &TIM_OC_InitType, ADVANCE_TIMER_CHANNEL);
    
    /* Configure the Break Dead Time.
    TIMx break and dead-time register (TIMx_BDTR)(x = 16 to 17

    Bit 11 OSSR: Off-state selection for Run mode
    This bit is used when MOE=1 on channels that have a complementary output which are
    configured as outputs. OSSR is not implemented if no complementary output is implemented
    in the timer.
    See tim_oc1/tim_oc1n enable description for more details (Section 30.8.8: TIMx
    capture/compare enable register (TIMx_CCER)(x = 16 to 17) on page 1424).
    0: When inactive, tim_oc1/tim_oc1n outputs are disabled (the timer releases the output
    control which is taken over by the GPIO, which forces a Hi-Z state)
    1: When inactive, tim_oc1/tim_oc1n outputs are enabled with their inactive level as soon as
    CC1E=1 or CC1NE=1 (the output is still controlled by the timer).
    Note: This bit can not be modified as soon as the LOCK level 2 has been programmed (LOCK
    bits in TIMx_BDTR register).
    Bit 10 OSSI: Off-state selection for Idle mode
    This bit is used when MOE=0 on channels configured as outputs.
    See tim_oc1/tim_oc1n enable description for more details (Section 30.8.8: TIMx
    capture/compare enable register (TIMx_CCER)(x = 16 to 17) on page 1424).
    0: When inactive, tim_oc1/tim_oc1n outputs are disabled (tim_oc1/tim_oc1n enable output
    signal=0)
    1: When inactive, tim_oc1/tim_oc1n outputs are forced first with their idle level as soon as
    CC1E=1 or CC1NE=1. tim_oc1/tim_oc1n enable output signal=1)
    Note: This bit can not be modified as soon as the LOCK level 2 has been programmed (LOCK
    bits in TIMx_BDTR register)
    */
    
    TIM_BreakDeadTimeConfigTypeDef TIM_BreakDeadTimeConfigType = { 0 };
    
    /*
    
    Bug in HAL:
    
    Bit 2 CC1NE: Capture/compare 1 complementary output enable
        0: Off - tim_oc1n is not active. tim_oc1n level is then function of MOE, OSSI, OSSR, OIS1,
        OIS1N and CC1E bits.
        1: On - tim_oc1n signal is output on the corresponding output pin depending on MOE, OSSI,
    OSSR, OIS1, OIS1N and CC1E bits.
    Note: On channels having a complementary output, this bit is preloaded. If the CCPC bit is
    set in the TIMx_CR2 register then the CC1NE active bit takes the new value from the
    preloaded bit only when a Commutation event is generated.
    
    */
    ADVANCE_TIMER_INSTANCE -> CCER |= 0x02;
    
    TIM_BreakDeadTimeConfigType.OffStateRunMode = TIM_OSSR_ENABLE;
    TIM_BreakDeadTimeConfigType.OffStateIDLEMode = TIM_OSSI_DISABLE;
    TIM_BreakDeadTimeConfigType.LockLevel = TIM_LOCKLEVEL_OFF;
    TIM_BreakDeadTimeConfigType.DeadTime = 0x0F;
    TIM_BreakDeadTimeConfigType.BreakState = TIM_BREAK_DISABLE;
    TIM_BreakDeadTimeConfigType.BreakPolarity = TIM_BREAKPOLARITY_LOW;
    TIM_BreakDeadTimeConfigType.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
    
    HAL_TIMEx_ConfigBreakDeadTime(&TIM8_Handle, &TIM_BreakDeadTimeConfigType);
}

static void InitGPIOs()
{
    /*
    
    Utilize the Motor Driver Board Port 1.
    7
    Initialize the TIM1_CH1 (PA8)  and its complementary output TIM1_CH1N (PB3).
    
    */
    
    GPIO_InitTypeDef GPIO_InitType = { 0 };   
    
    GPIO_InitType.Pin = TIMER_CHANNEL_GPIO_PIN;
    GPIO_InitType.Mode = GPIO_MODE_AF_PP;
    GPIO_InitType.Pull = GPIO_NOPULL;
    GPIO_InitType.Speed = GPIO_SPEED_HIGH;
    
    /*
    
    Product Specifications/stm32g474ve.
    Table 13. Alternate function
    */
    GPIO_InitType.Alternate = TIMER_CHANNEL_GPIO_PORT_ALTERNATE_FUNCTION;
    
    TIMER_CHANNEL_GPIO_CLOCK_ENABLE();
    HAL_GPIO_Init(TIMER_CHANNEL_GPIO_PORT, &GPIO_InitType);
    
    GPIO_InitType.Pin = TIMER_COMPLEMENTARY_CHANNEL_GPIO_PIN;
    /* GPIO_InitType.Alternate = TIMER_CIMPLEMENTARY_CHANNEL_GPIO_PORT_ALTERNATE_FUNCTION; */
    
    TIMER_COMPLEMENTARY_CHANNEL_GPIO_CLOCK_ENABLE();
    HAL_GPIO_Init(TIMER_COMPLEMENTARY_CHANNEL_GPIO_PORT, &GPIO_InitType);
    
    /*
    GPIO_InitType.Pin = TIMER_BRAKE_GPIO__PIN;
    
    TIMER_BRAKE_GPIO_CLK_ENABLE();
    HAL_GPIO_Init(TIMER_BRAKE_GPIO_PORT, &GPIO_InitType);
    */
}

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{

    if (htim -> Instance == ADVANCE_TIMER_INSTANCE)
        InitGPIOs();

}

