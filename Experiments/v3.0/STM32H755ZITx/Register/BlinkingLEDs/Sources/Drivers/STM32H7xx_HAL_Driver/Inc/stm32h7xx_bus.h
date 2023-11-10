#include "stm32h7xx.h"

#define APB4_GRP1_PERIPH_SYSCFG         RCC_APB4ENR_SYSCFGEN


/**
  * @}
  */


/** @defgroup BUS_EC_AHB4_GRP1_PERIPH  AHB4 GRP1 PERIPH
  * @{
  */
#define AHB4_GRP1_PERIPH_GPIOA          RCC_AHB4ENR_GPIOAEN
#define AHB4_GRP1_PERIPH_GPIOB          RCC_AHB4ENR_GPIOBEN
#define AHB4_GRP1_PERIPH_GPIOC          RCC_AHB4ENR_GPIOCEN
#define AHB4_GRP1_PERIPH_GPIOD          RCC_AHB4ENR_GPIODEN
#define AHB4_GRP1_PERIPH_GPIOE          RCC_AHB4ENR_GPIOEEN
#define AHB4_GRP1_PERIPH_GPIOF          RCC_AHB4ENR_GPIOFEN
#define AHB4_GRP1_PERIPH_GPIOG          RCC_AHB4ENR_GPIOGEN
#define AHB4_GRP1_PERIPH_GPIOH          RCC_AHB4ENR_GPIOHEN
#if defined(GPIOI)
#define AHB4_GRP1_PERIPH_GPIOI          RCC_AHB4ENR_GPIOIEN
#endif /* GPIOI */
#define AHB4_GRP1_PERIPH_GPIOJ          RCC_AHB4ENR_GPIOJEN
#define AHB4_GRP1_PERIPH_GPIOK          RCC_AHB4ENR_GPIOKEN
#if defined(RCC_AHB4ENR_CRCEN)
#define AHB4_GRP1_PERIPH_CRC            RCC_AHB4ENR_CRCEN
#endif /* RCC_AHB4ENR_CRCEN */
#if defined(BDMA2)
#define AHB4_GRP1_PERIPH_BDMA2          RCC_AHB4ENR_BDMA2EN
#define AHB4_GRP1_PERIPH_BDMA           AHB4_GRP1_PERIPH_BDMA2  /* for backward compatibility*/
#else
#define AHB4_GRP1_PERIPH_BDMA           RCC_AHB4ENR_BDMAEN
#endif /* BDMA2 */
#if defined(ADC3)
#define AHB4_GRP1_PERIPH_ADC3           RCC_AHB4ENR_ADC3EN
#endif /* ADC3 */
#if defined(HSEM) && defined(RCC_AHB4ENR_HSEMEN)
#define AHB4_GRP1_PERIPH_HSEM           RCC_AHB4ENR_HSEMEN
#endif /* HSEM && RCC_AHB4ENR_HSEMEN*/
#define AHB4_GRP1_PERIPH_BKPRAM         RCC_AHB4ENR_BKPRAMEN
#if defined(RCC_AHB4LPENR_SRAM4LPEN)
#define AHB4_GRP1_PERIPH_SRAM4          RCC_AHB4LPENR_SRAM4LPEN
#define AHB4_GRP1_PERIPH_D3SRAM1        AHB4_GRP1_PERIPH_SRAM4
#else
#define AHB4_GRP1_PERIPH_SRDSRAM        RCC_AHB4ENR_SRDSRAMEN
#define AHB4_GRP1_PERIPH_SRAM4          AHB4_GRP1_PERIPH_SRDSRAM  /* for backward compatibility*/
#define AHB4_GRP1_PERIPH_D3SRAM1        AHB4_GRP1_PERIPH_SRDSRAM  /* for backward compatibility*/
#endif /* RCC_AHB4ENR_D3SRAM1EN */

/**
  * @}
  */

/** @defgroup BUS_EF_APB4 APB4
  * @{
  */

/**
  * @brief  Enable APB4 peripherals clock.
  * @rmtoll APB4ENR      SYSCFGEN      APB4_GRP1_EnableClock\n
  *         APB4ENR      LPUART1EN     APB4_GRP1_EnableClock\n
  *         APB4ENR      SPI6EN        APB4_GRP1_EnableClock\n
  *         APB4ENR      I2C4EN        APB4_GRP1_EnableClock\n
  *         APB4ENR      LPTIM2EN      APB4_GRP1_EnableClock\n
  *         APB4ENR      LPTIM3EN      APB4_GRP1_EnableClock\n
  *         APB4ENR      LPTIM4EN      APB4_GRP1_EnableClock\n  (*)
  *         APB4ENR      LPTIM5EN      APB4_GRP1_EnableClock\n  (*)
  *         APB4ENR      DAC2EN        APB4_GRP1_EnableClock\n  (*)
  *         APB4ENR      COMP12EN      APB4_GRP1_EnableClock\n
  *         APB4ENR      VREFEN        APB4_GRP1_EnableClock\n
  *         APB4ENR      RTCAPBEN      APB4_GRP1_EnableClock\n
  *         APB4ENR      SAI4EN        APB4_GRP1_EnableClock\n  (*)
  *         APB4ENR      DTSEN         APB4_GRP1_EnableClock\n  (*)
  *         APB4ENR      DFSDM2EN      APB4_GRP1_EnableClock    (*)
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref APB4_GRP1_PERIPH_SYSCFG
  *         @arg @ref APB4_GRP1_PERIPH_LPUART1
  *         @arg @ref APB4_GRP1_PERIPH_SPI6
  *         @arg @ref APB4_GRP1_PERIPH_I2C4
  *         @arg @ref APB4_GRP1_PERIPH_LPTIM2
  *         @arg @ref APB4_GRP1_PERIPH_LPTIM3
  *         @arg @ref APB4_GRP1_PERIPH_LPTIM4 (*)
  *         @arg @ref APB4_GRP1_PERIPH_LPTIM5 (*)
  *         @arg @ref APB4_GRP1_PERIPH_DAC2   (*)
  *         @arg @ref APB4_GRP1_PERIPH_COMP12
  *         @arg @ref APB4_GRP1_PERIPH_VREF
  *         @arg @ref APB4_GRP1_PERIPH_RTCAPB
  *         @arg @ref APB4_GRP1_PERIPH_SAI4 (*)
  *         @arg @ref APB4_GRP1_PERIPH_DTS    (*)
  *         @arg @ref APB4_GRP1_PERIPH_DFSDM2 (*)
  *
  *         (*) value not defined in all devices.
  * @retval None
*/

