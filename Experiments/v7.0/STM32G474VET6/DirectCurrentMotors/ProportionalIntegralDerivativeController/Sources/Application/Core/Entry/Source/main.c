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
#include "../../../User/Peripherals/Include/gpio.h"
#include "../../../User/Peripherals/Include/AdvancedTimer.h"
#include "../../../User/Peripherals/Include/BrushedMotor.h"
#include "../../../User/Peripherals/Include/VelocityEncoder.h"
#include "../../../User/Peripherals/Include/PIDController.h"
#include "../../../Drivers/BSP/Include/PushButton.h"

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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/

/* USER CODE BEGIN PFP */

extern Motor_TypeDef Motor_Type;

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
    MX_GPIO_Init();
    
    InitAdvancedTimer(0, 8500 - 1, 0x00);
    InitDirectCurrenBrushedtMotor();
    
    InitVelocityEncoder(0, 0xFFFF);
    InitCalculatorTimer(170 - 1, 1000 - 1);
    InitPIDController(170 - 1, 1000 - 1);
    
    InitPushButtons();
    
    EventRecorderInitialize(EventRecordAll, 1U);
    EventRecorderStart();
    
    /*
    
   Auto Reload Duration = 170 000 000 / 85 00 = 20 000, Cycle Duration = 1 / 20 000 seconds = 50 us.
    */
    //InitDirectCurrenBrushedtMotor();
    
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

    PushButtonPressStates state;
    uint8_t i = 0;
    int32_t pwm = 0;
    
    while (1)
    {
        state = ScanButton();
        
        if (state == PUSH_BUTTON_0_PRESSED)
        {
            pwm += 400;
            
            if (pwm == 0)
                StopMotor();
            else
            {
                StartMotor();
                
                if (pwm > 8400)
                    pwm = 8400;
            }
            
            AdaptMotorVelocityDirection(pwm);
            
        }
        else if (state == PUSH_BUTTON_1_PRESSED)
        {
            pwm -= 400;
            
            if (pwm == 0)
                StopMotor();
            else
            {
                StartMotor();
                
                if (pwm < -8400)
                    pwm = -8400;
            }
            
            AdaptMotorVelocityDirection(pwm);
        }
        else if (state == PUSH_BUTTON_2_PRESSED)
        {
            HAL_GPIO_TogglePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin);
            
            StopMotor();
            
            pwm = 0;
            
            AdaptMotorVelocityDirection(pwm);
        }
        
        if (i++ % 50 == 0)
        {
            HAL_GPIO_TogglePin(RED_LED_GPIO_Port, RED_LED_Pin);
            
            printf("The velocity of motor now is %.1f RPM.\n", Motor_Type.velocity);
        }
        
        HAL_Delay(10);
        
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
    }
    /* USER CODE END 3 */
}

