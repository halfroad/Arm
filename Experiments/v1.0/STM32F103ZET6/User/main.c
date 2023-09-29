#include "stm32f10x.h"
#include "w25q64.h"
#include "at24c02.h"

static void CLOCK_Config(void);
static void NVIC_Config(void);
uint16_t ID=0;
int main(void)
{
	CLOCK_Config();		//ʱ������
	NVIC_Config();		//�ж����ȼ�����
	W25Q64_InitConfig();//��ʼ��W25Q64
	ID = W25Q64_GetID();//��ȡID
	uint8_t rx_data[40]={0};
	uint8_t tx_data[]="abcdefghijklnm123456789";
	W25Q64_WriteData(0x200000-10,tx_data,5);//д����
	W25Q64_WriteData(0x200000-5,tx_data,25);
	W25Q64_ReadData(0x200000-10,rx_data,30);//������
	while(1)
	{
		
	}
}
//����ʱ������
static void CLOCK_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
}
//�ж����ȼ���ʼ��
static void NVIC_IRQPriority_Init(uint8_t IRQChannel,uint8_t PreemptionPriority, uint8_t SubPriority)
{
	NVIC_InitTypeDef NVIC_IRQ_Init;
	NVIC_IRQ_Init.NVIC_IRQChannel = IRQChannel;
	NVIC_IRQ_Init.NVIC_IRQChannelPreemptionPriority = PreemptionPriority;
	NVIC_IRQ_Init.NVIC_IRQChannelSubPriority = SubPriority;
	NVIC_IRQ_Init.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_IRQ_Init);
}
//�����ж����ȼ�
static void NVIC_Config(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����
	//NVIC_IRQPriority_Init();
}
