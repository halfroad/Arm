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

/************************************* �������� ��· *****************************************************/

/* ����ӿڶ��� */
#define MOTOR_1 1
#define MOTOR_2 2

/* ֹͣ���Ų����궨�� 
 * �����ſ���H���Ƿ���Ч�Դﵽ�����͹رյ����Ч��
 */
#define SHUTDOWN1_Pin                 GPIO_PIN_13
#define SHUTDOWN1_GPIO_Port           GPIOC
#define SHUTDOWN1_GPIO_CLK_ENABLE()   do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)   /* PC��ʱ��ʹ�� */

#define SHUTDOWN2_Pin                 GPIO_PIN_11
#define SHUTDOWN2_GPIO_Port           GPIOD
#define SHUTDOWN2_GPIO_CLK_ENABLE()   do{ __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)   /* PD��ʱ��ʹ�� */

/* ���ֹͣ���Ŷ��� */
#define ENABLE_MOTOR1    HAL_GPIO_WritePin(SHUTDOWN1_GPIO_Port,SHUTDOWN1_Pin,GPIO_PIN_SET)
#define DISABLE_MOTOR1   HAL_GPIO_WritePin(SHUTDOWN1_GPIO_Port,SHUTDOWN1_Pin,GPIO_PIN_RESET)

#define ENABLE_MOTOR2    HAL_GPIO_WritePin(SHUTDOWN2_GPIO_Port,SHUTDOWN2_Pin,GPIO_PIN_SET)
#define DISABLE_MOTOR2   HAL_GPIO_WritePin(SHUTDOWN2_GPIO_Port,SHUTDOWN2_Pin,GPIO_PIN_RESET)

/******************************************************************************************/

void dcmotor_init(void);                        /* ֱ����ˢ�����ʼ�� */
void dcmotor_start(uint8_t id);                 /* ������� */
void dcmotor_stop(uint8_t id);                  /* �رյ�� */  
void dcmotor_speed(uint16_t para,uint8_t id);   /* ���õ���ٶ� */
void dcmotor_dir(uint8_t para,uint8_t id);      /* ���õ������ */
void motor_pwm_set(float para,uint8_t id);      /* ���õ��PWMռ�ձ� */
    
#endif




