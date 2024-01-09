#include "timer.h"
#include "led.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
//ALIENTEK STM32H7������
//��ʱ�� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2019/4/21
//�汾��V1.1
//��Ȩ���У�����ؾ���
//Copyright(C) �������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
//********************************************************************************
//�޸�˵��
//V1.1 20190421
//����TIM3_PWM_Init����,����PWM���	
//V1.2 20190421
//����TIM5_CH1_Cap_Init����,�������벶��	
////////////////////////////////////////////////////////////////////////////////// 	 


//��ʱ��3�жϷ������	 
void TIM3_IRQHandler(void)
{ 		  
	static u8 led1sta=1;
	if(TIM3->SR&0X0001)//����ж�
	{
		LED1(led1sta^=1);	 
	}				   
	TIM3->SR&=~(1<<0);//����жϱ�־λ 	    
}
//ͨ�ö�ʱ��3�жϳ�ʼ��
//TIM3��ʱ������APB1,��D2PPRE1��2��Ƶ��ʱ��
//TIM3��ʱ��ΪAPB1ʱ�ӵ�2������APB1Ϊ100M
//����,TIM3��ʱ��Ƶ��Ϊ200Mhz
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
//����ʹ�õ��Ƕ�ʱ��3!
void TIM3_Int_Init(u16 arr,u16 psc)
{
	RCC->APB1LENR|=1<<1;				//TIM3ʱ��ʹ��  
	//�������TIM3ʱ�Ӻ�,ֱ�ӿ�ʼ����TIM3,���������
	while((RCC->APB1LENR&(1<<1))==0);	//�ȴ�ʱ������OK  
 	TIM3->ARR=arr;  				//�趨�������Զ���װֵ 
	TIM3->PSC=psc;  				//Ԥ��Ƶ��
	TIM3->SMCR=0;					//SMS[3:0]=0000,CK_INT��Ϊʱ��Դ
	TIM3->DIER|=1<<0;  				//���������ж�	  
	TIM3->CR1&=~(3<<5);				//CMS[1:0]=00,���ض���ģʽ
	TIM3->CR1&=~(1<<4);				//DIR=0,���ϼ���ģʽ
	TIM3->CR1|=0x01;				//ʹ�ܶ�ʱ��3
  	MY_NVIC_Init(1,3,TIM3_IRQn,2);	//��ռ1�������ȼ�3����2		 
}


//TIM3 PWM���ֳ�ʼ�� 
//TIM3��ʱ������APB1,��D2PPRE1��2��Ƶ��ʱ��
//TIM3��ʱ��ΪAPB1ʱ�ӵ�2������APB1Ϊ100M
//����,TIM3��ʱ��Ƶ��Ϊ200Mhz
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM3_PWM_Init(u32 arr,u32 psc)
{		 					 
	//�˲������ֶ��޸�IO������
	RCC->APB1LENR|=1<<1;	//TIM3ʱ��ʹ��    
	RCC->AHB4ENR|=1<<1;   	//ʹ��PORTBʱ��	
	GPIO_Set(GPIOB,PIN1,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_MID,GPIO_PUPD_PU);//���ù���,�������
	GPIO_AF_Set(GPIOB,1,2);	//PB1,AF2 
	
	TIM3->ARR=arr;			//�趨�������Զ���װֵ 
	TIM3->PSC=psc;			//Ԥ��Ƶ������Ƶ 
	TIM3->CCMR2|=6<<12;  	//CH4 PWM1ģʽ		 
	TIM3->CCMR2|=1<<11; 	//CH4 Ԥװ��ʹ��	   
	TIM3->CCER|=1<<12;   	//OC4 ���ʹ��	
	TIM3->CCER|=1<<13;   	//OC4 �͵�ƽ��Ч	   
	TIM3->CR1|=1<<7;   		//ARPEʹ�� 
	TIM3->CR1|=1<<0;    	//ʹ�ܶ�ʱ��3											  
}  

