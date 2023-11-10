#include "stm32h7xx.h"

/**
  * @}
  */

/** @defgroup RCC_EC_HSIDIV  HSI oscillator divider
  * @{
  */
#define RCC_HSI_DIV1                   RCC_CR_HSIDIV_1
#define RCC_HSI_DIV2                   RCC_CR_HSIDIV_2
#define RCC_HSI_DIV4                   RCC_CR_HSIDIV_4
#define RCC_HSI_DIV8                   RCC_CR_HSIDIV_8


/** @defgroup RCC_EC_PLLSOURCE   All PLLs entry clock source
  * @{
  */
#define RCC_PLLSOURCE_HSI               RCC_PLLCKSELR_PLLSRC_HSI
#define RCC_PLLSOURCE_CSI               RCC_PLLCKSELR_PLLSRC_CSI
#define RCC_PLLSOURCE_HSE               RCC_PLLCKSELR_PLLSRC_HSE
#define RCC_PLLSOURCE_NONE              RCC_PLLCKSELR_PLLSRC_NONE


/** @defgroup RCC_EC_PLLINPUTRANGE   All PLLs input range
  * @{
  */
#define RCC_PLLINPUTRANGE_1_2           (uint32_t)(0x00000000U)
#define RCC_PLLINPUTRANGE_2_4           (uint32_t)(0x00000001)
#define RCC_PLLINPUTRANGE_4_8           (uint32_t)(0x00000002)
#define RCC_PLLINPUTRANGE_8_16          (uint32_t)(0x00000003)

/** @defgroup RCC_EC_PLLVCORANGE   All PLLs VCO range
  * @{
  */
#define RCC_PLLVCORANGE_WIDE            (uint32_t)(0x00000000U)      /* VCO output range: 192 to 836 MHz   OR  128 to 544 MHz (*) */
#define RCC_PLLVCORANGE_MEDIUM          (uint32_t)(0x00000001)       /* VCO output range: 150 to 420 MHz */

/** @defgroup RCC_LL_EC_AHB_DIV  AHB prescaler
  * @{
  */
#if defined(RCC_D1CFGR_HPRE_DIV1)
#define RCC_AHB_DIV_1                   RCC_D1CFGR_HPRE_DIV1
#define RCC_AHB_DIV_2                   RCC_D1CFGR_HPRE_DIV2
#define RCC_AHB_DIV_4                   RCC_D1CFGR_HPRE_DIV4
#define RCC_AHB_DIV_8                   RCC_D1CFGR_HPRE_DIV8
#define RCC_AHB_DIV_16                  RCC_D1CFGR_HPRE_DIV16
#define RCC_AHB_DIV_64                  RCC_D1CFGR_HPRE_DIV64
#define RCC_AHB_DIV_128                 RCC_D1CFGR_HPRE_DIV128
#define RCC_AHB_DIV_256                 RCC_D1CFGR_HPRE_DIV256
#define RCC_AHB_DIV_512                 RCC_D1CFGR_HPRE_DIV512
#else
#define RCC_AHB_DIV_1                   RCC_CDCFGR1_HPRE_DIV1
#define RCC_AHB_DIV_2                   RCC_CDCFGR1_HPRE_DIV2
#define RCC_AHB_DIV_4                   RCC_CDCFGR1_HPRE_DIV4
#define RCC_AHB_DIV_8                   RCC_CDCFGR1_HPRE_DIV8
#define RCC_AHB_DIV_16                  RCC_CDCFGR1_HPRE_DIV16
#define RCC_AHB_DIV_64                  RCC_CDCFGR1_HPRE_DIV64
#define RCC_AHB_DIV_128                 RCC_CDCFGR1_HPRE_DIV128
#define RCC_AHB_DIV_256                 RCC_CDCFGR1_HPRE_DIV256
#define RCC_AHB_DIV_512                 RCC_CDCFGR1_HPRE_DIV512
#endif /* RCC_D1CFGR_HPRE_DIV1 */

