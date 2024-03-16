#include "../Include/AnglesConvertor.h"

/*
*   @brief      Convert the angle of Servo Motor to Timer Compare.
*   @param      angle:  the angle of Servo Motor to be converted.
*   @retval     The converted Timer Compare value.
*/
uint16_t ConvertAngle(float angle)
{
    uint16_t compare;
    
    if (angle < 0 || angle > 180)
        compare = 0;
    else
    {
        /*
        
        0��      --->    500,
        45��     --->    1000,
        90��     --->    1500,
        135��    --->    2000,
        135��    --->    2500,
        
        */
        compare = 1500 + (angle - 90) * 100 / 9;
        
        if (compare < 500 || compare > 2500)
            compare = 0;
    }
    
    return compare;
}