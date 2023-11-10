#ifndef STM32H7xx_GPIO_H
#define STM32H7xx_GPIO_H

#include "stm32h7xx.h"


/** @defgroup GPIO_LL_EC_PIN PIN
  * @{
  */
#define GPIO_PIN_0                      GPIO_BSRR_BS0 /*!< Select pin 0 */
#define GPIO_PIN_1                      GPIO_BSRR_BS1 /*!< Select pin 1 */
#define GPIO_PIN_2                      GPIO_BSRR_BS2 /*!< Select pin 2 */
#define GPIO_PIN_3                      GPIO_BSRR_BS3 /*!< Select pin 3 */
#define GPIO_PIN_4                      GPIO_BSRR_BS4 /*!< Select pin 4 */
#define GPIO_PIN_5                      GPIO_BSRR_BS5 /*!< Select pin 5 */
#define GPIO_PIN_6                      GPIO_BSRR_BS6 /*!< Select pin 6 */
#define GPIO_PIN_7                      GPIO_BSRR_BS7 /*!< Select pin 7 */
#define GPIO_PIN_8                      GPIO_BSRR_BS8 /*!< Select pin 8 */
#define GPIO_PIN_9                      GPIO_BSRR_BS9 /*!< Select pin 9 */
#define GPIO_PIN_10                     GPIO_BSRR_BS10 /*!< Select pin 10 */
#define GPIO_PIN_11                     GPIO_BSRR_BS11 /*!< Select pin 11 */
#define GPIO_PIN_12                     GPIO_BSRR_BS12 /*!< Select pin 12 */
#define GPIO_PIN_13                     GPIO_BSRR_BS13 /*!< Select pin 13 */
#define GPIO_PIN_14                     GPIO_BSRR_BS14 /*!< Select pin 14 */
#define GPIO_PIN_15                     GPIO_BSRR_BS15 /*!< Select pin 15 */
#define GPIO_PIN_ALL                    (GPIO_BSRR_BS0 | GPIO_BSRR_BS1  | GPIO_BSRR_BS2  | \
                                           GPIO_BSRR_BS3  | GPIO_BSRR_BS4  | GPIO_BSRR_BS5  | \
                                           GPIO_BSRR_BS6  | GPIO_BSRR_BS7  | GPIO_BSRR_BS8  | \
                                           GPIO_BSRR_BS9  | GPIO_BSRR_BS10 | GPIO_BSRR_BS11 | \
                                           GPIO_BSRR_BS12 | GPIO_BSRR_BS13 | GPIO_BSRR_BS14 | \
                                           GPIO_BSRR_BS15) /*!< Select all pins */
/**
  * @}
  */

/** @defgroup GPIO_EC_MODE Mode
  * @{
  */
#define GPIO_MODE_INPUT                 (0x00000000U) /*!< Select input mode */
#define GPIO_MODE_OUTPUT                GPIO_MODER_MODE0_0  /*!< Select output mode */
#define GPIO_MODE_ALTERNATE             GPIO_MODER_MODE0_1  /*!< Select alternate function mode */
#define GPIO_MODE_ANALOG                GPIO_MODER_MODE0    /*!< Select analog mode */

/**
  * @}
  */

/** @defgroup GPIO_EC_SPEED Output Speed
  * @{
  */
#define GPIO_SPEED_FREQ_LOW             (0x00000000U) /*!< Select I/O low output speed    */
#define GPIO_SPEED_FREQ_MEDIUM          GPIO_OSPEEDR_OSPEED0_0 /*!< Select I/O medium output speed */
#define GPIO_SPEED_FREQ_HIGH            GPIO_OSPEEDR_OSPEED0_1 /*!< Select I/O fast output speed   */
#define GPIO_SPEED_FREQ_VERY_HIGH       GPIO_OSPEEDR_OSPEED0   /*!< Select I/O high output speed   */

/**
  * @}
  */

/** @defgroup GPIO_EC_OUTPUT Output Type
  * @{
  */
#define GPIO_OUTPUT_PUSHPULL            (0x00000000U) /*!< Select push-pull as output type */
#define GPIO_OUTPUT_OPENDRAIN           GPIO_OTYPER_OT0 /*!< Select open-drain as output type */

