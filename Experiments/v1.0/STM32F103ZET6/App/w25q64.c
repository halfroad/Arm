
#include "w25q64.h"
#include "systick.h"
/*
	�������ӣ�
	PA4	- #CS
	PA5 - SCK
	PA6 - MISO
	PA7 - MOSI
*/
#if 0
#define W25Q64_SCK_H() GPIO_SetBits(GPIOA,GPIO_Pin_5)
#define W25Q64_SCK_L() GPIO_ResetBits(GPIOA,GPIO_Pin_5)
#define W25Q64_OUT_H() GPIO_SetBits(GPIOA,GPIO_Pin_7)
#define W25Q64_OUT_L() GPIO_ResetBits(GPIOA,GPIO_Pin_7)
#define W25Q64_IN()	   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)

uint8_t W25Q64_ReadWriteByte(uint8_t bety)
{
	uint8_t i;
	uint8_t data=0;
	W25Q64_SCK_L();
	for(i=0;i<8;i++)
	{
		bety & 0x80? W25Q64_OUT_H():W25Q64_OUT_L();
		bety <<=1;
		W25Q64_SCK_H();
		if(W25Q64_IN())
		{
			data |= (1<<(7-i));
		}
		W25Q64_SCK_L();
	}
	return data;
}

void W25Q64_InitConfig(void)
{
	//1.����GPIO
	GPIO_InitTypeDef w25q64_GPIO;
	w25q64_GPIO.GPIO_Pin = GPIO_Pin_4| GPIO_Pin_5 | GPIO_Pin_7;
	w25q64_GPIO.GPIO_Mode = GPIO_Mode_Out_PP;//�����������
	w25q64_GPIO.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&w25q64_GPIO);
	
	w25q64_GPIO.GPIO_Pin = GPIO_Pin_6;
	w25q64_GPIO.GPIO_Mode = GPIO_Mode_IPU;//��������
	w25q64_GPIO.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&w25q64_GPIO);
	
	W25Q64_CS_H();//ȡ��ѡ��״̬
}
#else

void W25Q64_InitConfig(void)
{
	//1.����GPIO
	GPIO_InitTypeDef w25q64_GPIO;
	w25q64_GPIO.GPIO_Pin = GPIO_Pin_5| GPIO_Pin_6 | GPIO_Pin_7;
	w25q64_GPIO.GPIO_Mode = GPIO_Mode_AF_PP;//�����������
	w25q64_GPIO.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&w25q64_GPIO);
	
	w25q64_GPIO.GPIO_Pin = GPIO_Pin_4;
	w25q64_GPIO.GPIO_Mode = GPIO_Mode_Out_PP;//ͨ���������
	w25q64_GPIO.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&w25q64_GPIO);
	
	W25Q64_CS_H();//ȡ��ѡ��״̬
	
	//2.SPI����
	SPI_I2S_DeInit(SPI1);
	SPI_InitTypeDef w25q64_SPI;
	w25q64_SPI.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//˫��˫��ȫ˫��
	w25q64_SPI.SPI_Mode = SPI_Mode_Master;		//����ģʽ
	w25q64_SPI.SPI_DataSize = SPI_DataSize_8b;	//8λ����ģʽ
	w25q64_SPI.SPI_CPOL = SPI_CPOL_High;		//ʱ�Ӽ��Ը�
	w25q64_SPI.SPI_CPHA = SPI_CPHA_2Edge;		//�ڶ�������
	w25q64_SPI.SPI_NSS = SPI_NSS_Soft;			//���������
	w25q64_SPI.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	w25q64_SPI.SPI_FirstBit = SPI_FirstBit_MSB;//��λ�ȷ�
	w25q64_SPI.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1,&w25q64_SPI);
	//3.���ø����ж�
	//SPI_I2S_ITConfig(SPI1,SPI_I2S_IT_RXNE,ENABLE);
	//4.ʹ��SPI1
	SPI_Cmd(SPI1,ENABLE);
}
uint8_t W25Q64_ReadWriteByte(uint8_t bety)
{
	//uint16_t count=4096;
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE) == RESET);//�ȴ����ͻ�������
	SPI_I2S_SendData(SPI1,bety);
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE) == RESET);//�ȴ����ջ������ǿ�
	return SPI_I2S_ReceiveData(SPI1);
}

