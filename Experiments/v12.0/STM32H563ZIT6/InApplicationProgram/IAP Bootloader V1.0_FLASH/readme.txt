实验器材:
	北极星STM32H750/F750开发板 

实验目的:
	学习STM32的IAP功能,掌握IAP的使用.
	
硬件资源:
	1,DS0(连接在PF9) 
	2,串口1(波特率:115200,PA9/PA10连接在板载USB转串口芯片CH340上面)
	3,ALIENTEK 2.8/3.5/4.3/7寸LCD模块(包括MCU屏和RGB屏,都支持) 
	4,按键KEY0(PH3)/KEY1(PH2)/KEY2(PC13)/KEY_UP(PA0,也称之为WK_UP)
	
	
实验现象:
	IAP部分,包括两份代码:
	IAP Bootloader V1.0_FLASH, 用于下载到目标开发板,执行IAP BootLoader.
	IAP Bootloader V1.0_ITCM, 用于生成具体的IAP Bootloader动作,实现IAP过程.
	
	IAP Bootloader V1.0_ITCM工程生成.bin文件需要通过ATK-C2B软件转成数组形式,然后把数组拷贝到IAP 
	Bootloader V1.0_FLASH的bl_itcm.c,替换里面的acApp2数组内容,然后编译IAP Bootloader V1.0_FLASH,
	并下载到STM32开发板上.
	
	IAP Bootloader V1.0_FLASH工程实际上只是完成了将IAP Bootloader V1.0_ITCM拷贝到ITCM并运行的工作.
	IAP Bootloader V1.0_ITCM工程生成的BIN文件转成数组后由IAP Bootloader V1.0_FLASH工程存储到外部
	QSPI FLASH.
	
	本实验开机的时候先显示提示信息，然后等待串口输入接收APP程序（无校验，一次性接收），在串口接收
	到APP程序之后，即可执行IAP。	
	
	1,对于SRAM APP，通过按下KEY0即可执行这个收到的SRAM APP程序。

	2,对于FLASH APP，则需要先按下KEY_UP按键，将串口接收到的APP程序存放到STM32的FLASH，之后再按KEY1即
	可以执行这个FLASH APP程序。
	
	3,对于FLASH&QSPI APP,则会生成两个.bin文件(ER_m_stmflash和ER_m_qspiflash),先接收ER_m_stmflash固
	件,按KEY_UP按键，将串口接收到的ER_m_stmflash存放到STM32的内部FLASH,然后再接收ER_m_qspiflash,按KEY2
	将串口接收到的ER_m_qspiflash存放到外部的QSPI FLASH(W25Q64),完成以上步骤后,按KEY1按键,可以执行这个
	FLASH&QSPI APP程序.
	
注意事项:
	1,4.3寸和7寸屏需要比较大电流,USB供电可能不足,请用外部电源适配器(推荐外接12V 1A电源).
	2,本例程支持除ALIENTEK V1版本7寸屏以外的其他所有LCD屏模块（包括RGB屏和MCU屏）.
	3,本实验下载成功后,需用串口调试助手发送SRAM APP/FLASH APP等APP代码(.bin文件)验证IAP功能.
	4,SRAM APP代码的起始地址必须是：0X24001000，FLASH APP代码的起始地址必须是：0x08020000。
	5,在创建APP代码的时候，切记要设置中断向量偏移量！！
	  
	 

					正点原子@ALIENTEK
					2019-5-17
					广州市星翼电子科技有限公司
					电话：020-38271790
					传真：020-36773971
					购买：http://shop62103354.taobao.com
					http://shop62057469.taobao.com
					公司网站：www.alientek.com
					技术论坛：www.openedv.com