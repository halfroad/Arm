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

#include "./Initializers/Include/SystemInitializer.h"
#include "./Buttons/Include/PushButton.h"
#include "./LEDs/Include/LED.h"
#include "./Displays/OLEDs/Include/OLED.h"
#include "./Communications/SerialCommunications/Include/RS485.h"

#include "../Include/main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define OLED_MAXIMUM_BUFFER_LENGTH              21

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

uint8_t buffer[OLED_MAXIMUM_BUFFER_LENGTH]      = { 0 };
uint8_t eventBits                               = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/

/* USER CODE BEGIN PFP */

static void onBytesReceivedHandler(void *protocol, uint8_t *bytes, uint16_t length);

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
    OLED_Init();
    
    InitRS485(onBytesReceivedHandler);
    
    InitPushButtons();
    
    //EventRecorderInitialize(EventRecordAll, 1U);
    //EventRecorderStart();
    
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
            memset(buffer, 0x00, OLED_MAXIMUM_BUFFER_LENGTH);
            
            sprintf((char *)buffer, "Hello, world! - %d", i);
            
            OLED_Clear();
            
            OLED_ShowString(0, 0, buffer, 8, 1);
            OLED_Refresh();
        }
        else if (state == KEY_1_PRESSED)
        {
            memset(buffer, 0x00, OLED_MAXIMUM_BUFFER_LENGTH);
            
            sprintf((char *)buffer, "Hello RS485! - %d.\n", i);
            
            SendRS485Message((char *)buffer);
        }
        else if (state == KEY_2_PRESSED)
        {
            
        }
        
        if (eventBits & (0x01 << 7))
        {
            OLED_Clear();
            OLED_ShowString(0, 0, buffer, 8, 1);
            OLED_Refresh();
            
            eventBits &= ~(0x01 << 7);
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

static void onBytesReceivedHandler(void *protocol, uint8_t *bytes, uint16_t length)
{
    memset(buffer, 0x00, OLED_MAXIMUM_BUFFER_LENGTH);
    
    if (length > OLED_MAXIMUM_BUFFER_LENGTH)
        memcpy(buffer, bytes, OLED_MAXIMUM_BUFFER_LENGTH);
    else
        memcpy(buffer, bytes, length);
    
    eventBits &= ~(0x01 << 7);
    eventBits |= 0x01 << 7;
}