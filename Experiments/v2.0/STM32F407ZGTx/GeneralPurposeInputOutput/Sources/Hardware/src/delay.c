#include "misc.h"

#include "delay.h"
 
static uint8_t  fac_us = 0;		   
static uint16_t fac_ms = 0;

void delay_init(uint8_t SYSCLK)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	
	fac_us = SYSCLK / 8;
	fac_ms = (uint32_t) SYSCLK * 1000;
}


void delay_us(uint32_t ticks)
{
	uint32_t time;
	
	SysTick -> LOAD = ticks * fac_us;  		 
	SysTick -> VAL = 0x00;
	SysTick -> CTRL |= SysTick_CTRL_ENABLE_Msk;
	
	do
	{
		time = SysTick -> CTRL;
		
	} while ((time & 0x01) && !(time & (1 << 16)));
  
	SysTick -> CTRL = SysTick_CTRL_ENABLE_Msk;
	SysTick -> VAL  = 0X00;  
}

void delay_xms(uint16_t ms)
{	 		  	  
	uint32_t time;
	
	SysTick -> LOAD = (uint32_t) ms * fac_ms;
	SysTick -> VAL  = 0x00;
	SysTick -> CTRL |= SysTick_CTRL_ENABLE_Msk ;
	
	do
	{
		time = SysTick -> CTRL;
		
	} while((time & 0x01) && !(time & (1 << 16)));
 
	SysTick -> CTRL = SysTick_CTRL_ENABLE_Msk;
	SysTick -> VAL  = 0X00;	  	    
} 
 
void delay_ms(uint16_t ms)
{
	uint8_t repeat = ms / 540;
	uint16_t remain = ms % 540;
	
	while(repeat)
	{
		delay_xms(540);
		repeat--;
	}
  
	if(remain)
		delay_xms(remain);
  
}