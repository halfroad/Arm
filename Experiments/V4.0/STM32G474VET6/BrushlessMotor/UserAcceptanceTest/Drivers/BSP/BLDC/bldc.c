/**
 ****************************************************************************************************
 * @file        bldc.c
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
 
#include "./BSP/BLDC/bldc.h"
#include "./BSP/TIMER/bldc_tim.h"

_bldc_obj g_bldc_motor1 = {STOP, 0, 0, CCW, 0, 0, 0, 0, 0, 0};  /* ����ṹ���ʼֵ */
_bldc_obj g_bldc_motor2 = {STOP,0,0,CCW,0,0,0,0,0,0};   /* ����ṹ�� */

/**
 * @brief       ��ˢ�����ʼ����������ʱ���������ӿ��Լ�SD���ų�ʼ��
 * @param       arr: �Զ���װֵ
 * @param       psc: ʱ��Ԥ��Ƶ��
 * @retval      ��
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
    
    hall_gpio_init();                       /* �����ӿڳ�ʼ�� */
    atim_timx_oc_chy_init(arr, psc);        /* �ӿ�һ��ʱ����ʼ�� */
    atim_timx2_oc_chy_init(arr,  psc);      /* �ӿڶ���ʱ����ʼ�� */    
}

/**
 * @brief       BLDC���ƺ���
 * @param       dir :�������, Duty:PWMռ�ձ�
 * @retval      ��
 */
void bldc_ctrl(uint8_t motor_id,int32_t dir,float duty)
{
    if(motor_id == MOTOR_1)
    {
        g_bldc_motor1.dir = dir;            /* ���� */
        g_bldc_motor1.pwm_duty = duty;      /* ռ�ձ� */
    }
    if(motor_id == MOTOR_2)
    {
        g_bldc_motor2.dir = dir;            /* ���� */
        g_bldc_motor2.pwm_duty = duty;      /* ռ�ձ� */
    }
}

/************************************ �����ӿڳ�ʼ�� ******************************************/

/**
  * @brief  �����������ӿڳ�ʼ��
  * @param  ��
  * @retval ��
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
    /* �����ӿ�һ ���ų�ʼ�� */
    gpio_init_struct.Pin = HALL1_TIM_CH1_PIN;
    gpio_init_struct.Mode = GPIO_MODE_INPUT;
    gpio_init_struct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(HALL1_TIM_CH1_GPIO, &gpio_init_struct);

    gpio_init_struct.Pin = HALL1_TIM_CH2_PIN;
    HAL_GPIO_Init(HALL1_TIM_CH2_GPIO, &gpio_init_struct);

    gpio_init_struct.Pin = HALL1_TIM_CH3_PIN;
    HAL_GPIO_Init(HALL1_TIM_CH3_GPIO, &gpio_init_struct);

    /* �����ӿڶ� ���ų�ʼ�� */
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
 * @brief       ��ȡ��������������״̬
 * @param       motor_id �� ����ӿں�
 * @retval      ��������������״̬
 */
uint32_t hallsensor_get_state(uint8_t motor_id)
{
    uint32_t state = 0;
    
    if(motor_id == MOTOR_1)
    {
        if(HAL_GPIO_ReadPin(HALL1_TIM_CH1_GPIO, HALL1_TIM_CH1_PIN) != GPIO_PIN_RESET)  /* ����������״̬��ȡ */
        {
            state |= 0x01U;
        }
        if(HAL_GPIO_ReadPin(HALL1_TIM_CH2_GPIO, HALL1_TIM_CH2_PIN) != GPIO_PIN_RESET)  /* ����������״̬��ȡ */
        {
            state |= 0x02U;
        }
        if(HAL_GPIO_ReadPin(HALL1_TIM_CH3_GPIO, HALL1_TIM_CH3_PIN) != GPIO_PIN_RESET)  /* ����������״̬��ȡ */
        {
            state |= 0x04U;
        }
    }
    else if(motor_id == MOTOR_2)
    {
        if(HAL_GPIO_ReadPin(HALL2_TIM_CH1_GPIO, HALL2_TIM_CH1_PIN) != GPIO_PIN_RESET)  /* ����������״̬��ȡ */
        {
            state |= 0x01U;
        }
        if(HAL_GPIO_ReadPin(HALL2_TIM_CH2_GPIO, HALL2_TIM_CH2_PIN) != GPIO_PIN_RESET)  /* ����������״̬��ȡ */
        {
            state |= 0x02U;
        }
        if(HAL_GPIO_ReadPin(HALL2_TIM_CH3_GPIO, HALL2_TIM_CH3_PIN) != GPIO_PIN_RESET)  /* ����������״̬��ȡ */
        {
            state |= 0x04U;
        }
    }
    return state;
}

/******************************************** �ӿ�һ��ͣ���� *********************************************/

/**
  * @brief  �رյ����ת
  * @param  ��
  * @retval ��
  */
