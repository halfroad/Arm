#include "stm32h5xx_ll_pwr.h"
#include "stm32h5xx_ll_rcc.h"
#include "stm32h5xx_ll_system.h"
#include "stm32h5xx_ll_utils.h"
#include "stm32h5xx_ll_icache.h"

static void SystemClock_Config(void);
static void CACHE_Enable(void);

void system_init(void)
{
	SystemClock_Config();
	CACHE_Enable();
}

/* ==============   BOARD SPECIFIC CONFIGURATION CODE BEGIN    ============== */
/**
 * @brief  System Clock Configuration
 *         The system Clock is configured as follows :
 *            System Clock source            = PLL (HSE BYPASS)
 *            SYSCLK(Hz)                     = 250000000  (CPU Clock)
 *            HCLK(Hz)                       = 250000000  (Bus matrix and AHBs Clock)
 *            AHB Prescaler                  = 1
 *            APB1 Prescaler                 = 1 (APB1 Clock  250MHz)
 *            APB2 Prescaler                 = 1 (APB2 Clock  250MHz)
 *            APB3 Prescaler                 = 1 (APB3 Clock  250MHz)
 *            HSE Frequency(Hz)              = 8000000
 *            PLL_M                          = 4
 *            PLL_N                          = 250
 *            PLL_P                          = 2
 *            PLL_Q                          = 2
 *            PLL_R                          = 2
 *            VDD(V)                         = 3.3
 *            Flash Latency(WS)              = 5
 * @param  None
 * @retval None
 */
static void SystemClock_Config(void)
{

	/* Set the regulator supply output voltage */
	LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE0);
	while (LL_PWR_IsActiveFlag_VOS() == 0)
	{
	}

	/* Enable HSE oscillator */
	LL_RCC_HSE_EnableBypass();
	LL_RCC_HSE_SetExternalClockType(LL_RCC_HSE_DIGITAL_TYPE);
	LL_RCC_HSE_Enable();
	while (LL_RCC_HSE_IsReady() != 1)
	{
	}

	/* Set FLASH latency */
	LL_FLASH_SetLatency(LL_FLASH_LATENCY_5);

	/* Configure PLL clock source */
	LL_RCC_PLL1_SetSource(LL_RCC_PLL1SOURCE_HSE);

	/* Main PLL configuration and activation */
	LL_RCC_PLL1P_Enable();
	LL_RCC_PLL1Q_Enable();
	LL_RCC_PLL1R_Enable();
	LL_RCC_PLL1FRACN_Disable();
	LL_RCC_PLL1_SetVCOInputRange(LL_RCC_PLLINPUTRANGE_2_4);
	LL_RCC_PLL1_SetVCOOutputRange(LL_RCC_PLLVCORANGE_WIDE);
	LL_RCC_PLL1_SetM(4);
	LL_RCC_PLL1_SetN(250);
	LL_RCC_PLL1_SetP(2);
	LL_RCC_PLL1_SetQ(2);
	LL_RCC_PLL1_SetR(2);

	LL_RCC_PLL1_Enable();
	while (LL_RCC_PLL1_IsReady() != 1)
	{
	}

	/* Set AHB, APB1, APB2 and APB3 prescalers */
	LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
	LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
	LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
	LL_RCC_SetAPB3Prescaler(LL_RCC_APB3_DIV_1);

	/* Set PLL1 as System Clock Source */
	LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL1);
	while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL1)
	{
	}

	/* Set systick to 1ms with frequency set to 250MHz */
	LL_Init1msTick(250000000);

	/* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
	LL_SetSystemCoreClock(250000000);
}

/**
 * @brief  Enable ICACHE with 2-ways set-associative configuration.
 * @param  None
 * @retval None
 */
static void CACHE_Enable(void)
{
	/* Enable instruction cache (default 2-ways set associative cache) */
	LL_ICACHE_Enable();
}