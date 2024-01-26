/**
 ****************************************************************************************************
 * @file        dcmotor_time.c
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

#include "./BSP/LED/led.h"
#include "./BSP/TIMER/dcmotor_tim.h"


/******************************* ���������������·��  ����������������Ƴ��� **************************************/

TIM_HandleTypeDef g_atimx1_cplm_pwm_handle;                              /* ��ʱ��x��� */
TIM_HandleTypeDef g_atimx2_cplm_pwm_handle;                              /* ��ʱ��x��� */

/**
 * @brief       �߼���ʱ��TIMX ������� ��ʼ��������ʹ��PWMģʽ1��
 * @note
 *              ���ø߼���ʱ��TIMX �������, һ·OCy һ·OCyN, ���ҿ�����������ʱ��
 *
 *              �߼���ʱ����ʱ������APB2, ��PCLK2 = 170Mhz, ��������PPRE2����Ƶ, ���
 *              �߼���ʱ��ʱ�� = 170Mhz
 *              ��ʱ�����ʱ����㷽��: Tout = ((arr + 1) * (psc + 1)) / Ft us.
 *              Ft=��ʱ������Ƶ��, ��λ : Mhz
 *
 * @param       arr: �Զ���װֵ
 * @param       psc: ʱ��Ԥ��Ƶ��
 * @retval      ��
 */
void atim_timx1_cplm_pwm_init(uint16_t arr, uint16_t psc)
{
    GPIO_InitTypeDef gpio_init_struct;
    TIM_OC_InitTypeDef tim_oc_cplm_pwm = {0};
    TIM_BreakDeadTimeConfigTypeDef sbreak_dead_time_config = {0};

    ATIM_TIMX1_CPLM_CHY_GPIO_CLK_ENABLE();                                   /* ͨ��X��ӦIO��ʱ��ʹ�� */
    ATIM_TIMX1_CPLM_CHYN_GPIO_CLK_ENABLE();                                  /* ͨ��X����ͨ����ӦIO��ʱ��ʹ�� */

    /* ����PWM��ͨ������ */
    gpio_init_struct.Pin = ATIM_TIMX1_CPLM_CHY_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;
    gpio_init_struct.Pull = GPIO_NOPULL;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH ;
    gpio_init_struct.Alternate = ATIM_TIMX1_CPLM_CHY_GPIO_AF;                /* �˿ڸ��� */   
    HAL_GPIO_Init(ATIM_TIMX1_CPLM_CHY_GPIO_PORT, &gpio_init_struct);

    /* ����PWM����ͨ������ */
    gpio_init_struct.Pin = ATIM_TIMX1_CPLM_CHYN_GPIO_PIN;
    HAL_GPIO_Init(ATIM_TIMX1_CPLM_CHYN_GPIO_PORT, &gpio_init_struct);

    ATIM_TIMX1_CPLM_CLK_ENABLE();                                            /* ��ʱ��xʱ��ʹ�� */

    g_atimx1_cplm_pwm_handle.Instance = ATIM_TIMX1_CPLM;                     /* ��ʱ��x */
    g_atimx1_cplm_pwm_handle.Init.Prescaler = psc;                           /* ��ʱ��Ԥ��Ƶϵ�� */
    g_atimx1_cplm_pwm_handle.Init.CounterMode = TIM_COUNTERMODE_UP;          /* ���ϼ���ģʽ */
    g_atimx1_cplm_pwm_handle.Init.Period = arr;                              /* �Զ���װ��ֵ */
    g_atimx1_cplm_pwm_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;    /* ʱ�ӷ�Ƶ���� */
    g_atimx1_cplm_pwm_handle.Init.RepetitionCounter = 0;                     /* �ظ��������Ĵ���Ϊ0 */
    g_atimx1_cplm_pwm_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;  /* ʹ��Ӱ�ӼĴ���TIMx_ARR */
    HAL_TIM_PWM_Init(&g_atimx1_cplm_pwm_handle) ;

    /* ����PWM��� */
    tim_oc_cplm_pwm.OCMode = TIM_OCMODE_PWM1;                                 /* PWMģʽ1 */
    tim_oc_cplm_pwm.Pulse = 0;                                                /* �Ƚ�ֵΪ0 */
    tim_oc_cplm_pwm.OCPolarity = TIM_OCPOLARITY_LOW;                          /* OCy �͵�ƽ��Ч */
    tim_oc_cplm_pwm.OCNPolarity = TIM_OCNPOLARITY_LOW;                        /* OCyN �͵�ƽ��Ч */
    tim_oc_cplm_pwm.OCFastMode = TIM_OCFAST_ENABLE;                           /* ��ʹ�ÿ���ģʽ */
    tim_oc_cplm_pwm.OCIdleState = TIM_OCIDLESTATE_RESET;                      /* ��ͨ���Ŀ���״̬ */
    tim_oc_cplm_pwm.OCNIdleState = TIM_OCNIDLESTATE_RESET;                    /* ����ͨ���Ŀ���״̬ */
    HAL_TIM_PWM_ConfigChannel(&g_atimx1_cplm_pwm_handle, &tim_oc_cplm_pwm, ATIM_TIMX1_CPLM_CHY);  /* ���ú�Ĭ����CCER�Ļ������λ */

    /* �����������������������ж� */
    sbreak_dead_time_config.OffStateRunMode = TIM_OSSR_ENABLE;                 /* OSSR����Ϊ1 */
    sbreak_dead_time_config.OffStateIDLEMode = TIM_OSSI_DISABLE;               /* OSSI����Ϊ0 */
    sbreak_dead_time_config.LockLevel = TIM_LOCKLEVEL_OFF;                     /* �ϵ�ֻ��дһ�Σ���Ҫ��������ʱ��ʱֻ���ô�ֵ */
    sbreak_dead_time_config.DeadTime = 0X0F;                                   /* ����ʱ�� */
    sbreak_dead_time_config.BreakState = TIM_BREAK_DISABLE;                    /* BKE = 0, �ر�BKIN��� */
    sbreak_dead_time_config.BreakPolarity = TIM_BREAKPOLARITY_LOW;             /* BKP = 1, BKIN�͵�ƽ��Ч */
    sbreak_dead_time_config.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;     /* ʹ��AOEλ������ɲ�����Զ��ָ���� */
    HAL_TIMEx_ConfigBreakDeadTime(&g_atimx1_cplm_pwm_handle, &sbreak_dead_time_config);  /* ����BDTR�Ĵ��� */
}