/**
  * @}
  */

/** @defgroup RCC_EC_SYS_CLKSOURCE  System clock switch
  * @{
  */
#define RCC_SYS_CLKSOURCE_HSI           RCC_CFGR_SW_HSI
#define RCC_SYS_CLKSOURCE_CSI           RCC_CFGR_SW_CSI
#define RCC_SYS_CLKSOURCE_HSE           RCC_CFGR_SW_HSE
#define RCC_SYS_CLKSOURCE_PLL1          RCC_CFGR_SW_PLL1

/**
  * @}
  */

/** @defgroup RCC_EC_SYS_CLKSOURCE_STATUS  System clock switch status
  * @{
  */
#define RCC_SYS_CLKSOURCE_STATUS_HSI    RCC_CFGR_SWS_HSI   /*!< HSI used as system clock */
#define RCC_SYS_CLKSOURCE_STATUS_CSI    RCC_CFGR_SWS_CSI   /*!< CSI used as system clock */
#define RCC_SYS_CLKSOURCE_STATUS_HSE    RCC_CFGR_SWS_HSE   /*!< HSE used as system clock */
#define RCC_SYS_CLKSOURCE_STATUS_PLL1   RCC_CFGR_SWS_PLL1  /*!< PLL1 used as system clock */

/**
  * @}
  */

/** @defgroup RCC_EC_SYSCLK_DIV  System prescaler
  * @{
  */
#if defined(RCC_D1CFGR_D1CPRE_DIV1)
#define RCC_SYSCLK_DIV_1                RCC_D1CFGR_D1CPRE_DIV1
#define RCC_SYSCLK_DIV_2                RCC_D1CFGR_D1CPRE_DIV2
#define RCC_SYSCLK_DIV_4                RCC_D1CFGR_D1CPRE_DIV4
#define RCC_SYSCLK_DIV_8                RCC_D1CFGR_D1CPRE_DIV8
#define RCC_SYSCLK_DIV_16               RCC_D1CFGR_D1CPRE_DIV16
#define RCC_SYSCLK_DIV_64               RCC_D1CFGR_D1CPRE_DIV64
#define RCC_SYSCLK_DIV_128              RCC_D1CFGR_D1CPRE_DIV128
#define RCC_SYSCLK_DIV_256              RCC_D1CFGR_D1CPRE_DIV256
#define RCC_SYSCLK_DIV_512              RCC_D1CFGR_D1CPRE_DIV512
#else
#define RCC_SYSCLK_DIV_1                RCC_CDCFGR1_CDCPRE_DIV1
#define RCC_SYSCLK_DIV_2                RCC_CDCFGR1_CDCPRE_DIV2
#define RCC_SYSCLK_DIV_4                RCC_CDCFGR1_CDCPRE_DIV4
#define RCC_SYSCLK_DIV_8                RCC_CDCFGR1_CDCPRE_DIV8
#define RCC_SYSCLK_DIV_16               RCC_CDCFGR1_CDCPRE_DIV16
#define RCC_SYSCLK_DIV_64               RCC_CDCFGR1_CDCPRE_DIV64
#define RCC_SYSCLK_DIV_128              RCC_CDCFGR1_CDCPRE_DIV128
#define RCC_SYSCLK_DIV_256              RCC_CDCFGR1_CDCPRE_DIV256
#define RCC_SYSCLK_DIV_512              RCC_CDCFGR1_CDCPRE_DIV512
#endif /* RCC_D1CFGR_D1CPRE_DIV1 */

/**
  * @}
  */

/** @defgroup RCC_EC_AHB_DIV  AHB prescaler
  * @{
  */
