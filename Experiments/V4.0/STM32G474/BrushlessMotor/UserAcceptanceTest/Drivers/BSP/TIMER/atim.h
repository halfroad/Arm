/**
 ****************************************************************************************************
 * @file        atim.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2023-08-01
 * @brief       �߼���ʱ�� ��������
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� STM32G474������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com/forum.php
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 * �޸�˵��
 * V1.0 20230801
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#ifndef __ATIM_H
#define __ATIM_H

#include "./SYSTEM/sys/sys.h"

/***************************************** �߼���ʱ�� �����������������ʵ����غ궨�� *************************************************/

/* TIMX ����������� */

/* �����ͨ������ */
#define ATIM_TIMX_CPLM_CHY_GPIO_PORT            GPIOA
#define ATIM_TIMX_CPLM_CHY_GPIO_PIN             GPIO_PIN_8
#define ATIM_TIMX_CPLM_CHY_GPIO_CLK_ENABLE()    do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0) /* PA��ʱ��ʹ�� */

/* �������ͨ������ */
#define ATIM_TIMX_CPLM_CHYN_GPIO_PORT           GPIOB
#define ATIM_TIMX_CPLM_CHYN_GPIO_PIN            GPIO_PIN_13
#define ATIM_TIMX_CPLM_CHYN_GPIO_CLK_ENABLE()   do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0) /* PB��ʱ��ʹ�� */

/* ɲ���������� */
#define ATIM_TIMX_CPLM_BKIN_GPIO_PORT           GPIOB
#define ATIM_TIMX_CPLM_BKIN_GPIO_PIN            GPIO_PIN_12
#define ATIM_TIMX_CPLM_BKIN_GPIO_CLK_ENABLE()   do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0) /* PB��ʱ��ʹ�� */

/* TIMX ���Ÿ�������
 * ��ΪPA8��PB13��PB12Ĭ�ϲ�����TIM8�Ĺ��ܽ�, ���뿪������, PA8��PB13��PB12��������TIM1�Ļ��������ɲ�����š�
 */
#define ATIM_TIMX_CPLM_CHY_GPIO_AF              GPIO_AF6_TIM1

#define ATIM_TIMX_CPLM                          TIM1
#define ATIM_TIMX_CPLM_CHY                      TIM_CHANNEL_1
#define ATIM_TIMX_CPLM_CHY_CCRY                 ATIM_TIMX_CPLM->CCR1
#define ATIM_TIMX_CPLM_CLK_ENABLE()             do{ __HAL_RCC_TIM1_CLK_ENABLE(); }while(0)  /* TIM1 ʱ��ʹ�� */

/******************************************************************************************/

void atim_timx_cplm_pwm_init(uint16_t arr, uint16_t psc);                                   /* �߼���ʱ�� ������� ��ʼ������ */
void atim_timx_cplm_pwm_set(uint16_t ccr, uint8_t dtg);                                     /* �߼���ʱ�� ������� ��������Ƚ�ֵ & ����ʱ�� */

#endif

















