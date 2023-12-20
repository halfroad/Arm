#include "sys.h" 
#include "usart.h" 
#include "delay.h" 
#include "led.h"
#include "key.h" 
#include "mpu.h" 
//ALIENTEK ������STM32H750/F750������ ʵ��12
//�ڴ汣����MPU��ʵ��  
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾


u8 mpudata[128] __attribute__((at(0X20002000)));	//����һ������
int main(void)
{  
	u8 i=0,led0sta=1;
	u8 key; 
	Stm32_Clock_Init(160,5,2,4);		//����ʱ��,400Mhz
    delay_init(400);					//��ʱ��ʼ��  
	uart_init(100,115200);				//���ڳ�ʼ��Ϊ115200 
	LED_Init();							//��ʼ����LED���ӵ�Ӳ���ӿ�  
	KEY_Init();							//��ʼ������
	printf("\r\n\r\nMPU closed!\r\n");	//��ʾMPU�ر�
	while(1)  
	{
		key=KEY_Scan(0);
        if(key==WKUP_PRES)              //ʹ��MPU�������� mpudata;
        {
            MPU_Set_Protection(0X20002000,128,0,0,MPU_REGION_PRIV_RO_URO,0,0,1);	//ֻ��,��ֹ����,��ֹcatch,������ 
            printf("MPU open!\r\n");	//��ʾMPU��
        }else if(key==KEY0_PRES)        //��������д�����ݣ����������MPU�����Ļ�������ڴ���ʴ���
        {
			printf("Start Writing data...\r\n");
			sprintf((char*)mpudata,"MPU test array %d",i);
			printf("Data Write finshed!\r\n");
        }else if(key==KEY1_PRES)		//�������ж�ȡ���ݣ�������û�п���MPU��������������ڴ���ʴ���
        {
            printf("Array data is:%s\r\n",mpudata);
        }else delay_ms(10);
		i++;
		if((i%50)==0) LED0(led0sta^=1);	//LED0ȡ�� 
	}
}


