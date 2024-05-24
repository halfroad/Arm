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

#include "./GPIOs/Include/GPIO.h"

#include "./Displays/DSS1603/Include/OLED.h"
#include "./Wireless/Nrf24xx/Include/Nrf24xx.h"

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

#define TRANSMITTER
/*
#define RECEIVER
*/

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

uint8_t rx_data[PAYLOAD_WIDTH] = { 0 };
uint8_t tx_data[PAYLOAD_WIDTH] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};

uint8_t state                   = 0x00;

uint8_t payload[PAYLOAD_WIDTH];

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
    
#ifdef RECEIVER

    Nrf24xxRxInit(2500, RadioFrequencyDataRate1Mpbs);

#endif

#ifdef TRANSMITTER

    Nrf24xxTxInit(2500, RadioFrequencyDataRate1Mpbs);
    
#endif

    OLED_Init();
    
    /* USER CODE BEGIN 2 */
    
   // EventRecorderInitialize(EventRecordAll, 1U);
   // EventRecorderStart();

    /* USER CODE END 2 */

    /* We should never get here as control is now taken by the scheduler */
    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    
    uint8_t buffer[50];
    uint8_t i = 0, state = 0x01;

    while (1)
    {
        if ((state & 0x08) == 0x08)
        {
            OLED_Clear();
            
            sprintf((char *)buffer, "Received payload: %#x",i);
            
            OLED_ShowString(0, 0, (uint8_t *)buffer, 8, 1);
            OLED_ShowString(0, 0, (uint8_t *)payload, 8, 1);
            
            OLED_Refresh();
            
            state &= ~0xFF;
            state = 0x01;
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

void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_PIN_4 == GPIO_Pin)
    {
        Nrf24xxReceive(payload);
        
        state |= 0x80;
    } 
    else if (GPIO_PIN_1 == GPIO_Pin)
    {
        
    }
}

void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_PIN_13 == GPIO_Pin)
    {
        Nrf24xxTransmit(tx_data);
    }
    
}