__STATIC_INLINE void APB4_GRP1_EnableClock(uint32_t Periphs)
{
    __IO uint32_t reg;
    
    SET_BIT(RCC -> APB4ENR, Periphs);
    
    reg = READ_BIT(RCC -> APB4ENR, Periphs);
    
    (void)reg;
}


/**
  * @}
  */

/** @defgroup BUS_LL_EF_AHB4 AHB4
  * @{
  */

/**
  * @brief  Enable AHB4 peripherals clock.
  * @rmtoll AHB4ENR      GPIOAEN       LL_AHB4_GRP1_EnableClock\n
  *         AHB4ENR      GPIOBEN       LL_AHB4_GRP1_EnableClock\n
  *         AHB4ENR      GPIOCEN       LL_AHB4_GRP1_EnableClock\n
  *         AHB4ENR      GPIODEN       LL_AHB4_GRP1_EnableClock\n
  *         AHB4ENR      GPIOEEN       LL_AHB4_GRP1_EnableClock\n
  *         AHB4ENR      GPIOFEN       LL_AHB4_GRP1_EnableClock\n
  *         AHB4ENR      GPIOGEN       LL_AHB4_GRP1_EnableClock\n
  *         AHB4ENR      GPIOHEN       LL_AHB4_GRP1_EnableClock\n
  *         AHB4ENR      GPIOIEN       LL_AHB4_GRP1_EnableClock\n (*)
  *         AHB4ENR      GPIOJEN       LL_AHB4_GRP1_EnableClock\n
  *         AHB4ENR      GPIOKEN       LL_AHB4_GRP1_EnableClock\n
  *         AHB4ENR      CRCEN         LL_AHB4_GRP1_EnableClock\n (*)
  *         AHB4ENR      BDMAEN        LL_AHB4_GRP1_EnableClock\n
  *         AHB4ENR      ADC3EN        LL_AHB4_GRP1_EnableClock\n (*)
  *         AHB4ENR      HSEMEN        LL_AHB4_GRP1_EnableClock\n (*)
  *         AHB4ENR      BKPRAMEN      LL_AHB4_GRP1_EnableClock\n
  *         AHB4ENR      SRAM4EN       LL_AHB4_GRP1_EnableClock
  * @param  Periphs This parameter can be a combination of the following values:
  *         @arg @ref LL_AHB4_GRP1_PERIPH_GPIOA
  *         @arg @ref LL_AHB4_GRP1_PERIPH_GPIOB
  *         @arg @ref LL_AHB4_GRP1_PERIPH_GPIOC
  *         @arg @ref LL_AHB4_GRP1_PERIPH_GPIOD
  *         @arg @ref LL_AHB4_GRP1_PERIPH_GPIOE
  *         @arg @ref LL_AHB4_GRP1_PERIPH_GPIOF
  *         @arg @ref LL_AHB4_GRP1_PERIPH_GPIOG
  *         @arg @ref LL_AHB4_GRP1_PERIPH_GPIOH
  *         @arg @ref LL_AHB4_GRP1_PERIPH_GPIOI  (*)
  *         @arg @ref LL_AHB4_GRP1_PERIPH_GPIOJ
  *         @arg @ref LL_AHB4_GRP1_PERIPH_GPIOK
  *         @arg @ref LL_AHB4_GRP1_PERIPH_CRC   (*)
  *         @arg @ref LL_AHB4_GRP1_PERIPH_BDMA
  *         @arg @ref LL_AHB4_GRP1_PERIPH_ADC3  (*)
  *         @arg @ref LL_AHB4_GRP1_PERIPH_HSEM  (*)
  *         @arg @ref LL_AHB4_GRP1_PERIPH_BKPRAM
  *         @arg @ref LL_AHB4_GRP1_PERIPH_SRAM4
  *
  *         (*) value not defined in all devices.
  * @retval None
*/
__STATIC_INLINE void AHB4_GRP1_EnableClock(uint32_t Periphs)
{
  __IO uint32_t tmpreg;
  SET_BIT(RCC->AHB4ENR, Periphs);
  /* Delay after an RCC peripheral clock enabling */
  tmpreg = READ_BIT(RCC->AHB4ENR, Periphs);
  (void)tmpreg;
}