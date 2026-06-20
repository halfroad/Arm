#include "spdif.h"
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32H7开发板
//SPDIF 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2018/8/17
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
//********************************************************************************
//升级说明
//无
////////////////////////////////////////////////////////////////////////////////// 	 
SPDIFRX_HandleTypeDef SPDIFIN1_Handle;  //SPDIF IN1句柄
DMA_HandleTypeDef SPDIF_DTDMA_Handler;  //SPDIF音频数据DMA

spdif_rx_dev spdif_dev;				    //SPDIF控制结构体   

//初始化SPDIF
void SPDIF_RX_Init(void)
{
	spdif_dev.clock=200000000;                                      //设置SPDIF CLK的频率,为200Mhz,要支持192Khz采样率必须保证clock≥135.2Mhz
    SPDIFIN1_Handle.Instance=SPDIFRX;
    SPDIFIN1_Handle.Init.InputSelection=SPDIFRX_INPUT_IN1;              //SPDIF输入1
    SPDIFIN1_Handle.Init.Retries=SPDIFRX_MAXRETRIES_15;                 //同步阶段允许重试次数
    SPDIFIN1_Handle.Init.WaitForActivity=SPDIFRX_WAITFORACTIVITY_ON;    //等待同步
    SPDIFIN1_Handle.Init.ChannelSelection=SPDIFRX_CHANNEL_A;            //控制流从通道A获取通道状态
    SPDIFIN1_Handle.Init.DataFormat=SPDIFRX_DATAFORMAT_LSB;             //右对齐
    SPDIFIN1_Handle.Init.StereoMode=SPDIFRX_STEREOMODE_ENABLE;          //使能立体声模式
    SPDIFIN1_Handle.Init.PreambleTypeMask=SPDIFRX_PREAMBLETYPEMASK_OFF; //报头类型不复制到SPDIFRX_DR中
    SPDIFIN1_Handle.Init.ChannelStatusMask=SPDIFRX_CHANNELSTATUS_OFF;   //通道状态和用户位不复制到SPDIFRX_DR中
    SPDIFIN1_Handle.Init.ValidityBitMask=SPDIFRX_VALIDITYMASK_ON;      //有效性位不复制到SPDIFRX_DR中
    SPDIFIN1_Handle.Init.ParityErrorMask=SPDIFRX_PARITYERRORMASK_ON;   //奇偶校验错误位不复制到SPDIFRX_DR中
    HAL_SPDIFRX_Init(&SPDIFIN1_Handle);
    SPDIFIN1_Handle.Instance->CR|=SPDIFRX_CR_RXDMAEN;                   //SPDIF音频数据使用DMA来接收
    
    //使能SPDIF的上溢错误和奇偶校验错误  
    __HAL_SPDIFRX_ENABLE_IT(&SPDIFIN1_Handle,SPDIFRX_IT_IFEIE|SPDIFRX_IT_PERRIE); 
}

//SPDIF底层IO初始化和时钟使能
//此函数会被HAL_SPDIF_Init()调用
//hltdc:SPDIF句柄
void HAL_SPDIFRX_MspInit(SPDIFRX_HandleTypeDef *hspdif)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_SPDIFRX_CLK_ENABLE();             //使能SPDIF RX时钟
    __HAL_RCC_GPIOG_CLK_ENABLE();               //使能GPIOG时钟
 
    //初始化PG12，SPDIF IN引脚
    GPIO_Initure.Pin=GPIO_PIN_12;               //PG12，SPDIF IN引脚
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;          //复用
    GPIO_Initure.Pull=GPIO_NOPULL;              //无上下拉      
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH; //高速
    GPIO_Initure.Alternate=GPIO_AF8_SPDIF;      //复用为SPDIF RX
    HAL_GPIO_Init(GPIOG,&GPIO_Initure);
	
	HAL_NVIC_SetPriority(SPDIF_RX_IRQn,1,0);    //SPDIF中断
    HAL_NVIC_EnableIRQ(SPDIF_RX_IRQn); 
}

