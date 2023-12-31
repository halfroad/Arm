/**
 ****************************************************************************************************
 * @file        atim.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2023-08-01
 * @brief       高级定时器 驱动代码
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 STM32G474开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com/forum.php
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 * 修改说明
 * V1.0 20230801
 * 第一次发布
 *
 ****************************************************************************************************
 */

#include "./BSP/TIMER/atim.h"
#include "./BSP/LED/led.h"


/********************************* 高级定时器 互补输出带死区控制实验程序 *************************************/

TIM_HandleTypeDef g_timx_cplm_pwm_handle;                                /* 定时器x句柄 */
TIM_BreakDeadTimeConfigTypeDef g_sbreak_dead_time_config = {0};          /* 死区时间设置 */

/**
 * @brief       高级定时器TIMX 互补输出 初始化函数（使用PWM模式1）
 * @note
 *              配置高级定时器TIMX 互补输出, 一路OCy 一路OCyN, 并且可以设置死区时间
 *
 *              高级定时器的时钟来自APB2, 而PCLK2 = 170Mhz, 我们设置PPRE2不分频, 因此
 *              高级定时器时钟 = 170Mhz
 *              定时器溢出时间计算方法: Tout = ((arr + 1) * (psc + 1)) / Ft us.
 *              Ft=定时器工作频率, 单位 : Mhz
 *
 * @param       arr: 自动重装值。
 * @param       psc: 预分频系数
 * @retval      无
 */

void atim_timx_cplm_pwm_init(uint16_t arr, uint16_t psc)
{
    GPIO_InitTypeDef gpio_init_struct = {0};
    TIM_OC_InitTypeDef tim_oc_cplm_pwm = {0};

    ATIM_TIMX_CPLM_CLK_ENABLE();                                                        /* 开启定时器X时钟 */
    ATIM_TIMX_CPLM_CHY_GPIO_CLK_ENABLE();                                               /* 开启主输出通道的GPIO时钟 */
    ATIM_TIMX_CPLM_CHYN_GPIO_CLK_ENABLE();                                              /* 开启互补通道的GPIO时钟 */
    ATIM_TIMX_CPLM_BKIN_GPIO_CLK_ENABLE();                                              /* 开启刹车输入的GPIO时钟 */
  
    gpio_init_struct.Pin = ATIM_TIMX_CPLM_CHY_GPIO_PIN;                                 /* 主输出通道的IO */
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;                                            /* 复用推挽输出 */
    gpio_init_struct.Pull = GPIO_PULLUP;                                                /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;                                      /* 高速 */
    gpio_init_struct.Alternate = ATIM_TIMX_CPLM_CHY_GPIO_AF;                            /* IO复用为定时器功能 */
    HAL_GPIO_Init(ATIM_TIMX_CPLM_CHY_GPIO_PORT, &gpio_init_struct);                     /* 初始化IO */

    gpio_init_struct.Pin = ATIM_TIMX_CPLM_CHYN_GPIO_PIN;                                /* 互补通道的IO */
    HAL_GPIO_Init(ATIM_TIMX_CPLM_CHYN_GPIO_PORT, &gpio_init_struct);

    gpio_init_struct.Pin = ATIM_TIMX_CPLM_BKIN_GPIO_PIN;                                /* 刹车输入的IO */
    HAL_GPIO_Init(ATIM_TIMX_CPLM_BKIN_GPIO_PORT, &gpio_init_struct);

    /* 设置定时器 */
    g_timx_cplm_pwm_handle.Instance = ATIM_TIMX_CPLM;                                   /* 定时器x */
    g_timx_cplm_pwm_handle.Init.Prescaler = psc;                                        /* 预分频系数 */
    g_timx_cplm_pwm_handle.Init.CounterMode = TIM_COUNTERMODE_UP;                       /* 递增计数模式 */
    g_timx_cplm_pwm_handle.Init.Period = arr;                                           /* 自动重装载值 */
    g_timx_cplm_pwm_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV4;                 /* CKD[1:0] = 10, tDTS = 4 * tCK_INT = Ft / 4 = 42.5Mhz */
    g_timx_cplm_pwm_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;      /* 使能影子寄存器TIMx_ARR */
    HAL_TIM_PWM_Init(&g_timx_cplm_pwm_handle) ;

    /* 设置PWM输出 */
    tim_oc_cplm_pwm.OCMode = TIM_OCMODE_PWM1;                                           /* PWM模式1 */
    tim_oc_cplm_pwm.OCPolarity = TIM_OCPOLARITY_LOW;                                    /* OCy 低电平有效 */
    tim_oc_cplm_pwm.OCNPolarity = TIM_OCNPOLARITY_LOW;                                  /* OCyN 低电平有效 */
    HAL_TIM_PWM_ConfigChannel(&g_timx_cplm_pwm_handle, &tim_oc_cplm_pwm, ATIM_TIMX_CPLM_CHY);

    /* 设置死区参数，开启死区中断 */
    g_sbreak_dead_time_config.LockLevel = TIM_LOCKLEVEL_OFF;                            /* 不用寄存器锁功能 */
    g_sbreak_dead_time_config.BreakState = TIM_BREAK_ENABLE;                            /* 使能刹车输入 */
    g_sbreak_dead_time_config.BreakPolarity = TIM_BREAKPOLARITY_LOW;                    /* 刹车输入有效信号极性为低 */
    g_sbreak_dead_time_config.AutomaticOutput = TIM_AUTOMATICOUTPUT_ENABLE;             /* 使能AOE位，允许刹车结束后自动恢复输出 */
    HAL_TIMEx_ConfigBreakDeadTime(&g_timx_cplm_pwm_handle, &g_sbreak_dead_time_config);

    HAL_TIM_PWM_Start(&g_timx_cplm_pwm_handle, ATIM_TIMX_CPLM_CHY);                     /* OCy 输出使能 */
    HAL_TIMEx_PWMN_Start(&g_timx_cplm_pwm_handle, ATIM_TIMX_CPLM_CHY);                  /* OCyN 输出使能 */
}

/**
 * @brief       定时器TIMX 设置输出比较值 & 死区时间
 * @param       ccr: 输出比较值
 * @param       dtg: 死区时间
 *   @arg       DTGF[7:5]=0xx时, 死区时间 = DTGF[7:0] * tDTS
 *   @arg       DTGF[7:5]=10x时, 死区时间 = (64 + DTGF[5:0]) * 2  * tDTS
 *   @arg       DTGF[7:5]=110时, 死区时间 = (32 + DTGF[4:0]) * 8  * tDTS
 *   @arg       DTGF[7:5]=111时, 死区时间 = (32 + DTGF[4:0]) * 16 * tDTS
 *   @note      tDTS = 1 / (Ft /  CKD[1:0]) = 1 / 42.5M = 23.5ns
 * @retval      无
 */
void atim_timx_cplm_pwm_set(uint16_t ccr, uint8_t dtg)
{
    g_sbreak_dead_time_config.DeadTime = dtg;
    HAL_TIMEx_ConfigBreakDeadTime(&g_timx_cplm_pwm_handle, &g_sbreak_dead_time_config);  /*重设死区时间*/
    __HAL_TIM_MOE_ENABLE(&g_timx_cplm_pwm_handle);                                       /* MOE=1,使能主输出 */
    ATIM_TIMX_CPLM_CHY_CCRY = ccr;                                                       /* 设置比较寄存器 */

}

