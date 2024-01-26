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
#include "../../../User/Peripherals/Include/GPIO.h"
#include "../../../User/Peripherals/Include/BasicTimer.h"

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
    
    /*
    
    If the Core Clock Frequency is 170Mhz£¬as is 170 000 000Hz.

    When the prescaler is 170 - 1:
    
    The frequency of Timer will be divided into 170 000 000 / 170 = 1 000 000 = 1Mhz£¬that means the cycle is 0.000001s = 1us
    To count down 1 second, the counter has to be counted as 1/0.000001 = 1 000 000 times¡£


    When the prescaler is 170 000 - 1:
    
    The frequency of Timer will be divided into 170 00 000 / 170 000 = 1000Hz = 1KHz£¬that means the cycle is 0.001s = 1ms¡£
    To count down 1 second, the counter has to be counted as 1 / 0.001 = 1 000 times.

    */
    
    /*
    
    To acquire a Timer with divided frequency is 1000Hz = 1kHz, the SystemCoreClock is 170 000 000Hz = 170MHz,
    the precaler has to be 170 000 - 1 = 169 999 = __HAL_TIM_CALC_PSC(SystemCoreClock == 170 000 000, Frequency to acquire = 1000Hz = 1kHz)
    
    */
    uint32_t precaler = __HAL_TIM_CALC_PSC(SystemCoreClock, 1e3);
    uint32_t period = __HAL_TIM_CALC_PERIOD(SystemCoreClock, precaler, 1e3);
    
    InitBasicTimer(precaler, 1e5);
    
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
   
    while (1)
    {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
    }
    /* USER CODE END 3 */
}

