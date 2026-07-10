#include <stm32h5xx.h>

#include "../../Systems/Initializers/Includes/SystemInitializers.h"
#include "../../BSP/Includes/LED.h"
#include "../../BSP/Includes/USART.h"

#define USART_BAUD_RATE             115200

int main (void)
{
    HAL_Init ();
    SystemClock_Config ();
    
    InitLEDs ();
    InitUSART(USART_BAUD_RATE);
    
    while (1)
    {
        TransmitReceive();
        /*
        
        ToggleLED (GREEN_LED);
        ToggleLED (YELLOW_LED);
        ToggleLED (RED_LED);
        
        HAL_Delay (250);
        */
    }
    
    return 0;
}