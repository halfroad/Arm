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

#include "./Initializers/Include/SystemInitializer.h"
#include "./Buttons/Include/PushButton.h"
#include "./LEDs/Include/LED.h"
#include "./Communications/SerialCommunications/Include/SerialCommunications.h"

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

// #define TRANSMITTER
#define RECEIVER

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

uint8_t rx_data[PAYLOAD_WIDTH] = { 0 };
uint8_t tx_data[PAYLOAD_WIDTH] = { 0, 1, 2, 3, 4, 5, 6, 7 };

uint8_t state                   = 0x00;

uint8_t payload[PAYLOAD_WIDTH];

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
    
#ifdef RECEIVER

    Nrf24xxRxInit(2500, RadioFrequencyDataRate1Mpbs);

#endif

#ifdef TRANSMITTER

    Nrf24xxTxInit(2500, RadioFrequencyDataRate1Mpbs);
    
#endif
        
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
    uint8_t i, j = 0;
    
    const uint8_t TEXT[] = { "Communications via Nrf24L01P." };
    const uint32_t FLASH_VOLUME = 16 * 1024 * 1024;
    
    uint8_t buffer[sizeof(TEXT)];
    
    while (1)
    {
        state = ScanButton();

        if (state == KEY_0_PRESSED)
        {
            // change tx datas
            for (j = 0; j < 8; j ++)
                tx_data[j] ++;

            // transmit
            Nrf24xxTransmit(tx_data);
            
            printf("Payload transmitted.\n");
        }
        else if (state == KEY_1_PRESSED)
        {                        
            printf("Bytes are read: %s\n", (char *)buffer);
        }
        else if (state == KEY_2_PRESSED)
        {
        }
        
        HAL_Delay(10);
        
        if ( i++ == 25)
        {
            ToggleLED(LightChoiceGreenLED);
            
            i = 0;
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

void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_PIN_8 == GPIO_Pin)
    {
        Nrf24xxReceive(payload);
        
        state |= 0x80;
    }
}