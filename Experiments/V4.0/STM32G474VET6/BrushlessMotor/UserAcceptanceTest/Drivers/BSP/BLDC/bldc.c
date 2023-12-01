/**
 ****************************************************************************************************
 * @file        bldc.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2023-08-01
 * @brief       BLDC 驱动代码
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
 
#include "./BSP/BLDC/bldc.h"
#include "./BSP/TIMER/bldc_tim.h"

_bldc_obj g_bldc_motor1 = {STOP, 0, 0, CCW, 0, 0, 0, 0, 0, 0};  /* 电机结构体初始值 */
_bldc_obj g_bldc_motor2 = {STOP,0,0,CCW,0,0,0,0,0,0};   /* 电机结构体 */

/**
 * @brief       无刷电机初始化，包括定时器，霍尔接口以及SD引脚初始化
 * @param       arr: 自动重装值
 * @param       psc: 时钟预分频数
 * @retval      无
 */
void bldc_init(uint16_t arr, uint16_t psc)
{       
    GPIO_InitTypeDef gpio_init_struct;
    
    SHUTDOWN_PIN_GPIO_CLK_ENABLE();
    SHUTDOWN2_PIN_GPIO_CLK_ENABLE();
  
    gpio_init_struct.Pin = SHUTDOWN_PIN;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Pull = GPIO_NOPULL;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(SHUTDOWN_PIN_GPIO, &gpio_init_struct);    
    
    gpio_init_struct.Pin = SHUTDOWN2_PIN;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Pull = GPIO_NOPULL;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(SHUTDOWN2_PIN_GPIO, &gpio_init_struct);  
    
    hall_gpio_init();                       /* 霍尔接口初始化 */
    atim_timx_oc_chy_init(arr, psc);        /* 接口一定时器初始化 */
    atim_timx2_oc_chy_init(arr,  psc);      /* 接口二定时器初始化 */    
}

/**
 * @brief       BLDC控制函数
 * @param       dir :电机方向, Duty:PWM占空比
 * @retval      无
 */
void bldc_ctrl(uint8_t motor_id,int32_t dir,float duty)
{
    if(motor_id == MOTOR_1)
    {
        g_bldc_motor1.dir = dir;            /* 方向 */
        g_bldc_motor1.pwm_duty = duty;      /* 占空比 */
    }
    if(motor_id == MOTOR_2)
    {
        g_bldc_motor2.dir = dir;            /* 方向 */
        g_bldc_motor2.pwm_duty = duty;      /* 占空比 */
    }
}

/************************************ 霍尔接口初始化 ******************************************/

/**
  * @brief  霍尔传感器接口初始化
  * @param  无
  * @retval 无
  */
void hall_gpio_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    HALL1_U_GPIO_CLK_ENABLE();
    HALL1_V_GPIO_CLK_ENABLE();
    HALL1_W_GPIO_CLK_ENABLE();
    
    HALL2_U_GPIO_CLK_ENABLE();
    HALL2_V_GPIO_CLK_ENABLE();
    HALL2_W_GPIO_CLK_ENABLE();
    /* 霍尔接口一 引脚初始化 */
    gpio_init_struct.Pin = HALL1_TIM_CH1_PIN;
    gpio_init_struct.Mode = GPIO_MODE_INPUT;
    gpio_init_struct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(HALL1_TIM_CH1_GPIO, &gpio_init_struct);

    gpio_init_struct.Pin = HALL1_TIM_CH2_PIN;
    HAL_GPIO_Init(HALL1_TIM_CH2_GPIO, &gpio_init_struct);

    gpio_init_struct.Pin = HALL1_TIM_CH3_PIN;
    HAL_GPIO_Init(HALL1_TIM_CH3_GPIO, &gpio_init_struct);

    /* 霍尔接口二 引脚初始化 */
    gpio_init_struct.Pin = HALL2_TIM_CH1_PIN;
    gpio_init_struct.Mode = GPIO_MODE_INPUT;
    gpio_init_struct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(HALL2_TIM_CH1_GPIO, &gpio_init_struct);

    gpio_init_struct.Pin = HALL2_TIM_CH2_PIN;
    HAL_GPIO_Init(HALL2_TIM_CH2_GPIO, &gpio_init_struct);

    gpio_init_struct.Pin = HALL2_TIM_CH3_PIN;
    HAL_GPIO_Init(HALL2_TIM_CH3_GPIO, &gpio_init_struct);
}


/**
 * @brief       获取霍尔传感器引脚状态
 * @param       motor_id ： 电机接口号
 * @retval      霍尔传感器引脚状态
 */
