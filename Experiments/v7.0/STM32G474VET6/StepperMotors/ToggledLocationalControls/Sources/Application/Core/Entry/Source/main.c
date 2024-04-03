/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <EventRecorder.h>

#include "../../Initializers/Include/SystemInitializer.h"
#include "../../../Drivers/BSP/Buttons/Include/PushButton.h"
#include "../../../Drivers/BSP/LEDs/Include/LED.h"
#include "../../../User/Motors/StepperMotors/Include/StepperMotorController.h"

#include "../Include/main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

extern StepperMotorTypeDef stepperMotor;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
    /* USER CODE BEGIN 1 */

    /* USER CODE END 1 */

    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    
    InitLEDs();
    InitPushButtons();
    
    InitStepperMotorController();
    
    EventRecorderInitialize(EventRecordAll, 1U);
    EventRecorderStart();
    
    /* USER CODE BEGIN 2 */

    /* USER CODE END 2 */

    /* Init scheduler */
    /* Call init function for freertos objects (in freertos.c) */
    /* Start scheduler 
    InitFreeRTOS();
    */
    /* We should never get here as control is now taken by the scheduler */
    /* Infinite loop */
    /* USER CODE BEGIN WHILE */

    KeyPressStates state;
    float angle                                     = 0.0f;
    uint8_t i                                       = 0;
    
    while (1)
    {
        state                                       = ScanButton();

        if (state == KEY_0_PRESSED)
        {
            if (stepperMotor.state == StateIdle ||
                stepperMotor.state == StateStopped)
            {
                angle                               += 90.0f;
                
                if (angle >= 0.0f)
                {
                    stepperMotor.angle             = angle;
                    stepperMotor.rotaryDirection   = RotaryDirectionClockwise;
                }
                else
                {
                    stepperMotor.angle             = -angle;
                    stepperMotor.rotaryDirection   = RotaryDirectionAntiClockwise;
                }
                
                printf("Rotary Angle: %.2f.\n", angle);
            }
            
            /*
            
            switch (motorNumber)
            {
                case ConnectorNumberPseudo:
                    motorNumber             = ConnectorNumber0;
                break;
                
                case ConnectorNumber0:
                    motorNumber             = ConnectorNumber1;
                break;
                
                case ConnectorNumber1:
                    motorNumber             = ConnectorNumber0;
                break;
            }
            */
        }
        else if (state == KEY_1_PRESSED)
        {
            if (stepperMotor.state == StateIdle ||
                stepperMotor.state == StateStopped)
            {
                angle                               -= 90.0f;
                
                if (angle >= 0.0f)
                {
                    stepperMotor.angle             = angle;
                    stepperMotor.rotaryDirection   = RotaryDirectionClockwise;
                }
                else
                {
                    stepperMotor.angle             = -angle;
                    stepperMotor.rotaryDirection   = RotaryDirectionAntiClockwise;
                }
                
                printf("Rotary Angle: %.2f.\n", angle);
            }
        }
        else if (state == KEY_2_PRESSED)
        {
            if (stepperMotor.state == StateIdle ||
                stepperMotor.state == StateStopped)
            {
                angle                               = 0.0f;
                
                AssignAngle(stepperMotor.number, stepperMotor.rotaryDirection, stepperMotor.angle);
            }
        }
        
        HAL_Delay(10);
        
        if ( i++ % 10 == 0)
        {
            ToggleLED(LightChoiceRedLED);
            ToggleLED(LightChoiceGreenLED);
        }
        
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
    }
    /* USER CODE END 3 */
}