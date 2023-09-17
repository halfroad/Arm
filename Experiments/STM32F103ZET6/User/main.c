#include "stm32f10x.h"
#include "w25q64.h"
#include "at24c02.h"

static void CLOCK_Config(void);
static void NVIC_Config(void);
uint16_t ID=0;
int main(void)
{
	CLOCK_Config();		//时钟配置
	NVIC_Config();		//中断优先级配置
	W25Q64_InitConfig();//初始化W25Q64
	ID = W25Q64_GetID();//读取ID
	uint8_t rx_data[40]={0};
	uint8_t tx_data[]="abcdefghijklnm123456789";
	W25Q64_WriteData(0x200000-10,tx_data,5);//写数据
	W25Q64_WriteData(0x200000-5,tx_data,25);
	W25Q64_ReadData(0x200000-10,rx_data,30);//读数据
	while(1)
	{
		
	}
}
//外设时钟配置
static void CLOCK_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
}
//中断优先级初始化
static void NVIC_IRQPriority_Init(uint8_t IRQChannel,uint8_t PreemptionPriority, uint8_t SubPriority)
{
	NVIC_InitTypeDef NVIC_IRQ_Init;
	NVIC_IRQ_Init.NVIC_IRQChannel = IRQChannel;
	NVIC_IRQ_Init.NVIC_IRQChannelPreemptionPriority = PreemptionPriority;
	NVIC_IRQ_Init.NVIC_IRQChannelSubPriority = SubPriority;
	NVIC_IRQ_Init.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_IRQ_Init);
}
//配置中断优先级
static void NVIC_Config(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//配置中断优先级分组
	//NVIC_IRQPriority_Init();
}
