/**
 ****************************************************************************************************
 * @file        dc_motor.c
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

#include "./BSP/DC_MOTOR/dc_motor.h"
#include "./SYSTEM/delay/delay.h"


/***************************************** �������� ��· *************************************************/

extern TIM_HandleTypeDef g_atimx1_cplm_pwm_handle;                             /* ��ʱ��x��� */
extern TIM_HandleTypeDef g_atimx2_cplm_pwm_handle;                             /* ��ʱ��x��� */

/**
 * @brief       �����ʼ��
 * @param       ��
 * @retval      ��
 */
void dcmotor_init(void)
{
    SHUTDOWN1_GPIO_CLK_ENABLE();
    SHUTDOWN2_GPIO_CLK_ENABLE();
    GPIO_InitTypeDef gpio_init_struct = {0};
    
    /* SD�������ã�����Ϊ������� */
    gpio_init_struct.Pin = SHUTDOWN1_Pin;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Pull = GPIO_NOPULL;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(SHUTDOWN1_GPIO_Port, &gpio_init_struct);
    
    gpio_init_struct.Pin = SHUTDOWN2_Pin;
    HAL_GPIO_Init(SHUTDOWN2_GPIO_Port, &gpio_init_struct);
    
    HAL_GPIO_WritePin(SHUTDOWN1_GPIO_Port, SHUTDOWN1_Pin, GPIO_PIN_RESET);      /* SD���ͣ��ر���� */
    HAL_GPIO_WritePin(SHUTDOWN2_GPIO_Port, SHUTDOWN2_Pin, GPIO_PIN_RESET);      /* SD���ͣ��ر���� */
    
    /* �ӿ�1 */
    dcmotor_stop(MOTOR_1);          /* ֹͣ��� */
    dcmotor_dir(0,MOTOR_1);         /* ������ת */
    dcmotor_speed(0,MOTOR_1);       /* �ٶ�����Ϊ0 */
    dcmotor_start(MOTOR_1);         /* ������� */
    
    /* �ӿ�2 */
    dcmotor_stop(MOTOR_2);          /* ֹͣ��� */
    dcmotor_dir(0,MOTOR_2);         /* ������ת */
    dcmotor_speed(0,MOTOR_2);       /* �ٶ�����Ϊ0 */
    dcmotor_start(MOTOR_2);         /* ������� */
}

/**
 * @brief       �������
 * @param       ��
 * @retval      ��
 */
void dcmotor_start(uint8_t id)
{
    if(id == MOTOR_1)                 /* �ӿ�1 */
    {
        ENABLE_MOTOR1;                                   /* ����SD���ţ�������� */
    }
    else if(id == MOTOR_2)            /* �ӿ�2 */
    {
        ENABLE_MOTOR2;                                   /* ����SD���ţ�������� */
    }
}

/**
 * @brief       ���ֹͣ
 * @param       ��
 * @retval      ��
 */
void dcmotor_stop(uint8_t id)
{
    if(id == MOTOR_1)                   /* �ӿ�1 */
    {
        HAL_TIM_PWM_Stop(&g_atimx1_cplm_pwm_handle, TIM_CHANNEL_1);          /* �ر���ͨ����� */
        HAL_TIMEx_PWMN_Stop(&g_atimx1_cplm_pwm_handle, TIM_CHANNEL_1);       /* �رջ���ͨ����� */
        DISABLE_MOTOR1;                                                      /* ����SD���ţ�ֹͣ��� */
    }
    else if(id == MOTOR_2)              /* �ӿ�2 */
    {
        HAL_TIM_PWM_Stop(&g_atimx2_cplm_pwm_handle, TIM_CHANNEL_1);          /* �ر���ͨ����� */
        HAL_TIMEx_PWMN_Stop(&g_atimx2_cplm_pwm_handle, TIM_CHANNEL_1);       /* �رջ���ͨ����� */
        DISABLE_MOTOR2;                                                      /* ����SD���ţ�ֹͣ��� */
    }
}

/**
 * @brief       ����ٶ�����
 * @param       para:�ȽϼĴ���ֵ
 * @retval      ��
 */
void dcmotor_speed(uint16_t para,uint8_t id)
{
    if(id == MOTOR_1)                   /* �ӿ�1 */
    {
        if (para < (__HAL_TIM_GetAutoreload(&g_atimx1_cplm_pwm_handle) - 0x0F))  /* ���� */
        {  
            __HAL_TIM_SetCompare(&g_atimx1_cplm_pwm_handle, TIM_CHANNEL_1, para);
        }
    }
    else if(id == MOTOR_2)              /* �ӿ�2 */
    {
        if (para < (__HAL_TIM_GetAutoreload(&g_atimx2_cplm_pwm_handle) - 0x0F))  /* ���� */
        {  
            __HAL_TIM_SetCompare(&g_atimx2_cplm_pwm_handle, TIM_CHANNEL_1, para);
        }
    }
}

/**
 * @brief       �����ת��������
 * @param       para:���� 0��ת��1��ת
 * @note        �Ե�����棬˳ʱ�뷽����תΪ��ת
 * @retval      ��
 */
void dcmotor_dir(uint8_t para,uint8_t id)
{
    if(id == MOTOR_1)
    {
        HAL_TIM_PWM_Stop(&g_atimx1_cplm_pwm_handle, TIM_CHANNEL_1);          /* �ر���ͨ����� */
        HAL_TIMEx_PWMN_Stop(&g_atimx1_cplm_pwm_handle, TIM_CHANNEL_1);       /* �رջ���ͨ����� */

        if (para == 0)              /* ��ת */
        {
            HAL_TIM_PWM_Start(&g_atimx1_cplm_pwm_handle, TIM_CHANNEL_1);     /* ��ͨ����� */
        } 
        else if (para == 1)         /* ��ת */
        {
            HAL_TIMEx_PWMN_Start(&g_atimx1_cplm_pwm_handle, TIM_CHANNEL_1);  /* ����ͨ����� */
        }
    }
    else if(id == MOTOR_2)
    {
        HAL_TIM_PWM_Stop(&g_atimx2_cplm_pwm_handle, TIM_CHANNEL_1);          /* �ر���ͨ����� */
        HAL_TIMEx_PWMN_Stop(&g_atimx2_cplm_pwm_handle, TIM_CHANNEL_1);       /* �رջ���ͨ����� */

        if (para == 0)              /* ��ת */
        {
            HAL_TIM_PWM_Start(&g_atimx2_cplm_pwm_handle, TIM_CHANNEL_1);     /* ��ͨ����� */
        } 
        else if (para == 1)         /* ��ת */
        {
            HAL_TIMEx_PWMN_Start(&g_atimx2_cplm_pwm_handle, TIM_CHANNEL_1);  /* ����ͨ����� */
        }
    }
}

/**
 * @brief       ���õ��PWMռ�ձ�
 * @param       para: pwm�Ƚ�ֵ ,�������Ϊ��ת������Ϊ��ת
 * @param       id:����ӿ�(1��2��
 * @retval      ��
 */
void motor_pwm_set(float para,uint8_t id)
{
    int val = (int)para;

    if (val >= 0) 
    {
        dcmotor_dir(0,id);          /* ��ת */
        dcmotor_speed(val,id);
    } 
    else
    {
        dcmotor_dir(1,id);          /* ��ת */
        dcmotor_speed(-val,id);
    }
}

