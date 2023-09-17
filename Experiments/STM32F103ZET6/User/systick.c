#include "systick.h"

uint32_t systick_count;

void Delay_us(uint32_t us)
{
	if(SysTick_Config(SystemCoreClock/1000000))//1us进一次中断
	{
		while(1);
	}
	systick_count = us;
	while(0U != systick_count);
	
	SysTick->CTRL  = 0x00;		//失能滴答定时器
	SysTick->VAL   = 0;			/* Load the SysTick Counter Value */
}
void Delay_ms(uint16_t ms)
{
	Delay_us(ms * 1000);
}

void Delay_count(void)
{
	if(0U != systick_count)
	{
		systick_count--;
	}
}

