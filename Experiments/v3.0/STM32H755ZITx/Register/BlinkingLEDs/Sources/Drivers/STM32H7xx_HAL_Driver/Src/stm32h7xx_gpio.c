#include "stm32h7xx_gpio.h"

#ifdef  USE_FULL_ASSERT
#include "stm32_assert.h"
#else
#define assert_param(expr) ((void)0U)
#endif

/**
  * @brief  Initialize GPIO registers according to the specified parameters in GPIO_InitStruct.
  * @param  GPIOx GPIO Port
  * @param GPIO_InitStruct pointer to a @ref GPIO_InitTypeDef structure
  *         that contains the configuration information for the specified GPIO peripheral.
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: GPIO registers are initialized according to GPIO_InitStruct content
  *          - ERROR:   Not applicable
  */
ErrorStatus GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef *GPIO_InitStruct)
{
  uint32_t pinpos, currentpin;

  /* Check the parameters */
  assert_param(IS_GPIO_AINSTANCE(GPIOx));
  assert_param(IS_GPIO_PIN(GPIO_InitStruct->Pin));
  assert_param(IS_GPIO_MODE(GPIO_InitStruct->Mode));
  assert_param(IS_GPIO_PULL(GPIO_InitStruct->Pull));

  /* ------------------------- Configure the port pins ---------------- */
  /* Initialize  pinpos on first pin set */
  pinpos = POSITION_VAL(GPIO_InitStruct->Pin);

  /* Configure the port pins */
  while (((GPIO_InitStruct->Pin) >> pinpos) != 0x00000000U)
  {
    /* Get current io position */
    currentpin = (GPIO_InitStruct->Pin) & (0x00000001UL << pinpos);

    if (currentpin != 0x00000000U)
    {

      if ((GPIO_InitStruct->Mode == GPIO_MODE_OUTPUT) || (GPIO_InitStruct->Mode == GPIO_MODE_ALTERNATE))
      {
        /* Check Speed mode parameters */
        assert_param(IS_GPIO_SPEED(GPIO_InitStruct->Speed));

        /* Speed mode configuration */
        GPIO_SetPinSpeed(GPIOx, currentpin, GPIO_InitStruct->Speed);

        /* Check Output mode parameters */
        assert_param(IS_GPIO_OUTPUT_TYPE(GPIO_InitStruct->OutputType));

        /* Output mode configuration*/
        GPIO_SetPinOutputType(GPIOx, GPIO_InitStruct->Pin, GPIO_InitStruct->OutputType);

      }

      /* Pull-up Pull down resistor configuration*/
      GPIO_SetPinPull(GPIOx, currentpin, GPIO_InitStruct->Pull);

      if (GPIO_InitStruct->Mode == GPIO_MODE_ALTERNATE)
      {
        /* Check Alternate parameter */
        assert_param(IS_GPIO_ALTERNATE(GPIO_InitStruct->Alternate));

        /* Alternate function configuration */
        if (currentpin < GPIO_PIN_8)
        {
          GPIO_SetAFPin_0_7(GPIOx, currentpin, GPIO_InitStruct->Alternate);
        }
        else
        {
          GPIO_SetAFPin_8_15(GPIOx, currentpin, GPIO_InitStruct->Alternate);
        }
      }

      /* Pin Mode configuration */
      GPIO_SetPinMode(GPIOx, currentpin, GPIO_InitStruct->Mode);
    }
    pinpos++;
  }

  return (SUCCESS);
}