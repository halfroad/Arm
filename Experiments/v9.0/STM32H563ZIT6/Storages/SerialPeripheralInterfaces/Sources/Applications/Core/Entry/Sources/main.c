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
#include <string.h>
#include <EventRecorder.h>

#include "./Miscellaneous/Include/dcache.h"
#include "./Miscellaneous/Include/icache.h"
#include "./Miscellaneous/Include/memorymap.h"

#include "./Initializers/Include/SystemInitializer.h"

#include "./Peripherals/GPIOs/Include/GPIO.h"
#include "./Peripherals/Displays/DSS1603/Include/OLED.h"
#include "./Peripherals/Storages/W25Qxx/Include/W25Qxx.h"

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
    
    InitW25Qxx();
    
    OLED_Init();
    
    /* USER CODE BEGIN 2 */
    
   // EventRecorderInitialize(EventRecordAll, 1U);
   // EventRecorderStart();

    /* USER CODE END 2 */

    /* We should never get here as control is now taken by the scheduler */
    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    
    const uint8_t TEXT[]            = { "Communications via Serial Peripheral Interface." };
    const uint32_t FLASH_VOLUME     = 16 * 1024 * 1024;
    
    uint8_t buffer[sizeof(TEXT)];
    
    uint8_t i                       = 0;
    uint8_t state                   = 0x01;
    
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
                
                if ((state & 0x01) == 0x01)
                {
                    sprintf((char *)buffer, "Device Id = %#x", W25QxxReadManufactureIdentifier());
                    
                    OLED_ShowString(0, 0, (uint8_t *)buffer, 8, 1);
                    
                    state &= ~0xFF;
                    state |= 0x02;
                }
                else if ((state & 0x02) == 0x02)
                {
                    sprintf((char *)buffer, "Writing bytes.");
                    OLED_ShowString(0, 10, (uint8_t *)buffer, 8, 1);
                    
                    sprintf((char *)buffer, "%s", (char *)TEXT);
                    
                    W25QxxWrite((uint8_t *)buffer, FLASH_VOLUME - 100, sizeof(TEXT));
                    
                    state &= ~0xFF;
                    state |= 0x04;
                }
                else if ((state & 0x04) == 0x04)
                {
                    W25QxxRead(buffer, FLASH_VOLUME - 100, sizeof(TEXT));
                    
                    OLED_ShowString(0, 20, buffer, 8, 1);
                    
                    state &= ~0xFF;
                    state |= 0x01;
                }
                
                OLED_Refresh();
            }
        }
        
        if (++ i == 25)
        {
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
