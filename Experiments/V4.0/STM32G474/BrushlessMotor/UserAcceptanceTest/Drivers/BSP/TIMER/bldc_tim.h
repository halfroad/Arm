/**
 ****************************************************************************************************
 * @file        bldc_tim.h
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

#ifndef __BLDC_TIM_H
#define __BLDC_TIM_H

#include "./SYSTEM/sys/sys.h"

/******************************************* TIM1初始化宏（接口1） **************************************************************/

/* TIM_CHx通道 上桥臂IO定义 */
#define ATIM_TIMX_PWM_CH1_GPIO_PORT            GPIOA
#define ATIM_TIMX_PWM_CH1_GPIO_PIN             GPIO_PIN_8
#define ATIM_TIMX_PWM_CH1_GPIO_CLK_ENABLE()    do{  __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)     /* PA口时钟使能 */

#define ATIM_TIMX_PWM_CH2_GPIO_PORT            GPIOA
#define ATIM_TIMX_PWM_CH2_GPIO_PIN             GPIO_PIN_9
#define ATIM_TIMX_PWM_CH2_GPIO_CLK_ENABLE()    do{  __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)     /* PA口时钟使能 */

#define ATIM_TIMX_PWM_CH3_GPIO_PORT            GPIOA
#define ATIM_TIMX_PWM_CH3_GPIO_PIN             GPIO_PIN_10
#define ATIM_TIMX_PWM_CH3_GPIO_CLK_ENABLE()    do{  __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)     /* PA口时钟使能 */

/* 下桥臂IO定义 */
#define M1_LOW_SIDE_U_PORT                      GPIOB
#define M1_LOW_SIDE_U_PIN                       GPIO_PIN_13
#define M1_LOW_SIDE_U_GPIO_CLK_ENABLE()         do{  __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)    /* PB口时钟使能 */

#define M1_LOW_SIDE_V_PORT                      GPIOB
#define M1_LOW_SIDE_V_PIN                       GPIO_PIN_14
#define M1_LOW_SIDE_V_GPIO_CLK_ENABLE()         do{  __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)    /* PB口时钟使能 */

#define M1_LOW_SIDE_W_PORT                      GPIOB
#define M1_LOW_SIDE_W_PIN                       GPIO_PIN_15
#define M1_LOW_SIDE_W_GPIO_CLK_ENABLE()         do{  __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)    /* PB口时钟使能 */

/* 复用到TIM1 */
#define ATIM_TIMX_PWM_CHY_GPIO_AF              GPIO_AF6_TIM1

#define ATIM_TIMX_PWM                          TIM1
#define ATIM_TIMX_PWM_IRQn                     TIM1_UP_TIM16_IRQn
#define ATIM_TIMX_PWM_IRQHandler               TIM1_UP_TIM16_IRQHandler
#define ATIM_TIMX_PWM_CH1                      TIM_CHANNEL_1                                    /* 通道1 */
#define ATIM_TIMX_PWM_CH2                      TIM_CHANNEL_2                                    /* 通道2 */
#define ATIM_TIMX_PWM_CH3                      TIM_CHANNEL_3                                    /* 通道3 */
#define ATIM_TIMX_PWM_CHY_CLK_ENABLE()         do{ __HAL_RCC_TIM1_CLK_ENABLE(); }while(0)       /* TIM1 时钟使能 */


/******************************************* TIM8初始化宏（接口2） **************************************************************/

#define ATIM_TIMX2_PWM_CH1_GPIO_PORT            GPIOC
#define ATIM_TIMX2_PWM_CH1_GPIO_PIN             GPIO_PIN_6
#define ATIM_TIMX2_PWM_CH1_GPIO_CLK_ENABLE()    do{  __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)    /* PC口时钟使能 */

#define ATIM_TIMX2_PWM_CH2_GPIO_PORT            GPIOC
#define ATIM_TIMX2_PWM_CH2_GPIO_PIN             GPIO_PIN_7
#define ATIM_TIMX2_PWM_CH2_GPIO_CLK_ENABLE()    do{  __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)    /* PC口时钟使能 */

#define ATIM_TIMX2_PWM_CH3_GPIO_PORT            GPIOC
#define ATIM_TIMX2_PWM_CH3_GPIO_PIN             GPIO_PIN_8
#define ATIM_TIMX2_PWM_CH3_GPIO_CLK_ENABLE()    do{  __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)    /* PC口时钟使能 */

/* 互补通道IO */
#define M2_LOW_SIDE_U_PORT                      GPIOC
#define M2_LOW_SIDE_U_PIN                       GPIO_PIN_10
#define M2_LOW_SIDE_U_GPIO_CLK_ENABLE()         do{  __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)    /* PC口时钟使能 */

#define M2_LOW_SIDE_V_PORT                      GPIOC
#define M2_LOW_SIDE_V_PIN                       GPIO_PIN_11
#define M2_LOW_SIDE_V_GPIO_CLK_ENABLE()         do{  __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)    /* PC口时钟使能 */

#define M2_LOW_SIDE_W_PORT                      GPIOC
#define M2_LOW_SIDE_W_PIN                       GPIO_PIN_12
#define M2_LOW_SIDE_W_GPIO_CLK_ENABLE()         do{  __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)    /* PC口时钟使能 */


#define ATIM_TIMX2_PWM_CHY_GPIO_AF              GPIO_AF4_TIM8
#define ATIM_TIMX2_PWM                          TIM8
#define ATIM_TIMX2_PWM_IRQn                     TIM8_UP_IRQn
#define ATIM_TIMX2_PWM_IRQHandler               TIM8_UP_IRQHandler
#define ATIM_TIMX2_PWM_CH1                      TIM_CHANNEL_1                                   /* 通道1 */
#define ATIM_TIMX2_PWM_CH2                      TIM_CHANNEL_2                                   /* 通道2 */
#define ATIM_TIMX2_PWM_CH3                      TIM_CHANNEL_3                                   /* 通道3 */

#define ATIM_TIMX2_PWM_CHY_CLK_ENABLE()         do{ __HAL_RCC_TIM8_CLK_ENABLE(); }while(0)      /* TIM8 时钟使能 */

extern TIM_HandleTypeDef g_atimx_handle;                                                        /* 定时器x句柄 */
extern TIM_HandleTypeDef g_atimx2_handle;                                                       /* 定时器x句柄 */
/******************************************************************************************/

void atim_timx_oc_chy_init(uint16_t arr, uint16_t psc);                                         /* 高级定时器 PWM初始化函数 */
void atim_timx2_oc_chy_init(uint16_t arr, uint16_t psc);


#endif

















