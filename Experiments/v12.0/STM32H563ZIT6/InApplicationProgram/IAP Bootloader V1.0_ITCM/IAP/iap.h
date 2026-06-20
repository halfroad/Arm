#ifndef __IAP_H__
#define __IAP_H__
#include "sys.h"  
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32H7开发板
//IAP 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2019/5/17
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

typedef  void (*iapfun)(void);				//定义一个函数类型的参数.   
#define FLASH_APP1_ADDR		0x08002000  	//第一个应用程序起始地址(存放在内部FLASH),保留0X08000000~0X08001FFF的空间为Bootloader使用(共8KB)	
#define QSPI_APP1_ADDR		0X90100000	  	//第一个应用程序QSPI起始地址(存放在QSPI FLASH),保留0X90000000~0X900FFFFF的空间为Bootloader使用(共1MB)


void iap_load_app(u32 appxaddr);			//跳转到APP程序执行
void iap_write_appbin(u32 appxaddr,u8 *appbuf,u32 applen);	//在指定地址开始,写入bin
#endif







































