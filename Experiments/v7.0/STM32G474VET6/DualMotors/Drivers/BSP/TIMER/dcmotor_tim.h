/**
 ****************************************************************************************************
 * @file        dcmotor_tim.h
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

#ifndef __DCMOTOR_TIM_H
#define __DCMOTOR_TIM_H

#include "./SYSTEM/sys/sys.h"


/********************************* 电机基本驱动 互补输出带死区控制 ************************************/

/* TIMX 互补输出模式 定义 */

/* 接口1 主输出通道引脚 */
#define ATIM_TIMX1_CPLM_CHY_GPIO_PORT            GPIOA
#define ATIM_TIMX1_CPLM_CHY_GPIO_PIN             GPIO_PIN_8
#define ATIM_TIMX1_CPLM_CHY_GPIO_CLK_ENABLE()    do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* PA口时钟使能 */

/* 接口1 互补输出通道引脚 */
#define ATIM_TIMX1_CPLM_CHYN_GPIO_PORT           GPIOB
#define ATIM_TIMX1_CPLM_CHYN_GPIO_PIN            GPIO_PIN_13
#define ATIM_TIMX1_CPLM_CHYN_GPIO_CLK_ENABLE()   do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)   /* PB口时钟使能 */

/* 接口2 主输出通道引脚 */
#define ATIM_TIMX2_CPLM_CHY_GPIO_PORT            GPIOC
#define ATIM_TIMX2_CPLM_CHY_GPIO_PIN             GPIO_PIN_6
#define ATIM_TIMX2_CPLM_CHY_GPIO_CLK_ENABLE()    do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)   /* PC口时钟使能 */

/* 接口2 互补输出通道引脚 */
#define ATIM_TIMX2_CPLM_CHYN_GPIO_PORT           GPIOC
#define ATIM_TIMX2_CPLM_CHYN_GPIO_PIN            GPIO_PIN_10
#define ATIM_TIMX2_CPLM_CHYN_GPIO_CLK_ENABLE()   do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)   /* PC口时钟使能 */

/* TIMX 引脚复用设置
 * 因为PA8/PB13  PC6/PC10, 默认并不是TIM1、TIM8的功能脚, 必须开启复用
 * 这样,PA8/PB13才能用作TIM1的CH1/CH1N功能，PC6/PC10才能用作TIM8的CH1/CH1N功能
 */
#define ATIM_TIMX1_CPLM_CHY_GPIO_AF              GPIO_AF6_TIM1
#define ATIM_TIMX2_CPLM_CHY_GPIO_AF              GPIO_AF4_TIM8

/* 互补输出使用的定时器 */
#define ATIM_TIMX1_CPLM                          TIM1
#define ATIM_TIMX1_CPLM_CHY                      TIM_CHANNEL_1
#define ATIM_TIMX1_CPLM_CLK_ENABLE()             do{ __HAL_RCC_TIM1_CLK_ENABLE(); }while(0)    /* TIM1 时钟使能 */

#define ATIM_TIMX2_CPLM                          TIM8
#define ATIM_TIMX2_CPLM_CHY                      TIM_CHANNEL_1
#define ATIM_TIMX2_CPLM_CLK_ENABLE()             do{ __HAL_RCC_TIM8_CLK_ENABLE(); }while(0)    /* TIM8 时钟使能 */

/******************************************************************************************/

void atim_timx1_cplm_pwm_init(uint16_t arr, uint16_t psc);   /* 高级定时器 互补输出 初始化函数 */
void atim_timx2_cplm_pwm_init(uint16_t arr, uint16_t psc);   /* 高级定时器 互补输出 初始化函数 */

#endif


