/**
 ***************************************************************************************************
 * 实验简介
 * 实验名称：BLDC基本驱动(双电机) 实验
 * 实验平台：正点原子 STM32G474开发板
 * 实验目的：学习 BLDC基础驱动

 ***************************************************************************************************
 * 硬件资源及引脚分配
 * 1 LED灯
     DS0（RED）     : LED0 - PE0
     DS1（GREEN）   : LED1 - PE1
 * 2 独立按键
     KEY0    - PE12
     KEY1    - PE13
     KEY2    - PE14
 * 3 无刷/有刷接口一和接口二分别对应：
     定时器1：  TIM1_CH1： PA8，  对应无刷电机U相
                TIM1_CH2： PA9，  对应无刷电机V相
                TIM1_CH3： PA10， 对应无刷电机W相
                TIM1_CHN1：PB13， 对应无刷电机U_LOW相
                TIM1_CHN2：PB14， 对应无刷电机V_LOW相
                TIM1_CHN3：PB15， 对应无刷电机W_LOW相
     定时器8：  TIM8_CH1： PC6，  对应无刷电机U相
                TIM8_CH2： PC7，  对应无刷电机V相
                TIM8_CH3： PC8，  对应无刷电机W相
                TIM8_CHN1：PC10， 对应无刷电机U_LOW相
                TIM8_CHN2：PC11， 对应无刷电机V_LOW相
                TIM8_CHN3：PC12， 对应无刷电机W_LOW相
 * 4 霍尔引脚： HALL1: PA0 PA1 PA2(分别对应：U V W)
                HALL2: PD3 PD4 PD7(分别对应：U V W)
 * 5 停止引脚： SHUTDOWN1: PC13
                SHUTDOWN2: PD11
 * 6 无刷电机 * 2
 * 7 无刷电机驱动器 * 2
 * 8 12~60V DC电源
 
 ***************************************************************************************************
 * 实验现象
 * 1 LED0用来指示程序运行,200ms变更一次状态。
 * 2 接入对应的无刷接口一或者接口二，按键KEY0开启电机正转继续按则加速，KEY1开启电机反转继续按则加速，KEY2关闭电机旋转°

 ***************************************************************************************************
 * 注意事项
 * 1 注意观察无刷驱动板是否亮红灯。亮代表过流，需要检查硬件连接，注意不可快速反转
 * 2 需要将驱动板的JP3跳线帽连接Z&H和HALL端

 ***********************************************************************************************************
 * 公司名称：广州市星翼电子科技有限公司（正点原子）
 * 电话号码：020-38271790
 * 传真号码：020-36773971
 * 公司网址：www.alientek.com
 * 购买地址：zhengdianyuanzi.tmall.com
 * 技术论坛：http://www.openedv.com/forum.php
 * 最新资料：www.openedv.com/docs/index.html
 *
 * 在线视频：www.yuanzige.com
 * B 站视频：space.bilibili.com/394620890
 * 公 众 号：mp.weixin.qq.com/s/y--mG3qQT8gop0VRuER9bw
 * 抖    音：douyin.com/user/MS4wLjABAAAAi5E95JUBpqsW5kgMEaagtIITIl15hAJvMO8vQMV1tT6PEsw-V5HbkNLlLMkFf1Bd
 ***********************************************************************************************************
 */