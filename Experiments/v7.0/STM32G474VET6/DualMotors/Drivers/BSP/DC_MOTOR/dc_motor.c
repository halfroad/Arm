/**
 ****************************************************************************************************
 * @file        dc_motor.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2023-08-01
 * @brief       直流有刷电机控制代码
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

#include "./BSP/DC_MOTOR/dc_motor.h"
#include "./SYSTEM/delay/delay.h"


/***************************************** 基本驱动 两路 *************************************************/

extern TIM_HandleTypeDef g_atimx1_cplm_pwm_handle;                             /* 定时器x句柄 */
extern TIM_HandleTypeDef g_atimx2_cplm_pwm_handle;                             /* 定时器x句柄 */

/**
 * @brief       电机初始化
 * @param       无
 * @retval      无
 */
void dcmotor_init(void)
{
    SHUTDOWN1_GPIO_CLK_ENABLE();
    SHUTDOWN2_GPIO_CLK_ENABLE();
    GPIO_InitTypeDef gpio_init_struct = {0};
    
    /* SD引脚设置，设置为推挽输出 */
    gpio_init_struct.Pin = SHUTDOWN1_Pin;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Pull = GPIO_NOPULL;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(SHUTDOWN1_GPIO_Port, &gpio_init_struct);
    
    gpio_init_struct.Pin = SHUTDOWN2_Pin;
    HAL_GPIO_Init(SHUTDOWN2_GPIO_Port, &gpio_init_struct);
    
    HAL_GPIO_WritePin(SHUTDOWN1_GPIO_Port, SHUTDOWN1_Pin, GPIO_PIN_RESET);      /* SD拉低，关闭输出 */
    HAL_GPIO_WritePin(SHUTDOWN2_GPIO_Port, SHUTDOWN2_Pin, GPIO_PIN_RESET);      /* SD拉低，关闭输出 */
    
    /* 接口1 */
    dcmotor_stop(MOTOR_1);          /* 停止电机 */
    dcmotor_dir(0,MOTOR_1);         /* 设置正转 */
    dcmotor_speed(0,MOTOR_1);       /* 速度设置为0 */
    dcmotor_start(MOTOR_1);         /* 开启电机 */
    
    /* 接口2 */
    dcmotor_stop(MOTOR_2);          /* 停止电机 */
    dcmotor_dir(0,MOTOR_2);         /* 设置正转 */
    dcmotor_speed(0,MOTOR_2);       /* 速度设置为0 */
    dcmotor_start(MOTOR_2);         /* 开启电机 */
}

/**
 * @brief       电机开启
 * @param       无
 * @retval      无
 */
void dcmotor_start(uint8_t id)
{
    if(id == MOTOR_1)                 /* 接口1 */
    {
        ENABLE_MOTOR1;                                   /* 拉高SD引脚，开启电机 */
    }
    else if(id == MOTOR_2)            /* 接口2 */
    {
        ENABLE_MOTOR2;                                   /* 拉高SD引脚，开启电机 */
    }
}

/**
 * @brief       电机停止
 * @param       无
 * @retval      无
 */
void dcmotor_stop(uint8_t id)
{
    if(id == MOTOR_1)                   /* 接口1 */
    {
        HAL_TIM_PWM_Stop(&g_atimx1_cplm_pwm_handle, TIM_CHANNEL_1);          /* 关闭主通道输出 */
        HAL_TIMEx_PWMN_Stop(&g_atimx1_cplm_pwm_handle, TIM_CHANNEL_1);       /* 关闭互补通道输出 */
        DISABLE_MOTOR1;                                                      /* 拉低SD引脚，停止电机 */
    }
    else if(id == MOTOR_2)              /* 接口2 */
    {
        HAL_TIM_PWM_Stop(&g_atimx2_cplm_pwm_handle, TIM_CHANNEL_1);          /* 关闭主通道输出 */
        HAL_TIMEx_PWMN_Stop(&g_atimx2_cplm_pwm_handle, TIM_CHANNEL_1);       /* 关闭互补通道输出 */
        DISABLE_MOTOR2;                                                      /* 拉低SD引脚，停止电机 */
    }
}

/**
 * @brief       电机速度设置
 * @param       para:比较寄存器值
 * @retval      无
 */
void dcmotor_speed(uint16_t para,uint8_t id)
{
    if(id == MOTOR_1)                   /* 接口1 */
    {
        if (para < (__HAL_TIM_GetAutoreload(&g_atimx1_cplm_pwm_handle) - 0x0F))  /* 限速 */
        {  
            __HAL_TIM_SetCompare(&g_atimx1_cplm_pwm_handle, TIM_CHANNEL_1, para);
        }
    }
    else if(id == MOTOR_2)              /* 接口2 */
    {
        if (para < (__HAL_TIM_GetAutoreload(&g_atimx2_cplm_pwm_handle) - 0x0F))  /* 限速 */
        {  
            __HAL_TIM_SetCompare(&g_atimx2_cplm_pwm_handle, TIM_CHANNEL_1, para);
        }
    }
}

/**
 * @brief       电机旋转方向设置
 * @param       para:方向 0正转，1反转
 * @note        以电机正面，顺时针方向旋转为正转
 * @retval      无
 */
void dcmotor_dir(uint8_t para,uint8_t id)
{
    if(id == MOTOR_1)
    {
        HAL_TIM_PWM_Stop(&g_atimx1_cplm_pwm_handle, TIM_CHANNEL_1);          /* 关闭主通道输出 */
        HAL_TIMEx_PWMN_Stop(&g_atimx1_cplm_pwm_handle, TIM_CHANNEL_1);       /* 关闭互补通道输出 */

        if (para == 0)              /* 正转 */
        {
            HAL_TIM_PWM_Start(&g_atimx1_cplm_pwm_handle, TIM_CHANNEL_1);     /* 主通道输出 */
        } 
        else if (para == 1)         /* 反转 */
        {
            HAL_TIMEx_PWMN_Start(&g_atimx1_cplm_pwm_handle, TIM_CHANNEL_1);  /* 互补通道输出 */
        }
    }
    else if(id == MOTOR_2)
    {
        HAL_TIM_PWM_Stop(&g_atimx2_cplm_pwm_handle, TIM_CHANNEL_1);          /* 关闭主通道输出 */
        HAL_TIMEx_PWMN_Stop(&g_atimx2_cplm_pwm_handle, TIM_CHANNEL_1);       /* 关闭互补通道输出 */

        if (para == 0)              /* 正转 */
        {
            HAL_TIM_PWM_Start(&g_atimx2_cplm_pwm_handle, TIM_CHANNEL_1);     /* 主通道输出 */
        } 
        else if (para == 1)         /* 反转 */
        {
            HAL_TIMEx_PWMN_Start(&g_atimx2_cplm_pwm_handle, TIM_CHANNEL_1);  /* 互补通道输出 */
        }
    }
}

/**
 * @brief       设置电机PWM占空比
 * @param       para: pwm比较值 ,正数电机为正转，负数为反转
 * @param       id:电机接口(1或2）
 * @retval      无
 */
void motor_pwm_set(float para,uint8_t id)
{
    int val = (int)para;

    if (val >= 0) 
    {
        dcmotor_dir(0,id);          /* 正转 */
        dcmotor_speed(val,id);
    } 
    else
    {
        dcmotor_dir(1,id);          /* 反转 */
        dcmotor_speed(-val,id);
    }
}

