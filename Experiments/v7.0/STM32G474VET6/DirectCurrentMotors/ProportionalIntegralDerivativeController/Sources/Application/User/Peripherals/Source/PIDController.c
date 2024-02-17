#include "../Include/PID.h"
#include "../Include/PIDController.h"

#ifdef USE_POSITIONAL_PID

#define KP                                                              8.50f
#define KI                                                              5.00f
#define KD                                                              0.10f

#else

#define KP                                                              10.00f
#define KI                                                              6.00f
#define KD                                                              0.50f

#endif

#define SAMPLING_CYCLE                                                  50  /* Unit milliseconds. */

#define PID_UPDATE_TIMER                   TIM2
#define ENABLE_PID_UPDATE_TIMER_CLOCK()    do { __HAL_RCC_TIM2_CLK_ENABLE(); } while(0)
#define PID_UPDATE_TIMER_IRQN              TIM2_IRQn
#define PID_UPDATE_TIMER_IRQHANDLER        TIM2_IRQHandler

static void InitUpdateTimer(uint32_t prescaler, uint32_t period);

static void PIDControllerUpdateTimerMspInitCallback(TIM_HandleTypeDef *htim);
void PIDControllerUpdateTimerPeriodElapsedCallback(TIM_HandleTypeDef *htim);

PIDTypeDef PIDType = { 0 };
TIM_HandleTypeDef TIM_HandleType =  { 0 };

void InitPIDController(uint32_t prescaler, uint32_t period)
{
    PIDType.TargetValue = 0.0f;
    PIDType.ActualValue = 0.0f;
    
    PIDType.ProportionalFactor = KP;
    PIDType.IntegralFactor = KI;
    PIDType.DerivativeFactor = KD;
    
    PIDType.LastSecondError = 0.0f;
    PIDType.LastFirstError = 0.0f;
    PIDType.Error = 0.0f;
    
    PIDType.AccumulativeErrors = 0.0f;
    
    /*
    Use Encoder instead.
    InitUpdateTimer(prescaler, period);
    */
}

static void InitUpdateTimer(uint32_t prescaler, uint32_t period)
{
    TIM_HandleType.Instance = PID_UPDATE_TIMER;
    
    TIM_HandleType.Init.Prescaler = prescaler;
    TIM_HandleType.Init.CounterMode = TIM_COUNTERMODE_UP;
    TIM_HandleType.Init.Period = period;
    TIM_HandleType.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    
    TIM_HandleType.Base_MspInitCallback = PIDControllerUpdateTimerMspInitCallback;
    /* TIM_HandleType.PeriodElapsedCallback = PIDControllerUpdateTimerPeriodElapsedCallback; */
    
    HAL_TIM_Base_Init(&TIM_HandleType);
    
    HAL_TIM_Base_Start_IT(&TIM_HandleType);
    __HAL_TIM_CLEAR_IT(&TIM_HandleType, TIM_IT_UPDATE);
    
}

static void PIDControllerUpdateTimerMspInitCallback(TIM_HandleTypeDef *htim)
{
    if (htim -> Instance == PID_UPDATE_TIMER)
    {
        ENABLE_PID_UPDATE_TIMER_CLOCK();
        
        HAL_NVIC_SetPriority(PID_UPDATE_TIMER_IRQN, 2, 2);
        HAL_NVIC_EnableIRQ(PID_UPDATE_TIMER_IRQN);
    }
}

void PIDControllerUpdateTimerPeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    uint8_t i = 0;
}

void PID_UPDATE_TIMER_IRQHANDLER(void)
{
    HAL_TIM_IRQHandler(&TIM_HandleType);
}