void stop_motor1(void)
{
    /* �رհ���оƬ��� */
    SHUTDOWN_OFF;
    /* �ر�PWM��� */
    HAL_TIM_PWM_Stop(&g_atimx_handle, TIM_CHANNEL_1);
    HAL_TIM_PWM_Stop(&g_atimx_handle, TIM_CHANNEL_2);
    HAL_TIM_PWM_Stop(&g_atimx_handle, TIM_CHANNEL_3);
    /* �����ű�ȫ���ض� */
    g_atimx_handle.Instance->CCR2 = 0;
    g_atimx_handle.Instance->CCR1 = 0;
    g_atimx_handle.Instance->CCR3 = 0;
    HAL_GPIO_WritePin(M1_LOW_SIDE_U_PORT, M1_LOW_SIDE_U_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(M1_LOW_SIDE_V_PORT, M1_LOW_SIDE_V_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(M1_LOW_SIDE_W_PORT, M1_LOW_SIDE_W_PIN, GPIO_PIN_RESET);
}

/**
  * @brief  ���������ת
  * @param  ��
  * @retval ��
  */
void start_motor1(void)
{
    SHUTDOWN_EN;
    /* ʹ��PWM��� */
    HAL_TIM_PWM_Start(&g_atimx_handle, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&g_atimx_handle, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&g_atimx_handle, TIM_CHANNEL_3);
}

/******************************************** �ӿڶ���ͣ���� *********************************************/

/**
  * @brief  �رյ����ת
  * @param  ��
  * @retval ��
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
  * @brief  ���������ת
  * @param  ��
  * @retval ��
  */
void start_motor2(void)
{
    SHUTDOWN2_EN;
    HAL_TIM_PWM_Start(&g_atimx2_handle, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&g_atimx2_handle, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&g_atimx2_handle, TIM_CHANNEL_3);
}

/*************************** �����ű۵ĵ�ͨ�������6�֣�Ҳ��Ϊ6�����򣨽ӿ�һ�� ****************************/

/*  ����������ָ������ */
pctr pfunclist_m1[6] =
{
    &m1_uhwl, &m1_vhul, &m1_vhwl,
    &m1_whvl, &m1_uhvl, &m1_whul
};

/**
  * @brief  U�����ű۵�ͨ��V�����ű۵�ͨ
  * @param  ��
  * @retval ��
  */
void m1_uhvl(void)
{
    g_atimx_handle.Instance->CCR1 = g_bldc_motor1.pwm_duty;                     /* U�����ű�PWM */
    g_atimx_handle.Instance->CCR2 = 0;
    g_atimx_handle.Instance->CCR3 = 0;
    
    HAL_GPIO_WritePin(M1_LOW_SIDE_V_PORT, M1_LOW_SIDE_V_PIN, GPIO_PIN_SET);     /* V�����ű۵�ͨ */
    HAL_GPIO_WritePin(M1_LOW_SIDE_U_PORT, M1_LOW_SIDE_U_PIN, GPIO_PIN_RESET);   /* U�����ű۹ر� */
    HAL_GPIO_WritePin(M1_LOW_SIDE_W_PORT, M1_LOW_SIDE_W_PIN, GPIO_PIN_RESET);   /* W�����ű۹ر� */
}

/**
  * @brief  U�����ű۵�ͨ��W�����ű۵�ͨ
  * @param  ��
  * @retval ��
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
  * @brief  V�����ű۵�ͨ��W�����ű۵�ͨ
  * @param  ��
  * @retval ��
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
  * @brief  V�����ű۵�ͨ��U�����ű۵�ͨ
  * @param  ��
  * @retval ��
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
  * @brief  W�����ű۵�ͨ��U�����ű۵�ͨ
  * @param  ��
  * @retval ��
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
  * @brief  W�����ű۵�ͨ��V�����ű۵�ͨ
  * @param  ��
  * @retval ��
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

/*************************** �����ű۵ĵ�ͨ�������6�֣�Ҳ��Ϊ6�����򣨽ӿڶ��� ****************************/

pctr pfunclist_m2[6] =
{
    &m2_uhwl, &m2_vhul, &m2_vhwl,
    &m2_whvl, &m2_uhvl, &m2_whul
};

void m2_uhvl(void)
{
    g_atimx2_handle.Instance->CCR2 = 0;
    g_atimx2_handle.Instance->CCR1 = g_bldc_motor2.pwm_duty;/*U�����ű�PWM*/
    g_atimx2_handle.Instance->CCR3 = 0;
    HAL_GPIO_WritePin(M2_LOW_SIDE_V_PORT,M2_LOW_SIDE_V_PIN,GPIO_PIN_SET);/*V�����ű۵�ͨ*/
    HAL_GPIO_WritePin(M2_LOW_SIDE_U_PORT,M2_LOW_SIDE_U_PIN,GPIO_PIN_RESET);/*U�����ű۹ر�*/
    HAL_GPIO_WritePin(M2_LOW_SIDE_W_PORT,M2_LOW_SIDE_W_PIN,GPIO_PIN_RESET);/*W�����ű۹ر�*/
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

