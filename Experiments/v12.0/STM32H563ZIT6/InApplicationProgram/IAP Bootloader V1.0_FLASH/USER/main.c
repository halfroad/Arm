#include "sys.h"
#include "usart.h" 
#include "delay.h" 
#include "led.h"
#include "mpu.h"   
#include "lcd.h"  
#include "sdram.h" 
#include "ltdc.h"   
#include "key.h" 

//北极星STM32H750/F750开发板 实验56
//串口IAP 实验-IAP Bootloader V1.0_FLASH 
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司 

//例程说明:
//本例程用于将IAP Bootloader V1.0_ITCM工程生产的.c数组(由.bin文件生成),存放在QSPI FLASH
//然后把QSPI FLASH的这个代码加载到ITCM(0X0000 0000开始,64KB大小)里面,然后跳转到ITCM运行,
//执行:IAP Bootloader V1.0_ITCM,完成BootLoader功能!



#define BOOTLOADER_RUN_ADDR		0x00000000	//Bootloader运行地址,即ITCM首地址


typedef  void (*iapfun)(void);				//定义一个函数类型的参数.   
iapfun jump2app; 							//假函数,用于跳转

extern const unsigned char acApp2[];		//在bl_itcm.c里面定义,存放IAP Bootloader V1.0_ITCM固件数组
extern 	u32 bl_itcm_size;					//在bl_itcm.c里面定义,存放IAP Bootloader V1.0_ITCM固件大小

//跳转到应用程序段
//appxaddr:用户代码起始地址.
void iap_load_app(u32 appxaddr)
{ 
	if(((*(vu32*)appxaddr)&0x2FF00000)==0x20000000)	//检查栈顶地址是否合法.ITCM BootLoader的内存放在DTCM里面
	{ 
		jump2app=(iapfun)*(vu32*)(appxaddr+4);		//用户代码区第二个字为程序开始地址(复位地址)		
		MSR_MSP(*(vu32*)appxaddr);					//初始化APP堆栈指针(用户代码区的第一个字用于存放栈顶地址)
		jump2app();									//跳转到APP.
	}
}		

int main(void)
{
	u8 *pbr=BOOTLOADER_RUN_ADDR;	//指针指向Bootloader运行首地址 
	u32 i=0;
	Cache_Enable();                			//打开L1-Cache
	HAL_Init();				        		//初始化HAL库
	Stm32_Clock_Init(160,5,2,4);  		    //设置时钟,400Mhz 
	delay_init(400);						//延时初始化
	uart_init(115200);						//串口初始化
	LED_Init();						//初始化与LED连接的硬件接口  
	MPU_Memory_Protection();		//保护相关存储区域 
	printf("ITCM BootLoader size is:%d\r\n",bl_itcm_size);
	for(i=0;i<bl_itcm_size;i++)
	{
		pbr[i]=acApp2[i];			//搬运数据到BOOTLOADER_RUN_ADDR
	}
	if(((*(vu32*)(BOOTLOADER_RUN_ADDR+4))&0xFF000000)==0x00000000)	//判断是否为0X00XXXXXX.
	{	 
		printf("Run ITCM BootLoader...\r\n\r\n"); 
		delay_ms(10);
		iap_load_app(BOOTLOADER_RUN_ADDR);			//运行ITCM BootLoader
	}else 
	{
		printf("ITCM BootLoader addr error!\r\n");    
	}	
	while(1)
	{
		printf("Error!\r\n");
		LED0_Toggle;			//DS0闪烁
		delay_ms(500);
	}
}