//设置SPDIF的工作模式
//mode:0,IDLE模式
//     1,RX同步模式
//     2,保留
//     3,正常工作模式
void SPDIF_RX_Mode(u8 mode)
{
    if(mode==0)
       __HAL_SPDIFRX_IDLE(&SPDIFIN1_Handle); 
    else if(mode==1)
        __HAL_SPDIFRX_SYNC(&SPDIFIN1_Handle);
    else if(mode==3)
        __HAL_SPDIFRX_RCV(&SPDIFIN1_Handle);
}

//SPDIF RX数据DMA配置
//设置为双缓冲模式,并开启DMA传输完成中断
//buf0:M0AR地址.
//buf1:M1AR地址.
//num:每次传输数据量
//width:位宽(存储器和外设,同时设置),0,8位;1,16位;2,32位;
void SPDIF_RXDATA_DMA_Init(u32* buf0,u32 *buf1,u16 num,u8 width)
{ 
    u32 memwidth=0,perwidth=0;      //外设和存储器位宽
    switch(width)
    {
        case 0:         //8位
            memwidth=DMA_MDATAALIGN_BYTE;
            perwidth=DMA_PDATAALIGN_BYTE;
            break;
        case 1:         //16位
            memwidth=DMA_MDATAALIGN_HALFWORD;
            perwidth=DMA_PDATAALIGN_HALFWORD;
            break;
        case 2:         //32位
            memwidth=DMA_MDATAALIGN_WORD;
            perwidth=DMA_PDATAALIGN_WORD;
            break;
            
    }
	__HAL_RCC_DMA1_CLK_ENABLE();               			 			//使能DMA1时钟
	__HAL_LINKDMA(&SPDIFIN1_Handle,hdmaDrRx,SPDIF_DTDMA_Handler);   //将DMA与SPDIF联系起来
    SPDIF_DTDMA_Handler.Instance=DMA1_Stream1;                       //DMA1数据流1
	SPDIF_DTDMA_Handler.Init.Request=DMA_REQUEST_SPDIF_RX_DT;	    //SAI1 Bock B	
    SPDIF_DTDMA_Handler.Init.Direction=DMA_PERIPH_TO_MEMORY;         //外设到存储器模式
    SPDIF_DTDMA_Handler.Init.PeriphInc=DMA_PINC_DISABLE;             //外设非增量模式
    SPDIF_DTDMA_Handler.Init.MemInc=DMA_MINC_ENABLE;                 //存储器增量模式
    SPDIF_DTDMA_Handler.Init.PeriphDataAlignment=perwidth;           //外设数据长度:16/32位
    SPDIF_DTDMA_Handler.Init.MemDataAlignment=memwidth;              //存储器数据长度:16/32位
    SPDIF_DTDMA_Handler.Init.Mode=DMA_CIRCULAR;                      //使用循环模式 
    SPDIF_DTDMA_Handler.Init.Priority=DMA_PRIORITY_MEDIUM;           //中等优先级
    SPDIF_DTDMA_Handler.Init.FIFOMode=DMA_FIFOMODE_DISABLE;          //不使用FIFO
    SPDIF_DTDMA_Handler.Init.MemBurst=DMA_MBURST_SINGLE;             //存储器单次突发传输
    SPDIF_DTDMA_Handler.Init.PeriphBurst=DMA_PBURST_SINGLE;          //外设突发单次传输 
    HAL_DMA_DeInit(&SPDIF_DTDMA_Handler);                            //先清除以前的设置
    HAL_DMA_Init(&SPDIF_DTDMA_Handler);	                            //初始化DMA
    
    HAL_DMAEx_MultiBufferStart(&SPDIF_DTDMA_Handler,(u32)&SPDIFRX->DR,(u32)buf0,(u32)buf1,num);//开启双缓冲
}
 
//等待进入同步状态,同步完成以后自动进入接收状态
//返回值:0,未同步;1,已同步
u8 SPDIF_RX_WaitSync(void)
{
    u8 res=0;
    u8 timeout=0;
	SPDIF_RX_Mode(SPDIF_RX_SYNC);	//设置为同步模式
	while(1)
	{
        timeout++;
        delay_ms(2);
		if(timeout>100)break;
        if(__HAL_SPDIFRX_GET_FLAG(&SPDIFIN1_Handle,SPDIFRX_FLAG_SYNCD))//同步完成?
		{
			res=1;				//标记同步完成
			SPDIF_RX_Mode(SPDIF_RX_RCV);//进入接收模式
			break;	  
		}
	}
	return res;
}

