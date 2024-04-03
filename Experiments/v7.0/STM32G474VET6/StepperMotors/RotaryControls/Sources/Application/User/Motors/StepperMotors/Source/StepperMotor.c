#include <stm32g4xx.h>

#include "../../../Peripherals/Timers/Include/StepperMotorNumbers.h"
#include "../Include/StepperMotor.h"

#define STEPPER_MOTOR1_DIRECTION_GPIO_PORT                                          GPIOF
#define RCC_STEPPER_MOTOR1_DIRECTION_GPIO_PORT_ENABLE()                             do  {   __HAL_RCC_GPIOF_CLK_ENABLE();   }   while(0)
#define STEPPER_MOTOR1_DIRECTION_GPIO_PIN                                           GPIO_PIN_2

#define STEPPER_MOTOR1_ENABLE_GPIO_PORT                                             GPIOF
#define RCC_STEPPER_MOTOR1_ENABLE_GPIO_PORT_ENABLE()                                do  {   __HAL_RCC_GPIOF_CLK_ENABLE();   }   while(0)
#define STEPPER_MOTOR1_ENABLE_GPIO_PIN                                              GPIO_PIN_9

#define STEPPER_MOTOR2_DIRECTION_GPIO_PORT                                          GPIOE
#define RCC_STEPPER_MOTOR2_DIRECTION_GPIO_PORT_ENABLE()                             do  {   __HAL_RCC_GPIOE_CLK_ENABLE();   }   while(0)
#define STEPPER_MOTOR2_DIRECTION_GPIO_PIN                                           GPIO_PIN_5

#define STEPPER_MOTOR2_ENABLE_GPIO_PORT                                             GPIOE
#define RCC_STEPPER_MOTOR2_ENABLE_GPIO_PORT_ENABLE()                                do  {   __HAL_RCC_GPIOE_CLK_ENABLE();   }   while(0)
#define STEPPER_MOTOR2_ENABLE_GPIO_PIN                                              GPIO_PIN_6

#define CONFIGURE_GPIO_STATE(port, pin, state)                                      do  {   HAL_GPIO_WritePin(port, pin, state);    }   while(0)
    
void InitStepperMotor(void)
{
    GPIO_InitTypeDef GPIO_InitType = { 0 };
    
    GPIO_InitType.Pin = STEPPER_MOTOR1_DIRECTION_GPIO_PIN;
    GPIO_InitType.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitType.Pull = GPIO_PULLDOWN;
    
    RCC_STEPPER_MOTOR1_DIRECTION_GPIO_PORT_ENABLE();
    HAL_GPIO_Init(STEPPER_MOTOR1_DIRECTION_GPIO_PORT, &GPIO_InitType);
    
    GPIO_InitType.Pin = STEPPER_MOTOR1_ENABLE_GPIO_PIN;
    
    RCC_STEPPER_MOTOR1_ENABLE_GPIO_PORT_ENABLE();
    HAL_GPIO_Init(STEPPER_MOTOR1_ENABLE_GPIO_PORT, &GPIO_InitType);
    
    GPIO_InitType.Pin = STEPPER_MOTOR2_DIRECTION_GPIO_PIN;
    
    RCC_STEPPER_MOTOR2_DIRECTION_GPIO_PORT_ENABLE();
    HAL_GPIO_Init(STEPPER_MOTOR2_DIRECTION_GPIO_PORT, &GPIO_InitType);
    
    GPIO_InitType.Pin = STEPPER_MOTOR2_ENABLE_GPIO_PIN;
    
    RCC_STEPPER_MOTOR2_ENABLE_GPIO_PORT_ENABLE();
    HAL_GPIO_Init(STEPPER_MOTOR2_ENABLE_GPIO_PORT, &GPIO_InitType);
}

void DivertStepperMotor(StepperMotorNumbers stepperMotorNumber, StepperMotorDirections stepperMotorDirection)
{
    switch (stepperMotorNumber)
    {
        case StepperMotorNumber0:
        {
            switch (stepperMotorDirection)
            {
                case StepperMotorDirectionClockwise:
                    CONFIGURE_GPIO_STATE(STEPPER_MOTOR1_DIRECTION_GPIO_PORT, STEPPER_MOTOR1_DIRECTION_GPIO_PIN, GPIO_PIN_SET);
                    break;
                
                case StepperMotorDirectionAntiClockwise:
                    CONFIGURE_GPIO_STATE(STEPPER_MOTOR1_DIRECTION_GPIO_PORT, STEPPER_MOTOR1_DIRECTION_GPIO_PIN, GPIO_PIN_RESET);
                    break;
                
                default:
                    break;
            }
        }
            break;
        
        case StepperMotorNumber1:
        {
            switch (stepperMotorDirection)
            {
                case StepperMotorDirectionClockwise:
                    CONFIGURE_GPIO_STATE(STEPPER_MOTOR2_DIRECTION_GPIO_PORT, STEPPER_MOTOR2_DIRECTION_GPIO_PIN, GPIO_PIN_SET);
                    break;
                
                case StepperMotorDirectionAntiClockwise:
                    CONFIGURE_GPIO_STATE(STEPPER_MOTOR2_DIRECTION_GPIO_PORT, STEPPER_MOTOR2_DIRECTION_GPIO_PIN, GPIO_PIN_RESET);
                    break;
                
                default:
                    break;
            }
        }
            break;
                
        default:
            break;
    }
}

void ActivateStepperMotor(StepperMotorNumbers stepperMotorNumber, StepperMotorStates stepperMotorState)
{
    switch (stepperMotorNumber)
    {
        case StepperMotorNumber0:
        {
            switch (stepperMotorState)
            {
                case StepperMotorStateActive:
                    CONFIGURE_GPIO_STATE(STEPPER_MOTOR1_ENABLE_GPIO_PORT, STEPPER_MOTOR1_ENABLE_GPIO_PIN, GPIO_PIN_SET);
                    break;
                
                case StepperMotorStateInactive:
                    CONFIGURE_GPIO_STATE(STEPPER_MOTOR1_ENABLE_GPIO_PORT, STEPPER_MOTOR1_ENABLE_GPIO_PIN, GPIO_PIN_SET);
                    break;
                
                default:
                    break;
            }
        }
            break;
        
        case StepperMotorNumber1:
        {
            switch (stepperMotorState)
            {
                case StepperMotorStateActive:
                    CONFIGURE_GPIO_STATE(STEPPER_MOTOR2_ENABLE_GPIO_PORT, STEPPER_MOTOR2_ENABLE_GPIO_PIN, GPIO_PIN_SET);
                    break;
                
                case StepperMotorStateInactive:
                    CONFIGURE_GPIO_STATE(STEPPER_MOTOR2_ENABLE_GPIO_PORT, STEPPER_MOTOR2_ENABLE_GPIO_PIN, GPIO_PIN_SET);
                    break;
                
                default:
                    break;
            }
        }
            break;
                
        default:
            break;
    }
}
