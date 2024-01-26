/**
 ****************************************************************************************************
 * @file        main.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2023-08-01
 * @brief       ֱ����ˢ�������������· ʵ��
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
#include "./BSP/KEY/key.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/SPI/spi.h"
#include "./BSP/TIMER/dcmotor_tim.h"
#include "./BSP/DC_MOTOR/dc_motor.h"


int main(void)
{
    uint8_t key,t;
    int32_t motor_pwm = 0;
    
    HAL_Init();                                 /* ��ʼ��HAL�� */
    sys_stm32_clock_init(85, 2, 2, 4, 8);       /* ����ʱ��,170Mhz */
    delay_init(170);                            /* ��ʱ��ʼ�� */
    usart_init(115200);                         /* ���ڳ�ʼ��Ϊ115200 */
    led_init();                                 /* ��ʼ��LED */
    lcd_init();                                 /* ��ʼ��LCD */
    key_init();                                 /* ��ʼ������ */
    //atim_timx1_cplm_pwm_init(8500 - 1, 0);      /* 170 000 000 / 1 = 170 000 000 170Mhz�ļ���Ƶ�ʣ�����8500��Ϊ50us */
    atim_timx2_cplm_pwm_init(8500 - 1, 0);      /* 170 000 000 / 1 = 170 000 000 170Mhz�ļ���Ƶ�ʣ�����8500��Ϊ50us */
    //dcmotor_init();                             /* ��ʼ����� */
    
    lcd_show_string(10, 10, 200, 24, 24, "DcMotor Test", RED);
    lcd_show_string(10, 40, 200, 24, 24, "KEY0:Forward", RED);
    lcd_show_string(10, 70, 200, 24, 24, "KEY1:Backward", RED);
    lcd_show_string(10, 100, 200, 24, 24, "KEY2:Stop", RED);

    printf("KEY0�����ӱȽ�ֵ��KEY1����С�Ƚ�ֵ��KEY2��ֹͣ���\r\n");
    
    while (1)
    {
        key = key_scan(0);                       /* ����ɨ�� */
        if(key == KEY0_PRES)                     /* ��key0���� */
        {
            motor_pwm += 400;                    /* ��Ϊ��ͬ�ĵ����С������ѹ��ͬ�������ڵ�һ�����ӵ�ʱ����������ת���� */
            if (motor_pwm == 0) 
            {
                dcmotor_stop(MOTOR_1);           /* ֹͣ��������Ӧ */
                dcmotor_stop(MOTOR_2);
                motor_pwm = 0;
            } 
            else 
            {
                dcmotor_start(MOTOR_1);          /* ������� */
                dcmotor_start(MOTOR_2);
                if (motor_pwm >= 8200)           /* ���� */
                {
                    motor_pwm = 8200;
                }               
            }
            motor_pwm_set(motor_pwm,MOTOR_1);    /* ���õ������ת�� */
            motor_pwm_set(motor_pwm,MOTOR_2);
        }
        
        else if(key == KEY1_PRES)                /* ��key1���� */
        {
            motor_pwm -= 400;
            if (motor_pwm == 0) 
            {
                dcmotor_stop(MOTOR_1);           /* ֹͣ��������Ӧ */
                dcmotor_stop(MOTOR_2);
                motor_pwm = 0;
            } 
            else 
            {
                dcmotor_start(MOTOR_1);          /* ������� */
                dcmotor_start(MOTOR_2);
                if (motor_pwm <= -8200)          /* ���� */
                {
                    motor_pwm = -8200;
                }               
            }
            motor_pwm_set(motor_pwm,MOTOR_1);    /* ���õ������ת�� */
            motor_pwm_set(motor_pwm,MOTOR_2);
        }
        
        else if(key == KEY2_PRES)                /* ��key2���� */
        {
            LED1_TOGGLE();
            dcmotor_stop(MOTOR_1);               /* �رյ�� */
            dcmotor_stop(MOTOR_2);
            motor_pwm = 0;
            motor_pwm_set(motor_pwm,MOTOR_1);    /* ���õ������ת�� */
            motor_pwm_set(motor_pwm,MOTOR_2);
        }
        
        delay_ms(10);
        t++;
        if(t % 20 == 0)
        {
            LED0_TOGGLE();                       /*LED0(���) ��ת*/
        }
    }
}




