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

#include <EventRecorder.h>

#include "./Miscellaneous/Include/dcache.h"
#include "./Miscellaneous/Include/icache.h"
#include "./Miscellaneous/Include/memorymap.h"

#include "./Initializers/Include/SystemInitializer.h"

#include "./Peripherals/GPIOs/Include/GPIO.h"
#include "./Peripherals/Displays/DSS1603/Include/OLED.h"

#include "./Peripherals/DigitalAnalogyConverter/Include/DigitalAnalogyConverter.h"

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
    
    MX_DCACHE1_Init();
    MX_ICACHE_Init();
    
    OLED_Init();
    
    InitDigitalAnalogyConverter();
    
    /* USER CODE BEGIN 2 */
    
    EventRecorderInitialize(EventRecordAll, 1U);
    EventRecorderStart();

    /* USER CODE END 2 */

    /* We should never get here as control is now taken by the scheduler */
    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    
    uint8_t i           = 0;
    uint16_t voltage    = 0;
    
    while (1)
    {
        HAL_Delay(250);
        
        i ++;
        
        if (i == 5)
        {
            AssignOutputVoltage(2300 + i);
        }
        else if (i == 10)
        {
            voltage = AcquireAssignedVoltage();
            
            OLED_Clear();
            
            OLED_ShowString(0, 0, (uint8_t *)"Volt Acquired: ", 8, 1);
            OLED_ShowNum(85, 0, voltage, 5, 8, 1);
            
            OLED_Refresh();
            
            i = 0;
        }
        
        ToggleLED(GreenLED);
        ToggleLED(YellowLED);
        ToggleLED(RedLED);
        
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
    }
    /* USER CODE END 3 */
}
