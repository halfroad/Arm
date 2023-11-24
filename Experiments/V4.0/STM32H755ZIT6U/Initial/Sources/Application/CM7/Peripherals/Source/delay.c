#ifndef DELAY_H

#define DELAY_G

#include "delay.h"


static uint8_t fac_microseconds = 0;
static uint16_t fac_milliseconds = 0;

void DelayInit(uint16_t systemTicks)
{
     /* __IOM uint32_t CTRL;                   !< Offset: 0x000 (R/W)  SysTick Control and Status Register */
     /* __IOM uint32_t LOAD;                   !< Offset: 0x004 (R/W)  SysTick Reload Value Register */
     /* __IOM uint32_t VAL;                    !< Offset: 0x008 (R/W)  SysTick Current Value Register */
     /* __IM  uint32_t CALIB;                  !< Offset: 0x00C (R/ )  SysTick Calibration Register */
    
    SysTick -> CTRL |= 0x01 << 2;
    
    fac_microseconds = systemTicks;
    fac_milliseconds = (uint16_t) fac_microseconds * 1000;
}

void DelayMicroseconds(uint32_t numberOfMicroseconds)
{
    uint32_t times;
    
    SysTick -> LOAD = numberOfMicroseconds * fac_microseconds;  /* Load the durartion of delay. */
    SysTick -> VAL = 0x00;                                      /* Clear the counter. */
    SysTick -> CTRL |= SysTick_CTRL_ENABLE_Msk;                               /* Enbale the countdown. */
    
    do
    {
        times = SysTick -> CTRL;
    }
    while ((times & 0x01) && !(times & (0x01 << 16)));          /* Wait till the countdown reaches the end. */
    
    SysTick -> CTRL &= ~SysTick_CTRL_ENABLE_Msk;                            /* Disable the countdown. */
    SysTick -> VAL = 0x00;                                      /* Reset the counter. */
}


static void DelayNumberOfMilliseconds(uint16_t numberOfMilliseconds)
{	 		  	  
	uint32_t times;
    
	SysTick -> LOAD = (uint32_t) numberOfMilliseconds * fac_milliseconds;
	SysTick -> VAL = 0x00;
	SysTick -> CTRL |= SysTick_CTRL_ENABLE_Msk;
    
	do
	{
		times = SysTick -> CTRL;
        
	}
    while((times & 0x01) &&!(times & (0x01 << 16)));
    
	SysTick -> CTRL &= ~SysTick_CTRL_ENABLE_Msk;
	SysTick -> VAL = 0X00;   
} 


void DelayMilliseconds(uint16_t numberOfMilliseconds)
{	 	 
	uint8_t repetitions = numberOfMilliseconds / 540;
	uint16_t residual = numberOfMilliseconds % 540;
    
	while(repetitions)
	{
		DelayNumberOfMilliseconds(540);
		repetitions --;
	}
    
	if(residual)
        DelayNumberOfMilliseconds(residual);
} 

#endif