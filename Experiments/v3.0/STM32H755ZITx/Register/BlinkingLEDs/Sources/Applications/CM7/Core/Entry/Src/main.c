#include "stm32h7xx_rcc.h"
#include "stm32h7xx_bus.h"
#include "stm32h7xx_nvic.h"
#include "stm32h7xx_system.h"
#include "stm32h7xx_pwr.h"
#include "stm32h7xx_utils.h"

#include "gpio.h"

void ConfigSystemClock(void);

int main(void)
{
    /* Enable Data and Instructives Caches. */
    SCB_EnableDCache();
    SCB_EnableICache();
    
    APB4_GRP1_EnableClock(APB4_GRP1_PERIPH_SYSCFG);
    
    NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
    NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 15, 0));
    
    ConfigSystemClock();
    
    init_gpios();
    
    while (1)
    {
        GPIO_TogglePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin);
        
        mDelay(500);
        
    }
}

void ConfigSystemClock(void)
{
    MODIFY_REG (FLASH -> ACR, FLASH_ACR_LATENCY, FLASH_LATENCY_4);
    
    while (FLASH_GetLatency() != FLASH_LATENCY_4)
        ;
    
    PWR_ConfigSupply(PWR_LDO_SUPPLY);
    PWR_SetRegulVoltageScaling(PWR_REGU_VOLTAGE_SCALE0);
    
    while (PWR_IsActiveFlag_VOS() == 0)
        ;
    
    RCC_HSI_Enable();
    
    /* Wait till HSI is ready */
    while (RCC_HSI_IsReady() != 1)
        ;
    
    RCC_HSI_SetCalibTrimming(64);
    RCC_HSI_SetDivider(RCC_HSI_DIV1);
    RCC_PLL_SetSource(RCC_PLLSOURCE_HSI);
    RCC_PLL1_SetVCOInputRange(RCC_PLLINPUTRANGE_8_16);
    RCC_PLL1_SetVCOOutputRange(RCC_PLLVCORANGE_WIDE);
    
    RCC_PLL1_SetM(4);
    RCC_PLL1_SetN(60);
    RCC_PLL1_SetP(2);
    RCC_PLL1_SetQ(2);
    RCC_PLL1_SetR(2);
    
    RCC_PLL1_Enable();
    
    /* Wait till PLL is ready. */
    while (RCC_PLL1_IsReady() != 1)
        ;
    
    /* Intermediate AHB prescaler 2 when targe frequency clock is higher than 80 MHz. */
    RCC_SetAHBPrescaler(RCC_AHB_DIV_2);
    RCC_SetSysClkSource(RCC_SYS_CLKSOURCE_PLL1);
    
    /* Wait till system clock is ready. */
    while (RCC_GetSysClkSource() != RCC_SYS_CLKSOURCE_STATUS_PLL1)
        ;
    
    RCC_SetSysPrescaler(RCC_SYSCLK_DIV_1);
    RCC_SetAHBPrescaler(RCC_AHB_DIV_2);
    RCC_SetAPB1Prescaler(RCC_APB1_DIV_2);
    RCC_SetAPB2Prescaler(RCC_APB2_DIV_2);
    RCC_SetAPB3Prescaler(RCC_APB3_DIV_2);
    RCC_SetAPB4Prescaler(RCC_APB4_DIV_2);
    
    Init1msTick(480000000);
    SetSystemCoreClock(480000000);
}
