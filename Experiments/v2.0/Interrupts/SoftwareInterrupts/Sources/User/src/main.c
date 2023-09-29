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
#include "SoftwareInterrupts.h"

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
	
	init_delay(168);
	init_software_interrupt_line_0();
	
	/*
	
	To achieve GPIO toggling maximum frequency, the following  sequence is mandatory. 
     You can monitor PF9 or PF10 on the scope to measure the output signal. 
     If you need to fine tune this frequency, you can add more GPIO set/reset 
     cycles to minimize more the infinite loop timing.
     This code needs to be compiled with high speed optimization option.
	 
	*/  
	
	while (1)
	{
		trigger_software_interrupt();
		
		delay_ms(1000);
	}
}