uint32_t hallsensor_get_state(uint8_t motor_id)
{
    uint32_t state = 0;
    
    if(motor_id == MOTOR_1)
    {
        if(HAL_GPIO_ReadPin(HALL1_TIM_CH1_GPIO, HALL1_TIM_CH1_PIN) != GPIO_PIN_RESET)  /* 霍尔传感器状态获取 */
        {
            state |= 0x01U;
        }
        if(HAL_GPIO_ReadPin(HALL1_TIM_CH2_GPIO, HALL1_TIM_CH2_PIN) != GPIO_PIN_RESET)  /* 霍尔传感器状态获取 */
        {
            state |= 0x02U;
        }
        if(HAL_GPIO_ReadPin(HALL1_TIM_CH3_GPIO, HALL1_TIM_CH3_PIN) != GPIO_PIN_RESET)  /* 霍尔传感器状态获取 */
        {
            state |= 0x04U;
        }
    }
    else if(motor_id == MOTOR_2)
    {
        if(HAL_GPIO_ReadPin(HALL2_TIM_CH1_GPIO, HALL2_TIM_CH1_PIN) != GPIO_PIN_RESET)  /* 霍尔传感器状态获取 */
        {
            state |= 0x01U;
        }
        if(HAL_GPIO_ReadPin(HALL2_TIM_CH2_GPIO, HALL2_TIM_CH2_PIN) != GPIO_PIN_RESET)  /* 霍尔传感器状态获取 */
        {
            state |= 0x02U;
        }
        if(HAL_GPIO_ReadPin(HALL2_TIM_CH3_GPIO, HALL2_TIM_CH3_PIN) != GPIO_PIN_RESET)  /* 霍尔传感器状态获取 */
        {
            state |= 0x04U;
        }
    }
    return state;
}

/******************************************** 接口一启停函数 *********************************************/

/**
  * @brief  关闭电机运转
  * @param  无
  * @retval 无
  */
