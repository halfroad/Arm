#include "../Include/BrushedMotor.h"
#include "../Include/PID.h"
#include "../Include/VelocityEncoder.h"
#include "../../../Middlewares/Third_Party/UpperHostCommunications/Include/UpperHostCommunications.h"

/*

    PD12    TIM4_CH1    PM2_ENCA    AF2     TIM4_CH1
    PD13    TIM4_CH2    PM2_ENCB    AF2     TIM4_CH2

*/

#define ENCODER_GPIO_PORT                                   GPIOD
#define ENCODER_GPIO_PORT_CLOCK_ENABLE()                    do { __HAL_RCC_GPIOD_CLK_ENABLE(); } while (0)
#define ENCODER_GPIO_PIN_A                                  GPIO_PIN_12
#define ENCODER_GPIO_PIN_B                                  GPIO_PIN_13


#define ENCODER_TIMER                                       TIM4
#define ENCODER_TIMER_CLOCK_ENABLE()                        do { __HAL_RCC_TIM4_CLK_ENABLE(); } while (0)
#define ENCODER_GPIO_PIN_A_ALTERNATE_FUNCTION               GPIO_AF2_TIM4
#define ENCODER_GPIO_PIN_A_CHANNEL                          TIM_CHANNEL_1

#define ENCODER_GPIO_PIN_B_ALTERNATE_FUNCTION               GPIO_AF2_TIM4
#define ENCODER_GPIO_PIN_B_CHANNEL                          TIM_CHANNEL_2

#define ENCODER_TIMER_IRQN                                  TIM4_IRQn
#define VELOCITY_ENCODER_TIMER_IRQHANDLER                   TIM4_IRQHandler

#define CALCULATOR_TIMER                                    TIM5
#define CALCULATOR_TIMER_CLOCK_ENABLE()                     do { __HAL_RCC_TIM5_CLK_ENABLE(); } while (0)
#define CALCULATOR_TIMER_IRQN                               TIM5_IRQn
#define VELOCITY_CALCULATOR_TIMER_IRQHANDLER                TIM5_IRQHandler

#define SAMPLING_CYCLE                                      50
#define MOTOR_PEAK_SPEED                                    8400

extern PIDTypeDef PIDType;
extern MotorControlProtocol motorControlProtocol;

TIM_HandleTypeDef Encoder_TIM_HandleType = { 0 };
TIM_HandleTypeDef Calculator_TIM_HandleType = { 0 };

uint16_t encoderCountOverflow = 0;

extern void Error_Handler(void);
extern void ComputeVelocity(int32_t code, uint8_t milliseconds);

void EncoderMspInitCallback(TIM_HandleTypeDef *htim);
void CalculatorTimerMspInitCallback(TIM_HandleTypeDef *htim);
void PeriodElapsedCallback(TIM_HandleTypeDef *htim);
static uint32_t AcquireCounterNumber(void);

void InitGPIOPs(void)
{
    GPIO_InitTypeDef GPIO_InitType = { 0 };
    
    ENCODER_GPIO_PORT_CLOCK_ENABLE();
    
    GPIO_InitType.Pin = ENCODER_GPIO_PIN_A;
    GPIO_InitType.Mode = GPIO_MODE_AF_PP;
    GPIO_InitType.Pull = GPIO_NOPULL;
    GPIO_InitType.Speed = GPIO_SPEED_HIGH;
    GPIO_InitType.Alternate = ENCODER_GPIO_PIN_A_ALTERNATE_FUNCTION;
    
    HAL_GPIO_Init(ENCODER_GPIO_PORT, &GPIO_InitType);
    
    GPIO_InitType.Pin = ENCODER_GPIO_PIN_B;
    GPIO_InitType.Alternate = ENCODER_GPIO_PIN_B_ALTERNATE_FUNCTION;
    
    HAL_GPIO_Init(ENCODER_GPIO_PORT, &GPIO_InitType);
}

