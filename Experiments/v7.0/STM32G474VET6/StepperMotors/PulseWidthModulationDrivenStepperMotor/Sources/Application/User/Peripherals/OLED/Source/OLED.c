#include <stdlib.h>

#include "../Include/OLED.h"
#include "../Include/OLEDFont.h"       

//----------------- OLED Port definitions ---------------- 

#define RCC_DOMAIN                              AHB2ENR

#define I2C_SCL_GPIO_PORT                       GPIOB
#define I2C_SCL_GPIO_PORT_BIT_NUMBER            1
#define I2C_SCL_PIN                             3      /*  I2C SCL: PB3       */

#define I2C_SDA_GPIO_PORT                       GPIOB
#define I2C_SDA_GPIO_PORT_BIT_NUMBER            1
#define I2C_SDA_PIN                             5       /*  I2C SDA: PB5        */

/*
#define I2C_GPIO_PORT                           GPIOF
#define I2C_GPIO_PORT_BIT_NUMBER                5

#define I2C_SCL_PIN                             14
#define I2C_SDA_PIN                             15
*/
#define I2C_CS_PIN                              10

#define OLED_SCL_CLR()                          I2C_SCL_GPIO_PORT -> BSRR |= 0x01 << (I2C_SCL_PIN + 16)
#define OLED_SCL_SET()                          I2C_SCL_GPIO_PORT -> BSRR |= 0x01 << I2C_SCL_PIN

#define OLED_SDA_CLR()                          I2C_SDA_GPIO_PORT -> BSRR |= 0x01 << (I2C_SDA_PIN + 16)
#define OLED_SDA_SET()                          I2C_SDA_GPIO_PORT -> BSRR |= 0x01 << I2C_SDA_PIN

#define OLED_RES_CLR()                          I2C_GPIO_PORT -> BSRR |= 0x01 << (I2C_CS_PIN + 16)
#define OLED_RES_SET()                          I2C_GPIO_PORT -> BSRR |= 0x01 << I2C_CS_PIN

#define OLED_CMD                                0    // Write COMMAND
#define OLED_DATA                               1    // Write DATA


uint8_t OLED_GRAM[144][8];

