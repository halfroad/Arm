/**
 ******************************************************************************
 * @file    Templates_LL/TrustZoneDisabled/Src/main.c
 * @author  MCD Application Team
 * @brief   Main program body through the LL API
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32H5xx_LL_Examples
 * @{
 */

/** @addtogroup Templates_LL
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
extern void system_init(void);
extern void GPIO_Init(void);
extern void flash_led(void);

extern void usart_gpio_port_init(void);

/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Main program
 * @param  None
 * @retval None
 */
int main(void)
{
	/* Enable the Instruction Cache
	CACHE_Enable();
	*/

	/* Configure the system clock to 250 MHz
	SystemClock_Config();
	 */
	/* Add your application code here */
	
	system_init();
	
	GPIO_Init();
	
	usart_gpio_port_init();
	
	/* Infinite loop */
	while (1)
	{
		flash_led();
	}
}

/* ==============   BOARD SPECIFIC CONFIGURATION CODE END      ============== */

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
 * @}
 */

/**
 * @}
 */
