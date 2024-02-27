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
#include "../../../User/Peripherals/Include/BrushedMotor.h"
#include "../../../Middlewares/Third_Party/UpperHostCommunications/Include/UpperHostCommunications.h"
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

extern MotorControlProtocol motorControlProtocol;
extern PIDTypeDef PIDType;

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
    
    InitMotor();
    InitUpperHostCommunications();
    
    InitPushButtons();
    
    EventRecorderInitialize(EventRecordAll, 1U);
    EventRecorderStart();
    
    /*
    
   Auto Reload Duration = 170 000 000 / 85 00 = 20 000, Cycle Duration = 1 / 20 000 seconds = 50 us.
    */
    //InitMotor();
    
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
            PIDType.TargetValue += 10;
            
            if (PIDType.TargetValue >= MOTOR_PEAK_SPEED)
                PIDType.TargetValue = MOTOR_PEAK_SPEED;            
        }
        else if (state == KEY_1_PRESSED)
        {
            PIDType.TargetValue -= 10;
            
            if (PIDType.TargetValue <= -MOTOR_PEAK_SPEED)
                PIDType.TargetValue = -MOTOR_PEAK_SPEED;
        }
        else if (state == KEY_2_PRESSED)
        {
            PIDType.TargetValue = 0;
            
            HAL_GPIO_TogglePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin);
        }
        
        HAL_Delay(10);
        
        if ( i++ % 50 == 0)
        {
            HAL_GPIO_TogglePin(RED_LED_GPIO_Port, RED_LED_Pin);
            
            /*  printf("The velocity of motor now is %.1f RPM.\n", motorControlProtocol.velocity);   */
        }
        
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
    }
    /* USER CODE END 3 */
}