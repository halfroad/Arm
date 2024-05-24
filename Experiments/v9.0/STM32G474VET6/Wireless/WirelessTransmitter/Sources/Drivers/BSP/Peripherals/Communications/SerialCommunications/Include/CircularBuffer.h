#ifndef __CIRCULAR_BUFFER_H
#define __CIRCULAR_BUFFER_H

#include <stm32h5xx.h>

#define MAXIMUM_CIRCULAR_BUFFER_LENGTH              128

typedef struct
{
    uint16_t head;
    uint16_t tail;
    
    uint16_t length;
    
    uint8_t bytesBuffer[MAXIMUM_CIRCULAR_BUFFER_LENGTH];
    
} CircularBufferTypeDef;

typedef enum
{
    CircularBufferStateOK,
    CircularBufferStateError,
    
} CircularBufferStates;

void InitCircularBuffer(void);

CircularBufferStates CircularBufferWriteByte(uint8_t byte, CircularBufferTypeDef *circularBufferType);
CircularBufferStates CircularBufferReadByte(uint8_t *byte, CircularBufferTypeDef *circularBufferType);
#endif  /*  #ifndef __CIRCULAR_BUFFER_H */