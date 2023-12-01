/**
 ****************************************************************************************************
 * @file        main.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2023-08-01
 * @brief       BLDC��������(˫���) ʵ��
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
 ****************************************************************************************************
 */

#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/TIMER/bldc_tim.h"
#include "./BSP/KEY/key.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/BLDC/bldc.h"


int main(void)
{
    uint8_t key, t;
    char buf[32];
    int16_t pwm_duty_temp = 0;
    
    HAL_Init();                                 /* ��ʼ��HAL�� */
    sys_stm32_clock_init(85, 2, 2, 4, 8);       /* ����ʱ��,170Mhz */
    delay_init(170);                            /* ��ʱ��ʼ�� */
    usart_init(115200);                         /* ���ڳ�ʼ��Ϊ115200 */
    led_init();                                 /* ��ʼ��LED */
    key_init();                                 /* ��ʼ������ */
    lcd_init();                                 /* ��ʼ��LCD */
    bldc_init((170000 / 18) - 1, 0);            /* �ж�Ƶ��18K��Լ55us, Ƶ�ʲ��ܹ�������죬���򶼻ᵼ�µ����ת */
    bldc_ctrl(MOTOR_1, CW, 0);                  /* ��ʼ��ˢ����ӿ�1�ٶ� */
    bldc_ctrl(MOTOR_2, CW, 0);                  /* ��ʼ��ˢ����ӿ�2�ٶ� */
    
    /* ��ʾ��ʾ��Ϣ */    
    lcd_show_string(10, 10, 239, 24, 24, "BLDC Motor Test", RED);
    lcd_show_string(10, 40, 239, 24, 24, "KEY0:Start forward", RED);
    lcd_show_string(10, 70, 239, 24, 24, "KEY1:Start backward", RED);
    lcd_show_string(10, 100, 239, 24, 24, "KEY2:Stop", RED);
    printf("����KEY0 ��ʼ��ת����\r\n");
    printf("����KEY1 ��ʼ��ת����\r\n");
    printf("����KEY2 ֹͣ���\r\n");
    
    while (1)
    {
        t++;
        if(t % 20 == 0)
        {
            sprintf(buf,"PWM_Duty:%.1f%% ",(float)((g_bldc_motor1.pwm_duty / MAX_PWM_DUTY) * 100));  /* ��ʾ����PWMռ�ձ� */
            lcd_show_string(10, 130, 239, 24, 24, buf, BLUE);
            LED0_TOGGLE();                          /* LED0(���) ��ת */
        }

        key = key_scan(0);
        if(key == KEY0_PRES)                        /* ����KEY0���ñȽ�ֵ+500 */
        {
            pwm_duty_temp += 500;
            if(pwm_duty_temp >= MAX_PWM_DUTY / 2)   /* ���� */
                pwm_duty_temp = MAX_PWM_DUTY / 2;
            if(pwm_duty_temp > 0)                   /* ͨ���ж�������������ת���� */
            {
                g_bldc_motor1.pwm_duty = pwm_duty_temp;
                g_bldc_motor1.dir = CW;
                
                g_bldc_motor2.pwm_duty = pwm_duty_temp;
                g_bldc_motor2.dir = CW;
            }
            else
            {
                g_bldc_motor1.pwm_duty = -pwm_duty_temp;
                g_bldc_motor1.dir = CCW;
                
                g_bldc_motor2.pwm_duty = -pwm_duty_temp;
                g_bldc_motor2.dir = CCW;
            }
            g_bldc_motor1.run_flag = RUN;           /* �������б�־ */
            g_bldc_motor2.run_flag = RUN;           /* �������б�־ */
            start_motor1();                         /* �������� */
            start_motor2();                         /* �������� */
        }
        else if(key == KEY1_PRES)                   /* ����KEY1���ñȽ�ֵ-500 */
        {
            pwm_duty_temp -= 500;
            if(pwm_duty_temp <= -MAX_PWM_DUTY / 2)
                pwm_duty_temp = -MAX_PWM_DUTY / 2;
            if(pwm_duty_temp < 0)                   /* ͨ���ж�������������ת���� */
            {
                g_bldc_motor1.pwm_duty = -pwm_duty_temp;
                g_bldc_motor1.dir = CCW;
                
                g_bldc_motor2.pwm_duty = -pwm_duty_temp;
                g_bldc_motor2.dir = CCW;
                
            }
            else
            {
                g_bldc_motor1.pwm_duty = pwm_duty_temp;
                g_bldc_motor1.dir = CW;
                
                g_bldc_motor2.pwm_duty = pwm_duty_temp;
                g_bldc_motor2.dir = CW;
            }                                                  
            g_bldc_motor1.run_flag = RUN;           /* �������� */
            start_motor1();                         /* ���е�� */
            
            g_bldc_motor2.run_flag = RUN;          
            start_motor2(); 
        }
        else if(key == KEY2_PRES)                   /* ����KEY2�رյ�� */
        {
            stop_motor1();                          /* ͣ�� */
            g_bldc_motor1.run_flag = STOP;          /* ���ͣ�� */
            pwm_duty_temp = 0;                      /* ������0 */
            g_bldc_motor1.pwm_duty = 0;
            
            stop_motor2();                          /* ͣ�� */
            g_bldc_motor2.run_flag = STOP;          /* ���ͣ�� */
            g_bldc_motor2.pwm_duty = 0;
        }
        delay_ms(10);
    }
}




