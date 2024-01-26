/**
 ****************************************************************************************************
 * @file        dc_motor.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2023-08-01
 * @brief       ֱ����ˢ������ƴ���
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

#ifndef __DCMOTOR_H
#define __DCMOTOR_H

#include "./SYSTEM/sys/sys.h"

/************************************* �������� *****************************************************/

/* ֹͣ���Ų����궨�� 
 * �����ſ���H���Ƿ���Ч�Դﵽ�����͹رյ����Ч��
 */
#define SHUTDOWN1_Pin                 GPIO_PIN_12
#define SHUTDOWN1_GPIO_Port           GPIOB
#define SHUTDOWN1_GPIO_CLK_ENABLE()   do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)   /* PC��ʱ��ʹ�� */

#define SHUTDOWN2_Pin                 GPIO_PIN_11
#define SHUTDOWN2_GPIO_Port           GPIOD
#define SHUTDOWN2_GPIO_CLK_ENABLE()   do{ __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)   /* PD��ʱ��ʹ�� */

/* ���ֹͣ���Ŷ��� ����Ĭ���ǽӿ�1 */
#define ENABLE_MOTOR    HAL_GPIO_WritePin(SHUTDOWN1_GPIO_Port,SHUTDOWN1_Pin,GPIO_PIN_SET)
#define DISABLE_MOTOR   HAL_GPIO_WritePin(SHUTDOWN1_GPIO_Port,SHUTDOWN1_Pin,GPIO_PIN_RESET)

/******************************************************************************************/

void dcmotor_init(void);                /* ֱ����ˢ�����ʼ�� */
void dcmotor_start(void);               /* ������� */
void dcmotor_stop(void);                /* �رյ�� */  
void dcmotor_dir(uint8_t para);         /* ���õ������ */
void dcmotor_speed(uint16_t para);      /* ���õ���ٶ� */
void motor_pwm_set(float para);         /* ������� */
    
#endif




