#include "../Include/RingBuffer.h"

RingBufferStates RingBufferWriteByte(uint8_t byte, RingBufferTypeDef *ringBuffer)
{
    if (ringBuffer -> length >= MAXIMUM_RING_BUFFER_LENGTH)
        return RingBufferStateError;
    
    ringBuffer -> bytesBuffer[ringBuffer -> tail] = byte;
    ringBuffer -> tail = (ringBuffer -> tail + 1) % MAXIMUM_RING_BUFFER_LENGTH;
    
    ringBuffer -> length ++;
    
    return RingBufferStateOK;
}

RingBufferStates RingBufferReadByte(uint8_t *byte, RingBufferTypeDef *ringBuffer)
{
    if (ringBuffer -> length == 0)
        return RingBufferStateError;
    else
    {
        *byte = ringBuffer -> bytesBuffer[ringBuffer -> head];
        
        ringBuffer -> head = (ringBuffer -> head + 1) % MAXIMUM_RING_BUFFER_LENGTH;
        ringBuffer -> length --;
        
        return RingBufferStateOK;
    }
}