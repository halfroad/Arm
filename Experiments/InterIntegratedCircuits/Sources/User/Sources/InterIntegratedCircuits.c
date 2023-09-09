#include "stm32f4xx_i2c.h"
#include "stm32f4xx_gpio.h"

#include "InterIntegratedCircuits.h"
#include "Timer.h"

#define SCL_GPIO_PIN		GPIO_Pin_8	/* I2C1_SCL: PB6 */
#define SDA_GPIO_PIN		GPIO_Pin_9	/* I2C_SDA: PB7 */

#define SET_SCL_HIGH()		GPIO_SetBits(GPIOB, SCL_GPIO_PIN)
#define SET_SCL_LOW()		GPIO_ResetBits(GPIOB, SCL_GPIO_PIN)

#define SET_SDA_HIGH()		GPIO_SetBits(GPIOB, SDA_GPIO_PIN)
#define SET_SDA_LOW()		GPIO_ResetBits(GPIOB, SDA_GPIO_PIN)

#define READ_SDA_INPUT()	GPIO_ReadInputDataBit(GPIOB, SDA_GPIO_PIN)

#define DELAY()				delay_us(5);

/*

Initialize the I2C settings, set the SCL and SDA to high.
*/
void i2c_init(void)
{
	/* Enable RCC clock for GPIOB.
	
	https://blog.51cto.com/weidongshan/6609372,
	https://blog.csdn.net/lnfiniteloop/article/details/121951590
	https://www.cnblogs.com/blog-xh/p/17184510.html
	
	*/
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
}

/*

Configure I2C SDA to input.

*/
void set_sda_in(void)
{
	GPIO_InitTypeDef gpio_init_struct;
	
	gpio_init_struct.GPIO_Pin = SDA_GPIO_PIN;
	gpio_init_struct.GPIO_Mode = GPIO_Mode_IN;
	
	GPIO_Init(GPIOB, &gpio_init_struct);
}

/*

Configure I2C SDA to ouput.

*/
void set_sda_out(void)
{
	GPIO_InitTypeDef gpio_init_struct;
	
	gpio_init_struct.GPIO_Pin = SDA_GPIO_PIN;
	gpio_init_struct.GPIO_Mode = GPIO_Mode_OUT;
	gpio_init_struct.GPIO_OType = GPIO_OType_PP;
	gpio_init_struct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	gpio_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB, &gpio_init_struct);
}

void i2c_start(void)
{
	set_sda_out();		/* Set mode to OUT. */
	
	SET_SCL_HIGH();		/* SCL High */
	DELAY();			/* Wait for a while 5 us. */
	
	SET_SDA_HIGH();		/* SDA High */
	DELAY();			/* Wait for a while 5 us. */
	
	SET_SDA_LOW();		/* Start Bit, Falling edge. */
	DELAY();			/* Wait for a while 5 us. */
	
	SET_SCL_LOW();
	DELAY();
}

void i2c_stop(void)
{
	set_sda_out();
	
	SET_SDA_LOW();
	DELAY();
	
	SET_SCL_HIGH();
	DELAY();
	
	SET_SDA_HIGH();
	DELAY();
}

/*

Master acknowledges the slave (, and then the Slave reads the acknowledgement).

*/
void acknowledge(uint8_t acknowldgement)
{
	set_sda_out();
	
	SET_SCL_LOW();	/* The SDA will be set in this moment. */
	DELAY();
	
	if (ACKNOWLEDGEMENT == acknowldgement)
		SET_SDA_LOW();	/* ACK (0)*/
	else
		SET_SDA_HIGH();	/* NO ACK (1)*/
	
	DELAY();
	
	SET_SCL_HIGH();
	DELAY();
	
	SET_SCL_LOW();
	DELAY();
}

/*

Master acquires acknowledgement from slave (Slave acknowledges the master prior to the acquisition.).

*/

uint8_t acquire_acknowledgement(void)
{	
	SET_SCL_LOW();
	DELAY();
	
	set_sda_in();
	
	SET_SCL_HIGH();
	DELAY();
	
	uint8_t acknowledgement = 0;
	uint8_t time = 0;
		
	while (READ_SDA_INPUT())
	{
		time ++;
		
		if (time > 250)
		{
			SET_SCL_LOW();
			
			acknowledgement = 1;
			
			break;
		}
	}
	
	SET_SCL_LOW();
	DELAY();
	
	return acknowledgement;
}

uint8_t i2c_send(uint8_t byte)
{
	set_sda_out();
	
	for (uint8_t i = 0; i < 8; i ++)
	{
		SET_SCL_LOW();
		DELAY();
		
		/* 0b1000 0000. */
		if (byte & 0x80)
			SET_SDA_HIGH();
		else
			SET_SDA_LOW();
		
		byte <<= 1;
		
		DELAY();
		
		SET_SCL_HIGH();
		DELAY();
	}
	
	SET_SCL_LOW();
	DELAY();
	
	return acquire_acknowledgement();
}

uint8_t i2c_read(uint8_t acknowledgement)
{
	SET_SCL_LOW();
	DELAY();
	
	set_sda_in();
	
	uint8_t byte = 0xff;		/* 0b1111 1111. */
	
	for (uint8_t i = 0; i < 8; i ++)
	{
		SET_SCL_HIGH();
		DELAY();
		
		/* 0b1111 1110. */
		byte <<= 1;
		
		if (READ_SDA_INPUT())	/* SDA is high. */
			byte |= 0x01;		/* 0b0000 0001. */
		
		SET_SCL_LOW();
		DELAY();
	}
		
	// Acknowledge, 0 means ack, 1 means non ack.
		
	acknowledge(acknowledgement);
	
	return byte;
}