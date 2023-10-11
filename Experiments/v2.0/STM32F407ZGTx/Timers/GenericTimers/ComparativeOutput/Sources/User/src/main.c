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

#include "ComparativeOutput.h"


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

/**	1
  * @brief  Main prograq7m
  * @param  None
  * @retval None
  */
int main(void)
{
	/*
	
	At this stage the microcontroller clock setting is already configured, 
	this is done through SystemInit() function which is called from startup
	files (startup_stm32f40_41xxx.s/startup_stm32f427_437xx.s/startup_stm32f429_439xx.s)
	before to branch to application main.
	To reconfigure the default setting of SystemInit() function, refer to system_stm32f4xx.c file.
	
	*/
	
	generate_pwm_waves();
	
	while (1)
	{
		/*
		static uint16_t comparation = 0;
		
		assign_pwm_comparation(comparation ++);
		
		if (comparation == __UINT16_MAX__)
			comparation = 0;
		*/
	}
}