#if defined(RCC_D1CFGR_HPRE_DIV1)
#define RCC_AHB_DIV_1                   RCC_D1CFGR_HPRE_DIV1
#define RCC_AHB_DIV_2                   RCC_D1CFGR_HPRE_DIV2
#define RCC_AHB_DIV_4                   RCC_D1CFGR_HPRE_DIV4
#define RCC_AHB_DIV_8                   RCC_D1CFGR_HPRE_DIV8
#define RCC_AHB_DIV_16                  RCC_D1CFGR_HPRE_DIV16
#define RCC_AHB_DIV_64                  RCC_D1CFGR_HPRE_DIV64
#define RCC_AHB_DIV_128                 RCC_D1CFGR_HPRE_DIV128
#define RCC_AHB_DIV_256                 RCC_D1CFGR_HPRE_DIV256
#define RCC_AHB_DIV_512                 RCC_D1CFGR_HPRE_DIV512
#else
#define RCC_AHB_DIV_1                   RCC_CDCFGR1_HPRE_DIV1
#define RCC_AHB_DIV_2                   RCC_CDCFGR1_HPRE_DIV2
#define RCC_AHB_DIV_4                   RCC_CDCFGR1_HPRE_DIV4
#define RCC_AHB_DIV_8                   RCC_CDCFGR1_HPRE_DIV8
#define RCC_AHB_DIV_16                  RCC_CDCFGR1_HPRE_DIV16
#define RCC_AHB_DIV_64                  RCC_CDCFGR1_HPRE_DIV64
#define RCC_AHB_DIV_128                 RCC_CDCFGR1_HPRE_DIV128
#define RCC_AHB_DIV_256                 RCC_CDCFGR1_HPRE_DIV256
#define RCC_AHB_DIV_512                 RCC_CDCFGR1_HPRE_DIV512
#endif /* RCC_D1CFGR_HPRE_DIV1 */


/**
  * @}
  */

/** @defgroup RCC_EC_APB1_DIV  APB low-speed prescaler (APB1)
  * @{
  */
#if defined(RCC_D2CFGR_D2PPRE1_DIV1)
#define RCC_APB1_DIV_1                  RCC_D2CFGR_D2PPRE1_DIV1
#define RCC_APB1_DIV_2                  RCC_D2CFGR_D2PPRE1_DIV2
#define RCC_APB1_DIV_4                  RCC_D2CFGR_D2PPRE1_DIV4
#define RCC_APB1_DIV_8                  RCC_D2CFGR_D2PPRE1_DIV8
#define RCC_APB1_DIV_16                 RCC_D2CFGR_D2PPRE1_DIV16
#else
#define RCC_APB1_DIV_1                  RCC_CDCFGR2_CDPPRE1_DIV1
#define RCC_APB1_DIV_2                  RCC_CDCFGR2_CDPPRE1_DIV2
#define RCC_APB1_DIV_4                  RCC_CDCFGR2_CDPPRE1_DIV4
#define RCC_APB1_DIV_8                  RCC_CDCFGR2_CDPPRE1_DIV8
#define RCC_APB1_DIV_16                 RCC_CDCFGR2_CDPPRE1_DIV16
#endif /* RCC_D2CFGR_D2PPRE1_DIV1 */

/**
  * @}
  */

/** @defgroup RCC_EC_APB2_DIV  APB low-speed prescaler (APB2)
  * @{
  */
#if defined(RCC_D2CFGR_D2PPRE2_DIV1)
#define RCC_APB2_DIV_1                  RCC_D2CFGR_D2PPRE2_DIV1
#define RCC_APB2_DIV_2                  RCC_D2CFGR_D2PPRE2_DIV2
#define RCC_APB2_DIV_4                  RCC_D2CFGR_D2PPRE2_DIV4
#define RCC_APB2_DIV_8                  RCC_D2CFGR_D2PPRE2_DIV8
#define RCC_APB2_DIV_16                 RCC_D2CFGR_D2PPRE2_DIV16
#else
#define RCC_APB2_DIV_1                  RCC_CDCFGR2_CDPPRE2_DIV1
#define RCC_APB2_DIV_2                  RCC_CDCFGR2_CDPPRE2_DIV2
#define RCC_APB2_DIV_4                  RCC_CDCFGR2_CDPPRE2_DIV4
#define RCC_APB2_DIV_8                  RCC_CDCFGR2_CDPPRE2_DIV8
#define RCC_APB2_DIV_16                 RCC_CDCFGR2_CDPPRE2_DIV16
#endif /* RCC_D2CFGR_D2PPRE2_DIV1 */