void InitVelocityEncoder(uint32_t prescaler, uint32_t autoReload)
{
    ENCODER_TIMER_CLOCK_ENABLE();
    
    TIM_Encoder_InitTypeDef TIM_Encoder_InitType = { 0 };

    Encoder_TIM_HandleType.Instance = ENCODER_TIMER;
    Encoder_TIM_HandleType.Init.Prescaler = prescaler;
    Encoder_TIM_HandleType.Init.Period = autoReload;
    Encoder_TIM_HandleType.Init.ClockDivision= TIM_CLOCKDIVISION_DIV1;
    
    Encoder_TIM_HandleType.Encoder_MspInitCallback = EncoderMspInitCallback;
    /* Encoder_TIM_HandleType.PeriodElapsedCallback = PeriodElapsedCallback; */
       
    /*
    
    Bits 16, 2, 1, 0 SMS[3:0]: Slave mode selection
    When external signals are selected the active edge of the trigger signal (tim_trgi) is linked to
    the polarity selected on the external input (see Input Control register and Control Register
    description.
    0000: Slave mode disabled - if CEN = ¡®1¡¯ then the prescaler is clocked directly by the
    internal clock.
        0001: Reserved
        0010: Reserved
        0011: Reserved
        0100: Reset Mode - Rising edge of the selected trigger input (tim_trgi) reinitializes the
        counter and generates an update of the registers.
        0101: Gated Mode - The counter clock is enabled when the trigger input (tim_trgi) is high.
        The counter stops (but is not reset) as soon as the trigger becomes low. Both start
        and stop of the counter are controlled.
        0110: Trigger Mode - The counter starts at a rising edge of the trigger tim_trgi (but it is not
        reset). Only the start of the counter is controlled.
        0111: External Clock Mode 1 - Rising edges of the selected trigger (tim_trgi) clock the
        counter.
        1000: Combined reset + trigger mode - Rising edge of the selected trigger input (tim_trgi)
        reinitializes the counter, generates an update of the registers and starts the counter.
        1001: Combined gated + reset mode - The counter clock is enabled when the trigger input
    (tim_trgi) is high. The counter stops and is reset) as soon as the trigger becomes low.
    Both start and stop of the counter are controlled.
    Others: Reserved.
    Note: The gated mode must not be used if tim_ti1f_ed is selected as the trigger input
    (TS=¡¯00100¡¯). Indeed, tim_ti1f_ed outputs 1 pulse for each transition on tim_ti1f,
    whereas the gated mode checks the level of the trigger signal.
    Note: The clock of the slave peripherals (timer, ADC, ...) receiving the tim_trgo signal must be
    enabled prior to receive events from the master timer, and the clock frequency
    (prescaler) must not be changed on-the-fly while triggers are received from the master
    timer.
    
    */
    
    TIM_Encoder_InitType.EncoderMode = TIM_ENCODERMODE_TI12;    /* Both TI1 and TI2 triggered. */
    
    TIM_Encoder_InitType.IC1Polarity = TIM_ICPOLARITY_RISING;
    /* TIM_Encoder_InitType.IC1Polarity = TIM_ICPOLARITY_BOTHEDGE; */
    
    /*
    
    Bits 1:0 CC1S[1:0]: Capture/compare 1 Selection
    This bit-field defines the direction of the channel (input/output) as well as the used input.
        00: CC1 channel is configured as output
        01: CC1 channel is configured as input, tim_ic1 is mapped on tim_ti1
        10: CC1 channel is configured as input, tim_ic1 is mapped on tim_ti2
        11: CC1 channel is configured as input, tim_ic1 is mapped on tim_trc. This mode is working only if an
    internal trigger input is selected through TS bit (TIMx_SMCR register)
    Note: CC1S bits are writable only when the channel is OFF (CC1E = ¡®0¡¯ in TIMx_CCER).
    
    */
    TIM_Encoder_InitType.IC1Selection = TIM_ICSELECTION_DIRECTTI;
    TIM_Encoder_InitType.IC1Prescaler = TIM_ICPSC_DIV1;
    TIM_Encoder_InitType.IC1Filter = 0x0A;
    
    TIM_Encoder_InitType.IC2Polarity = TIM_ICPOLARITY_RISING;
    /*  TIM_Encoder_InitType.IC2Polarity = TIM_ICPOLARITY_BOTHEDGE;  */
    TIM_Encoder_InitType.IC2Selection = TIM_ICSELECTION_DIRECTTI;
    TIM_Encoder_InitType.IC2Prescaler = TIM_ICPSC_DIV1;
    TIM_Encoder_InitType.IC2Filter = 10;
       
    if (HAL_OK != HAL_TIM_Encoder_Init(&Encoder_TIM_HandleType, &TIM_Encoder_InitType))
        Error_Handler();
    
    /**
      * @brief  Starts the TIM Encoder Interface.
      * @param  htim TIM Encoder Interface handle
      * @param  Channel TIM Channels to be enabled
      *          This parameter can be one of the following values:
      *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
      *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
      *            @arg TIM_CHANNEL_ALL: TIM Channel 1 and TIM Channel 2 are selected
      * @retval HAL status
      */
    if (HAL_OK != HAL_TIM_Encoder_Start(&Encoder_TIM_HandleType, ENCODER_GPIO_PIN_A_CHANNEL))
        Error_Handler();
    
    if (HAL_OK != HAL_TIM_Encoder_Start(&Encoder_TIM_HandleType, ENCODER_GPIO_PIN_B_CHANNEL))
        Error_Handler();
    
    /** @brief  Clear the specified TIM interrupt flag.
      * @param  __HANDLE__ specifies the TIM Handle.
      * @param  __FLAG__ specifies the TIM interrupt flag to clear.
      *        This parameter can be one of the following values:
      *            @arg TIM_FLAG_UPDATE: Update interrupt flag
      *            @arg TIM_FLAG_CC1: Capture/Compare 1 interrupt flag
      *            @arg TIM_FLAG_CC2: Capture/Compare 2 interrupt flag
      *            @arg TIM_FLAG_CC3: Capture/Compare 3 interrupt flag
      *            @arg TIM_FLAG_CC4: Capture/Compare 4 interrupt flag
      *            @arg TIM_FLAG_CC5: Compare 5 interrupt flag
      *            @arg TIM_FLAG_CC6: Compare 6 interrupt flag
      *            @arg TIM_FLAG_COM:  Commutation interrupt flag
      *            @arg TIM_FLAG_TRIGGER: Trigger interrupt flag
      *            @arg TIM_FLAG_BREAK: Break interrupt flag
      *            @arg TIM_FLAG_BREAK2: Break 2 interrupt flag
      *            @arg TIM_FLAG_SYSTEM_BREAK: System Break interrupt flag
      *            @arg TIM_FLAG_CC1OF: Capture/Compare 1 overcapture flag
      *            @arg TIM_FLAG_CC2OF: Capture/Compare 2 overcapture flag
      *            @arg TIM_FLAG_CC3OF: Capture/Compare 3 overcapture flag
      *            @arg TIM_FLAG_CC4OF: Capture/Compare 4 overcapture flag
      *            @arg TIM_FLAG_IDX: Index interrupt flag
      *            @arg TIM_FLAG_DIR: Direction change interrupt flag
      *            @arg TIM_FLAG_IERR: Index error interrupt flag
      *            @arg TIM_FLAG_TERR: Transition error interrupt flag
      * @retval The new state of __FLAG__ (TRUE or FALSE).
      */
      __HAL_TIM_CLEAR_FLAG(&Encoder_TIM_HandleType, TIM_IT_UPDATE);
    
    /** @brief  Enable the specified TIM interrupt.
      * @param  __HANDLE__ specifies the TIM Handle.
      * @param  __INTERRUPT__ specifies the TIM interrupt source to enable.
      *          This parameter can be one of the following values:
      *            @arg TIM_IT_UPDATE: Update interrupt
      *            @arg TIM_IT_CC1:   Capture/Compare 1 interrupt
      *            @arg TIM_IT_CC2:  Capture/Compare 2 interrupt
      *            @arg TIM_IT_CC3:  Capture/Compare 3 interrupt
      *            @arg TIM_IT_CC4:  Capture/Compare 4 interrupt
      *            @arg TIM_IT_COM:   Commutation interrupt
      *            @arg TIM_IT_TRIGGER: Trigger interrupt
      *            @arg TIM_IT_BREAK: Break interrupt
      *            @arg TIM_IT_IDX: Index interrupt
      *            @arg TIM_IT_DIR: Direction change interrupt
      *            @arg TIM_IT_IERR: Index error interrupt
      *            @arg TIM_IT_TERR: Transition error interrupt
      * @retval None
      */
    __HAL_TIM_ENABLE_IT(&Encoder_TIM_HandleType, TIM_IT_UPDATE);
}

