#ifndef __TPAD_H
#define __TPAD_H
#include "sys.h"
#include "timer.h"		
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
//ALIENTEK STM32H7������
//TPAD ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2019/4/21
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) �������������ӿƼ����޹�˾ 2014-2024
//All rights reserved		 
////////////////////////////////////////////////////////////////////////////////// 	


//���ص�ʱ��(û���ְ���),��������Ҫ��ʱ��
//���ֵӦ����ÿ�ο�����ʱ�򱻳�ʼ��һ��
extern vu16 tpad_default_val;
							   	    
void TPAD_Reset(void);
u16  TPAD_Get_Val(void);
u16 TPAD_Get_MaxVal(u8 n);
u8   TPAD_Init(u8 systick);
u8   TPAD_Scan(u8 mode);
void TIM2_CH1_Cap_Init(u32 arr,u16 psc);    
#endif






















