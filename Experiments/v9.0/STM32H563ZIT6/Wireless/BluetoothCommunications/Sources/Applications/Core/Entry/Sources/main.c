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
#include <stdio.h>

#include "./Miscellaneous/Include/dcache.h"
#include "./Miscellaneous/Include/icache.h"
#include "./Miscellaneous/Include/memorymap.h"

#include "./Initializers/Include/SystemInitializer.h"

#include "./Peripherals/GPIOs/Include/GPIO.h"
#include "./Peripherals/Displays/DSS1603/Include/OLED.h"
#include "./Peripherals/SerialBlueTooth/Include/ZS040.h"

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

extern CircularBufferTypeDef circularBufferType;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/

static void OnBytesReceivedCallback(void);

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
    
    InitSerialBlueTooth(9600, OnBytesReceivedCallback);
    
    /* USER CODE BEGIN 2 */
    
    EventRecorderInitialize(EventRecordAll, 1U);
    EventRecorderStart();

    /* USER CODE END 2 */

    /* We should never get here as control is now taken by the scheduler */
    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    
    uint8_t i, byte     = 0;
    char buffer[50]     = { 0 };
    
    while (1)
    {
        if (0x2000 == (GPIOC -> IDR & 0x2000))
        {
            HAL_Delay(10);
            
            if (0x2000 == (GPIOC -> IDR & 0x2000))
            {
                /*    0b 10 0000 0000 0000 = 0x2000    */
                while (0x2000 == (GPIOC -> IDR & 0x2000))
                    ;
                
                OLED_Clear();
                
                OLED_ShowString(0, 0, (uint8_t *)"Send message: ", 8, 1);
                
                sprintf(buffer, "Hello world! - {%d}.", i);
                
                OLED_ShowString(0, 10, (uint8_t *)buffer, 8, 1);
                
                Tranmist((uint8_t *)buffer, sizeof(buffer));
                
                OLED_Refresh();
            }
        }
        
        if (++ i == 25)
        {
            OLED_ShowString(0, 20, (uint8_t *)"Received message: ", 8, 1);
            OLED_ShowString(0, 30, circularBufferType.bytesBuffer, 8, 1);
            
            OLED_Refresh();
            
            ToggleLED(GreenLED);
            ToggleLED(YellowLED);
            ToggleLED(RedLED);
            
            i = 0;
        }
        
        HAL_Delay(10);
        
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
    }
    /* USER CODE END 3 */
}

static void OnBytesReceivedCallback(void)
{
    
}
