#include "sys.h" 
#include "usart.h" 
#include "delay.h" 
#include "led.h"
#include "key.h"
#include "lcd.h"  
#include "sdram.h" 
#include "ltdc.h"
#include "rtc.h"
//ALIENTEK 北极星STM32H750/F750开发板 实验17
//RTC 实验  
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司 

int main(void)
{
	u8 hour,min,sec,ampm;
	u8 year,month,date,week;
	u8 tbuf[40];
	u8 t=0;  	
	Cache_Enable();								//打开L1-Cache
	HAL_Init();				        		//初始化HAL库
	Stm32_Clock_Init(160,5,2,4);	//设置时钟,400Mhz 
	QSPI_Enable_Memmapmode();			//QSPI内存映射模式
	delay_init(400);							//延时初始化
	uart_init(115200);						//串口初始化
	LED_Init();										//初始化LED时钟
	KEY_Init();
	SDRAM_Init();                   		//初始化SDRAM
	LCD_Init();						//初始化LCD
	RTC_Init();		 				//初始化RTC
	RTC_Set_WakeUp(4,0);			//配置WAKE UP中断,1秒钟中断一次 
	POINT_COLOR=RED;
	LCD_ShowString(30,50,200,16,16,"POLARIS H750/F750"); 
	LCD_ShowString(30,70,200,16,16,"RTC TEST");	
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,"2019/4/28");	
  while(1) 
	{		
		t++;
		if((t%10)==0)	//每100ms更新一次显示数据
		{
			RTC_Get_Time(&hour,&min,&sec,&ampm);
			sprintf((char*)tbuf,"Time:%02d:%02d:%02d",hour,min,sec); 
			LCD_ShowString(30,140,210,16,16,tbuf);	
			RTC_Get_Date(&year,&month,&date,&week);
			sprintf((char*)tbuf,"Date:20%02d-%02d-%02d",year,month,date); 
			LCD_ShowString(30,160,210,16,16,tbuf);	
			sprintf((char*)tbuf,"Week:%d",week); 
			LCD_ShowString(30,180,210,16,16,tbuf);
		} 
		if((t%20)==0)LED0_Toggle;//每200ms,翻转一次LED0 
		delay_ms(10);
	} 
}
