#include <string.h>

#include "SystemInitializer.h"
#include "Delay.h"
#include "GPIO.h"
#include "OLED.h"
#include "MemoryProtectiveUnit.h"

//#define ARRAY_STORE_ADDRESS     "0x20002000"

//uint8_t array[128]  __attribute__((section(".ARM.__at_0x20002000")));
uint8_t array[128] __attribute__((section(".ARM.__at_0x30000000")));
uint8_t buttonPressed = 0x00;

int main(void)
{
    /*
    
    User LD1: a green user LED is connected to the STM32H7 I/O PB0 (SB65 OFF and SB54
    ON) or PA5 (SB65 ON and SB54 OFF) corresponding to the ST Zio D13.
    
    */

    /*
    
    B1 user (Blue button): the user button is connected to the I/O PC13 by default (Tamper
    support, SB82 ON and SB81 OFF) or PA0 (Wakeup support, SB81 ON and SB82 OFF) of
    the STM32H7 microcontroller.
    
    */
    
#ifdef USE_HSE
    InitSystem(1, 120, 2, 2, 2);
#else
    InitSystem(4, 60, 2, 2, 2);
#endif
    InitDelay(240);
    InitGPIOs();
    OLED_Init();
    
    /* 500ms to reload. */
    SetMemoryProtectiveRegion(0x30000000, 128, 7 ,0, MPU_REGION_PRIV_RO_URO, 0, 0, 1);
     
    while (1)
    {
    }
}

void EXTI15_10_IRQHandler(void)
{
    memcpy(array, "aaaaaaaaaaaa", 10);
    int i = 1;
}

void MemManage_Handler(void)
{
    OLED_ShowString(8, 16, (uint8_t *)"Memeory Access Error.", 8, 1);
    
    DelayMilliseconds(1000);
    
    OLED_ShowString(8, 24, (uint8_t *)"Soft Reset now....", 8, 1);
    
    DelayMilliseconds(1000);
    
    OLED_Refresh();
    
   // SoftResetSystem();
}