#endif
/*
��ȡW25Q64�豸ID
����ֵ����8λ����ID
		��8λ����ID
*/

uint16_t W25Q64_GetID(void)
{
	uint16_t id=0;
	//���Ͷ�����/������ָ�� 0x90
	W25Q64_CS_L();
	W25Q64_ReadWriteByte(0x90);
	//����24λ��ַ
	W25Q64_ReadWriteByte(0x00);
	W25Q64_ReadWriteByte(0x00);
	W25Q64_ReadWriteByte(0x00);
	id = W25Q64_ReadWriteByte(0xFF)<<8;	//����ID
	id |= W25Q64_ReadWriteByte(0xFF);		//����ID
	W25Q64_CS_H();
	return id;
}
//��ȡ״̬�Ĵ���
uint8_t W25Q64_ReadState(void)
{
	uint8_t state=0;
	W25Q64_CS_L();
	W25Q64_ReadWriteByte(0x05);
	state = W25Q64_ReadWriteByte(0xFF);//��ȡ״ֵ̬
	W25Q64_CS_H();
	return state;
}
//дʹ��
void W25Q64_WriteENABLE(void)
{
	W25Q64_CS_L();
	W25Q64_ReadWriteByte(0x06);//дʹ��
	W25Q64_CS_H();
}
/*
	\brief��	��������
	\param��	addr 24λ������ַ
	\retval��	none
*/
void W25Q64_SectorErase(uint32_t addr)
{
	while(W25Q64_ReadState() & W25Q64_BUSY);//�ȴ�æ����
	W25Q64_WriteENABLE();//дʹ��
	W25Q64_CS_L();
	W25Q64_ReadWriteByte(0x20);//��������
	W25Q64_ReadWriteByte((addr&0xFF0000)>>16);//����24λ��ַ
	W25Q64_ReadWriteByte((addr&0xFF00)>>8);
	W25Q64_ReadWriteByte((addr&0xFF));
	W25Q64_CS_H();
}
/*
	\brief��	�����
	\param��	addr 24λ������ַ
	\retval��	none
*/
void W25Q64_BlockErase(uint32_t addr)
{
	W25Q64_CS_L();
	W25Q64_ReadWriteByte(0x06);//дʹ��
	W25Q64_ReadWriteByte(0xDB);//��������
	W25Q64_ReadWriteByte((uint8_t )(addr&0x00FF0000)>>16);//����24λ��ַ
	W25Q64_ReadWriteByte((uint8_t )(addr&0x0000FF00)>>8);
	W25Q64_ReadWriteByte((uint8_t )(addr&0x000000FF));
	W25Q64_CS_H();
}
/*
	\brief��	������
	\param��	addr��Ҫ��ȡ�ĵ�ַ
				data�������ȡ������
				size����ȡ���ֽ��������ݳ��ȣ�
	\retval��	none
*/
void W25Q64_ReadData(uint32_t addr,uint8_t *data,uint16_t size)
{
	if(addr+size > W25Q64_END_ADDR) 
	{
		return ;
	}
	uint8_t *pData=data;
	while(W25Q64_ReadState() & W25Q64_BUSY);//�ȴ�æ����
	W25Q64_CS_L();
	W25Q64_ReadWriteByte(0x03);//������ָ��
	W25Q64_ReadWriteByte((addr&0xFF0000)>>16);//����24λ��ַ
	W25Q64_ReadWriteByte((addr&0xFF00)>>8);
	W25Q64_ReadWriteByte((addr&0xFF));
	while(size--)
	{
		*pData=W25Q64_ReadWriteByte(0xFF);//��������
		pData++;
	}
	W25Q64_CS_H();
}
//ҳ���
void W25Q64_PageWrite(uint32_t addr,uint8_t *data,uint16_t size)
{
	if(addr+size > W25Q64_END_ADDR) 
		return ;
	uint8_t *pData = data;
	while(W25Q64_ReadState() & W25Q64_BUSY);//�ȴ�æ����
	W25Q64_WriteENABLE();//дʹ��
	W25Q64_CS_L();
	//while(!(W25Q64_ReadState() & W25Q64_WEL));//�ȴ�дʹ�����
	W25Q64_ReadWriteByte(0x02);//ҳдָ��
	W25Q64_ReadWriteByte((addr&0xFF0000)>>16);//����24λ��ַ
	W25Q64_ReadWriteByte((addr&0xFF00)>>8);
	W25Q64_ReadWriteByte((addr&0xFF));
	while(size--)
	{
		W25Q64_ReadWriteByte(*pData);
		pData++;
	}
	W25Q64_CS_H();
}
/*
	\brief��	�ɿ�ҳд���ݣ������ǲ�������Ϊд��ĵ�ַ��Ϊ0xFF��
	\param��	addr��Ҫд��ĵ�ַ
				data��д�������
				size�����ݵ��������ֽ�����
	\retval��	none
*/
void W25Q64_StepOverPageWrite(uint32_t addr,uint8_t *data,uint32_t size)
{
	uint32_t addr_remain= 256 - addr%256;//��ǰҳ��ַʣ��
	uint8_t *pData=data;
	if(size <= addr_remain)
	{
		addr_remain = size;
	}
	while(1)
	{
		W25Q64_PageWrite(addr,pData,addr_remain);
		if(addr_remain == size) break;		//����ȫ��д��
		pData += addr_remain;	//���ݵ�ַƫ��
		addr += addr_remain;	//��ַƫ��
		size -= addr_remain;	//����ʣ������
		addr_remain = 256;//д��һҳ����
		if(size <= addr_remain)	//���㵱ǰҳ�Ƿ�д��ʣ������
		{
			addr_remain = size;
		}
	}
}
/*
	\brief��	�ɿ�ҳд���ݣ����ǲ�����ԭ�����ݣ�
	\param��	addr��Ҫд��ĵ�ַ
				data��д�������
				size�����ݵ��������ֽ�����
	\retval��	none
*/

