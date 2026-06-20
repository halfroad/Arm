#include <stm32h5xx.h>

#include "../../BSP/Includes/LED.h"
#include "../../BSP/Includes/USART.h"

int main (void)
{
    HAL_Init();
    
    InitLEDs();
    InitUSART(115200);
    
    while (1)
    {
        TransmitReceive();
        
        Toggle(GREEN_LED);
        Toggle(YELLOW_LED);
        Toggle(RED_LED);
        
        HAL_Delay (250);
    }
    
    return 0;
}