/**
  * @}
  */

/** @defgroup RCC_EC_APB3_DIV  APB low-speed prescaler (APB3)
  * @{
  */
#if defined(RCC_D1CFGR_D1PPRE_DIV1)
#define RCC_APB3_DIV_1                  RCC_D1CFGR_D1PPRE_DIV1
#define RCC_APB3_DIV_2                  RCC_D1CFGR_D1PPRE_DIV2
#define RCC_APB3_DIV_4                  RCC_D1CFGR_D1PPRE_DIV4
#define RCC_APB3_DIV_8                  RCC_D1CFGR_D1PPRE_DIV8
#define RCC_APB3_DIV_16                 RCC_D1CFGR_D1PPRE_DIV16
#else
#define RCC_APB3_DIV_1                  RCC_CDCFGR1_CDPPRE_DIV1
#define RCC_APB3_DIV_2                  RCC_CDCFGR1_CDPPRE_DIV2
#define RCC_APB3_DIV_4                  RCC_CDCFGR1_CDPPRE_DIV4
#define RCC_APB3_DIV_8                  RCC_CDCFGR1_CDPPRE_DIV8
#define RCC_APB3_DIV_16                 RCC_CDCFGR1_CDPPRE_DIV16
#endif /* RCC_D1CFGR_D1PPRE_DIV1 */

/**
  * @}
  */

/** @defgroup RCC_EC_APB4_DIV  APB low-speed prescaler (APB4)
  * @{
  */
#if defined(RCC_D3CFGR_D3PPRE_DIV1)
#define RCC_APB4_DIV_1                  RCC_D3CFGR_D3PPRE_DIV1
#define RCC_APB4_DIV_2                  RCC_D3CFGR_D3PPRE_DIV2
#define RCC_APB4_DIV_4                  RCC_D3CFGR_D3PPRE_DIV4
#define RCC_APB4_DIV_8                  RCC_D3CFGR_D3PPRE_DIV8
#define RCC_APB4_DIV_16                 RCC_D3CFGR_D3PPRE_DIV16
#else
#define RCC_APB4_DIV_1                  RCC_SRDCFGR_SRDPPRE_DIV1
#define RCC_APB4_DIV_2                  RCC_SRDCFGR_SRDPPRE_DIV2
#define RCC_APB4_DIV_4                  RCC_SRDCFGR_SRDPPRE_DIV4
#define RCC_APB4_DIV_8                  RCC_SRDCFGR_SRDPPRE_DIV8
#define RCC_APB4_DIV_16                 RCC_SRDCFGR_SRDPPRE_DIV16
#endif /* RCC_D3CFGR_D3PPRE_DIV1 */

/**
  * @brief  Set HSI Calibration trimming
  * @note user-programmable trimming value that is added to the HSICAL
  * @note Default value is 64 (32 for Cut1.x), which, when added to the HSICAL value,
  *       should trim the HSI to 64 MHz +/- 1 %
  * @rmtoll HSICFGR        HSITRIM       RCC_HSI_SetCalibTrimming
  * @param  Value can be a value between 0 and 127 (63 for Cut1.x)
  * @retval None
  */
