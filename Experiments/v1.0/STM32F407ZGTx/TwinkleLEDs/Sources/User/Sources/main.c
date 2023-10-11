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

/** @addtogroup STM32F4xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup GPIO_IOToggle
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
GPIO_InitTypeDef  GPIO_InitStructure;

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

  /* GPIOG Peripheral clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);

  /* Configure PF9 and PF10 in output pushpull mode */
  GPIO_InitStructure.GPIO_Pin = LED1_PIN | LED2_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOF, &GPIO_InitStructure);

  /* To achieve GPIO toggling maximum frequency, the following  sequence is mandatory. 
     You can monitor PF9 or PF10 on the scope to measure the output signal. 
     If you need to fine tune this frequency, you can add more GPIO set/reset 
     cycles to minimize more the infinite loop timing.
     This code needs to be compiled with high speed optimization option.  */  
  while (1)
  {
	/*
	  #define LED1_PIN GPIO_Pin_9,
	  #define GPIO_Pin_9	((uint16_t)0x0200),	0b0000 0010, 0000 0000,	low 16 bits
	  #define GPIO_Pin_10	((uint16_t)0x0400,	0b0000 0100, 0000 0000,	low 16 bits
	  
	  GPIO_Pin_9 | GPIO_Pin_10, 0b0000 0110, 0000 0000. For those low 16 bit (0 ~ 15) pins whose bits are 1,
	  GPIOx->BSRR will set the GPIO port to 1 (high level). The ports will not be changed for the bits are 0.
	  GPIOF->BSRR = 0b0000 0110, 0000 0000, the GPIOF ports at bit 9 and 10 will be set to 1, the bits equal 0 will keep unchanged.
	*/
    /* Set PF9 and PF10 */
	GPIOF->BSRR = (uint32_t)(LED1_PIN | LED2_PIN);
    /* Reset PF9 and PF10 */
	/*
	  (uint32_t)((LED1_PIN | LED2_PIN)<< 16), 0b0000 0110, 0000 0000 << 16,
	  0b0000 0110, 0000 0000, 0000 0000, 0000 0000. For those high 16 bit (16 ~ 31) pins whose bits are
	  are 1 , i.e. 16 ~ 31, GPIOx->BSRR will reset the GPIO port to 0. the bits equal 0 will keep unchanged. 
	  */
    GPIOF->BSRR = (uint32_t)((LED1_PIN | LED2_PIN)<< 16);

    /* Set PF9 and PF10 */
    GPIOF->BSRR = (uint32_t)(LED1_PIN | LED2_PIN);
    /* Reset PF9 and PF10 */
    GPIOF->BSRR = (uint32_t)((LED1_PIN | LED2_PIN)<< 16);

    /* Set PF9 and PF10 */
    GPIOG->BSRR = (uint32_t)(LED1_PIN | LED2_PIN);
    /* Reset PF9 and PF10 */
    GPIOG->BSRR = (uint32_t)((LED1_PIN | LED2_PIN)<< 16);

    /* Set PF9 and PF10 */
    GPIOF->BSRR = (uint32_t)(LED1_PIN | LED2_PIN);
    /* Reset PF9 and PF10 */
    GPIOF->BSRR = (uint32_t)((LED1_PIN | LED2_PIN)<< 16);

    /* Set PF9 and PF10 */
    GPIOF->BSRR = (uint32_t)(LED1_PIN | LED2_PIN);
    /* Reset PF9 and PF10 */
    GPIOF->BSRR = (uint32_t)((LED1_PIN | LED2_PIN)<< 16);;

    /* Set PF9 and PF10 */
    GPIOF->BSRR = (uint32_t)(LED1_PIN | LED2_PIN);
    /* Reset PF9 and PF10 */
    GPIOF->BSRR = (uint32_t)((LED1_PIN | LED2_PIN)<< 16);

    /* Set PF9 and PF10 */
    GPIOF->BSRR = (uint32_t)(LED1_PIN | LED2_PIN);
    /* Reset PF9 and PF10 */
    GPIOF->BSRR = (uint32_t)((LED1_PIN | LED2_PIN)<< 16);

    /* Set PF9 and PF10 */
    GPIOF->BSRR = (uint32_t)(LED1_PIN | LED2_PIN);
    /* Reset PF9 and PF10 */
    GPIOF->BSRR = (uint32_t)((LED1_PIN | LED2_PIN)<< 16);

    /* Set PF9 and PF10 */
    GPIOF->BSRR = (uint32_t)(LED1_PIN | LED2_PIN);
    /* Reset PF9 and PF10 */
    GPIOF->BSRR = (uint32_t)((LED1_PIN | LED2_PIN)<< 16);

    /* Set PF9 and PF10 */
    GPIOF->BSRR = (uint32_t)(LED1_PIN | LED2_PIN);
    /* Reset PF9 and PF10 */
    GPIOF->BSRR = (uint32_t)((LED1_PIN | LED2_PIN)<< 16);
  }
}

#ifdef  USE_FULL_ASSERT

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