//OLED�ĳ�ʼ��
void OLED_Init(void)
{
    /*
     
     Bit 1 GPIOBEN: I2C_GPIO_PORT peripheral clock enable
     Set and reset by software.
     0: I2C_GPIO_PORT peripheral clock disabled (default after reset)
     1: I2C_GPIO_PORT peripheral clock enabled
     
     */
    
    RCC -> RCC_DOMAIN |= 0x01 << I2C_SCL_GPIO_PORT_BIT_NUMBER; /* Enable the RCC clock for I2C_SCL_GPIO_PORT. */
    RCC -> RCC_DOMAIN |= 0x01 << I2C_SDA_GPIO_PORT_BIT_NUMBER; /* Enable the RCC clock for I2C_SDA_GPIO_PORT. */

    /* Configure I2C1_SCL on PB8. */

    /*
     
     Bits 31:0 MODER[15:0][1:0]: Port x configuration I/O pin y (y = 15 to 0)
     These bits are written by software to configure the I/O mode.
     00: Input mode
     01: General purpose output mode
     10: Alternate function mode
     11: Analog mode (reset state)
     */

    I2C_SCL_GPIO_PORT -> MODER &= ~(0x03 << I2C_SCL_PIN * 2); /* Generic output. */
    I2C_SCL_GPIO_PORT -> MODER |= 0x01 << I2C_SCL_PIN * 2;

    /*
     
     Bits 15:0 OT[15:0]: Port x configuration I/O pin y (y = 15 to 0)
     These bits are written by software to configure the I/O output type.
     0: Output push-pull (reset state)
     1: Output open-drain
     
     I2C_GPIO_PORT  ->  OTYPER &= ~(0x01 << I2C_SCL_PIN);
     
     */

    I2C_SCL_GPIO_PORT -> OTYPER |= 0x01 << I2C_SCL_PIN; /* Open Drain. */

    /*
     
     Bits 31:0 OSPEEDR[15:0][1:0]: Port x configuration I/O pin y (y = 15 to 0)
     These bits are written by software to configure the I/O output speed.
     00: Low speed
     01: Medium speed
     10: High speed
     11: Very high speed
     Note: Refer to the product datasheets for the values of OSPEEDRy bits versus VDD range
     and external load.
     
     */
    I2C_SCL_GPIO_PORT -> OSPEEDR &= ~(0x03 << I2C_SCL_PIN * 2);
    I2C_SCL_GPIO_PORT -> OSPEEDR |= 0x02 << I2C_SCL_PIN * 2;

    /*
     
     Bits 31:0 PUPDR[15:0][1:0]: Port x configuration I/O pin y (y = 15 to 0)
     These bits are written by software to configure the I/O pull-up or pull-down
     00: No pull-up, pull-down
     01: Pull-up
     10: Pull-down
     11: Reserved
     
     */

    I2C_SCL_GPIO_PORT -> PUPDR &= ~(0x03 << I2C_SCL_PIN * 2);
    I2C_SCL_GPIO_PORT -> PUPDR |= 0x01 << I2C_SCL_PIN * 2;

    /* Configure I2C1_SDA on PB9. */

    I2C_SDA_GPIO_PORT -> MODER &= ~(0x03 << I2C_SDA_PIN * 2);
    I2C_SDA_GPIO_PORT -> MODER |= 0x01 << I2C_SDA_PIN * 2;

    I2C_SDA_GPIO_PORT -> OTYPER |= 0x01 << I2C_SDA_PIN;

    I2C_SDA_GPIO_PORT -> OSPEEDR &= ~(0x03 << I2C_SDA_PIN * 2);
    I2C_SDA_GPIO_PORT -> OSPEEDR |= 0x02 << I2C_SDA_PIN * 2;

    I2C_SDA_GPIO_PORT -> PUPDR &= ~(0x03 << I2C_SDA_PIN * 2);
    I2C_SDA_GPIO_PORT -> PUPDR |= 0x01 << I2C_SDA_PIN * 2;

    /*  OLED_RES_CLR(); */

    HAL_Delay(180);

    /*  OLED_RES_SET(); */

    OLED_WR_Byte(0xFD, OLED_CMD);
    OLED_WR_Byte(0x12, OLED_CMD);
    OLED_WR_Byte(0xAE, OLED_CMD); //--turn off oled panel
    OLED_WR_Byte(0xd5, OLED_CMD); //--set display clock divide ratio/oscillator frequency
    OLED_WR_Byte(0xA0, OLED_CMD);
    OLED_WR_Byte(0xA8, OLED_CMD); //--set multiplex ratio(1 to 64)
    OLED_WR_Byte(0x3f, OLED_CMD); //--1/64 duty
    OLED_WR_Byte(0xD3, OLED_CMD); //-set display offset    Shift Mapping RAM Counter (0x00~0x3F)
    OLED_WR_Byte(0x00, OLED_CMD); //-not offset
    OLED_WR_Byte(0x40, OLED_CMD); //--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    OLED_WR_Byte(0xA1, OLED_CMD); //--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
    OLED_WR_Byte(0xC8, OLED_CMD); //Set COM/Row Scan Direction   0xc0���·��� 0xc8����
    OLED_WR_Byte(0xDA, OLED_CMD); //--set com pins hardware configuration
    OLED_WR_Byte(0x12, OLED_CMD);
    OLED_WR_Byte(0x81, OLED_CMD); //--set contrast control register
    OLED_WR_Byte(0x7F, OLED_CMD); // Set SEG Output Current Brightness
    OLED_WR_Byte(0xD9, OLED_CMD); //--set pre-charge period
    OLED_WR_Byte(0x82, OLED_CMD); //Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    OLED_WR_Byte(0xDB, OLED_CMD); //--set vcomh
    OLED_WR_Byte(0x34, OLED_CMD); //Set VCOM Deselect Level
    OLED_WR_Byte(0xA4, OLED_CMD); // Disable Entire Display On (0xa4/0xa5)
    OLED_WR_Byte(0xA6, OLED_CMD); // Disable Inverse Display On (0xa6/a7)
    
    OLED_Clear();
    
    OLED_WR_Byte(0xAF, OLED_CMD);
}

//���Ժ���
void OLED_ColorTurn(uint8_t i)
{
    if (i == 0)
    {
        OLED_WR_Byte(0xA6, OLED_CMD); //������ʾ
    }
    if (i == 1)
    {
        OLED_WR_Byte(0xA7, OLED_CMD); //��ɫ��ʾ
    }
}

//��Ļ��ת180��
void OLED_DisplayTurn(uint8_t i)
{
    if (i == 0)
    {
        OLED_WR_Byte(0xC8, OLED_CMD); //������ʾ
        OLED_WR_Byte(0xA1, OLED_CMD);
    }
    if (i == 1)
    {
        OLED_WR_Byte(0xC0, OLED_CMD); //��ת��ʾ
        OLED_WR_Byte(0xA0, OLED_CMD);
    }
}

