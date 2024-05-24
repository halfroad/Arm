#include "../Include/CircularBuffer.h"

CircularBufferStates CircularBufferWriteByte(uint8_t byte, CircularBufferTypeDef *circularBufferType)
{
    if (circularBufferType -> length >= MAXIMUM_CIRCULAR_BUFFER_LENGTH)
        return CircularBufferStateError;
    
    circularBufferType -> bytesBuffer[circularBufferType -> tail] = byte;
    circularBufferType -> tail = (circularBufferType -> tail + 1) % MAXIMUM_CIRCULAR_BUFFER_LENGTH;
    
    circularBufferType -> length ++;
    
    return CircularBufferStateOK;
}

CircularBufferStates CircularBufferReadByte(uint8_t *byte, CircularBufferTypeDef *circularBufferType)
{
    if (circularBufferType -> length == 0)
        return CircularBufferStateError;
    else
    {
        *byte = circularBufferType -> bytesBuffer[circularBufferType -> head];
        
        circularBufferType -> head = (circularBufferType -> head + 1) % MAXIMUM_CIRCULAR_BUFFER_LENGTH;
        circularBufferType -> length --;
        
        return CircularBufferStateOK;
    }
}
