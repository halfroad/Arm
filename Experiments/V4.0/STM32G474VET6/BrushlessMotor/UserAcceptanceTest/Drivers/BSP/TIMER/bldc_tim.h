/**
 ****************************************************************************************************
 * @file        bldc_tim.h
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

#ifndef __BLDC_TIM_H
#define __BLDC_TIM_H

#include "./SYSTEM/sys/sys.h"

/******************************************* TIM1��ʼ���꣨�ӿ�1�� **************************************************************/

/* TIM_CHxͨ�� ���ű�IO���� */
#define ATIM_TIMX_PWM_CH1_GPIO_PORT            GPIOA
#define ATIM_TIMX_PWM_CH1_GPIO_PIN             GPIO_PIN_8
#define ATIM_TIMX_PWM_CH1_GPIO_CLK_ENABLE()    do{  __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)     /* PA��ʱ��ʹ�� */

#define ATIM_TIMX_PWM_CH2_GPIO_PORT            GPIOA
#define ATIM_TIMX_PWM_CH2_GPIO_PIN             GPIO_PIN_9
#define ATIM_TIMX_PWM_CH2_GPIO_CLK_ENABLE()    do{  __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)     /* PA��ʱ��ʹ�� */

#define ATIM_TIMX_PWM_CH3_GPIO_PORT            GPIOA
#define ATIM_TIMX_PWM_CH3_GPIO_PIN             GPIO_PIN_10
#define ATIM_TIMX_PWM_CH3_GPIO_CLK_ENABLE()    do{  __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)     /* PA��ʱ��ʹ�� */

/* ���ű�IO���� */
#define M1_LOW_SIDE_U_PORT                      GPIOB
#define M1_LOW_SIDE_U_PIN                       GPIO_PIN_13
#define M1_LOW_SIDE_U_GPIO_CLK_ENABLE()         do{  __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)    /* PB��ʱ��ʹ�� */

#define M1_LOW_SIDE_V_PORT                      GPIOB
#define M1_LOW_SIDE_V_PIN                       GPIO_PIN_14
#define M1_LOW_SIDE_V_GPIO_CLK_ENABLE()         do{  __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)    /* PB��ʱ��ʹ�� */

#define M1_LOW_SIDE_W_PORT                      GPIOB
#define M1_LOW_SIDE_W_PIN                       GPIO_PIN_15
#define M1_LOW_SIDE_W_GPIO_CLK_ENABLE()         do{  __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)    /* PB��ʱ��ʹ�� */

/* ���õ�TIM1 */
#define ATIM_TIMX_PWM_CHY_GPIO_AF              GPIO_AF6_TIM1

#define ATIM_TIMX_PWM                          TIM1
#define ATIM_TIMX_PWM_IRQn                     TIM1_UP_TIM16_IRQn
#define ATIM_TIMX_PWM_IRQHandler               TIM1_UP_TIM16_IRQHandler
#define ATIM_TIMX_PWM_CH1                      TIM_CHANNEL_1                                    /* ͨ��1 */
#define ATIM_TIMX_PWM_CH2                      TIM_CHANNEL_2                                    /* ͨ��2 */
#define ATIM_TIMX_PWM_CH3                      TIM_CHANNEL_3                                    /* ͨ��3 */
#define ATIM_TIMX_PWM_CHY_CLK_ENABLE()         do{ __HAL_RCC_TIM1_CLK_ENABLE(); }while(0)       /* TIM1 ʱ��ʹ�� */


/******************************************* TIM8��ʼ���꣨�ӿ�2�� **************************************************************/

#define ATIM_TIMX2_PWM_CH1_GPIO_PORT            GPIOC
#define ATIM_TIMX2_PWM_CH1_GPIO_PIN             GPIO_PIN_6
#define ATIM_TIMX2_PWM_CH1_GPIO_CLK_ENABLE()    do{  __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)    /* PC��ʱ��ʹ�� */

#define ATIM_TIMX2_PWM_CH2_GPIO_PORT            GPIOC
#define ATIM_TIMX2_PWM_CH2_GPIO_PIN             GPIO_PIN_7
#define ATIM_TIMX2_PWM_CH2_GPIO_CLK_ENABLE()    do{  __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)    /* PC��ʱ��ʹ�� */

#define ATIM_TIMX2_PWM_CH3_GPIO_PORT            GPIOC
#define ATIM_TIMX2_PWM_CH3_GPIO_PIN             GPIO_PIN_8
#define ATIM_TIMX2_PWM_CH3_GPIO_CLK_ENABLE()    do{  __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)    /* PC��ʱ��ʹ�� */

/* ����ͨ��IO */
#define M2_LOW_SIDE_U_PORT                      GPIOC
#define M2_LOW_SIDE_U_PIN                       GPIO_PIN_10
#define M2_LOW_SIDE_U_GPIO_CLK_ENABLE()         do{  __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)    /* PC��ʱ��ʹ�� */

#define M2_LOW_SIDE_V_PORT                      GPIOC
#define M2_LOW_SIDE_V_PIN                       GPIO_PIN_11
#define M2_LOW_SIDE_V_GPIO_CLK_ENABLE()         do{  __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)    /* PC��ʱ��ʹ�� */

#define M2_LOW_SIDE_W_PORT                      GPIOC
#define M2_LOW_SIDE_W_PIN                       GPIO_PIN_12
#define M2_LOW_SIDE_W_GPIO_CLK_ENABLE()         do{  __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)    /* PC��ʱ��ʹ�� */


#define ATIM_TIMX2_PWM_CHY_GPIO_AF              GPIO_AF4_TIM8
#define ATIM_TIMX2_PWM                          TIM8
#define ATIM_TIMX2_PWM_IRQn                     TIM8_UP_IRQn
#define ATIM_TIMX2_PWM_IRQHandler               TIM8_UP_IRQHandler
#define ATIM_TIMX2_PWM_CH1                      TIM_CHANNEL_1                                   /* ͨ��1 */
#define ATIM_TIMX2_PWM_CH2                      TIM_CHANNEL_2                                   /* ͨ��2 */
#define ATIM_TIMX2_PWM_CH3                      TIM_CHANNEL_3                                   /* ͨ��3 */

#define ATIM_TIMX2_PWM_CHY_CLK_ENABLE()         do{ __HAL_RCC_TIM8_CLK_ENABLE(); }while(0)      /* TIM8 ʱ��ʹ�� */

extern TIM_HandleTypeDef g_atimx_handle;                                                        /* ��ʱ��x��� */
extern TIM_HandleTypeDef g_atimx2_handle;                                                       /* ��ʱ��x��� */
/******************************************************************************************/

void atim_timx_oc_chy_init(uint16_t arr, uint16_t psc);                                         /* �߼���ʱ�� PWM��ʼ������ */
void atim_timx2_oc_chy_init(uint16_t arr, uint16_t psc);


#endif

















