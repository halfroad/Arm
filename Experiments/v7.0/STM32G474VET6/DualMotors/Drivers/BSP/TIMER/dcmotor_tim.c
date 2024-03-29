/**
 ****************************************************************************************************
 * @file        dcmotor_time.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2023-08-01
 * @brief       定时器 驱动代码
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 STM32G474开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 * 修改说明
 * V1.0 20230801
 * 第一次发布
 *
 ****************************************************************************************************
 */

#include "./BSP/LED/led.h"
#include "./BSP/TIMER/dcmotor_tim.h"


/******************************* 电机基本驱动（两路）  互补输出带死区控制程序 **************************************/

TIM_HandleTypeDef g_atimx1_cplm_pwm_handle;                              /* 定时器x句柄 */
TIM_HandleTypeDef g_atimx2_cplm_pwm_handle;                              /* 定时器x句柄 */

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
 * @param       arr: 自动重装值
 * @param       psc: 时钟预分频数
 * @retval      无
 */
void atim_timx1_cplm_pwm_init(uint16_t arr, uint16_t psc)
{
    GPIO_InitTypeDef gpio_init_struct;
    TIM_OC_InitTypeDef tim_oc_cplm_pwm = {0};
    TIM_BreakDeadTimeConfigTypeDef sbreak_dead_time_config = {0};

    ATIM_TIMX1_CPLM_CHY_GPIO_CLK_ENABLE();                                   /* 通道X对应IO口时钟使能 */
    ATIM_TIMX1_CPLM_CHYN_GPIO_CLK_ENABLE();                                  /* 通道X互补通道对应IO口时钟使能 */

    /* 配置PWM主通道引脚 */
    gpio_init_struct.Pin = ATIM_TIMX1_CPLM_CHY_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;
    gpio_init_struct.Pull = GPIO_NOPULL;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH ;
    gpio_init_struct.Alternate = ATIM_TIMX1_CPLM_CHY_GPIO_AF;                /* 端口复用 */   
    HAL_GPIO_Init(ATIM_TIMX1_CPLM_CHY_GPIO_PORT, &gpio_init_struct);

    /* 配置PWM互补通道引脚 */
    gpio_init_struct.Pin = ATIM_TIMX1_CPLM_CHYN_GPIO_PIN;
    HAL_GPIO_Init(ATIM_TIMX1_CPLM_CHYN_GPIO_PORT, &gpio_init_struct);

    ATIM_TIMX1_CPLM_CLK_ENABLE();                                            /* 定时器x时钟使能 */

    g_atimx1_cplm_pwm_handle.Instance = ATIM_TIMX1_CPLM;                     /* 定时器x */
    g_atimx1_cplm_pwm_handle.Init.Prescaler = psc;                           /* 定时器预分频系数 */
    g_atimx1_cplm_pwm_handle.Init.CounterMode = TIM_COUNTERMODE_UP;          /* 向上计数模式 */
    g_atimx1_cplm_pwm_handle.Init.Period = arr;                              /* 自动重装载值 */
    g_atimx1_cplm_pwm_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;    /* 时钟分频因子 */
    g_atimx1_cplm_pwm_handle.Init.RepetitionCounter = 0;                     /* 重复计数器寄存器为0 */
    g_atimx1_cplm_pwm_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;  /* 使能影子寄存器TIMx_ARR */
    HAL_TIM_PWM_Init(&g_atimx1_cplm_pwm_handle) ;

    /* 设置PWM输出 */
    tim_oc_cplm_pwm.OCMode = TIM_OCMODE_PWM1;                                 /* PWM模式1 */
    tim_oc_cplm_pwm.Pulse = 0;                                                /* 比较值为0 */
    tim_oc_cplm_pwm.OCPolarity = TIM_OCPOLARITY_LOW;                          /* OCy 低电平有效 */
    tim_oc_cplm_pwm.OCNPolarity = TIM_OCNPOLARITY_LOW;                        /* OCyN 低电平有效 */
    tim_oc_cplm_pwm.OCFastMode = TIM_OCFAST_ENABLE;                           /* 不使用快速模式 */
    tim_oc_cplm_pwm.OCIdleState = TIM_OCIDLESTATE_RESET;                      /* 主通道的空闲状态 */
    tim_oc_cplm_pwm.OCNIdleState = TIM_OCNIDLESTATE_RESET;                    /* 互补通道的空闲状态 */
    HAL_TIM_PWM_ConfigChannel(&g_atimx1_cplm_pwm_handle, &tim_oc_cplm_pwm, ATIM_TIMX1_CPLM_CHY);  /* 配置后默认清CCER的互补输出位 */

    /* 设置死区参数，开启死区中断 */
    sbreak_dead_time_config.OffStateRunMode = TIM_OSSR_ENABLE;                 /* OSSR设置为1 */
    sbreak_dead_time_config.OffStateIDLEMode = TIM_OSSI_DISABLE;               /* OSSI设置为0 */
    sbreak_dead_time_config.LockLevel = TIM_LOCKLEVEL_OFF;                     /* 上电只能写一次，需要更新死区时间时只能用此值 */
    sbreak_dead_time_config.DeadTime = 0X0F;                                   /* 死区时间 */
    sbreak_dead_time_config.BreakState = TIM_BREAK_DISABLE;                    /* BKE = 0, 关闭BKIN检测 */
    sbreak_dead_time_config.BreakPolarity = TIM_BREAKPOLARITY_LOW;             /* BKP = 1, BKIN低电平有效 */
    sbreak_dead_time_config.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;     /* 使能AOE位，允许刹车后自动恢复输出 */
    HAL_TIMEx_ConfigBreakDeadTime(&g_atimx1_cplm_pwm_handle, &sbreak_dead_time_config);  /* 设置BDTR寄存器 */
}

