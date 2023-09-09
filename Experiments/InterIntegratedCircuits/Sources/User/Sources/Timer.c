#include "stm32f4xx_tim.h"
#include "stm32f4xx_rcc.h"

#include "timer.h"

RCC_ClocksTypeDef RCC_Clocks;

void timer_init(void)
{
	RCC_GetClocksFreq(&RCC_Clocks);
}

void delay_us(uint32_t us)
{
	uint32_t tick;
	
	SysTick -> LOAD = RCC_Clocks.HCLK_Frequency / 1000000 / 8 * us;
	SysTick -> VAL = 0X00;// 清空计数器
	SysTick -> CTRL = 0X01; //使能，减到零是无动作，采用外部时钟源
	
	do
	{
		tick = SysTick -> CTRL;//读取当前倒计数值
	} while((tick & 0x01) && (!(tick & ( 1<<16 ))));//等待时间到达
  
	SysTick -> CTRL = 0x00; //关闭计数器
	SysTick -> VAL = 0X00; //清空计数器
}

void delay_ms(uint16_t ms)
{
	uint32_t tick;
	
	RCC_ClocksTypeDef RCC_Clocks;
	
	RCC_GetClocksFreq(&RCC_Clocks);
	
	SysTick -> LOAD = RCC_Clocks.HCLK_Frequency / 1000 / 8 * ms;
	SysTick -> VAL = 0X00;//清空计数器
	SysTick -> CTRL = 0X01;//使能，减到零是无动作，采用外部时钟源
	
	do
	{
		tick = SysTick -> CTRL;//读取当前倒计数值
		
	} while ((tick & 0x01) && (!(tick & ( 1 << 16 ))));//等待时间到达
		
	SysTick -> CTRL = 0x00; //关闭计数器
	SysTick -> VAL = 0X00; //清空计数器
}