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

uint16_t rounds              = 1;
uint32_t acceleration       = 25;
uint32_t velocity           = 1000;
uint32_t deceleration       = 20;

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
    
    uint8_t i = 0;
    
    while (1)
    {
        state = ScanButton();

        if (state == KEY_0_PRESSED)
        {
            rounds ++;
            
            if (rounds > 100)
                rounds = 1;
            
            /*
            SelectMotorNumber(motorNumber);
            
            motorNumber ^= 0x01 << 0;
            */
            printf("Rounds = %d.\n", rounds);
            
        }
        else if (state == KEY_1_PRESSED)
        {
            rounds --;
            
            if (rounds < 1)
                rounds = 100;
            
            printf("steps = %d.\n", rounds);
        }
        else if (state == KEY_2_PRESSED)
        {
            ApplyTrapezoidalMotions(rounds, acceleration, velocity, deceleration);
            
            rounds          = 1;
            acceleration    = 25;
            velocity        = 1000;
            deceleration    = 20;
        }
        
        HAL_Delay(10);
        
        if ( i++ % 25 == 0)
        {
            ToggleLED(LightChoiceRedLED);
            ToggleLED(LightChoiceGreenLED);
        }
        
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
    }
    /* USER CODE END 3 */
}