#include <stm32g4xx.h>

#include "../Include/SystemInitializer.h"

#define RCC_OSCILLIATOR_PLL_M                       RCC_PLLM_DIV2
#define RCC_OSCILLIATOR_PLL_N                       85
#define RCC_OSCILLIATOR_PLL_P                       RCC_PLLP_DIV2
#define RCC_OSCILLIATOR_PLL_Q                       RCC_PLLQ_DIV2
#define RCC_OSCILLIATOR_PLL_R                       RCC_PLLR_DIV2

void Error_Handler(void);

/*
* @brief System Clock Configuration
* @retval None
*/
void SystemClockConfig(void)
{
    /*
    Configure the main internal regulator output voltage.
    */
    HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);
    
    /*
    Initialiazes the RCC Oscillators according to the specified parameters in the RCC_InitTypeDef structure.
    */
    RCC_OscInitTypeDef RCC_OscInitType = { 0 };
    
    RCC_OscInitType.OscillatorType      = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitType.HSEState            = RCC_HSE_ON;
    
    RCC_OscInitType.PLL.PLLState        = RCC_PLL_ON;
    RCC_OscInitType.PLL.PLLSource       = RCC_PLLSOURCE_HSE;
    /*
    PLL (SYS_CLK) = HSE_OSCILLIATOR_FREQUENCY (8MHz) * PLL_N (85) / (PLL_M (RCC_PLLM_DIV2) = 8 * 85 / 2 / 2 = 170MHz.
    */
    RCC_OscInitType.PLL.PLLM            = RCC_OSCILLIATOR_PLL_M;
    RCC_OscInitType.PLL.PLLN            = RCC_OSCILLIATOR_PLL_N;
    RCC_OscInitType.PLL.PLLP            = RCC_OSCILLIATOR_PLL_P;
    RCC_OscInitType.PLL.PLLQ            = RCC_OSCILLIATOR_PLL_Q;
    RCC_OscInitType.PLL.PLLR            = RCC_OSCILLIATOR_PLL_R;
    
    if (HAL_OK != HAL_RCC_OscConfig(&RCC_OscInitType))
        Error_Handler();
    
    /*
    Initializes the CPU, AHB, APB buses clocks.
    */
    RCC_ClkInitTypeDef RCC_ClkInitType = { 0 };
    
    RCC_ClkInitType.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitType.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    /*!< SYSCLK not divided */
    RCC_ClkInitType.AHBCLKDivider = RCC_SYSCLK_DIV1;
     /*!< HCLK not divided */
    RCC_ClkInitType.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitType.APB2CLKDivider = RCC_HCLK_DIV1;
    
    /*
    Table 19. Number of wait states according to CPU clock (HCLK) frequency
    
    Wait states (WS)(LATENCY)                                           HCLK (MHz)
                                    VCORE Range 1 boost mode    VCORE Range 1 normal mode       VCORE Range 2
    
    0 WS (1 CPU cycles)                     ¡Ü 34                        ¡Ü 30                    ¡Ü 12
    1 WS (2 CPU cycles)                     ¡Ü 68                        ¡Ü 60                    ¡Ü 24
    2 WS (3 CPU cycles)                     ¡Ü 102                       ¡Ü 90                    ¡Ü 26
    3 WS (3 CPU cycles)                     ¡Ü 136                       ¡Ü 120
    4 WS (5 CPU cycles)                     ¡Ü 70                        ¡Ü 150
    
    Flash access control register (FLASH_ACR)
    
    Bits 3:0 LATENCY[3:0]: Latency
    These bits represent the ratio of the SYSCLK (system clock) period to the Flash
    access time.
        0000: Zero wait state
        0001: One wait state
        0010: Two wait states
        0011: Three wait states
        0100: Four wait states
        ...1111: Fifteen wait states
    */
    /*! $define FLASH_LATENCY_4           FLASH_ACR_LATENCY_4WS    < FLASH Four wait states 
    */
    if (HAL_OK != HAL_RCC_ClockConfig(&RCC_ClkInitType, FLASH_LATENCY_4))
        Error_Handler();
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1)
    {
    }
    /* USER CODE END Error_Handler_Debug */
}

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
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */