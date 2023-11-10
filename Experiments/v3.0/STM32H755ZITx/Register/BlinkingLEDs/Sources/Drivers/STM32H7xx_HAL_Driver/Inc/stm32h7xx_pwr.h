#include "stm32h7xx.h"

/**
  * @}
  */

/** @defgroup PWR_EC_SUPPLY_PWR Power supply source configuration
  * @{
  */
#define PWR_LDO_SUPPLY                     PWR_CR3_LDOEN                                                               /*!< Core domains are supplied from the LDO                                                                     */
#if defined (SMPS)
#define PWR_DIRECT_SMPS_SUPPLY             PWR_CR3_SMPSEN                                                              /*!< Core domains are supplied from the SMPS                                                                    */
#define PWR_SMPS_1V8_SUPPLIES_LDO          (PWR_CR3_SMPSLEVEL_0 | PWR_CR3_SMPSEN | PWR_CR3_LDOEN)                      /*!< The SMPS 1.8V output supplies the LDO which supplies the Core domains                                       */
#define PWR_SMPS_2V5_SUPPLIES_LDO          (PWR_CR3_SMPSLEVEL_1 | PWR_CR3_SMPSEN | PWR_CR3_LDOEN)                      /*!< The SMPS 2.5V output supplies the LDO which supplies the Core domains                                       */
#define PWR_SMPS_1V8_SUPPLIES_EXT_AND_LDO  (PWR_CR3_SMPSLEVEL_0 | PWR_CR3_SMPSEXTHP | PWR_CR3_SMPSEN | PWR_CR3_LDOEN)  /*!< The SMPS 1.8V output supplies an external circuits and the LDO. The Core domains are supplied from the LDO */
#define PWR_SMPS_2V5_SUPPLIES_EXT_AND_LDO  (PWR_CR3_SMPSLEVEL_1 | PWR_CR3_SMPSEXTHP | PWR_CR3_SMPSEN | PWR_CR3_LDOEN)  /*!< The SMPS 2.5V output supplies an external circuits and the LDO. The Core domains are supplied from the LDO */
#define PWR_SMPS_1V8_SUPPLIES_EXT          (PWR_CR3_SMPSLEVEL_0 | PWR_CR3_SMPSEXTHP | PWR_CR3_SMPSEN | PWR_CR3_BYPASS) /*!< The SMPS 1.8V output supplies an external source which supplies the Core domains                            */
#define PWR_SMPS_2V5_SUPPLIES_EXT          (PWR_CR3_SMPSLEVEL_1 | PWR_CR3_SMPSEXTHP | PWR_CR3_SMPSEN | PWR_CR3_BYPASS) /*!< The SMPS 2.5V output supplies an external source which supplies the Core domains                            */
#endif /* SMPS */
#define PWR_EXTERNAL_SOURCE_SUPPLY         PWR_CR3_BYPASS  

/**
  * @}
  */

/** @defgroup PWR_EC_REGU_VOLTAGE Run mode Regulator Voltage Scaling
  * @{
  */
#if defined (PWR_CPUCR_PDDS_D2)
#define PWR_REGU_VOLTAGE_SCALE3      PWR_D3CR_VOS_0                    /*!< Select voltage scale 3 */
#define PWR_REGU_VOLTAGE_SCALE2      PWR_D3CR_VOS_1                    /*!< Select voltage scale 2 */
#define PWR_REGU_VOLTAGE_SCALE1      (PWR_D3CR_VOS_0 | PWR_D3CR_VOS_1) /*!< Select voltage scale 1 */
#if defined (SYSCFG_PWRCR_ODEN) /* STM32H74xxx and STM32H75xxx lines */
#define PWR_REGU_VOLTAGE_SCALE0      (PWR_D3CR_VOS_0 | PWR_D3CR_VOS_1) /*!< Select voltage scale 0 */
#else
#define PWR_REGU_VOLTAGE_SCALE0      0x00000000U                       /*!< Select voltage scale 0 */
#endif /* defined (SYSCFG_PWRCR_ODEN) */
#else
#define PWR_REGU_VOLTAGE_SCALE3      0x00000000U                       /*!< Select voltage scale 3 */
#define PWR_REGU_VOLTAGE_SCALE2      PWR_D3CR_VOS_0                    /*!< Select voltage scale 2 */
#define PWR_REGU_VOLTAGE_SCALE1      PWR_D3CR_VOS_1                    /*!< Select voltage scale 1 */
#define PWR_REGU_VOLTAGE_SCALE0      (PWR_D3CR_VOS_0 | PWR_D3CR_VOS_1) /*!< Select voltage scale 0 */
#endif /* PWR_CPUCR_PDDS_D2 */

#if defined (SMPS)
/**
  * @brief  Configure the PWR supply
  * @rmtoll CR3          BYPASS          PWR_ConfigSupply
  * @rmtoll CR3          LDOEN           PWR_ConfigSupply
  * @rmtoll CR3          SMPSEN          PWR_ConfigSupply
  * @rmtoll CR3          SMPSEXTHP       PWR_ConfigSupply
  * @rmtoll CR3          SMPSLEVEL       PWR_ConfigSupply
  * @param  SupplySource This parameter can be one of the following values:
  *         @arg @ref PWR_LDO_SUPPLY
  *         @arg @ref PWR_DIRECT_SMPS_SUPPLY
  *         @arg @ref PWR_SMPS_1V8_SUPPLIES_LDO
  *         @arg @ref PWR_SMPS_2V5_SUPPLIES_LDO
  *         @arg @ref PWR_SMPS_1V8_SUPPLIES_EXT_AND_LDO
  *         @arg @ref PWR_SMPS_2V5_SUPPLIES_EXT_AND_LDO
  *         @arg @ref PWR_SMPS_1V8_SUPPLIES_EXT
  *         @arg @ref PWR_SMPS_2V5_SUPPLIES_EXT
  *         @arg @ref PWR_EXTERNAL_SOURCE_SUPPLY
  * @retval None
  */
__STATIC_INLINE void PWR_ConfigSupply(uint32_t SupplySource)
{
  /* Set the power supply configuration */
  MODIFY_REG(PWR->CR3, (PWR_CR3_SMPSLEVEL | PWR_CR3_SMPSEXTHP | PWR_CR3_SMPSEN | PWR_CR3_LDOEN | PWR_CR3_BYPASS), SupplySource);
}
#else
/**
  * @brief  Configure the PWR supply
  * @rmtoll CR3          BYPASS          PWR_ConfigSupply
  * @rmtoll CR3          LDOEN           PWR_ConfigSupply
  * @rmtoll CR3          SCUEN           PWR_ConfigSupply
  * @param  SupplySource This parameter can be one of the following values:
  *         @arg @ref PWR_LDO_SUPPLY
  *         @arg @ref PWR_EXTERNAL_SOURCE_SUPPLY
  * @retval None
  */
__STATIC_INLINE void PWR_ConfigSupply(uint32_t SupplySource)
{
  /* Set the power supply configuration */
  MODIFY_REG(PWR->CR3, (PWR_CR3_SCUEN | PWR_CR3_LDOEN | PWR_CR3_BYPASS), SupplySource);
}
#endif /* defined (SMPS) */

/**
  * @brief  Set the main internal Regulator output voltage
  * @rmtoll D3CR    VOS       PWR_SetRegulVoltageScaling
  * @param  VoltageScaling This parameter can be one of the following values:
  *         @arg @ref PWR_REGU_VOLTAGE_SCALE0
  *         @arg @ref PWR_REGU_VOLTAGE_SCALE1
  *         @arg @ref PWR_REGU_VOLTAGE_SCALE2
  *         @arg @ref PWR_REGU_VOLTAGE_SCALE3
  * @note   For all H7 lines except STM32H7Axxx and STM32H7Bxxx lines, VOS0
  *         is applied when PWR_D3CR_VOS[1:0] = 0b11 and  SYSCFG_PWRCR_ODEN = 0b1.
  * @retval None
  */
__STATIC_INLINE void PWR_SetRegulVoltageScaling(uint32_t VoltageScaling)
{
#if defined (PWR_CPUCR_PDDS_D2)
  MODIFY_REG(PWR->D3CR, PWR_D3CR_VOS, VoltageScaling);
#else
  MODIFY_REG(PWR->SRDCR, PWR_SRDCR_VOS, VoltageScaling);
#endif /* PWR_CPUCR_PDDS_D2 */
}

/**
  * @brief  Indicate whether the Regulator is ready in the selected voltage range
  *         or if its output voltage is still changing to the required voltage level
  * @rmtoll D3CR   VOSRDY       PWR_IsActiveFlag_VOS
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t PWR_IsActiveFlag_VOS(void)
{
#if defined (PWR_CPUCR_PDDS_D2)
  return ((READ_BIT(PWR->D3CR, PWR_D3CR_VOSRDY) == (PWR_D3CR_VOSRDY)) ? 1UL : 0UL);
#else
  return ((READ_BIT(PWR->SRDCR, PWR_SRDCR_VOSRDY) == (PWR_SRDCR_VOSRDY)) ? 1UL : 0UL);
#endif /* PWR_CPUCR_PDDS_D2 */
}

/**
  * @}
  */

/** @defgroup RCC_EF_HSI HSI
  * @{
  */

/**
  * @brief  Enable HSI oscillator
  * @rmtoll CR           HSION         RCC_HSI_Enable
  * @retval None
  */
__STATIC_INLINE void RCC_HSI_Enable(void)
{
  SET_BIT(RCC->CR, RCC_CR_HSION);
}

/**
  * @brief  Check if HSI clock is ready
  * @rmtoll CR           HSIRDY        RCC_HSI_IsReady
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t RCC_HSI_IsReady(void)
{
  return ((READ_BIT(RCC->CR, RCC_CR_HSIRDY) == (RCC_CR_HSIRDY)) ? 1UL : 0UL);
}