//��ʱ
void IIC_delay(void)
{
    uint8_t t = 10;
    while (t--)
        ;
}

//��ʼ�ź�
void I2C_Start(void)
{
    OLED_SDA_SET();
    OLED_SCL_SET();
    
    IIC_delay();
    
    OLED_SDA_CLR();
    
    IIC_delay();
    
    OLED_SCL_CLR();
    
    IIC_delay();
}

//�����ź�
void I2C_Stop(void)
{
    OLED_SDA_CLR();
    OLED_SCL_SET();
    
    IIC_delay();
    
    OLED_SDA_SET();
}

//�ȴ��ź���Ӧ
void I2C_WaitAck(void) //�������źŵĵ�ƽ
{
    OLED_SDA_SET();
    
    IIC_delay();
    
    OLED_SCL_SET();
    
    IIC_delay();
    
    OLED_SCL_CLR();
    
    IIC_delay();
}

//д��һ���ֽ�
void Send_Byte(uint8_t dat)
{
    uint8_t i;
    
    for (i = 0; i < 8; i++)
    {
        if (dat & 0x80) //��dat��8λ�����λ����д��
        {
            OLED_SDA_SET();
        }
        else
        {
            OLED_SDA_CLR();
        }
        
        IIC_delay();
        
        OLED_SCL_SET();
        
        IIC_delay();
        
        OLED_SCL_CLR(); //��ʱ���ź�����Ϊ�͵�ƽ
        
        dat <<= 1;
    }
}

//����һ���ֽ�
//mode:����/�����־ 0,��ʾ����;1,��ʾ����;
void OLED_WR_Byte(uint8_t dat, uint8_t mode)
{
    I2C_Start();
    
    Send_Byte(0x78);
    
    I2C_WaitAck();
    
    if (mode)
        Send_Byte(0x40);
    else
        Send_Byte(0x00);
    
    I2C_WaitAck();
    
    Send_Byte(dat);
    
    I2C_WaitAck();
    
    I2C_Stop();
}

//����OLED��ʾ 
void OLED_DisPlay_On(void)
{
    OLED_WR_Byte(0x8D, OLED_CMD); //��ɱ�ʹ��
    OLED_WR_Byte(0x14, OLED_CMD); //������ɱ�
    OLED_WR_Byte(0xAF, OLED_CMD); //������Ļ
}

//�ر�OLED��ʾ 
void OLED_DisPlay_Off(void)
{
    OLED_WR_Byte(0x8D, OLED_CMD); //��ɱ�ʹ��
    OLED_WR_Byte(0x10, OLED_CMD); //�رյ�ɱ�
    OLED_WR_Byte(0xAE, OLED_CMD); //�ر���Ļ
}

//�����Դ浽OLED    
void OLED_Refresh(void)
{
    uint8_t i, n;
    
    for (i = 0; i < 8; i++)
    {
        OLED_WR_Byte(0xb0 + i, OLED_CMD); //��������ʼ��ַ
        OLED_WR_Byte(0x00, OLED_CMD);   //���õ�����ʼ��ַ
        OLED_WR_Byte(0x10, OLED_CMD);   //���ø�����ʼ��ַ
        
        I2C_Start();
        
        Send_Byte(0x78);
        
        I2C_WaitAck();
        
        Send_Byte(0x40);
        
        I2C_WaitAck();
        
        for (n = 0; n < 128; n++)
        {
            Send_Byte(OLED_GRAM[n][i]);
            I2C_WaitAck();
        }
        
        I2C_Stop();
    }
}
//��������
void OLED_Clear(void)
{
    uint8_t i, n;
    
    for (i = 0; i < 8; i++)
    {
        for (n = 0; n < 128; n++)
            OLED_GRAM[n][i] = 0;   //�����������
    }
    
    OLED_Refresh();   //������ʾ
}

//���� 
//x:0~127
//y:0~63
//t:1 ��� 0,���    
void OLED_DrawPoint(uint8_t x, uint8_t y, uint8_t t)
{
    uint8_t i, m, n;
    i = y / 8;
    m = y % 8;
    n = 1 << m;
    
    if (t)
        OLED_GRAM[x][i] |= n;
    else
    {
        OLED_GRAM[x][i] = ~OLED_GRAM[x][i];
        OLED_GRAM[x][i] |= n;
        OLED_GRAM[x][i] = ~OLED_GRAM[x][i];
    }
}

