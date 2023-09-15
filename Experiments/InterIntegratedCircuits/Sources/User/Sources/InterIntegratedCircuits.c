#include "stm32f4xx_i2c.h"
#include "stm32f4xx_gpio.h"

#include "InterIntegratedCircuits.h"
#include "Timer.h"

#define SCL_GPIO_PIN		GPIO_Pin_6	/* I2C1_SCL: PB6 */
#define SDA_GPIO_PIN		GPIO_Pin_7	/* I2C_SDA: PB7 */

#define I2C_SET_SCL(x)		GPIO_WriteBit(GPIOB, SCL_GPIO_PIN, (BitAction)(x))
#define I2C_SET_SDA(x)		GPIO_WriteBit(GPIOB, SDA_GPIO_PIN, (BitAction)(x))

/*

Initialize the I2C settings, set the SCL and SDA to high.
*/
void i2c_init(void)
{
	/* Enable RCC clock for GPIOB.
	
	https://blog.51cto.com/weidongshan/6609372
	https://blog.csdn.net/lnfiniteloop/article/details/121951590
	https://www.cnblogs.com/blog-xh/p/17184510.html
	
	*/
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef gpio_init_struct;
	
	gpio_init_struct.GPIO_Pin = SCL_GPIO_PIN | SDA_GPIO_PIN;
	gpio_init_struct.GPIO_Mode = GPIO_Mode_OUT;
	gpio_init_struct.GPIO_OType = GPIO_OType_OD;
	gpio_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB, &gpio_init_struct);
}

void i2c_generate_start_conditions(void)
{
	I2C_SET_SDA(1);
	delay_us(10);
	
	I2C_SET_SCL(1);
	delay_us(10);
	
	I2C_SET_SDA(0);
	delay_us(10);
	
	/* Ready to write bit on SDA wire, and 
		avoid mischief (i.e. Start/Stop condition when SCL is high.)
	
	I2C_SET_SCL(0);
	delay_us(10);
	
	*/
}

void i2c_generate_stop_conditions(void)
{
	I2C_SET_SDA(0);
	delay_us(10);
	
	I2C_SET_SCL(1);
	delay_us(10);
	
	I2C_SET_SDA(1);
	delay_us(10);
}

void i2c_acknowledge(uint8_t acknowledgement)
{
	I2C_SET_SCL(0);
	delay_us(10);
	
	if (acknowledgement)
		I2C_SET_SDA(0);
	else
		I2C_SET_SDA(1);
	
	delay_us(10);
}

uint8_t i2c_send(uint8_t byte, uint8_t needAcknowledgement)
{
	uint8_t bit = 0;
	
	/* Send 8 bits. */
	for (uint8_t i = 0; i < 8; i ++)
	{
		bit = byte >> 7;
		
		I2C_SET_SDA(bit);
		
		byte <<= 1;
		
		delay_us(10);
		
		I2C_SET_SCL(1);
		delay_us(10);
		
		/*
			Slave receives the updated SDA, and supposedly,
			Slave will lower the SCL, but not.
		
		I2C_SET_SCL(0);
		delay_us(10);
		
		*/
	}
	
	/*
		From now on, the Slave has the control on both SCL and SDA.
		In the case the Slave well received the byte, the Salve will
		acknowledge the Master with SCL = 1 and SDA = 0 or SDA = 1 means Non ACK (unsuccess).
	*/
	
	if (needAcknowledgement)
	{
		uint8_t times = 0;
		
		while (GPIO_ReadInputDataBit(GPIOB, SDA_GPIO_PIN) == SET)
			/* Stopped by Slave. */
			if (times > 50)
				return 1;
	}
	else
		i2c_generate_stop_conditions();
		
	return 0;
}

uint8_t i2c_read (uint8_t needAcknowledgement)
{
	uint8_t byte = 0;
	
	for (uint8_t i = 0; i < 8; i ++)
	{
		I2C_SET_SCL(0);
		delay_us(10);
		
		I2C_SET_SCL(1);
		delay_us(10);
		
		byte <<= 1;
		uint8_t bits = GPIO_ReadInputDataBit(GPIOB, SDA_GPIO_PIN);
		byte |= bits;
	}
	
	i2c_acknowledge(needAcknowledgement);
	
	return byte;
}