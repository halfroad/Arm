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

#include "../../Initializers/Include/SystemInitializer.h"
#include "../../../User/RTOS/Tasks/Include/TasksWrapper.h"
#include "../../../User/Peripherals/Include/gpio.h"
#include "../../../User/Peripherals/Include/AdvancedTimer.h"
#include "../../../User/Peripherals/Include/DirectCurrenBrushedtMotor.h"
#include "../../../Drivers/BSP/Include/Key.h"

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
    InitPushButtons();
    
    int precaler = __HAL_TIM_CALC_PSC(SystemCoreClock, 1000);
    int period = __HAL_TIM_CALC_PERIOD(SystemCoreClock, precaler, 2000);
    
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

    PressedPushButtons key;
    uint8_t i = 0;
    int32_t pwm = 0;
    
    while (1)
    {
        key = KeyScan();
        
        if (key == PUSH_BUTTON_PRESSED_0)
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
            
            AdaptMotorSpeedDirection(pwm);
            
        }
        else if (key == PUSH_BUTTON_PRESSED_1)
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
            
            AdaptMotorSpeedDirection(pwm);
        }
        else if (key == PUSH_BUTTON_PRESSED_2)
        {
            HAL_GPIO_TogglePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin);
            
            StopMotor();
            
            pwm = 0;
            
            AdaptMotorSpeedDirection(pwm);
        }
        
        HAL_Delay(10);
        
        if ( i++ % 20 == 0)
            HAL_GPIO_TogglePin(RED_LED_GPIO_Port, RED_LED_Pin);
        
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
    }
    /* USER CODE END 3 */
}

