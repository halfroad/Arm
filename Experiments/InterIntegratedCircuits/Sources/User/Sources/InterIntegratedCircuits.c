#include "stm32f4xx_i2c.h"
#include "stm32f4xx_gpio.h"

#include "InterIntegratedCircuits.h"
#include "Timer.h"

#define SCL_GPIO_PIN		GPIO_Pin_8	/* I2C1_SCL: PB6 */
#define SDA_GPIO_PIN		GPIO_Pin_9	/* I2C_SDA: PB7 */

#define I2C_SET_SCL(x)		GPIO_WriteBit(GPIOB, SCL_GPIO_PIN, (BitAction)(x))
#define I2C_SET_SDA(x)		GPIO_WriteBit(GPIOB, SDA_GPIO_PIN, (BitAction)(x))

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
	
	GPIO_InitTypeDef gpio_init_struct;
	
	gpio_init_struct.GPIO_Pin = SCL_GPIO_PIN | SDA_GPIO_PIN;
	gpio_init_struct.GPIO_Mode = GPIO_Mode_OUT;
	gpio_init_struct.GPIO_OType = GPIO_OType_OD;
	gpio_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB, &gpio_init_struct);
	
	I2C_SET_SCL(1);
	I2C_SET_SDA(1);
}

/*

Configure I2C SDA to input.

*/

void i2c_generate_start_conditions(void)
{
	I2C_SET_SDA(1);
	delay_us(10);
	
	I2C_SET_SCL(1);
	delay_us(10);
	
	I2C_SET_SDA(0);
	delay_us(10);
	
	I2C_SET_SCL(0);
	delay_us(10);
}

void i2c_generate_stopt_conditions(void)
{
	I2C_SET_SDA(0);
	delay_us(10);
	
	I2C_SET_SCL(1);
	delay_us(10);
	
	I2C_SET_SDA(10);
	delay_us(10);
}

uint8_t i2c_send(uint8_t byte, uint8_t acknowledgement)
{
	for (uint8_t i = 0; i < 8; i ++)
	{
		I2C_SET_SDA(byte & 0x80);
		
		byte <<= 1;
		
		delay_us(10);
		
		I2C_SET_SCL(1);
		delay_us(10);
		
		I2C_SET_SCL(0);
		delay_us(10);
	}
	
	I2C_SET_SDA(1);		/* No Ack. */
	delay_us(10);
	
	I2C_SET_SCL(1);
	
	uint8_t times = 0;
	
	while (GPIO_ReadInputDataBit(GPIOB, SDA_GPIO_PIN) == SET
		&& acknowledgement == 1)
	{
		times ++;
		
		if (times > 200)
		{
			I2C_SET_SCL(0);
			delay_us(10);
			
			return 0;
		}
	}
	
	I2C_SET_SCL(0);
	delay_us(10);
	
	return 1;
}

uint8_t i2c_read()
{
	uint8_t byte = 0x00;
	
	/*SCL = 0 at this moment. */
	
	I2C_SET_SDA(1);
	delay_us(10);
	
	for (uint8_t i = 0; i < 8; i ++)
	{
		I2C_SET_SCL(1);
		delay_us(10);
		
		byte <<= 1;
		byte |= GPIO_ReadInputDataBit(GPIOB, SDA_GPIO_PIN);
		
		delay_us(10);
		
		I2C_SET_SCL(10);
		delay_us(10);
	}
	
	return byte;
}