/**
 ****************************************************************************************************
 * @file        atim.c
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

#include "./BSP/TIMER/atim.h"
#include "./BSP/LED/led.h"


/********************************* �߼���ʱ�� �����������������ʵ����� *************************************/

TIM_HandleTypeDef g_timx_cplm_pwm_handle;                                /* ��ʱ��x��� */
TIM_BreakDeadTimeConfigTypeDef g_sbreak_dead_time_config = {0};          /* ����ʱ������ */

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
 * @param       arr: �Զ���װֵ��
 * @param       psc: Ԥ��Ƶϵ��
 * @retval      ��
 */

void atim_timx_cplm_pwm_init(uint16_t arr, uint16_t psc)
{
    GPIO_InitTypeDef gpio_init_struct = {0};
    TIM_OC_InitTypeDef tim_oc_cplm_pwm = {0};

    ATIM_TIMX_CPLM_CLK_ENABLE();                                                        /* ������ʱ��Xʱ�� */
    ATIM_TIMX_CPLM_CHY_GPIO_CLK_ENABLE();                                               /* ���������ͨ����GPIOʱ�� */
    ATIM_TIMX_CPLM_CHYN_GPIO_CLK_ENABLE();                                              /* ��������ͨ����GPIOʱ�� */
    ATIM_TIMX_CPLM_BKIN_GPIO_CLK_ENABLE();                                              /* ����ɲ�������GPIOʱ�� */
  
    gpio_init_struct.Pin = ATIM_TIMX_CPLM_CHY_GPIO_PIN;                                 /* �����ͨ����IO */
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;                                            /* ����������� */
    gpio_init_struct.Pull = GPIO_PULLUP;                                                /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;                                      /* ���� */
    gpio_init_struct.Alternate = ATIM_TIMX_CPLM_CHY_GPIO_AF;                            /* IO����Ϊ��ʱ������ */
    HAL_GPIO_Init(ATIM_TIMX_CPLM_CHY_GPIO_PORT, &gpio_init_struct);                     /* ��ʼ��IO */

    gpio_init_struct.Pin = ATIM_TIMX_CPLM_CHYN_GPIO_PIN;                                /* ����ͨ����IO */
    HAL_GPIO_Init(ATIM_TIMX_CPLM_CHYN_GPIO_PORT, &gpio_init_struct);

    gpio_init_struct.Pin = ATIM_TIMX_CPLM_BKIN_GPIO_PIN;                                /* ɲ�������IO */
    HAL_GPIO_Init(ATIM_TIMX_CPLM_BKIN_GPIO_PORT, &gpio_init_struct);

    /* ���ö�ʱ�� */
    g_timx_cplm_pwm_handle.Instance = ATIM_TIMX_CPLM;                                   /* ��ʱ��x */
    g_timx_cplm_pwm_handle.Init.Prescaler = psc;                                        /* Ԥ��Ƶϵ�� */
    g_timx_cplm_pwm_handle.Init.CounterMode = TIM_COUNTERMODE_UP;                       /* ��������ģʽ */
    g_timx_cplm_pwm_handle.Init.Period = arr;                                           /* �Զ���װ��ֵ */
    g_timx_cplm_pwm_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV4;                 /* CKD[1:0] = 10, tDTS = 4 * tCK_INT = Ft / 4 = 42.5Mhz */
    g_timx_cplm_pwm_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;      /* ʹ��Ӱ�ӼĴ���TIMx_ARR */
    HAL_TIM_PWM_Init(&g_timx_cplm_pwm_handle) ;

    /* ����PWM��� */
    tim_oc_cplm_pwm.OCMode = TIM_OCMODE_PWM1;                                           /* PWMģʽ1 */
    tim_oc_cplm_pwm.OCPolarity = TIM_OCPOLARITY_LOW;                                    /* OCy �͵�ƽ��Ч */
    tim_oc_cplm_pwm.OCNPolarity = TIM_OCNPOLARITY_LOW;                                  /* OCyN �͵�ƽ��Ч */
    HAL_TIM_PWM_ConfigChannel(&g_timx_cplm_pwm_handle, &tim_oc_cplm_pwm, ATIM_TIMX_CPLM_CHY);

    /* �����������������������ж� */
    g_sbreak_dead_time_config.LockLevel = TIM_LOCKLEVEL_OFF;                            /* ���üĴ��������� */
    g_sbreak_dead_time_config.BreakState = TIM_BREAK_ENABLE;                            /* ʹ��ɲ������ */
    g_sbreak_dead_time_config.BreakPolarity = TIM_BREAKPOLARITY_LOW;                    /* ɲ��������Ч�źż���Ϊ�� */
    g_sbreak_dead_time_config.AutomaticOutput = TIM_AUTOMATICOUTPUT_ENABLE;             /* ʹ��AOEλ������ɲ���������Զ��ָ���� */
    HAL_TIMEx_ConfigBreakDeadTime(&g_timx_cplm_pwm_handle, &g_sbreak_dead_time_config);

    HAL_TIM_PWM_Start(&g_timx_cplm_pwm_handle, ATIM_TIMX_CPLM_CHY);                     /* OCy ���ʹ�� */
    HAL_TIMEx_PWMN_Start(&g_timx_cplm_pwm_handle, ATIM_TIMX_CPLM_CHY);                  /* OCyN ���ʹ�� */
}

/**
 * @brief       ��ʱ��TIMX ��������Ƚ�ֵ & ����ʱ��
 * @param       ccr: ����Ƚ�ֵ
 * @param       dtg: ����ʱ��
 *   @arg       DTGF[7:5]=0xxʱ, ����ʱ�� = DTGF[7:0] * tDTS
 *   @arg       DTGF[7:5]=10xʱ, ����ʱ�� = (64 + DTGF[5:0]) * 2  * tDTS
 *   @arg       DTGF[7:5]=110ʱ, ����ʱ�� = (32 + DTGF[4:0]) * 8  * tDTS
 *   @arg       DTGF[7:5]=111ʱ, ����ʱ�� = (32 + DTGF[4:0]) * 16 * tDTS
 *   @note      tDTS = 1 / (Ft /  CKD[1:0]) = 1 / 42.5M = 23.5ns
 * @retval      ��
 */
void atim_timx_cplm_pwm_set(uint16_t ccr, uint8_t dtg)
{
    g_sbreak_dead_time_config.DeadTime = dtg;
    HAL_TIMEx_ConfigBreakDeadTime(&g_timx_cplm_pwm_handle, &g_sbreak_dead_time_config);  /*��������ʱ��*/
    __HAL_TIM_MOE_ENABLE(&g_timx_cplm_pwm_handle);                                       /* MOE=1,ʹ������� */
    ATIM_TIMX_CPLM_CHY_CCRY = ccr;                                                       /* ���ñȽϼĴ��� */

}