/** @defgroup GPIO_EC_PULL Pull Up Pull Down
  * @{
  */
#define GPIO_PULL_NO                    (0x00000000U) /*!< Select I/O no pull */
#define GPIO_PULL_UP                    GPIO_PUPDR_PUPD0_0 /*!< Select I/O pull up */
#define GPIO_PULL_DOWN                  GPIO_PUPDR_PUPD0_1 /*!< Select I/O pull down */

/**
  * @brief LL GPIO Init Structure definition
  */
typedef struct
{
  uint32_t Pin;          /*!< Specifies the GPIO pins to be configured.
                              This parameter can be any value of @ref GPIO_EC_PIN */

  uint32_t Mode;         /*!< Specifies the operating mode for the selected pins.
                              This parameter can be a value of @ref GPIO_EC_MODE.

                              GPIO HW configuration can be modified afterwards using unitary function @ref GPIO_SetPinMode().*/

  uint32_t Speed;        /*!< Specifies the speed for the selected pins.
                              This parameter can be a value of @ref GPIO_EC_SPEED.

                              GPIO HW configuration can be modified afterwards using unitary function @ref GPIO_SetPinSpeed().*/

  uint32_t OutputType;   /*!< Specifies the operating output type for the selected pins.
                              This parameter can be a value of @ref GPIO_EC_OUTPUT.

                              GPIO HW configuration can be modified afterwards using unitary function @ref GPIO_SetPinOutputType().*/

  uint32_t Pull;         /*!< Specifies the operating Pull-up/Pull down for the selected pins.
                              This parameter can be a value of @ref GPIO_EC_PULL.

                              GPIO HW configuration can be modified afterwards using unitary function @ref GPIO_SetPinPull().*/

  uint32_t Alternate;    /*!< Specifies the Peripheral to be connected to the selected pins.
                              This parameter can be a value of @ref GPIO_EC_AF.

                              GPIO HW configuration can be modified afterwards using unitary function @ref GPIO_SetAFPin_0_7() and GPIO_SetAFPin_8_15().*/
} GPIO_InitTypeDef;


/**
  * @brief  Set several pins to low level on dedicated gpio port.
  * @rmtoll BSRR         BRy           GPIO_ResetOutputPin
  * @param  GPIOx GPIO Port
  * @param  PinMask This parameter can be a combination of the following values:
  *         @arg @ref GPIO_PIN_0
  *         @arg @ref GPIO_PIN_1
  *         @arg @ref GPIO_PIN_2
  *         @arg @ref GPIO_PIN_3
  *         @arg @ref GPIO_PIN_4
  *         @arg @ref GPIO_PIN_5
  *         @arg @ref GPIO_PIN_6
  *         @arg @ref GPIO_PIN_7
  *         @arg @ref GPIO_PIN_8
  *         @arg @ref GPIO_PIN_9
  *         @arg @ref GPIO_PIN_10
  *         @arg @ref GPIO_PIN_11
  *         @arg @ref GPIO_PIN_12
  *         @arg @ref GPIO_PIN_13
  *         @arg @ref GPIO_PIN_14
  *         @arg @ref GPIO_PIN_15
  *         @arg @ref GPIO_PIN_ALL
  * @retval None
  */
__STATIC_INLINE void GPIO_ResetOutputPin(GPIO_TypeDef *GPIOx, uint32_t PinMask)
{
  WRITE_REG(GPIOx->BSRR, PinMask << 16U);
}