//����
//x1,y1:�������
//x2,y2:��������
void OLED_DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t mode)
{
    uint16_t t;
    
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, uRow, uCol;
    
    delta_x = x2 - x1; //������������ 
    delta_y = y2 - y1;
    uRow = x1; //�����������
    uCol = y1;
    
    if (delta_x > 0)
        incx = 1; //���õ������� 
    else if (delta_x == 0)
        incx = 0; //��ֱ�� 
    else
    {
        incx = -1;
        delta_x = -delta_x;
    }
    
    if (delta_y > 0)
        incy = 1;
    else if (delta_y == 0)
        incy = 0; //ˮƽ�� 
    else
    {
        incy = -1;
        delta_y = -delta_x;
    }
    
    if (delta_x > delta_y)
        distance = delta_x; //ѡȡ�������������� 
    else
        distance = delta_y;
    for (t = 0; t < distance + 1; t++)
    {
        OLED_DrawPoint(uRow, uCol, mode); //����
        xerr += delta_x;
        yerr += delta_y;
        if (xerr > distance)
        {
            xerr -= distance;
            uRow += incx;
        }
        if (yerr > distance)
        {
            yerr -= distance;
            uCol += incy;
        }
    }
}
//x,y:Բ������
//r:Բ�İ뾶
void OLED_DrawCircle(uint8_t x, uint8_t y, uint8_t r)
{
    int a, b, num;
    
    a = 0;
    b = r;
    
    while (2 * b * b >= r * r)
    {
        OLED_DrawPoint(x + a, y - b, 1);
        OLED_DrawPoint(x - a, y - b, 1);
        OLED_DrawPoint(x - a, y + b, 1);
        OLED_DrawPoint(x + a, y + b, 1);

        OLED_DrawPoint(x + b, y + a, 1);
        OLED_DrawPoint(x + b, y - a, 1);
        OLED_DrawPoint(x - b, y - a, 1);
        OLED_DrawPoint(x - b, y + a, 1);

        a++;
        num = (a * a + b * b) - r * r; //���㻭�ĵ���Բ�ĵľ���
        
        if (num > 0)
        {
            b--;
            a--;
        }
    }
}

//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//size1:ѡ������ 6x8/6x12/8x16/12x24
//mode:0,��ɫ��ʾ;1,������ʾ
void OLED_ShowChar(uint8_t x, uint8_t y, unsigned char chr, uint8_t size1, uint8_t mode)
{
    uint8_t i, m, temp, size2, chr1;
    uint8_t x0 = x, y0 = y;
    
    if (size1 == 8)
        size2 = 6;
    else
        size2 = (size1 / 8 + ((size1 % 8) ? 1 : 0)) * (size1 / 2); //�õ�����һ���ַ���Ӧ������ռ���ֽ���
    chr1 = chr - ' ';  //����ƫ�ƺ��ֵ
    
    for (i = 0; i < size2; i++)
    {
        if (size1 == 8)
            temp = asc2_0806[chr1][i];
        //����0806����
        else if (size1 == 12)
            temp = asc2_1206[chr1][i];
        //����1206����
        else if (size1 == 16)
            temp = asc2_1608[chr1][i];//����1608����
        else if (size1 == 24)
            temp = asc2_2412[chr1][i];//����2412����
        else
            return;
        
        for (m = 0; m < 8; m++)
        {
            if (temp & 0x01)
                OLED_DrawPoint(x, y, mode);
            else
                OLED_DrawPoint(x, y, !mode);
            temp >>= 1;
            y++;
        }
        
        x++;
        
        if ((size1 != 8) && ((x - x0) == size1 / 2))
        {
            x = x0;
            y0 = y0 + 8;
        }
        y = y0;
    }
}

//��ʾ�ַ���
//x,y:�������  
//size1:�����С 
//*chr:�ַ�����ʼ��ַ 
//mode:0,��ɫ��ʾ;1,������ʾ
void OLED_ShowString(uint8_t x, uint8_t y, unsigned char *chr, uint8_t size1, uint8_t mode)
{
    while ((*chr >= ' ') && (*chr <= '~')) //�ж��ǲ��ǷǷ��ַ�!
    {
        OLED_ShowChar(x, y, *chr, size1, mode);
        
        if (size1 == 8)
            x += 6;
        else
            x += size1 / 2;
        
        chr++;
    }
}