void stop_motor1(void)
{
    /* 关闭半桥芯片输出 */
    SHUTDOWN_OFF;
    /* 关闭PWM输出 */
    HAL_TIM_PWM_Stop(&g_atimx_handle, TIM_CHANNEL_1);
    HAL_TIM_PWM_Stop(&g_atimx_handle, TIM_CHANNEL_2);
    HAL_TIM_PWM_Stop(&g_atimx_handle, TIM_CHANNEL_3);
    /* 上下桥臂全部关断 */
    g_atimx_handle.Instance->CCR2 = 0;
    g_atimx_handle.Instance->CCR1 = 0;
    g_atimx_handle.Instance->CCR3 = 0;
    HAL_GPIO_WritePin(M1_LOW_SIDE_U_PORT, M1_LOW_SIDE_U_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(M1_LOW_SIDE_V_PORT, M1_LOW_SIDE_V_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(M1_LOW_SIDE_W_PORT, M1_LOW_SIDE_W_PIN, GPIO_PIN_RESET);
}

/**
  * @brief  开启电机运转
  * @param  无
  * @retval 无
  */
void start_motor1(void)
{
    SHUTDOWN_EN;
    /* 使能PWM输出 */
    HAL_TIM_PWM_Start(&g_atimx_handle, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&g_atimx_handle, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&g_atimx_handle, TIM_CHANNEL_3);
}

/******************************************** 接口二启停函数 *********************************************/

/**
  * @brief  关闭电机运转
  * @param  无
  * @retval 无
  */
void stop_motor2(void)
{
    SHUTDOWN2_OFF;
    HAL_TIM_PWM_Stop(&g_atimx2_handle, TIM_CHANNEL_1);
    HAL_TIM_PWM_Stop(&g_atimx2_handle, TIM_CHANNEL_2);
    HAL_TIM_PWM_Stop(&g_atimx2_handle, TIM_CHANNEL_3);
    HAL_GPIO_WritePin(M2_LOW_SIDE_U_PORT, M2_LOW_SIDE_U_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(M2_LOW_SIDE_V_PORT, M2_LOW_SIDE_V_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(M2_LOW_SIDE_W_PORT, M2_LOW_SIDE_W_PIN, GPIO_PIN_RESET);
}

/**
  * @brief  开启电机运转
  * @param  无
  * @retval 无
  */
void start_motor2(void)
{
    SHUTDOWN2_EN;
    HAL_TIM_PWM_Start(&g_atimx2_handle, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&g_atimx2_handle, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&g_atimx2_handle, TIM_CHANNEL_3);
}

/*************************** 上下桥臂的导通情况，共6种，也称为6步换向（接口一） ****************************/

/*  六步换向函数指针数组 */
pctr pfunclist_m1[6] =
{
    &m1_uhwl, &m1_vhul, &m1_vhwl,
    &m1_whvl, &m1_uhvl, &m1_whul
};

/**
  * @brief  U相上桥臂导通，V相下桥臂导通
  * @param  无
  * @retval 无
  */
void m1_uhvl(void)
{
    g_atimx_handle.Instance->CCR1 = g_bldc_motor1.pwm_duty;                     /* U相上桥臂PWM */
    g_atimx_handle.Instance->CCR2 = 0;
    g_atimx_handle.Instance->CCR3 = 0;
    
    HAL_GPIO_WritePin(M1_LOW_SIDE_V_PORT, M1_LOW_SIDE_V_PIN, GPIO_PIN_SET);     /* V相下桥臂导通 */
    HAL_GPIO_WritePin(M1_LOW_SIDE_U_PORT, M1_LOW_SIDE_U_PIN, GPIO_PIN_RESET);   /* U相下桥臂关闭 */
    HAL_GPIO_WritePin(M1_LOW_SIDE_W_PORT, M1_LOW_SIDE_W_PIN, GPIO_PIN_RESET);   /* W相下桥臂关闭 */
}

/**
  * @brief  U相上桥臂导通，W相下桥臂导通
  * @param  无
  * @retval 无
  */
void m1_uhwl(void)
{
    g_atimx_handle.Instance->CCR1 = g_bldc_motor1.pwm_duty;
    g_atimx_handle.Instance->CCR2 = 0;
    g_atimx_handle.Instance->CCR3 = 0;
    
    HAL_GPIO_WritePin(M1_LOW_SIDE_W_PORT, M1_LOW_SIDE_W_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(M1_LOW_SIDE_U_PORT, M1_LOW_SIDE_U_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(M1_LOW_SIDE_V_PORT, M1_LOW_SIDE_V_PIN, GPIO_PIN_RESET);
}

/**
  * @brief  V相上桥臂导通，W相下桥臂导通
  * @param  无
  * @retval 无
  */
void m1_vhwl(void)
{
    g_atimx_handle.Instance->CCR1=0;
    g_atimx_handle.Instance->CCR2 = g_bldc_motor1.pwm_duty;
    g_atimx_handle.Instance->CCR3=0;
    
    HAL_GPIO_WritePin(M1_LOW_SIDE_W_PORT, M1_LOW_SIDE_W_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(M1_LOW_SIDE_U_PORT, M1_LOW_SIDE_U_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(M1_LOW_SIDE_V_PORT, M1_LOW_SIDE_V_PIN, GPIO_PIN_RESET);
}

/**
  * @brief  V相上桥臂导通，U相下桥臂导通
  * @param  无
  * @retval 无
  */
void m1_vhul(void)
{
    g_atimx_handle.Instance->CCR1 = 0;
    g_atimx_handle.Instance->CCR2 = g_bldc_motor1.pwm_duty;
    g_atimx_handle.Instance->CCR3 = 0;
    
    HAL_GPIO_WritePin(M1_LOW_SIDE_U_PORT, M1_LOW_SIDE_U_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(M1_LOW_SIDE_V_PORT, M1_LOW_SIDE_V_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(M1_LOW_SIDE_W_PORT, M1_LOW_SIDE_W_PIN, GPIO_PIN_RESET);
}

/**
  * @brief  W相上桥臂导通，U相下桥臂导通
  * @param  无
  * @retval 无
  */
void m1_whul(void)
{
    g_atimx_handle.Instance->CCR1 = 0;
    g_atimx_handle.Instance->CCR2 = 0;
    g_atimx_handle.Instance->CCR3 = g_bldc_motor1.pwm_duty;

    HAL_GPIO_WritePin(M1_LOW_SIDE_U_PORT, M1_LOW_SIDE_U_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(M1_LOW_SIDE_V_PORT, M1_LOW_SIDE_V_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(M1_LOW_SIDE_W_PORT, M1_LOW_SIDE_W_PIN, GPIO_PIN_RESET);
}

/**
  * @brief  W相上桥臂导通，V相下桥臂导通
  * @param  无
  * @retval 无
  */
void m1_whvl(void)
{
    g_atimx_handle.Instance->CCR1 = 0;
    g_atimx_handle.Instance->CCR2 = 0;
    g_atimx_handle.Instance->CCR3 = g_bldc_motor1.pwm_duty;

    HAL_GPIO_WritePin(M1_LOW_SIDE_V_PORT, M1_LOW_SIDE_V_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(M1_LOW_SIDE_U_PORT, M1_LOW_SIDE_U_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(M1_LOW_SIDE_W_PORT, M1_LOW_SIDE_W_PIN, GPIO_PIN_RESET);
}

/*************************** 上下桥臂的导通情况，共6种，也称为6步换向（接口二） ****************************/

pctr pfunclist_m2[6] =
{
    &m2_uhwl, &m2_vhul, &m2_vhwl,
    &m2_whvl, &m2_uhvl, &m2_whul
};

void m2_uhvl(void)
{
    g_atimx2_handle.Instance->CCR2 = 0;
    g_atimx2_handle.Instance->CCR1 = g_bldc_motor2.pwm_duty;/*U相上桥臂PWM*/
    g_atimx2_handle.Instance->CCR3 = 0;
    HAL_GPIO_WritePin(M2_LOW_SIDE_V_PORT,M2_LOW_SIDE_V_PIN,GPIO_PIN_SET);/*V相下桥臂导通*/
    HAL_GPIO_WritePin(M2_LOW_SIDE_U_PORT,M2_LOW_SIDE_U_PIN,GPIO_PIN_RESET);/*U相下桥臂关闭*/
    HAL_GPIO_WritePin(M2_LOW_SIDE_W_PORT,M2_LOW_SIDE_W_PIN,GPIO_PIN_RESET);/*W相下桥臂关闭*/
}

void m2_uhwl(void)
{
    g_atimx2_handle.Instance->CCR2 = 0;
    g_atimx2_handle.Instance->CCR1 = g_bldc_motor2.pwm_duty;
    g_atimx2_handle.Instance->CCR3 = 0;
    HAL_GPIO_WritePin(M2_LOW_SIDE_W_PORT,M2_LOW_SIDE_W_PIN,GPIO_PIN_SET);
    HAL_GPIO_WritePin(M2_LOW_SIDE_U_PORT,M2_LOW_SIDE_U_PIN,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(M2_LOW_SIDE_V_PORT,M2_LOW_SIDE_V_PIN,GPIO_PIN_RESET);
}

void m2_vhwl(void)
{
    g_atimx2_handle.Instance->CCR1=0;
    g_atimx2_handle.Instance->CCR2 = g_bldc_motor2.pwm_duty;
    g_atimx2_handle.Instance->CCR3=0;
    HAL_GPIO_WritePin(M2_LOW_SIDE_W_PORT,M2_LOW_SIDE_W_PIN,GPIO_PIN_SET);
    HAL_GPIO_WritePin(M2_LOW_SIDE_U_PORT,M2_LOW_SIDE_U_PIN,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(M2_LOW_SIDE_V_PORT,M2_LOW_SIDE_V_PIN,GPIO_PIN_RESET);
}

void m2_vhul(void)
{
    g_atimx2_handle.Instance->CCR1 = 0;
    g_atimx2_handle.Instance->CCR2 = g_bldc_motor2.pwm_duty;
    g_atimx2_handle.Instance->CCR3 = 0;
    HAL_GPIO_WritePin(M2_LOW_SIDE_U_PORT,M2_LOW_SIDE_U_PIN,GPIO_PIN_SET);
    HAL_GPIO_WritePin(M2_LOW_SIDE_V_PORT,M2_LOW_SIDE_V_PIN,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(M2_LOW_SIDE_W_PORT,M2_LOW_SIDE_W_PIN,GPIO_PIN_RESET);
}


void m2_whul(void)
{
    g_atimx2_handle.Instance->CCR2 = 0;
    g_atimx2_handle.Instance->CCR3 = g_bldc_motor2.pwm_duty;
    g_atimx2_handle.Instance->CCR1 = 0;
    HAL_GPIO_WritePin(M2_LOW_SIDE_U_PORT,M2_LOW_SIDE_U_PIN,GPIO_PIN_SET);
    HAL_GPIO_WritePin(M2_LOW_SIDE_V_PORT,M2_LOW_SIDE_V_PIN,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(M2_LOW_SIDE_W_PORT,M2_LOW_SIDE_W_PIN,GPIO_PIN_RESET);
}

void m2_whvl(void)
{
    g_atimx2_handle.Instance->CCR2 = 0;
    g_atimx2_handle.Instance->CCR3 = g_bldc_motor2.pwm_duty;
    g_atimx2_handle.Instance->CCR1 = 0;
    HAL_GPIO_WritePin(M2_LOW_SIDE_V_PORT,M2_LOW_SIDE_V_PIN,GPIO_PIN_SET);
    HAL_GPIO_WritePin(M2_LOW_SIDE_U_PORT,M2_LOW_SIDE_U_PIN,GPIO_PIN_RESET);
    HAL_GPIO_WritePin(M2_LOW_SIDE_W_PORT,M2_LOW_SIDE_W_PIN,GPIO_PIN_RESET);
}

