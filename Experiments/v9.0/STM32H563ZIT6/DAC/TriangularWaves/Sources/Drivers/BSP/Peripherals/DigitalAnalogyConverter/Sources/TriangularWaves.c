#include "./Peripherals/Timers/Include/Delay.h"

#include "../Include/TriangularWaves.h"

void GenerateTriangularWaves (uint16_t maximum, uint16_t duration, uint16_t samples, uint16_t quanlity)
{
    if (maximum + 1 > samples)
    {
        float increment = (maximum + 1) / (samples / 2);
        float current = 0.0f;
        
        uint16_t j = 0;
        
        for (uint16_t i = 0; i < quanlity; i ++)
        {
            AssignOutputQuantity(current);
            
            for (j = 0; j < samples / 2; j ++)
            {
                current += increment;
                
                AssignOutputQuantity(current);
                DelayUs(duration);
            }
            
            for (j = 0; j < samples / 2; j ++)
            {
                current -= increment;
                
                AssignOutputQuantity(current);
                DelayUs(duration);
            }
        }
    }
}