void atim_timx2_cplm_pwm_init(uint16_t arr, uint16_t psc)
{
    GPIO_InitTypeDef gpio_init_struct;
    TIM_OC_InitTypeDef tim_oc_cplm_pwm = {0};
    TIM_BreakDeadTimeConfigTypeDef sbreak_dead_time_config = {0};

    ATIM_TIMX2_CPLM_CHY_GPIO_CLK_ENABLE();                                      /* ͨ��X��ӦIO��ʱ��ʹ�� */
    ATIM_TIMX2_CPLM_CHYN_GPIO_CLK_ENABLE();                                     /* ͨ��X����ͨ����ӦIO��ʱ��ʹ�� */

    /* ����PWM��ͨ������ */
    gpio_init_struct.Pin = ATIM_TIMX2_CPLM_CHY_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;
    gpio_init_struct.Pull = GPIO_NOPULL;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH ;
    gpio_init_struct.Alternate = ATIM_TIMX2_CPLM_CHY_GPIO_AF;                   /* �˿ڸ��� */
    HAL_GPIO_Init(ATIM_TIMX2_CPLM_CHY_GPIO_PORT, &gpio_init_struct);

    /* ����PWM����ͨ������ */
    gpio_init_struct.Pin = ATIM_TIMX2_CPLM_CHYN_GPIO_PIN;
    HAL_GPIO_Init(ATIM_TIMX2_CPLM_CHYN_GPIO_PORT, &gpio_init_struct);

    ATIM_TIMX2_CPLM_CLK_ENABLE();                                               /* ��ʱ��xʱ��ʹ�� */

    g_atimx2_cplm_pwm_handle.Instance = ATIM_TIMX2_CPLM;                        /* ��ʱ��x */
    g_atimx2_cplm_pwm_handle.Init.Prescaler = psc;                              /* ��ʱ��Ԥ��Ƶϵ�� */
    g_atimx2_cplm_pwm_handle.Init.CounterMode = TIM_COUNTERMODE_UP;             /* ���ϼ���ģʽ */
    g_atimx2_cplm_pwm_handle.Init.Period = arr;                                 /* �Զ���װ��ֵ */
    g_atimx2_cplm_pwm_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;       /* ʱ�ӷ�Ƶ���� */
    g_atimx2_cplm_pwm_handle.Init.RepetitionCounter = 0;                        /* �ظ��������Ĵ���Ϊ0 */
    g_atimx2_cplm_pwm_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;  /* ʹ��Ӱ�ӼĴ���TIMx_ARR */
    HAL_TIM_PWM_Init(&g_atimx2_cplm_pwm_handle) ;

    tim_oc_cplm_pwm.OCMode = TIM_OCMODE_PWM1;                                   /* PWMģʽ1 */
    tim_oc_cplm_pwm.Pulse = 0;                                                  /* �Ƚ�ֵΪ0 */
    tim_oc_cplm_pwm.OCPolarity = TIM_OCPOLARITY_LOW;                            /* OCy �ߵ�ƽ��Ч */
    tim_oc_cplm_pwm.OCNPolarity = TIM_OCNPOLARITY_LOW;                          /* OCyN �ߵ�ƽ��Ч */
    tim_oc_cplm_pwm.OCFastMode = TIM_OCFAST_ENABLE;                             /* ��ʹ�ÿ���ģʽ */
    tim_oc_cplm_pwm.OCPolarity = TIM_OCIDLESTATE_RESET;                         /* ��ͨ���Ŀ���״̬ */
    tim_oc_cplm_pwm.OCNIdleState = TIM_OCNIDLESTATE_RESET;                      /* ����ͨ���Ŀ���״̬ */
    HAL_TIM_PWM_ConfigChannel(&g_atimx2_cplm_pwm_handle, &tim_oc_cplm_pwm, ATIM_TIMX2_CPLM_CHY);  /* ���ú�Ĭ����CCER�Ļ������λ */   
    
    ATIM_TIMX2_CPLM->CCER |= 0X02;         /* HAL��BUG���������CC1Pλ ��1 */
  
    /* �����������������������ж� */
    sbreak_dead_time_config.OffStateRunMode = TIM_OSSR_ENABLE;                  /* OSSR����Ϊ1 */
    sbreak_dead_time_config.OffStateIDLEMode = TIM_OSSI_DISABLE;                /* OSSI����Ϊ0 */
    sbreak_dead_time_config.LockLevel = TIM_LOCKLEVEL_OFF;                      /* �ϵ�ֻ��дһ�Σ���Ҫ��������ʱ��ʱֻ���ô�ֵ */
    sbreak_dead_time_config.DeadTime = 0X0F;                                    /* ����ʱ�� */
    sbreak_dead_time_config.BreakState = TIM_BREAK_DISABLE;                     /* BKE = 0, �ر�BKIN��� */
    sbreak_dead_time_config.BreakPolarity = TIM_BREAKPOLARITY_LOW;              /* BKP = 1, BKIN�͵�ƽ��Ч */
    sbreak_dead_time_config.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;      /* ʹ��AOEλ������ɲ�����Զ��ָ���� */
    HAL_TIMEx_ConfigBreakDeadTime(&g_atimx2_cplm_pwm_handle, &sbreak_dead_time_config);  /* ����BDTR�Ĵ��� */
}