/**
  * @brief  Initializes the TIM Encoder Interface MSP.
  * @param  htim TIM Encoder Interface handle
  * @retval None
  */
void EncoderMspInitCallback(TIM_HandleTypeDef *htim)
{
  /* Prevent unused argument(s) compilation warning
  UNUSED(htim);
    */
  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_TIM_Encoder_MspInit could be implemented in the user file
   */
    
    if (htim -> Instance == ENCODER_TIMER)
    {
        InitGPIOPs();
               
        HAL_NVIC_SetPriority(ENCODER_TIMER_IRQN, 2, 0);
        HAL_NVIC_EnableIRQ(ENCODER_TIMER_IRQN);
    }
}

void VELOCITY_ENCODER_TIMER_IRQHANDLER(void)
{
    HAL_TIM_IRQHandler(&Encoder_TIM_HandleType);
}

void InitCalculatorTimer(uint32_t prescaler, uint32_t autoReload)
{
    /* TIM_HandleTypeDef Calculator_TIM_HandleType = { 0 }; */
    
    Calculator_TIM_HandleType.Instance = CALCULATOR_TIMER;
    
    Calculator_TIM_HandleType.Init.Prescaler = prescaler;
    Calculator_TIM_HandleType.Init.CounterMode = TIM_COUNTERMODE_UP;
    Calculator_TIM_HandleType.Init.Period = autoReload;
    Calculator_TIM_HandleType.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    
    Calculator_TIM_HandleType.Base_MspInitCallback = CalculatorTimerMspInitCallback;
    /* Calculator_TIM_HandleType.PeriodElapsedCallback = PeriodElapsedCallback; */
    
    HAL_TIM_Base_Init(&Calculator_TIM_HandleType);
    
    HAL_TIM_Base_Start_IT(&Calculator_TIM_HandleType);
    
    __HAL_TIM_CLEAR_IT(&Calculator_TIM_HandleType, TIM_IT_UPDATE);
}