__STATIC_INLINE void RCC_HSI_SetCalibTrimming(uint32_t Value)
{
#if defined(RCC_VER_X)
  if ((DBGMCU->IDCODE & 0xF0000000U) == 0x10000000U)
  {
    /* STM32H7 Rev.Y */
    MODIFY_REG(RCC->HSICFGR, 0x3F000U, Value << 12U);
  }
  else
  {
    /* STM32H7 Rev.V */
    MODIFY_REG(RCC->HSICFGR, RCC_HSICFGR_HSITRIM, Value << RCC_HSICFGR_HSITRIM_Pos);
  }
#else
  MODIFY_REG(RCC->HSICFGR, RCC_HSICFGR_HSITRIM, Value << RCC_HSICFGR_HSITRIM_Pos);
#endif /* RCC_VER_X */
}

/**
  * @brief  Set HSI divider
  * @rmtoll CR           HSIDIV        RCC_HSI_SetDivider
  * @param  Divider This parameter can be one of the following values:
  *         @arg @ref RCC_HSI_DIV1
  *         @arg @ref RCC_HSI_DIV2
  *         @arg @ref RCC_HSI_DIV4
  *         @arg @ref RCC_HSI_DIV8
  * @retval None.
  */
__STATIC_INLINE void RCC_HSI_SetDivider(uint32_t Divider)
{
  MODIFY_REG(RCC->CR, RCC_CR_HSIDIV, Divider);
}

/** @defgroup RCC_EF_PLL PLL
  * @{
  */

/**
  * @brief  Set the oscillator used as PLL clock source.
  * @note   PLLSRC can be written only when All PLLs are disabled.
  * @rmtoll PLLCKSELR      PLLSRC        RCC_PSetSource
  * @param  PLLSource parameter can be one of the following values:
  *         @arg @ref RCC_PLLSOURCE_HSI
  *         @arg @ref RCC_PLLSOURCE_CSI
  *         @arg @ref RCC_PLLSOURCE_HSE
  *         @arg @ref RCC_PLLSOURCE_NONE
  * @retval None
  */
__STATIC_INLINE void RCC_PLL_SetSource(uint32_t PLLSource)
{
  MODIFY_REG(RCC->PLLCKSELR, RCC_PLLCKSELR_PLLSRC, PLLSource);
}

/**
  * @brief  Enable PLL1P
  * @note   This API shall be called only when PLL1 is disabled.
  * @rmtoll PLLCFGR           DIVP1EN         RCC_PLL1P_Enable
  * @retval None
  */
__STATIC_INLINE void RCC_PLL1P_Enable(void)
{
  SET_BIT(RCC->PLLCFGR, RCC_PLLCFGR_DIVP1EN);
}

/**
  * @brief  Set PLL1 VCO Input Range
  * @note   This API shall be called only when PLL1 is disabled.
  * @rmtoll PLLCFGR        PLL1RGE       LL_RCC_PLL1_SetVCOInputRange
  * @param  InputRange This parameter can be one of the following values:
  *         @arg @ref RCC_PLLINPUTRANGE_1_2
  *         @arg @ref RCC_PLLINPUTRANGE_2_4
  *         @arg @ref RCC_PLLINPUTRANGE_4_8
  *         @arg @ref RCC_PLLINPUTRANGE_8_16
  * @retval None
  */
__STATIC_INLINE void RCC_PLL1_SetVCOInputRange(uint32_t InputRange)
{
  MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLL1RGE, InputRange << RCC_PLLCFGR_PLL1RGE_Pos);
}

/**
  * @brief  Set PLL1 VCO OutputRange
  * @note   This API shall be called only when PLL1 is disabled.
  * @rmtoll PLLCFGR        PLL1VCOSEL       LL_RCC_PLL1_SetVCOOuputRange
  * @param  VCORange This parameter can be one of the following values:
  *         @arg @ref RCC_PLLVCORANGE_WIDE
  *         @arg @ref RCC_PLLVCORANGE_MEDIUM
  * @retval None
  */
__STATIC_INLINE void RCC_PLL1_SetVCOOutputRange(uint32_t VCORange)
{
  MODIFY_REG(RCC->PLLCFGR, RCC_PLLCFGR_PLL1VCOSEL, VCORange << RCC_PLLCFGR_PLL1VCOSEL_Pos);
}

