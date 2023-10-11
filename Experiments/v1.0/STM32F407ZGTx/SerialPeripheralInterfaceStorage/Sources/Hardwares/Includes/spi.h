#ifndef __SPI_H
#define __SPI_H

#include <stdint.h>

//////////////////////////////////////////////////////////////////////////////////	 
 
//SPI 驱动代码	   
//STM32F4工程-库函数版本
//淘宝店铺：http://mcudev.taobao.com										  
////////////////////////////////////////////////////////////////////////////////// 	


 
						  	    													  
void SPI1_Init(void);			 //初始化SPI1口
void SPI1_SetSpeed(uint8_t SpeedSet); //设置SPI1速度   
uint8_t SPI1_ReadWriteByte(uint8_t TxData);//SPI1总线读写一个字节
		 
#endif

