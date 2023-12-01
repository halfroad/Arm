/**
 ****************************************************************************************************
 * @file        main.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2023-08-01
 * @brief       BLDC基本驱动(双电机) 实验
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
    
    HAL_Init();                                 /* 初始化HAL库 */
    sys_stm32_clock_init(85, 2, 2, 4, 8);       /* 设置时钟,170Mhz */
    delay_init(170);                            /* 延时初始化 */
    usart_init(115200);                         /* 串口初始化为115200 */
    led_init();                                 /* 初始化LED */
    key_init();                                 /* 初始化按键 */
    lcd_init();                                 /* 初始化LCD */
    bldc_init((170000 / 18) - 1, 0);            /* 中断频率18K，约55us, 频率不能过慢或过快，否则都会导致电机堵转 */
    bldc_ctrl(MOTOR_1, CW, 0);                  /* 初始无刷电机接口1速度 */
    bldc_ctrl(MOTOR_2, CW, 0);                  /* 初始无刷电机接口2速度 */
    
    /* 显示提示信息 */    
    lcd_show_string(10, 10, 239, 24, 24, "BLDC Motor Test", RED);
    lcd_show_string(10, 40, 239, 24, 24, "KEY0:Start forward", RED);
    lcd_show_string(10, 70, 239, 24, 24, "KEY1:Start backward", RED);
    lcd_show_string(10, 100, 239, 24, 24, "KEY2:Stop", RED);
    printf("按下KEY0 开始正转加速\r\n");
    printf("按下KEY1 开始反转加速\r\n");
    printf("按下KEY2 停止电机\r\n");
    
    while (1)
    {
        t++;
        if(t % 20 == 0)
        {
            sprintf(buf,"PWM_Duty:%.1f%% ",(float)((g_bldc_motor1.pwm_duty / MAX_PWM_DUTY) * 100));  /* 显示控制PWM占空比 */
            lcd_show_string(10, 130, 239, 24, 24, buf, BLUE);
            LED0_TOGGLE();                          /* LED0(红灯) 翻转 */
        }

        key = key_scan(0);
        if(key == KEY0_PRES)                        /* 按下KEY0设置比较值+500 */
        {
            pwm_duty_temp += 500;
            if(pwm_duty_temp >= MAX_PWM_DUTY / 2)   /* 限速 */
                pwm_duty_temp = MAX_PWM_DUTY / 2;
            if(pwm_duty_temp > 0)                   /* 通过判断正负号设置旋转方向 */
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
            g_bldc_motor1.run_flag = RUN;           /* 开启运行标志 */
            g_bldc_motor2.run_flag = RUN;           /* 开启运行标志 */
            start_motor1();                         /* 开启运行 */
            start_motor2();                         /* 开启运行 */
        }
        else if(key == KEY1_PRES)                   /* 按下KEY1设置比较值-500 */
        {
            pwm_duty_temp -= 500;
            if(pwm_duty_temp <= -MAX_PWM_DUTY / 2)
                pwm_duty_temp = -MAX_PWM_DUTY / 2;
            if(pwm_duty_temp < 0)                   /* 通过判断正负号设置旋转方向 */
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
            g_bldc_motor1.run_flag = RUN;           /* 开启运行 */
            start_motor1();                         /* 运行电机 */
            
            g_bldc_motor2.run_flag = RUN;          
            start_motor2(); 
        }
        else if(key == KEY2_PRES)                   /* 按下KEY2关闭电机 */
        {
            stop_motor1();                          /* 停机 */
            g_bldc_motor1.run_flag = STOP;          /* 标记停机 */
            pwm_duty_temp = 0;                      /* 数据清0 */
            g_bldc_motor1.pwm_duty = 0;
            
            stop_motor2();                          /* 停机 */
            g_bldc_motor2.run_flag = STOP;          /* 标记停机 */
            g_bldc_motor2.pwm_duty = 0;
        }
        delay_ms(10);
    }
}




