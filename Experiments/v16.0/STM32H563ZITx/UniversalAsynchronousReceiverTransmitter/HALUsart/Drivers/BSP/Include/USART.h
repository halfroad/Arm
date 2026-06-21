
#ifndef __USART_H
#define __USART_H

#include <stm32h5xx.h>

/**
 * @brief Initilize Universal Asynchronous Receiver Transmitter
 * @retval None
 */
void InitilizeUniversalAsynchronousReceiverTransmitter(uint32_t baudRate);

HAL_StatusTypeDef Transmit (uint8_t *bytes, uint16_t size);
HAL_StatusTypeDef Receive (uint8_t *byte);

#endif  // __USART_H