//m^n
uint32_t OLED_Pow(uint8_t m, uint8_t n)
{
    uint32_t result = 1;
    
    while (n--)
        result *= m;
    
    return result;
}

//��ʾ����
//x,y :�������
//num :Ҫ��ʾ������
//len :���ֵ�λ��
//size:�����С
//mode:0,��ɫ��ʾ;1,������ʾ
void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size1, uint8_t mode)
{
    uint8_t t, temp, m = 0;
    
    if (size1 == 8)
        m = 2;
    
    for (t = 0; t < len; t++)
    {
        temp = (num / OLED_Pow(10, len - t - 1)) % 10;
        if (temp == 0)
            OLED_ShowChar(x + (size1 / 2 + m) * t, y, '0', size1, mode);
        else
            OLED_ShowChar(x + (size1 / 2 + m) * t, y, temp + '0', size1, mode);
    }
}

//��ʾ����
//x,y:�������
//num:���ֶ�Ӧ�����
//mode:0,��ɫ��ʾ;1,������ʾ
void OLED_ShowChinese(uint8_t x, uint8_t y, uint8_t num, uint8_t size1, uint8_t mode)
{
    uint8_t m, temp;
    uint8_t x0 = x, y0 = y;
    uint16_t i, size3 = (size1 / 8 + ((size1 % 8) ? 1 : 0)) * size1; //�õ�����һ���ַ���Ӧ������ռ���ֽ���
    
    for (i = 0; i < size3; i++)
    {
        if (size1 == 16)
            temp = Hzk1[num][i];
        //����16*16����
        else if (size1 == 24)
            temp = Hzk2[num][i];
        //����24*24����
        else if (size1 == 32)
            temp = Hzk3[num][i];//����32*32����
        else if (size1 == 64)temp = Hzk4[num][i];//����64*64����
        else
            return;
        
        for (m = 0; m < 8; m++)
        {
            if (temp & 0x01)
                OLED_DrawPoint(x, y, mode);
            else
                OLED_DrawPoint(x, y, !mode);
            temp >>= 1;
            y++;
        }
        
        x++;
        
        if ((x - x0) == size1)
        {
            x = x0;
            y0 = y0 + 8;
        }
        
        y = y0;
    }
}

//num ��ʾ���ֵĸ���
//space ÿһ����ʾ�ļ��
//mode:0,��ɫ��ʾ;1,������ʾ
void OLED_ScrollDisplay(uint8_t num, uint8_t space, uint8_t mode)
{
    uint8_t i, n, t = 0, m = 0, r;
    
    while (1)
    {
        if (m == 0)
        {
            OLED_ShowChinese(128, 24, t, 16, mode); //д��һ�����ֱ�����OLED_GRAM[][]������
            
            t++;
        }
        if (t == num)
        {
            for (r = 0; r < 16 * space; r++)      //��ʾ���
            {
                for (i = 1; i < 144; i++)
                {
                    for (n = 0; n < 8; n++)
                        OLED_GRAM[i - 1][n] = OLED_GRAM[i][n];
                }
                OLED_Refresh();
            }
            t = 0;
        }
        
        m++;
        
        if (m == 16)
            m = 0;
        
        for (i = 1; i < 144; i++)   //ʵ������
        {
            for (n = 0; n < 8; n++)
                OLED_GRAM[i - 1][n] = OLED_GRAM[i][n];
        }
        
        OLED_Refresh();
    }
}

//x,y���������
//sizex,sizey,ͼƬ����
//BMP[]��Ҫд���ͼƬ����
//mode:0,��ɫ��ʾ;1,������ʾ
void OLED_ShowPicture(uint8_t x, uint8_t y, uint8_t sizex, uint8_t sizey, uint8_t BMP[], uint8_t mode)
{
    uint16_t j = 0;
    uint8_t i, n, temp, m;
    uint8_t x0 = x, y0 = y;
    
    sizey = sizey / 8 + ((sizey % 8) ? 1 : 0);
    
    for (n = 0; n < sizey; n++)
    {
        for (i = 0; i < sizex; i++)
        {
            temp = BMP[j];
            
            j++;
            
            for (m = 0; m < 8; m++)
            {
                if (temp & 0x01)
                    OLED_DrawPoint(x, y, mode);
                else
                    OLED_DrawPoint(x, y, !mode);
                
                temp >>= 1;
                
                y++;
            }
            
            x++;
            
            if ((x - x0) == sizex)
            {
                x = x0;
                y0 = y0 + 8;
            }
            
            y = y0;
        }
    }
}