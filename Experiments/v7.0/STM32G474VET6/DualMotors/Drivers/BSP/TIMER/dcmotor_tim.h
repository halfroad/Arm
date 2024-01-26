/**
 ****************************************************************************************************
 * @file        dcmotor_tim.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2023-08-01
 * @brief       ��ʱ�� ��������
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� STM32G474������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 * �޸�˵��
 * V1.0 20230801
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#ifndef __DCMOTOR_TIM_H
#define __DCMOTOR_TIM_H

#include "./SYSTEM/sys/sys.h"


/********************************* ����������� ����������������� ************************************/

/* TIMX �������ģʽ ���� */

/* �ӿ�1 �����ͨ������ */
#define ATIM_TIMX1_CPLM_CHY_GPIO_PORT            GPIOA
#define ATIM_TIMX1_CPLM_CHY_GPIO_PIN             GPIO_PIN_8
#define ATIM_TIMX1_CPLM_CHY_GPIO_CLK_ENABLE()    do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* PA��ʱ��ʹ�� */

/* �ӿ�1 �������ͨ������ */
#define ATIM_TIMX1_CPLM_CHYN_GPIO_PORT           GPIOB
#define ATIM_TIMX1_CPLM_CHYN_GPIO_PIN            GPIO_PIN_13
#define ATIM_TIMX1_CPLM_CHYN_GPIO_CLK_ENABLE()   do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)   /* PB��ʱ��ʹ�� */

/* �ӿ�2 �����ͨ������ */
#define ATIM_TIMX2_CPLM_CHY_GPIO_PORT            GPIOC
#define ATIM_TIMX2_CPLM_CHY_GPIO_PIN             GPIO_PIN_6
#define ATIM_TIMX2_CPLM_CHY_GPIO_CLK_ENABLE()    do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)   /* PC��ʱ��ʹ�� */

/* �ӿ�2 �������ͨ������ */
#define ATIM_TIMX2_CPLM_CHYN_GPIO_PORT           GPIOC
#define ATIM_TIMX2_CPLM_CHYN_GPIO_PIN            GPIO_PIN_10
#define ATIM_TIMX2_CPLM_CHYN_GPIO_CLK_ENABLE()   do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)   /* PC��ʱ��ʹ�� */

/* TIMX ���Ÿ�������
 * ��ΪPA8/PB13  PC6/PC10, Ĭ�ϲ�����TIM1��TIM8�Ĺ��ܽ�, ���뿪������
 * ����,PA8/PB13��������TIM1��CH1/CH1N���ܣ�PC6/PC10��������TIM8��CH1/CH1N����
 */
#define ATIM_TIMX1_CPLM_CHY_GPIO_AF              GPIO_AF6_TIM1
#define ATIM_TIMX2_CPLM_CHY_GPIO_AF              GPIO_AF4_TIM8

/* �������ʹ�õĶ�ʱ�� */
#define ATIM_TIMX1_CPLM                          TIM1
#define ATIM_TIMX1_CPLM_CHY                      TIM_CHANNEL_1
#define ATIM_TIMX1_CPLM_CLK_ENABLE()             do{ __HAL_RCC_TIM1_CLK_ENABLE(); }while(0)    /* TIM1 ʱ��ʹ�� */

#define ATIM_TIMX2_CPLM                          TIM8
#define ATIM_TIMX2_CPLM_CHY                      TIM_CHANNEL_1
#define ATIM_TIMX2_CPLM_CLK_ENABLE()             do{ __HAL_RCC_TIM8_CLK_ENABLE(); }while(0)    /* TIM8 ʱ��ʹ�� */

/******************************************************************************************/

void atim_timx1_cplm_pwm_init(uint16_t arr, uint16_t psc);   /* �߼���ʱ�� ������� ��ʼ������ */
void atim_timx2_cplm_pwm_init(uint16_t arr, uint16_t psc);   /* �߼���ʱ�� ������� ��ʼ������ */

#endif


