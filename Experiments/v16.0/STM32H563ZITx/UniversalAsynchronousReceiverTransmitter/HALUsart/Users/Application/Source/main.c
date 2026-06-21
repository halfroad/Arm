#include <stm32h5xx.h>
#include <stdio.h>
#include <string.h>
#include <EventRecorder.h>

#include "../../Application/Include/SystemInitializers.h"
#include "../../../Drivers/BSP/Include/GPIO.h"
#include "../../../Drivers/BSP/Include/USART.h"


int main (void)
{
    HAL_Init();
    SystemClock_Config();
    
    InitializeGeneralPurposeInputOutput ();
    InitilizeUniversalAsynchronousReceiverTransmitter (115200);
    
    //EventRecorderInitialize(EventRecordAll, 1U);  // 1 = Start immediately
    //EventRecorderStart();                         // Start recording
    
    printf("Hello from Event Recorder!\r\n");
    
    char rx_bytes[] = "Hello World!";
    
    while (1)
    {
        Toggle(LED_1_GREEN);
        Toggle(LED_2_YELLOW);
        Toggle(LED_3_RED);
        
        Transmit((uint8_t *)rx_bytes, strlen(rx_bytes));
        
        HAL_Delay(250);
    }
}