#ifndef __RING_BUFFER_H
#define __RING_BUFFER_H

#include <stm32h7xx.h>

#define MAXIMUM_RING_BUFFER_LENGTH          128

typedef struct
{
    uint16_t head;
    uint16_t tail;
    
    uint16_t length;
    
    uint8_t bytesBuffer[MAXIMUM_RING_BUFFER_LENGTH];
    
} RingBufferTypeDef;

typedef enum
{
    RingBufferStateOK,
    RingBufferStateError,
    
} RingBufferStates;


RingBufferStates RingBufferWriteByte(uint8_t byte, RingBufferTypeDef *ringBuffer);
RingBufferStates RingBufferReadByte(uint8_t *byte, RingBufferTypeDef *ringBuffer);

#endif  /*  #ifndef __RING_BUFFER_H */