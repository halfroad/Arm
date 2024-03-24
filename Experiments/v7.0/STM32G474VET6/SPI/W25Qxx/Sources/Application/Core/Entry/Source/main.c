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

#include "../../Initializers/Include/SystemInitializer.h"
#include "../../../Drivers/BSP/Buttons/Include/PushButton.h"
#include "../../../Drivers/BSP/LEDs/Include/LED.h"
#include "../../../User/Peripherals/SerialCommunications/Include/SerialCommunications.h"
#include "../../../User/Peripherals/SerialPeripheralInterfaces/W25Qxx/Include/W25Qxx.h"

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

void onByteReceivedHandler(void *protocol, uint8_t *byte);

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
    
    InitSerialCommunications(115200, NULL, onByteReceivedHandler);
    
    InitW25Qxx();
        
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
    
    uint16_t identification = ReadManufacturerIdentification();
    
    while (identification == 0x00 || identification == 0xFF)
    {
        ToggleLED(LightChoiceRedLED);
        
        printf("Flash device is not present.\n");
        
        HAL_Delay(250);
    }
    
    printf("Flash device is detected.\n");

    KeyPressStates state;
    uint8_t i = 0;
    
    const uint8_t TEXT[] = { "Communications via Serial Peripheral Interface." };
    const uint32_t FLASH_VOLUME = 16 * 1024 * 1024;
    
    uint8_t buffer[sizeof(TEXT)];
    
    while (1)
    {
        state = ScanButton();

        if (state == KEY_0_PRESSED)
        {
            printf("Start writing the bytes.\n");
            
            sprintf((char *)buffer, "%s", (char *)TEXT);
            W25QxxWrite((uint8_t *)buffer, FLASH_VOLUME - 100, sizeof(TEXT));
            
            printf("Bytes are written.\n");
        }
        else if (state == KEY_1_PRESSED)
        {
            printf("Start reading the bytes.\n");
            
            W25QxxRead(buffer, FLASH_VOLUME - 100, sizeof(TEXT));
            
            printf("Bytes are read: %s\n", (char *)buffer);
        }
        else if (state == KEY_2_PRESSED)
        {
        }
        
        HAL_Delay(10);
        
        if ( i++ % 10 == 0)
        {
            ToggleLED(LightChoiceGreenLED);
        }
        
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
    }
    /* USER CODE END 3 */
}

void onByteReceivedHandler(void *protocol, uint8_t *byte)
{
    uint8_t i = 0;
}