/**
  * @brief  Set PLL1 M Coefficient
  * @note   This API shall be called only when PLL1 is disabled.
  * @rmtoll PLLCKSELR       DIVM1          RCC_PLL1_SetM
  * @param  M parameter can be a value between 0 and 63
  */
__STATIC_INLINE void RCC_PLL1_SetM(uint32_t M)
{
  MODIFY_REG(RCC->PLLCKSELR, RCC_PLLCKSELR_DIVM1, M << RCC_PLLCKSELR_DIVM1_Pos);
}

/**
  * @brief  Set PLL1 N Coefficient
  * @note   This API shall be called only when PLL1 is disabled.
  * @rmtoll PLL1DIVR        N1          RCC_PLL1_SetN
  * @param  N parameter can be a value between 4 and 512
  */
__STATIC_INLINE void RCC_PLL1_SetN(uint32_t N)
{
  MODIFY_REG(RCC->PLL1DIVR, RCC_PLL1DIVR_N1, (N - 1UL) << RCC_PLL1DIVR_N1_Pos);
}

/**
  * @brief  Set PLL1 P Coefficient
  * @note   This API shall be called only when PLL1 is disabled.
  * @rmtoll PLL1DIVR        P1          RCC_PLL1_SetP
  * @param  P parameter can be a value between 2 (or 1*) and 128 (ODD division factor not supported)
  *
  * (*) : For stm32h72xxx and stm32h73xxx family lines.
  */
__STATIC_INLINE void RCC_PLL1_SetP(uint32_t P)
{
  MODIFY_REG(RCC->PLL1DIVR, RCC_PLL1DIVR_P1, (P - 1UL) << RCC_PLL1DIVR_P1_Pos);
}

/**
  * @brief  Set PLL1 Q Coefficient
  * @note   This API shall be called only when PLL1 is disabled.
  * @rmtoll PLL1DIVR        Q1          RCC_PLL1_SetQ
  * @param  Q parameter can be a value between 1 and 128
  */
__STATIC_INLINE void RCC_PLL1_SetQ(uint32_t Q)
{
  MODIFY_REG(RCC->PLL1DIVR, RCC_PLL1DIVR_Q1, (Q - 1UL) << RCC_PLL1DIVR_Q1_Pos);
}

/**
  * @brief  Set PLL1 R Coefficient
  * @note   This API shall be called only when PLL1 is disabled.
  * @rmtoll PLL1DIVR        R1          RCC_PLL1_SetR
  * @param  R parameter can be a value between 1 and 128
  */
__STATIC_INLINE void RCC_PLL1_SetR(uint32_t R)
{
  MODIFY_REG(RCC->PLL1DIVR, RCC_PLL1DIVR_R1, (R - 1UL) << RCC_PLL1DIVR_R1_Pos);
}

/**
  * @brief  Enable PLL1
  * @rmtoll CR           PLL1ON         RCC_PLL1_Enable
  * @retval None
  */
__STATIC_INLINE void RCC_PLL1_Enable(void)
{
  SET_BIT(RCC->CR, RCC_CR_PLL1ON);
}

/**
  * @brief  Check if PLL1 Ready
  * @rmtoll CR           PLL1RDY        LL_RCC_PLL1_IsReady
  * @retval State of bit (1 or 0).
  */
__STATIC_INLINE uint32_t RCC_PLL1_IsReady(void)
{
  return ((READ_BIT(RCC->CR, RCC_CR_PLL1RDY) == (RCC_CR_PLL1RDY)) ? 1UL : 0UL);
}