//获取SPDIF RX收到的音频采样率
//返回值:0,错误的采样率
//  其他值,音频采样率
u32 SPDIF_RX_GetSampleRate(void)
{
    u16 spdif_w5;
    u32 samplerate; 
    spdif_w5=SPDIFRX->SR>>16;
    samplerate=(spdif_dev.clock*5)/(spdif_w5&0X7FFF);
    samplerate>>=6;     									//除以64 
    if((8000-1500<=samplerate)&&(samplerate<=8000+1500))samplerate=8000;			//8K的采样率
    else if((11025-1500<=samplerate)&&(samplerate<=11025+1500))samplerate=11025;	//11.025K的采样率
    else if((16000-1500<=samplerate)&&(samplerate<=16000+1500))samplerate=16000;	//16K的采样率
    else if((22050-1500<=samplerate)&&(samplerate<=22050+1500))samplerate=22050;	//22.05K的采样率
    else if((32000-1500<=samplerate)&&(samplerate<=32000+1500))samplerate=32000;	//32K的采样率
    else if((44100-1500<=samplerate)&&(samplerate<=44100+1500))samplerate=44100;	//44.1K的采样率
    else if((48000-1500<=samplerate)&&(samplerate<=48000+1500))samplerate=48000;	//48K的采样率
    else if((88200-1500<=samplerate)&&(samplerate<=88200+1500))samplerate=88200;	//88.2K的采样率
    else if((96000-1500<=samplerate)&&(samplerate<=96000+1500))samplerate=96000;	//96K的采样率
    else if((176400-6000<=samplerate)&&(samplerate<=176400+6000))samplerate=176400;	//176.4K的采
    else if((192000-6000<=samplerate)&&(samplerate<=192000+6000))samplerate=192000;	//192K的采
    else samplerate=0; 
	return samplerate;
}

//SAI DMA回调函数指针
void (*spdif_rx_stop_callback)(void);	//回调函数 

//SPDIF接收中断服务函数
void SPDIF_RX_IRQHandler(void)
{
	//发生超时、同步和帧错误中断,这三个中断一定要处理！
    if( __HAL_SPDIFRX_GET_FLAG(&SPDIFIN1_Handle,SPDIFRX_FLAG_FERR)||\
        __HAL_SPDIFRX_GET_FLAG(&SPDIFIN1_Handle,SPDIFRX_FLAG_SERR)||\
        __HAL_SPDIFRX_GET_FLAG(&SPDIFIN1_Handle,SPDIFRX_FLAG_TERR))
    {  
        SPDIF_RX_Stop();				//发生错误，关闭SPDIF播放
		spdif_rx_stop_callback();		//调用回调函数
		SPDIF_RX_Mode(SPDIF_RX_IDLE);	//当发生超时、同步和帧错误的时候要将SPDIFRXEN写0来清除中断
	}
	if(__HAL_SPDIFRX_GET_FLAG(&SPDIFIN1_Handle,SPDIFRX_FLAG_OVR))     //上溢错误
    {
        __HAL_SPDIFRX_CLEAR_IT(&SPDIFIN1_Handle, SPDIFRX_FLAG_OVR);         //清除上溢错误中断
    }
    if(__HAL_SPDIFRX_GET_FLAG(&SPDIFIN1_Handle,SPDIFRX_FLAG_PERR))    //奇偶校验错误
    {
        __HAL_SPDIFRX_CLEAR_IT(&SPDIFIN1_Handle, SPDIFRX_FLAG_PERR);        //清除奇偶校验错误
    }
} 

//SPDIF开始播放
void SPDIF_RX_Start(void)
{ 
    spdif_dev.consta=1;				//标记已经打开SPDIF
    __HAL_DMA_ENABLE(&SPDIF_DTDMA_Handler);	//开启DMA TX传输  	    
}

//SPDIF关闭
void SPDIF_RX_Stop(void)
{
    spdif_dev.consta=0;				//标记已经关闭SPDIF 
	spdif_dev.saisync=0;			//清空同步状态 
    __HAL_DMA_DISABLE(&SPDIF_DTDMA_Handler);	//开启DMA TX传输  	    
} 











