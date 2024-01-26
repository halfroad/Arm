/**
 ****************************************************************************************************
 * @file        main.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2023-08-01
 * @brief       直流有刷电机基础驱动 实验
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
//#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/KEY/key.h"
//#include "./BSP/LCD/lcd.h"
#include "./BSP/SPI/spi.h"
#include "./BSP/TIMER/dcmotor_tim.h"
#include "./BSP/DC_MOTOR/dc_motor.h"


int main(void)
{
    uint8_t key,t;
    int32_t motor_pwm = 0;
    
    HAL_Init();                                 /* 初始化HAL库 */
    sys_stm32_clock_init(85, 2, 2, 4, 8);       /* 设置时钟,170Mhz */
    delay_init(170);                            /* 延时初始化 */
   // usart_init(115200);                         /* 串口初始化为115200 */
    led_init();                                 /* 初始化LED */
    key_init();                                 /* 初始化按键 */
    atim_timx_cplm_pwm_init(8500 - 1, 0);       /* 170 000 000 / 1 = 170 000 000 170Mhz的计数频率，计数8500次为50us */
    dcmotor_init();                             /* 初始化电机 */
 
    while (1)
    {
        key = key_scan(0);                      /* 按键扫描 */
        if(key == KEY0_PRES)                    /* 当key0按下 */
        {
            motor_pwm += 400;                   /* 因为不同的电机最小启动电压不同，可能在第一次增加的时候电机还不能转起来 */
            
            if (motor_pwm == 0) 
            {
                dcmotor_stop();                 /* 停止则立刻响应 */
                motor_pwm = 0;
            } 
            else 
            {
                dcmotor_start();                /* 开启电机 */
                if (motor_pwm >= 8200)          /* 限速 */
                {
                    motor_pwm = 8200;
                }               
            }
            motor_pwm_set(motor_pwm);           /* 设置电机PWM的占空比 */
        }
        
        else if(key == KEY1_PRES)               /* 当key1按下 */
        {
            motor_pwm -= 400;
            if (motor_pwm == 0) 
            {
                dcmotor_stop();                 /* 停止则立刻响应 */
                motor_pwm = 0;
            } 
            else 
            {
                dcmotor_start();                /* 开启电机 */
                if (motor_pwm <= -8200)         /* 限速 */
                {
                    motor_pwm = -8200;
                }               
            }
            motor_pwm_set(motor_pwm);           /* 设置电机PWM的占空比 */
        }
        
        else if(key == KEY2_PRES)               /* 当key2按下 */
        {
            LED1_TOGGLE();
            dcmotor_stop();                     /* 关闭电机 */
            motor_pwm = 0;
            motor_pwm_set(motor_pwm);           /* 设置电机PWM的占空比 */
        }
        
        delay_ms(10);
        t++;
        if(t % 20 == 0)
        {
            LED0_TOGGLE();                      /*LED0(红灯) 翻转*/
        }
    }
}