/**
  * @brief  Configure gpio speed for a dedicated pin on dedicated port.
  * @note   I/O speed can be Low, Medium, Fast or High speed.
  * @note   Warning: only one pin can be passed as parameter.
  * @note   Refer to datasheet for frequency specifications and the power
  *         supply and load conditions for each speed.
  * @rmtoll OSPEEDR      OSPEEDy       GPIO_SetPinSpeed
  * @param  GPIOx GPIO Port
  * @param  Pin This parameter can be one of the following values:
  *         @arg @ref GPIO_PIN_0
  *         @arg @ref GPIO_PIN_1
  *         @arg @ref GPIO_PIN_2
  *         @arg @ref GPIO_PIN_3
  *         @arg @ref GPIO_PIN_4
  *         @arg @ref GPIO_PIN_5
  *         @arg @ref GPIO_PIN_6
  *         @arg @ref GPIO_PIN_7
  *         @arg @ref GPIO_PIN_8
  *         @arg @ref GPIO_PIN_9
  *         @arg @ref GPIO_PIN_10
  *         @arg @ref GPIO_PIN_11
  *         @arg @ref GPIO_PIN_12
  *         @arg @ref GPIO_PIN_13
  *         @arg @ref GPIO_PIN_14
  *         @arg @ref GPIO_PIN_15
  * @param  Speed This parameter can be one of the following values:
  *         @arg @ref GPIO_SPEED_FREQ_LOW
  *         @arg @ref GPIO_SPEED_FREQ_MEDIUM
  *         @arg @ref GPIO_SPEED_FREQ_HIGH
  *         @arg @ref GPIO_SPEED_FREQ_VERY_HIGH
  * @retval None
  */
__STATIC_INLINE void GPIO_SetPinSpeed(GPIO_TypeDef *GPIOx, uint32_t Pin, uint32_t  Speed)
{
  MODIFY_REG(GPIOx->OSPEEDR, ((Pin * Pin) * GPIO_OSPEEDR_OSPEED0), ((Pin * Pin) * Speed));
}

/**
  * @brief  Configure gpio output type for several pins on dedicated port.
  * @note   Output type as to be set when gpio pin is in output or
  *         alternate modes. Possible type are Push-pull or Open-drain.
  * @rmtoll OTYPER       OTy           GPIO_SetPinOutputType
  * @param  GPIOx GPIO Port
  * @param  PinMask This parameter can be a combination of the following values:
  *         @arg @ref GPIO_PIN_0
  *         @arg @ref GPIO_PIN_1
  *         @arg @ref GPIO_PIN_2
  *         @arg @ref GPIO_PIN_3
  *         @arg @ref GPIO_PIN_4
  *         @arg @ref GPIO_PIN_5
  *         @arg @ref GPIO_PIN_6
  *         @arg @ref GPIO_PIN_7
  *         @arg @ref GPIO_PIN_8
  *         @arg @ref GPIO_PIN_9
  *         @arg @ref GPIO_PIN_10
  *         @arg @ref GPIO_PIN_11
  *         @arg @ref GPIO_PIN_12
  *         @arg @ref GPIO_PIN_13
  *         @arg @ref GPIO_PIN_14
  *         @arg @ref GPIO_PIN_15
  *         @arg @ref GPIO_PIN_ALL
  * @param  OutputType This parameter can be one of the following values:
  *         @arg @ref GPIO_OUTPUT_PUSHPULL
  *         @arg @ref GPIO_OUTPUT_OPENDRAIN
  * @retval None
  */
__STATIC_INLINE void GPIO_SetPinOutputType(GPIO_TypeDef *GPIOx, uint32_t PinMask, uint32_t OutputType)
{
  MODIFY_REG(GPIOx->OTYPER, PinMask, (PinMask * OutputType));
}

/**
  * @brief  Configure gpio pull-up or pull-down for a dedicated pin on a dedicated port.
  * @note   Warning: only one pin can be passed as parameter.
  * @rmtoll PUPDR        PUPDy         GPIO_SetPinPull
  * @param  GPIOx GPIO Port
  * @param  Pin This parameter can be one of the following values:
  *         @arg @ref GPIO_PIN_0
  *         @arg @ref GPIO_PIN_1
  *         @arg @ref GPIO_PIN_2
  *         @arg @ref GPIO_PIN_3
  *         @arg @ref GPIO_PIN_4
  *         @arg @ref GPIO_PIN_5
  *         @arg @ref GPIO_PIN_6
  *         @arg @ref GPIO_PIN_7
  *         @arg @ref GPIO_PIN_8
  *         @arg @ref GPIO_PIN_9
  *         @arg @ref GPIO_PIN_10
  *         @arg @ref GPIO_PIN_11
  *         @arg @ref GPIO_PIN_12
  *         @arg @ref GPIO_PIN_13
  *         @arg @ref GPIO_PIN_14
  *         @arg @ref GPIO_PIN_15
  * @param  Pull This parameter can be one of the following values:
  *         @arg @ref GPIO_PUNO
  *         @arg @ref GPIO_PUUP
  *         @arg @ref GPIO_PUDOWN
  * @retval None
  */
