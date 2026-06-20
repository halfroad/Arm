#include "pwmdac.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F7开发板
//PWM DAC驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2015/12/28
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

TIM_HandleTypeDef TIM15_Handler;            //定时器15 PWM句柄 
TIM_OC_InitTypeDef TIM15_CH2Handler;	    //定时器15通道4句柄

//PWM DAC初始化(也就是TIM2通道4初始化)
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM15_CH2_PWM_Init(u16 arr,u16 psc)
{ 
    TIM15_Handler.Instance=TIM15;           //定时器15
    TIM15_Handler.Init.Prescaler=psc;       //定时器分频
    TIM15_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;//向上计数模式
    TIM15_Handler.Init.Period=arr;          //自动重装载值
    TIM15_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&TIM15_Handler);       //初始化PWM
    
    TIM15_CH2Handler.OCMode=TIM_OCMODE_PWM1; //模式选择PWM1
    TIM15_CH2Handler.Pulse=arr/2;            //设置比较值,此值用来确定占空比，
                                            //默认比较值为自动重装载值的一半,即占空比为50%
    TIM15_CH2Handler.OCPolarity=TIM_OCPOLARITY_HIGH; //输出比较极性为高
    HAL_TIM_PWM_ConfigChannel(&TIM15_Handler,&TIM15_CH2Handler,TIM_CHANNEL_2);//配置TIM15通道2
    HAL_TIM_PWM_Start(&TIM15_Handler,TIM_CHANNEL_2);//开启PWM通道2
}

//定时器底层驱动，时钟使能，引脚配置
//此函数会被HAL_TIM_PWM_Init()调用
//htim:定时器句柄
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
    GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_TIM15_CLK_ENABLE();			//使能定时器15
    __HAL_RCC_GPIOA_CLK_ENABLE();			//开启GPIOA时钟
	
    GPIO_Initure.Pin=GPIO_PIN_3;           	//PA3
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;  	//复用推完输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;     //高速
	GPIO_Initure.Alternate= GPIO_AF4_TIM15;	//PA3复用为TIM15_CH2
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
}

//设置TIM通道4的占空比
//TIM_TypeDef:定时器
//compare:比较值
void TIM_SetTIM15Compare2(u32 compare)
{
	TIM15->CCR2=compare;
}

