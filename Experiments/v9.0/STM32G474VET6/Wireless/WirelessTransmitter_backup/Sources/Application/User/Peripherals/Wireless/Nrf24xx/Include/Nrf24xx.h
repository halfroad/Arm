#ifndef __Nrf24xx_H
#define __Nrf24xx_H

#ifdef STM32H563xx

#include <stm32h5xx.h>

#else
                                                                        
#ifdef STM32G474xx

#include <stm32g4xx.h>

#endif
#endif

#define ADDRESS_WIDTH                                                   5
#define PAYLOAD_WIDTH                                                   32

typedef enum
{
    ChipEnabled,
    ChipDisabled,
    
} ChipEnablements;

typedef enum
{
    RadioFrequencyDataRate1Mpbs     ,
    RadioFrequencyDataRate2Mbps     ,
    RadioFrequencyDataRate250kpbs   ,

} RadioFrequencyDataRates;

typedef enum
{
    PackageTransceiveStateMaximumRetrialsReached,
    PackageTransceiveStateOK,
    PackageTransceiveStateError,
    
} PackageTransceiveStates;

void Nrf24xxRxInit(uint16_t channel, RadioFrequencyDataRates rate);
void Nrf24xxTxInit(uint16_t channel, RadioFrequencyDataRates rate);

HAL_StatusTypeDef Nrf24xxTransmit(uint8_t *payload);
HAL_StatusTypeDef Nrf24xxReceive(uint8_t *payload);

#endif  /*  #ifndef __Nrf24xx_H  */