/**
 ****************************************************************************************************
 * @file        bldc.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2023-08-01
 * @brief       BLDC ��������
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
#ifndef __BLDC_H
#define __BLDC_H

#include "./SYSTEM/sys/sys.h"

/***************************************** ���״̬�ṹ�� ***********************************************/
typedef struct {
    __IO uint8_t    run_flag;       /* ���б�־ */
    __IO uint8_t    locked_rotor;   /* ��ת��� */
    __IO uint8_t    step_sta;       /* ���λ���״̬ */
    __IO uint8_t    hall_single_sta;/* ��������״̬ */
    __IO uint8_t    hall_sta_edge;  /* ��������״̬���� */
    __IO uint8_t    step_last;      /* �ϴλ���״̬ */
    __IO uint8_t    dir;            /* �����ת���� */
    __IO int32_t    pos;            /* ���λ�� */
    __IO int32_t    speed;          /* ����ٶ� */
    __IO int16_t    current;        /* ����ٶ� */
    __IO uint16_t   pwm_duty;       /* ���ռ�ձ� */
    __IO uint32_t   hall_keep_t;    /* ��������ʱ�� */
    __IO uint32_t   hall_pul_num;   /* ���������������� */
    __IO uint32_t   lock_time;      /* �����תʱ�� */
    __IO uint32_t   no_single;
    __IO uint32_t   count_j;
} _bldc_obj;

/******************************************************************************************/
#define MOTOR_1                     1
#define MOTOR_2                     2

extern _bldc_obj g_bldc_motor1;
extern _bldc_obj g_bldc_motor2;
/***************************************** ����оƬ��ɲ������ ***************************************************/

#define SHUTDOWN_PIN                        GPIO_PIN_13                                     /* PC13 */
#define SHUTDOWN_PIN_GPIO                   GPIOC
#define SHUTDOWN_PIN_GPIO_CLK_ENABLE()      do{  __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)    /* PC��ʱ��ʹ�� */

#define SHUTDOWN2_PIN                       GPIO_PIN_11    
#define SHUTDOWN2_PIN_GPIO                  GPIOD
#define SHUTDOWN2_PIN_GPIO_CLK_ENABLE()     do{  __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)    /* PD��ʱ��ʹ�� */

#define SHUTDOWN_EN                         HAL_GPIO_WritePin(SHUTDOWN_PIN_GPIO, SHUTDOWN_PIN, GPIO_PIN_SET);   /* ʹ�ܰ���оƬ��SD���� */
#define SHUTDOWN_OFF                        HAL_GPIO_WritePin(SHUTDOWN_PIN_GPIO, SHUTDOWN_PIN, GPIO_PIN_RESET); /* ʧ�ܰ���оƬ��SD���� */
#define SHUTDOWN2_EN                        HAL_GPIO_WritePin(SHUTDOWN2_PIN_GPIO,SHUTDOWN2_PIN,GPIO_PIN_SET);
#define SHUTDOWN2_OFF                       HAL_GPIO_WritePin(SHUTDOWN2_PIN_GPIO,SHUTDOWN2_PIN,GPIO_PIN_RESET);

/***************************************** �����������ӿ� *************************************************/

#define HALL1_TIM_CH1_PIN                   GPIO_PIN_0      /* U */
#define HALL1_TIM_CH1_GPIO                  GPIOA
#define HALL1_U_GPIO_CLK_ENABLE()           do{  __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)    /* PA��ʱ��ʹ�� */

#define HALL1_TIM_CH2_PIN                   GPIO_PIN_1      /* V */
#define HALL1_TIM_CH2_GPIO                  GPIOA
#define HALL1_V_GPIO_CLK_ENABLE()           do{  __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)    /* PA��ʱ��ʹ�� */

#define HALL1_TIM_CH3_PIN                   GPIO_PIN_2      /* W */
#define HALL1_TIM_CH3_GPIO                  GPIOA
#define HALL1_W_GPIO_CLK_ENABLE()           do{  __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)    /* PA��ʱ��ʹ�� */


#define HALL2_TIM_CH1_PIN                   GPIO_PIN_3      /* U */
#define HALL2_TIM_CH1_GPIO                  GPIOD
#define HALL2_U_GPIO_CLK_ENABLE()           do{  __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)    /* PD��ʱ��ʹ�� */

#define HALL2_TIM_CH2_PIN                   GPIO_PIN_4      /* V */
#define HALL2_TIM_CH2_GPIO                  GPIOD
#define HALL2_V_GPIO_CLK_ENABLE()           do{  __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)    /* PD��ʱ��ʹ�� */

#define HALL2_TIM_CH3_PIN                   GPIO_PIN_7      /* W */
#define HALL2_TIM_CH3_GPIO                  GPIOD
#define HALL2_W_GPIO_CLK_ENABLE()           do{  __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)    /* PD��ʱ��ʹ�� */

/****************************************** ������ϵ�� **************************************************/

#define MAX_PWM_DUTY    (((170000 / 18) - 1) * 0.96)    /* ���ռ�ձ����� */

#define H_PWM_L_ON
#ifndef H_PWM_L_ON
#define H_PWM_L_PWM
#endif

#define CCW                         (1)                 /* ��ʱ�� */
#define CW                          (2)                 /* ˳ʱ�� */
#define HALL_ERROR                  (0xF0)              /* ���������־ */
#define RUN                         (1)                 /* ����˶���־ */
#define STOP                        (0)                 /* ���ͣ����־ */

typedef void(*pctr) (void);
void stop_motor1(void);                                 /* ͣ�� */
void start_motor1(void);                                /* ������� */
void stop_motor2(void);
void start_motor2(void);

extern pctr pfunclist_m1[6];                            /* �������ຯ��ָ������ */
extern pctr pfunclist_m2[6];                            /* �������ຯ��ָ������ */
/************************************* �������� *****************************************/

void bldc_init(uint16_t arr, uint16_t psc);             /* BLDC��ʼ�� */
void bldc_ctrl(uint8_t motor_id,int32_t dir,float duty);/* bldc���ƺ��� */
void hall_gpio_init(void);                              /* �����ӿڳ�ʼ�� */
uint32_t hallsensor_get_state(uint8_t motor_id);        /* ��ȡ����״̬ */
/*  �������� */
void m1_uhvl(void);
void m1_uhwl(void);
void m1_vhwl(void);
void m1_vhul(void);
void m1_whul(void);
void m1_whvl(void);

void m2_uhvl(void);
void m2_uhwl(void);
void m2_vhwl(void);
void m2_vhul(void);
void m2_whul(void);
void m2_whvl(void);
#endif