//��ʱ��5ͨ��1���벶������
//arr���Զ���װֵ(TIM2,TIM5��32λ��!!)
//psc��ʱ��Ԥ��Ƶ��
void TIM5_CH1_Cap_Init(u32 arr,u16 psc)
{		 
	RCC->APB1LENR|=1<<3;   	//TIM5 ʱ��ʹ�� 
	RCC->AHB4ENR|=1<<0;   	//ʹ��PORTAʱ��	
	GPIO_Set(GPIOA,PIN0,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_MID,GPIO_PUPD_PD);//���ù���,����
	GPIO_AF_Set(GPIOA,0,2);	//PA0,AF2
	  
 	TIM5->ARR=arr;  		//�趨�������Զ���װֵ   
	TIM5->PSC=psc;  		//Ԥ��Ƶ�� 

	TIM5->CCMR1|=1<<0;		//CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
 	TIM5->CCMR1|=0<<4; 		//IC1F=0000 ���������˲��� ���˲�
 	TIM5->CCMR1|=0<<10; 	//IC1PS=00 	���������Ƶ,����Ƶ 

	TIM5->CCER|=0<<1; 		//CC1P=0	�����ز���
	TIM5->CCER|=1<<0; 		//CC1E=1 	���������������ֵ������Ĵ�����

	TIM5->EGR=1<<0;			//�������Ʋ��������¼�,ʹд��PSC��ֵ������Ч,���򽫻�Ҫ�ȵ���ʱ������Ż���Ч!
	TIM5->DIER|=1<<1;   	//��������1�ж�				
	TIM5->DIER|=1<<0;   	//���������ж�	
	TIM5->CR1|=0x01;    	//ʹ�ܶ�ʱ��2
	MY_NVIC_Init(2,0,TIM5_IRQn,2);//��ռ2�������ȼ�0����2	   
}

//����״̬
//[7]:0,û�гɹ��Ĳ���;1,�ɹ�����һ��.
//[6]:0,��û���񵽵͵�ƽ;1,�Ѿ����񵽵͵�ƽ��.
//[5:0]:����͵�ƽ������Ĵ���(����32λ��ʱ����˵,1us��������1,���ʱ��:4294��)
u8  TIM5CH1_CAPTURE_STA=0;	//���벶��״̬		    				
u32	TIM5CH1_CAPTURE_VAL;	//���벶��ֵ(TIM2/TIM5��32λ)
//��ʱ��5�жϷ������	 
void TIM5_IRQHandler(void)
{ 		    
	u16 tsr;
	tsr=TIM5->SR;
 	if((TIM5CH1_CAPTURE_STA&0X80)==0)//��δ�ɹ�����	
	{
		if(tsr&0X01)//���
		{	     
			if(TIM5CH1_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
			{
				if((TIM5CH1_CAPTURE_STA&0X3F)==0X3F)//�ߵ�ƽ̫����
				{
					TIM5CH1_CAPTURE_STA|=0X80;		//��ǳɹ�������һ��
					TIM5CH1_CAPTURE_VAL=0XFFFFFFFF;
				}else TIM5CH1_CAPTURE_STA++;
			}	 
		}
		if(tsr&0x02)//����1���������¼�
		{	
			if(TIM5CH1_CAPTURE_STA&0X40)		//����һ���½��� 		
			{	  			
				TIM5CH1_CAPTURE_STA|=0X80;		//��ǳɹ�����һ�θߵ�ƽ����
			    TIM5CH1_CAPTURE_VAL=TIM5->CCR1;	//��ȡ��ǰ�Ĳ���ֵ.
	 			TIM5->CCER&=~(1<<1);			//CC1P=0 ����Ϊ�����ز���
			}else  								//��δ��ʼ,��һ�β���������
			{
				TIM5CH1_CAPTURE_STA=0;			//���
				TIM5CH1_CAPTURE_VAL=0;
				TIM5CH1_CAPTURE_STA|=0X40;		//��ǲ�����������
				TIM5->CR1&=~(1<<0)		;    	//ʹ�ܶ�ʱ��2
	 			TIM5->CNT=0;					//���������
	 			TIM5->CCER|=1<<1; 				//CC1P=1 ����Ϊ�½��ز���
				TIM5->CR1|=0x01;    			//ʹ�ܶ�ʱ��2
			}		    
		}			     	    					   
 	}
	TIM5->SR=0;//����жϱ�־λ   
}
















