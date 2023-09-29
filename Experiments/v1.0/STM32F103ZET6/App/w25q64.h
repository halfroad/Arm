#ifndef _W25Q64_H_
#define _W25Q64_H_

#include "stm32f10x.h"

#define W25Q64_CS_H() GPIO_SetBits(GPIOA,GPIO_Pin_4)	//片选取消
#define W25Q64_CS_L() GPIO_ResetBits(GPIOA,GPIO_Pin_4)	//片选选中
#define W25Q64_PAGE_SIZE	256		//页大小256字节
#define W25Q64_SECTOR_SIZE	4096	//扇区大小4096字节
#define W25Q64_START_ADDR	(uint32_t )0x000000		//起始地址
#define W25Q64_END_ADDR		(uint32_t )0x7FFFFF		//结束地址
//状态位
#define W25Q64_BUSY			0x01	//忙位
#define W25Q64_WEL			0x02	//写保护位

void W25Q64_InitConfig(void);
uint16_t W25Q64_GetID(void);
void W25Q64_SectorErase(uint32_t addr);
void W25Q64_BlockErase(uint32_t addr);
void W25Q64_WriteData(uint32_t addr,uint8_t *data,uint32_t size);
void W25Q64_ReadData(uint32_t addr,uint8_t *data,uint16_t size);
#endif