/**
  * @brief  Set AHB prescaler
  * @rmtoll D1CFGR/CDCFGR1        HPRE         RCC_SetAHBPrescaler
  * @param  Prescaler This parameter can be one of the following values:
  *         @arg @ref RCC_AHB_DIV_1
  *         @arg @ref RCC_AHB_DIV_2
  *         @arg @ref RCC_AHB_DIV_4
  *         @arg @ref RCC_AHB_DIV_8
  *         @arg @ref RCC_AHB_DIV_16
  *         @arg @ref RCC_AHB_DIV_64
  *         @arg @ref RCC_AHB_DIV_128
  *         @arg @ref RCC_AHB_DIV_256
  *         @arg @ref RCC_AHB_DIV_512
  * @retval None
  */
__STATIC_INLINE void RCC_SetAHBPrescaler(uint32_t Prescaler)
{
#if defined(RCC_D1CFGR_HPRE)
  MODIFY_REG(RCC->D1CFGR, RCC_D1CFGR_HPRE, Prescaler);
#else
  MODIFY_REG(RCC->CDCFGR1, RCC_CDCFGR1_HPRE, Prescaler);
#endif /* RCC_D1CFGR_HPRE */
}

/**
  * @}
  */

/** @defgroup RCC_EF_System System
  * @{
  */

/**
  * @brief  Configure the system clock source
  * @rmtoll CFGR         SW            RCC_SetSysClkSource
  * @param  Source This parameter can be one of the following values:
  *         @arg @ref RCC_SYS_CLKSOURCE_HSI
  *         @arg @ref RCC_SYS_CLKSOURCE_CSI
  *         @arg @ref RCC_SYS_CLKSOURCE_HSE
  *         @arg @ref RCC_SYS_CLKSOURCE_PLL1
  * @retval None
  */
__STATIC_INLINE void RCC_SetSysClkSource(uint32_t Source)
{
  MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, Source);
}

/**
  * @brief  Get the system clock source
  * @rmtoll CFGR         SWS           LL_RCC_GetSysClkSource
  * @retval Returned value can be one of the following values:
  *         @arg @ref LL_RCC_SYS_CLKSOURCE_STATUS_HSI
  *         @arg @ref LL_RCC_SYS_CLKSOURCE_STATUS_CSI
  *         @arg @ref LL_RCC_SYS_CLKSOURCE_STATUS_HSE
  *         @arg @ref LL_RCC_SYS_CLKSOURCE_STATUS_PLL1
  */
__STATIC_INLINE uint32_t RCC_GetSysClkSource(void)
{
  return (uint32_t)(READ_BIT(RCC->CFGR, RCC_CFGR_SWS));
}

/**
  * @brief  Set System prescaler
  * @rmtoll D1CFGR/CDCFGR1        D1CPRE/CDCPRE          RCC_SetSysPrescaler
  * @param  Prescaler This parameter can be one of the following values:
  *         @arg @ref RCC_SYSCLK_DIV_1
  *         @arg @ref RCC_SYSCLK_DIV_2
  *         @arg @ref RCC_SYSCLK_DIV_4
  *         @arg @ref RCC_SYSCLK_DIV_8
  *         @arg @ref RCC_SYSCLK_DIV_16
  *         @arg @ref RCC_SYSCLK_DIV_64
  *         @arg @ref RCC_SYSCLK_DIV_128
  *         @arg @ref RCC_SYSCLK_DIV_256
  *         @arg @ref RCC_SYSCLK_DIV_512
  * @retval None
  */
__STATIC_INLINE void RCC_SetSysPrescaler(uint32_t Prescaler)
{
#if defined(RCC_D1CFGR_D1CPRE)
  MODIFY_REG(RCC->D1CFGR, RCC_D1CFGR_D1CPRE, Prescaler);
#else
  MODIFY_REG(RCC->CDCFGR1, RCC_CDCFGR1_CDCPRE, Prescaler);
#endif /* RCC_D1CFGR_D1CPRE */
}

