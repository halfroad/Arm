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
#include "./Peripherals/SerialCommunications/Include/SerialCommunications.h"
#include "./ControllerAreaNetwork/Include/ControllerAreaNetwork.h"

#include "./Entry/Include/main.h"

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

#define FDCAN_BUFFER_LENGTH                 8

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
    uint8_t i, j                        = 0;
    
    FDCANOperatingModes operatingMode   = FDCANOperatingModeNone;
    
    uint8_t buffer[FDCAN_BUFFER_LENGTH] = { 0 };
    uint8_t receivedBytesLength         = 0;
    
    while (1)
    {
        state = ScanButton();

        if (state == KEY_0_PRESSED)
        {
            switch (operatingMode)
            {
                case FDCANOperatingModeNone:
                    operatingMode = FDCANOperatingModeInternalLoopback;
                    break;
                
                case FDCANOperatingModeInternalLoopback:
                    operatingMode = FDCANOperatingModeExternalLoopback;
                    break;
                
                case FDCANOperatingModeExternalLoopback:
                    operatingMode = FDCANOperatingModeNormal;
                    break;
                
                case FDCANOperatingModeNormal:
                    operatingMode = FDCANOperatingModeInternalLoopback;
                    break;
                
                default:
                    break;
            }
            
        }
        else if (state == KEY_1_PRESSED)
        {
            switch (operatingMode)
            {               
                case FDCANOperatingModeInternalLoopback:
                    
                    InitControllerAreaNetworkOnInternalLoopbackMode(17, 8, 11, 8);
                
                    break;
                
                case FDCANOperatingModeExternalLoopback:
                    
                    InitControllerAreaNetworkOnExternalLoopbackMode(17, 8, 11, 8);
                
                    break;
                
                case FDCANOperatingModeNormal:
                    
                    InitControllerAreaNetworkOnNormalMode(17, 8, 11, 8);
                
                    break;
                
                default:
                    break;
            }
        }
        else if (state == KEY_2_PRESSED)
        {
            switch (operatingMode)
            {               
                case FDCANOperatingModeInternalLoopback:
                case FDCANOperatingModeExternalLoopback:
                case FDCANOperatingModeNormal:
                {
                    for (j = 0; j < FDCAN_BUFFER_LENGTH; j ++)
                    {
                        buffer[j] = j + 'A';
                        
                        printf("buffer[%d] = %#x.\n", j, (j + 'A'));
                    }
                    
                    if (HAL_OK == FDCANTransmitFixedLengthMessage(buffer))
                        printf("Buffer transmitted.\n");
                    else
                        printf("Failed to transmit buffer = %s.\n", buffer);
                    }
                
                    break;
                
                default:
                    break;
            }
        }
        
        if (HAL_OK == FDCANReceiveMessage(buffer, &receivedBytesLength))
        {
            printf("New buffer available, the received bytes length = %d, and the buffer = [", receivedBytesLength);

            for (j = 0; j < receivedBytesLength; j ++)
            {
                if (j == receivedBytesLength - 1)
                    printf("%#x", buffer[j]);
                else
                    printf("%#x, ", buffer[j]);
            }

            printf("].\n");
                 
        }
        
        HAL_Delay(10);
        
        if ( i++ % 20 == 0)
        {
            ToggleLED(LightChoiceRedLED);
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