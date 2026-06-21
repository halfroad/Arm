#include <stm32h5xx.h>

#include "../../Initializers/Includes/SystemInitializers.h"
#include "../../BSP/Includes/LED.h"
#include "../../BSP/Includes/USART.h"

int main (void)
{
    HAL_Init ();
    SystemClock_Config ();
    
    InitLEDs();
    InitUSART(115200);
    
    while (1)
    {
        USARTTransmitReceive ();
        
        ToggleLED (GREEN_LED);
        /*
        ToggleLED (YELLOW_LED);
        ToggleLED (RED_LED);
        */
        
        HAL_Delay (250);
    }
    
    return 0;
}