/**
  * @brief  Set APB1 prescaler
  * @rmtoll D2CFGR/CDCFGR2         D2PPRE1/CDPPRE1         RCC_SetAPB1Prescaler
  * @param  Prescaler This parameter can be one of the following values:
  *         @arg @ref RCC_APB1_DIV_1
  *         @arg @ref RCC_APB1_DIV_2
  *         @arg @ref RCC_APB1_DIV_4
  *         @arg @ref RCC_APB1_DIV_8
  *         @arg @ref RCC_APB1_DIV_16
  * @retval None
  */
__STATIC_INLINE void RCC_SetAPB1Prescaler(uint32_t Prescaler)
{
#if defined(RCC_D2CFGR_D2PPRE1)
  MODIFY_REG(RCC->D2CFGR, RCC_D2CFGR_D2PPRE1, Prescaler);
#else
  MODIFY_REG(RCC->CDCFGR2, RCC_CDCFGR2_CDPPRE1, Prescaler);
#endif /* RCC_D2CFGR_D2PPRE1 */
}

/**
  * @brief  Set APB2 prescaler
  * @rmtoll D2CFGR/CDCFGR2         D2PPRE2/CDPPRE2         RCC_SetAPB2Prescaler
  * @param  Prescaler This parameter can be one of the following values:
  *         @arg @ref RCC_APB2_DIV_1
  *         @arg @ref RCC_APB2_DIV_2
  *         @arg @ref RCC_APB2_DIV_4
  *         @arg @ref RCC_APB2_DIV_8
  *         @arg @ref RCC_APB2_DIV_16
  * @retval None
  */
__STATIC_INLINE void RCC_SetAPB2Prescaler(uint32_t Prescaler)
{
#if defined(RCC_D2CFGR_D2PPRE2)
  MODIFY_REG(RCC->D2CFGR, RCC_D2CFGR_D2PPRE2, Prescaler);
#else
  MODIFY_REG(RCC->CDCFGR2, RCC_CDCFGR2_CDPPRE2, Prescaler);
#endif /* RCC_D2CFGR_D2PPRE2 */
}


/**
  * @brief  Set APB3 prescaler
  * @rmtoll D1CFGR/CDCFGR1         D1PPRE/CDPPRE         RCC_SetAPB3Prescaler
  * @param  Prescaler This parameter can be one of the following values:
  *         @arg @ref RCC_APB3_DIV_1
  *         @arg @ref RCC_APB3_DIV_2
  *         @arg @ref RCC_APB3_DIV_4
  *         @arg @ref RCC_APB3_DIV_8
  *         @arg @ref RCC_APB3_DIV_16
  * @retval None
  */
__STATIC_INLINE void RCC_SetAPB3Prescaler(uint32_t Prescaler)
{
#if defined(RCC_D1CFGR_D1PPRE)
  MODIFY_REG(RCC->D1CFGR, RCC_D1CFGR_D1PPRE, Prescaler);
#else
  MODIFY_REG(RCC->CDCFGR1, RCC_CDCFGR1_CDPPRE, Prescaler);
#endif /* RCC_D1CFGR_D1PPRE */
}

/**
  * @brief  Set APB4 prescaler
  * @rmtoll D3CFGR/SRDCFGR         D3PPRE/SRDPPRE         RCC_SetAPB4Prescaler
  * @param  Prescaler This parameter can be one of the following values:
  *         @arg @ref RCC_APB4_DIV_1
  *         @arg @ref RCC_APB4_DIV_2
  *         @arg @ref RCC_APB4_DIV_4
  *         @arg @ref RCC_APB4_DIV_8
  *         @arg @ref RCC_APB4_DIV_16
  * @retval None
  */
__STATIC_INLINE void RCC_SetAPB4Prescaler(uint32_t Prescaler)
{
#if defined(RCC_D3CFGR_D3PPRE)
  MODIFY_REG(RCC->D3CFGR, RCC_D3CFGR_D3PPRE, Prescaler);
#else
  MODIFY_REG(RCC->SRDCFGR, RCC_SRDCFGR_SRDPPRE, Prescaler);
#endif /* RCC_D3CFGR_D3PPRE */
}