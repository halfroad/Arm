#ifndef _W25Q64_H_
#define _W25Q64_H_

#include "stm32f10x.h"

#define W25Q64_CS_H() GPIO_SetBits(GPIOA,GPIO_Pin_4)	//Ƭѡȡ��
#define W25Q64_CS_L() GPIO_ResetBits(GPIOA,GPIO_Pin_4)	//Ƭѡѡ��
#define W25Q64_PAGE_SIZE	256		//ҳ��С256�ֽ�
#define W25Q64_SECTOR_SIZE	4096	//������С4096�ֽ�
#define W25Q64_START_ADDR	(uint32_t )0x000000		//��ʼ��ַ
#define W25Q64_END_ADDR		(uint32_t )0x7FFFFF		//������ַ
//״̬λ
#define W25Q64_BUSY			0x01	//æλ
#define W25Q64_WEL			0x02	//д����λ

void W25Q64_InitConfig(void);
uint16_t W25Q64_GetID(void);
void W25Q64_SectorErase(uint32_t addr);
void W25Q64_BlockErase(uint32_t addr);
void W25Q64_WriteData(uint32_t addr,uint8_t *data,uint32_t size);
void W25Q64_ReadData(uint32_t addr,uint8_t *data,uint16_t size);
#endif