void atim_timx2_cplm_pwm_init(uint16_t arr, uint16_t psc)
{
    GPIO_InitTypeDef gpio_init_struct;
    TIM_OC_InitTypeDef tim_oc_cplm_pwm = {0};
    TIM_BreakDeadTimeConfigTypeDef sbreak_dead_time_config = {0};

    ATIM_TIMX2_CPLM_CHY_GPIO_CLK_ENABLE();                                      /* 通道X对应IO口时钟使能 */
    ATIM_TIMX2_CPLM_CHYN_GPIO_CLK_ENABLE();                                     /* 通道X互补通道对应IO口时钟使能 */

    /* 配置PWM主通道引脚 */
    gpio_init_struct.Pin = ATIM_TIMX2_CPLM_CHY_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;
    gpio_init_struct.Pull = GPIO_NOPULL;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH ;
    gpio_init_struct.Alternate = ATIM_TIMX2_CPLM_CHY_GPIO_AF;                   /* 端口复用 */
    HAL_GPIO_Init(ATIM_TIMX2_CPLM_CHY_GPIO_PORT, &gpio_init_struct);

    /* 配置PWM互补通道引脚 */
    gpio_init_struct.Pin = ATIM_TIMX2_CPLM_CHYN_GPIO_PIN;
    HAL_GPIO_Init(ATIM_TIMX2_CPLM_CHYN_GPIO_PORT, &gpio_init_struct);

    ATIM_TIMX2_CPLM_CLK_ENABLE();                                               /* 定时器x时钟使能 */

    g_atimx2_cplm_pwm_handle.Instance = ATIM_TIMX2_CPLM;                        /* 定时器x */
    g_atimx2_cplm_pwm_handle.Init.Prescaler = psc;                              /* 定时器预分频系数 */
    g_atimx2_cplm_pwm_handle.Init.CounterMode = TIM_COUNTERMODE_UP;             /* 向上计数模式 */
    g_atimx2_cplm_pwm_handle.Init.Period = arr;                                 /* 自动重装载值 */
    g_atimx2_cplm_pwm_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;       /* 时钟分频因子 */
    g_atimx2_cplm_pwm_handle.Init.RepetitionCounter = 0;                        /* 重复计数器寄存器为0 */
    g_atimx2_cplm_pwm_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;  /* 使能影子寄存器TIMx_ARR */
    HAL_TIM_PWM_Init(&g_atimx2_cplm_pwm_handle) ;

    tim_oc_cplm_pwm.OCMode = TIM_OCMODE_PWM1;                                   /* PWM模式1 */
    tim_oc_cplm_pwm.Pulse = 0;                                                  /* 比较值为0 */
    tim_oc_cplm_pwm.OCPolarity = TIM_OCPOLARITY_LOW;                            /* OCy 高电平有效 */
    tim_oc_cplm_pwm.OCNPolarity = TIM_OCNPOLARITY_LOW;                          /* OCyN 高电平有效 */
    tim_oc_cplm_pwm.OCFastMode = TIM_OCFAST_ENABLE;                             /* 不使用快速模式 */
    tim_oc_cplm_pwm.OCPolarity = TIM_OCIDLESTATE_RESET;                         /* 主通道的空闲状态 */
    tim_oc_cplm_pwm.OCNIdleState = TIM_OCNIDLESTATE_RESET;                      /* 互补通道的空闲状态 */
    HAL_TIM_PWM_ConfigChannel(&g_atimx2_cplm_pwm_handle, &tim_oc_cplm_pwm, ATIM_TIMX2_CPLM_CHY);  /* 配置后默认清CCER的互补输出位 */   
    
    ATIM_TIMX2_CPLM->CCER |= 0X02;         /* HAL库BUG，需另外给CC1P位 置1 */
  
    /* 设置死区参数，开启死区中断 */
    sbreak_dead_time_config.OffStateRunMode = TIM_OSSR_ENABLE;                  /* OSSR设置为1 */
    sbreak_dead_time_config.OffStateIDLEMode = TIM_OSSI_DISABLE;                /* OSSI设置为0 */
    sbreak_dead_time_config.LockLevel = TIM_LOCKLEVEL_OFF;                      /* 上电只能写一次，需要更新死区时间时只能用此值 */
    sbreak_dead_time_config.DeadTime = 0X0F;                                    /* 死区时间 */
    sbreak_dead_time_config.BreakState = TIM_BREAK_DISABLE;                     /* BKE = 0, 关闭BKIN检测 */
    sbreak_dead_time_config.BreakPolarity = TIM_BREAKPOLARITY_LOW;              /* BKP = 1, BKIN低电平有效 */
    sbreak_dead_time_config.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;      /* 使能AOE位，允许刹车后自动恢复输出 */
    HAL_TIMEx_ConfigBreakDeadTime(&g_atimx2_cplm_pwm_handle, &sbreak_dead_time_config);  /* 设置BDTR寄存器 */
}