uint8_t sector_data[W25Q64_SECTOR_SIZE];
void W25Q64_WriteData(uint32_t addr,uint8_t *data,uint32_t size)
{
	uint16_t sector_offset = addr%4096;	//���㵱ǰ�����ĵ�ַƫ��
	uint16_t sector_remain = 4096 - sector_offset;	//���㵱ǰ����ʣ��
	uint32_t sector_addr = addr - sector_offset;	//���㵱ǰ��������ʼ��ַ
	uint8_t *pData=data;
	uint32_t i;
	if(size <= sector_remain)
	{
		sector_remain=(uint16_t )size;
	}
	while(1)
	{
		W25Q64_ReadData(addr,sector_data,sector_remain);//��ȡҪд���ַ������
		for(i=0;i<sector_remain;i++)
		{
			if(sector_data[i]!=0xFF) break;
		}
		if(i!=sector_remain)//�ж��Ƿ���Ҫ��������
		{
			//����ǰ���浱ǰ����ǰһ������
			W25Q64_ReadData(sector_addr,sector_data,sector_offset);
			//����ǰ���浱ǰ������һ������
			W25Q64_ReadData(addr + sector_remain,sector_data+(sector_offset+sector_remain),W25Q64_SECTOR_SIZE - (sector_offset+sector_remain));
			W25Q64_SectorErase(sector_addr);//��������
			//��Ҫд������ݲ��뻺����
			for(i=0;i<sector_remain;i++)
			{
				sector_data[sector_offset+i]= pData[i];
			}
			W25Q64_StepOverPageWrite(sector_addr,sector_data,W25Q64_SECTOR_SIZE);
			sector_offset = 0;
		}
		else
		{
			W25Q64_StepOverPageWrite(addr,pData,sector_remain);//��ǰ����д������
		}
		if(sector_remain == size) break;//ȫ��������ȫд��
		
		pData += sector_remain;	//���ݵ�ַƫ��
		addr += sector_remain;	//flash��ַƫ��
		sector_addr = addr;		//��ǰ������ʼ��ַ
		size -= sector_remain;	//����������
		sector_remain = W25Q64_SECTOR_SIZE;//��ǰ����ʣ��
		if(size <= W25Q64_SECTOR_SIZE)//���㵱ǰ�����Ƿ�д��ʣ������
		{
			sector_remain = size;
		}
	}
}
