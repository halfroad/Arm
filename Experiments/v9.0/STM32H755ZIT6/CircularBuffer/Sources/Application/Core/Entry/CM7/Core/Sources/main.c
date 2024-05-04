/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
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
#include "../Include/main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "./Initializers/CM7/Include/SystemInitializer.h"

#include "./Peripherals/CM7/Timers/Include/Delay.h"
#include "./Peripherals/CM7/Displays/OLEDs/Include/OLED.h"
#include "./Peripherals/CM7/GPIOs/Include/GPIOs.h"
#include "./Peripherals/CM7/LEDs/Include/LEDs.h"

#include "./Peripherals/CM7/Communications/SerialCommunications/Include/SerialCommunications.h"

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

extern RingBufferTypeDef ringBuffer;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/

void SystemClock_Config(void);
void onByteReceivedHandler(void *protocol);

extern void Error_Handler(void);

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

    /* Enable I-Cache---------------------------------------------------------*/
    SCB_EnableICache();

    /* Enable D-Cache---------------------------------------------------------*/
    SCB_EnableDCache();

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
    InitButtons();
    
    /* USER CODE BEGIN 2 */
    OLED_Init();
    InitSerialCommunications(115200, NULL, onByteReceivedHandler); 

    /* USER CODE END 2 */

    uint8_t i = 0, byte, length = 0;

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    
    char bytes[] = "Hello, USART delivered!\n";
    
    while (1)
    {
        /*  PC13 Blue Push Button.  */
        
        if (0x2000 == (GPIOC -> IDR & 0x2000))
        {
            DelayMs(10);
            
            if (0x2000 == (GPIOC -> IDR & 0x2000))
            {
                /*    0b 10 0000 0000 0000 = 0x2000    */
                while (0x2000 == (GPIOC -> IDR & 0x2000))
                    ;
                
                Tranmist((uint8_t *)bytes, sizeof(bytes));
                
                OLED_Clear();
                OLED_ShowString(0, 0, (uint8_t *)bytes, 8, 1);
                OLED_Refresh();
            }
        }
        
        if (RingBufferReadByte(&byte, &ringBuffer) == RingBufferStateOK)
        {
            Tranmist(ringBuffer.bytesBuffer, length);
        }
        
        DelayMs(1);
        
        if (i ++ == 250)
        {
            ToggleLED(GREED_LED);
            ToggleLED(YELLOW_LED);
            ToggleLED(RED_LED);
            
            i = 0;
        }
        
        

        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
    }
    /* USER CODE END 3 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

void onByteReceivedHandler(void *protocol)
{
}