__STATIC_INLINE void GPIO_SetPinPull(GPIO_TypeDef *GPIOx, uint32_t Pin, uint32_t Pull)
{
  MODIFY_REG(GPIOx->PUPDR, ((Pin * Pin) * GPIO_PUPDR_PUPD0), ((Pin * Pin) * Pull));
}

/**
  * @brief  Configure gpio alternate function of a dedicated pin from 0 to 7 for a dedicated port.
  * @note   Possible values are from AF0 to AF15 depending on target.
  * @note   Warning: only one pin can be passed as parameter.
  * @rmtoll AFRL         AFSELy        GPIO_SetAFPin_0_7
  * @param  GPIOx GPIO Port
  * @param  Pin This parameter can be one of the following values:
  *         @arg @ref GPIO_PIN_0
  *         @arg @ref GPIO_PIN_1
  *         @arg @ref GPIO_PIN_2
  *         @arg @ref GPIO_PIN_3
  *         @arg @ref GPIO_PIN_4
  *         @arg @ref GPIO_PIN_5
  *         @arg @ref GPIO_PIN_6
  *         @arg @ref GPIO_PIN_7
  * @param  Alternate This parameter can be one of the following values:
  *         @arg @ref GPIO_AF_0
  *         @arg @ref GPIO_AF_1
  *         @arg @ref GPIO_AF_2
  *         @arg @ref GPIO_AF_3
  *         @arg @ref GPIO_AF_4
  *         @arg @ref GPIO_AF_5
  *         @arg @ref GPIO_AF_6
  *         @arg @ref GPIO_AF_7
  *         @arg @ref GPIO_AF_8
  *         @arg @ref GPIO_AF_9
  *         @arg @ref GPIO_AF_10
  *         @arg @ref GPIO_AF_11
  *         @arg @ref GPIO_AF_12
  *         @arg @ref GPIO_AF_13
  *         @arg @ref GPIO_AF_14
  *         @arg @ref GPIO_AF_15
  * @retval None
  */
__STATIC_INLINE void GPIO_SetAFPin_0_7(GPIO_TypeDef *GPIOx, uint32_t Pin, uint32_t Alternate)
{
  MODIFY_REG(GPIOx->AFR[0], ((((Pin * Pin) * Pin) * Pin) * GPIO_AFRL_AFSEL0),
             ((((Pin * Pin) * Pin) * Pin) * Alternate));
}

/**
  * @brief  Configure gpio alternate function of a dedicated pin from 8 to 15 for a dedicated port.
  * @note   Possible values are from AF0 to AF15 depending on target.
  * @note   Warning: only one pin can be passed as parameter.
  * @rmtoll AFRH         AFSELy        GPIO_SetAFPin_8_15
  * @param  GPIOx GPIO Port
  * @param  Pin This parameter can be one of the following values:
  *         @arg @ref GPIO_PIN_8
  *         @arg @ref GPIO_PIN_9
  *         @arg @ref GPIO_PIN_10
  *         @arg @ref GPIO_PIN_11
  *         @arg @ref GPIO_PIN_12
  *         @arg @ref GPIO_PIN_13
  *         @arg @ref GPIO_PIN_14
  *         @arg @ref GPIO_PIN_15
  * @param  Alternate This parameter can be one of the following values:
  *         @arg @ref GPIO_AF_0
  *         @arg @ref GPIO_AF_1
  *         @arg @ref GPIO_AF_2
  *         @arg @ref GPIO_AF_3
  *         @arg @ref GPIO_AF_4
  *         @arg @ref GPIO_AF_5
  *         @arg @ref GPIO_AF_6
  *         @arg @ref GPIO_AF_7
  *         @arg @ref GPIO_AF_8
  *         @arg @ref GPIO_AF_9
  *         @arg @ref GPIO_AF_10
  *         @arg @ref GPIO_AF_11
  *         @arg @ref GPIO_AF_12
  *         @arg @ref GPIO_AF_13
  *         @arg @ref GPIO_AF_14
  *         @arg @ref GPIO_AF_15
  * @retval None
  */