/**
  * @brief  Initializes the TIM Base MSP.
  * @param  htim TIM Base handle
  * @retval None
  */
void CalculatorTimerMspInitCallback(TIM_HandleTypeDef *htim)
{
  /* Prevent unused argument(s) compilation warning 
  UNUSED(htim);
    */

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_TIM_Base_MspInit could be implemented in the user file
   */
    if (htim -> Instance == CALCULATOR_TIMER)
    {
        CALCULATOR_TIMER_CLOCK_ENABLE();
        
        HAL_NVIC_SetPriority(CALCULATOR_TIMER_IRQN, 2, 1);
        HAL_NVIC_EnableIRQ(CALCULATOR_TIMER_IRQN);
        
        InitPID();
    }
}

void VELOCITY_CALCULATOR_TIMER_IRQHANDLER(void)
{
    HAL_TIM_IRQHandler(&Calculator_TIM_HandleType);
}

/**
 * @brief  Period elapsed callback in non blocking mode
 * @note   This function is called  when TIM6 interrupt took place, inside
 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
 * a global variable "uwTick" used as application time base.
 * @param  htim : TIM handle
 * @retval None
 */
void PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    /* USER CODE BEGIN Callback 0 */

    /* USER CODE END Callback 0 */
    if (htim ->Instance == ENCODER_TIMER)
    {
        if (__HAL_TIM_IS_TIM_COUNTING_DOWN(&Encoder_TIM_HandleType))
            encoderCountOverflow --;
        else
            encoderCountOverflow ++;
    }
    else if (htim -> Instance == CALCULATOR_TIMER)
    {
        uint32_t counterNumber = AcquireCounterNumber();
        
        ComputeVelocity(counterNumber, 5);
        
        motorControlProtocol.state = PIDType.TargetValue == 0 ? MotorStateIdle: MotorStateRun;
        
        static uint8_t i = 0;
        
        if (i % SAMPLING_CYCLE == 0)
        {
            if (motorControlProtocol.state == MotorStateRun)
            {
                motorControlProtocol.pulseWidthModulation = ComposePID(&PIDType, motorControlProtocol.velocity);
                
                if (motorControlProtocol.pulseWidthModulation >= MOTOR_PEAK_SPEED - 200)
                    motorControlProtocol.pulseWidthModulation = MOTOR_PEAK_SPEED - 200;
                else if (motorControlProtocol.pulseWidthModulation <= -(MOTOR_PEAK_SPEED - 200))
                    motorControlProtocol.pulseWidthModulation = -(MOTOR_PEAK_SPEED - 200);
                
                AdaptMotorVelocityDirection(motorControlProtocol.pulseWidthModulation);
                
#ifdef UPPER_HOST_COMMUNICATIONS_ENABLED
                
                ReportWave(1, motorControlProtocol.velocity);
                ReportWave(2, PIDType.TargetValue);
                ReportWave(3, motorControlProtocol.pulseWidthModulation * 100 / MOTOR_PEAK_SPEED);
#endif
            }
        }
    }
    /* USER CODE BEGIN Callback 1 */

    /* USER CODE END Callback 1 */
}

static uint32_t AcquireCounterNumber(void)
{
    uint32_t counterNumber = (uint32_t)__HAL_TIM_GET_COUNTER(&Encoder_TIM_HandleType) + encoderCountOverflow * 65535;
    
    return counterNumber;
}
