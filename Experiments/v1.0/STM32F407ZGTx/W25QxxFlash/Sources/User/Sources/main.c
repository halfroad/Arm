/**
  ******************************************************************************
  * @file    GPIO/GPIO_IOToggle/main.c 
  * @author  MCD Application Team
  * @version V1.8.1
  * @date    27-January-2022
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 0 STMicroelectronics.
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

#include "delay.h"
#include "usart.h"
#include "w25qxx.h"

/** @addtogroup STM32F4xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup GPIO_IOToggle
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
	/*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       files (startup_stm32f40_41xxx.s/startup_stm32f427_437xx.s/startup_stm32f429_439xx.s)
       before to branch to application main. 
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f4xx.c file
	*/
	
	delay_init(168);
	usart_init(115200);
	w25qxx_init();
	
	/* To achieve GPIO toggling maximum frequency, the following  sequence is mandatory. 
     You can monitor PF9 or PF10 on the scope to measure the output signal. 
     If you need to fine tune this frequency, you can add more GPIO set/reset 
     cycles to minimize more the infinite loop timing.
     This code needs to be compiled with high speed optimization option.
	 */
	/*
	uint8_t bytes[] = "Hello Serial Peripheral Interfaces.";
	uint32_t length = sizeof bytes;
	uint32_t result = w25qxx_flash_page_write(0x00, bytes, length);
	uint32_t success = (4 + length) * 0xFF;
	
	uint8_t readBytes[100] = {0};
	*/
	
	while (1)
	{
		/*
		if (success == result)
		{
			w25qxx_flash_read(0x00, readBytes, 10);
		
			printf("%s\n", readBytes);
			
			delay_ms(500);
		}
		*/
		
		uint16_t identifier = w25qxx_flash_read_identifier();
		
		printf("%ud\n", identifier);
			
		delay_ms(500);
	}
}

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
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