__STATIC_INLINE void GPIO_SetAFPin_8_15(GPIO_TypeDef *GPIOx, uint32_t Pin, uint32_t Alternate)
{
  MODIFY_REG(GPIOx->AFR[1], (((((Pin >> 8U) * (Pin >> 8U)) * (Pin >> 8U)) * (Pin >> 8U)) * GPIO_AFRH_AFSEL8),
             (((((Pin >> 8U) * (Pin >> 8U)) * (Pin >> 8U)) * (Pin >> 8U)) * Alternate));
}

/**
  * @brief  Configure gpio mode for a dedicated pin on dedicated port.
  * @note   I/O mode can be Input mode, General purpose output, Alternate function mode or Analog.
  * @note   Warning: only one pin can be passed as parameter.
  * @rmtoll MODER        MODEy         GPIO_SetPinMode
  * @param  GPIOx GPIO Port
  * @param  Pin This parameter can be one of the following values:
  *         @arg @ref GPIO_PIN_0
  *         @arg @ref GPIO_PIN_1
  *         @arg @ref GPIO_PIN_2
  *         @arg @ref GPIO_PIN_3
  *         @arg @ref GPIO_PIN_4
  *         @arg @ref GPIO_PIN_5
  *         @arg @ref GPIO_PIN_6
  *         @arg @ref GPIO_PIN_7
  *         @arg @ref GPIO_PIN_8
  *         @arg @ref GPIO_PIN_9
  *         @arg @ref GPIO_PIN_10
  *         @arg @ref GPIO_PIN_11
  *         @arg @ref GPIO_PIN_12
  *         @arg @ref GPIO_PIN_13
  *         @arg @ref GPIO_PIN_14
  *         @arg @ref GPIO_PIN_15
  * @param  Mode This parameter can be one of the following values:
  *         @arg @ref GPIO_MODE_INPUT
  *         @arg @ref GPIO_MODE_OUTPUT
  *         @arg @ref GPIO_MODE_ALTERNATE
  *         @arg @ref GPIO_MODE_ANALOG
  * @retval None
  */
__STATIC_INLINE void GPIO_SetPinMode(GPIO_TypeDef *GPIOx, uint32_t Pin, uint32_t Mode)
{
  MODIFY_REG(GPIOx->MODER, ((Pin * Pin) * GPIO_MODER_MODE0), ((Pin * Pin) * Mode));
}

/**
  * @brief  Initialize GPIO registers according to the specified parameters in GPIO_InitStruct.
  * @param  GPIOx GPIO Port
  * @param GPIO_InitStruct pointer to a @ref GPIO_InitTypeDef structure
  *         that contains the configuration information for the specified GPIO peripheral.
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: GPIO registers are initialized according to GPIO_InitStruct content
  *          - ERROR:   Not applicable
  */
ErrorStatus GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef *GPIO_InitStruct);

/**
  * @brief  Toggle data value for several pin of dedicated port.
  * @rmtoll ODR          ODy           GPIO_TogglePin
  * @param  GPIOx GPIO Port
  * @param  PinMask This parameter can be a combination of the following values:
  *         @arg @ref GPIO_PIN_0
  *         @arg @ref GPIO_PIN_1
  *         @arg @ref GPIO_PIN_2
  *         @arg @ref GPIO_PIN_3
  *         @arg @ref GPIO_PIN_4
  *         @arg @ref GPIO_PIN_5
  *         @arg @ref GPIO_PIN_6
  *         @arg @ref GPIO_PIN_7
  *         @arg @ref GPIO_PIN_8
  *         @arg @ref GPIO_PIN_9
  *         @arg @ref GPIO_PIN_10
  *         @arg @ref GPIO_PIN_11
  *         @arg @ref GPIO_PIN_12
  *         @arg @ref GPIO_PIN_13
  *         @arg @ref GPIO_PIN_14
  *         @arg @ref GPIO_PIN_15
  *         @arg @ref GPIO_PIN_ALL
  * @retval None
  */
__STATIC_INLINE void GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint32_t PinMask)
{
  uint32_t odr = READ_REG(GPIOx->ODR);
  WRITE_REG(GPIOx->BSRR, ((odr & PinMask) << 16u) | (~odr & PinMask));
}

#endif