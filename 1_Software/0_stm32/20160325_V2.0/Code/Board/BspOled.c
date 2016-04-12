/**
*************************************************************************************************
* @file    BspOled.c
* @author  Ge Yue
* @version V1.0.0
* @date    2016.03.19
* @brief   OLED��ʾ�ײ�����
*************************************************************************************************
@verbatim 

- �汾��       ����      ������        ���� 
  V1.0      2016.03.19   Ge Yue    OLED��ʾ�ײ�����; 

- �汾��       ����      �޸���        ���� 
@endverbatim 
*/

/**
* @defgroup OLED BspOled: BSP Group
* @{
*/

#include "BspOled.h"
#include "Bsp.h"
#include "String.h"

/** @brief OLED���ų�ʼ������ */
static void OledGpioInit(void);
/** @brief ��OLED������д���� */
static void OledWriteCmd(u8 cmd);
/** @brief ���û���λ�� */
static void OledSetPosition(u8 x, u8 y);
/** @brief OLED�����ַ���ʾ���� */
static void OledShowChar(u8 x, u8 y, char chr, int xOffset, u8 inverse);
/** @brief ����Ҫ��ʾ��������9bit�ĸ�ʽ�����Դ��� */
static void addDataToBuf(u8 x, u8 y, u8 *data, u16 num, u8 inverse);
/** @brief ÿһ���������Դ��е�ƫ�� */
const static u16 offsetLine[] = {LINE1, LINE2, LINE3, LINE4, LINE5, LINE6, LINE7, LINE8};

/** @brief �Դ� */
u8 DispBuf[PAGE_SIZE];

/** 
* @brief  OLED��ʼ������
* @param  ������ ����˵��  
* @param  ��
* @retval ��
* @par ʹ��ȫ�ֱ��� \n
* @note �� ִ��ʱ��: \n
*       �� ��������: ��������, ��ʼ��ʱ���� \n
*       �� �ɷ���: ���� \n
*
* @par  ע��:
*       �� �� \n
*/	
void OledInit(void)
{
    OledClear();    
	OledGpioInit();
    SpiInit();
    
	OLED_RST_Set();
	delayMs(100);
	OLED_RST_Clr();
	delayMs(100);
	OLED_RST_Set(); 
					  
	OledWriteCmd(0xAE);//--turn off oled panel
	OledWriteCmd(0x02);//---set low column address
	OledWriteCmd(0x10);//---set high column address
	OledWriteCmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OledWriteCmd(0x81);//--set contrast control register
	OledWriteCmd(0xCF); // Set SEG Output Current Brightness
	OledWriteCmd(0xA1);//--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
	OledWriteCmd(0xC8);//Set COM/Row Scan Direction   0xc0���·��� 0xc8����
	OledWriteCmd(0xA6);//--set normal display
	OledWriteCmd(0xA8);//--set multiplex ratio(1 to 64)
	OledWriteCmd(0x3f);//--1/64 duty
	OledWriteCmd(0xD3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OledWriteCmd(0x00);//-not offset
	OledWriteCmd(0xd5);//--set display clock divide ratio/oscillator frequency
	OledWriteCmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
	OledWriteCmd(0xD9);//--set pre-charge period
	OledWriteCmd(0xF1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OledWriteCmd(0xDA);//--set com pins hardware configuration
	OledWriteCmd(0x12);
	OledWriteCmd(0xDB);//--set vcomh
	OledWriteCmd(0x40);//Set VCOM Deselect Level
	OledWriteCmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OledWriteCmd(0x02);//
	OledWriteCmd(0x8D);//--set Charge Pump enable/disable
	OledWriteCmd(0x14);//--set(0x10) disable
	OledWriteCmd(0xA4);// Disable Entire Display On (0xa4/0xa5)
	OledWriteCmd(0xA6);// Disable Inverse Display On (0xa6/a7) 
	OledWriteCmd(0xAF);//--turn on oled panel
	
	OledWriteCmd(0xAF); /*display ON*/ 
	OledClear();
	OledSetPosition(0,0); 	
}

/**
* @brief  OLEDʹ����ʾ����
* @param  ������ ����˵��  
* @param  ��
* @retval ��
* @par ʹ��ȫ�ֱ��� \n
* @note �� ִ��ʱ��: \n
*       �� ��������: �������� \n
*       �� �ɷ���: ���� \n
*
* @par  ע��:
*       �� �� \n
*/	
void OledDisplayOn(void)
{
	OledWriteCmd(0X8D);  //SET DCDC����
	OledWriteCmd(0X14);  //DCDC ON
	OledWriteCmd(0XAF);  //DISPLAY ON
}

/**
* @brief  OLEDʧ����ʾ����
* @param  ������ ����˵��  
* @param  ��
* @retval ��
* @par ʹ��ȫ�ֱ��� \n
* @note �� ִ��ʱ��: \n
*       �� ��������: �������� \n
*       �� �ɷ���: ���� \n
*
* @par  ע��:
*       �� �� \n
*/	
void OledDisplayOff(void)
{
	OledWriteCmd(0X8D);  //SET DCDC����
	OledWriteCmd(0X10);  //DCDC OFF
	OledWriteCmd(0XAE);  //DISPLAY OFF
}

/**
* @brief  OLED��������
* @param  ������ ����˵��  
* @param  ��
* @retval ��
* @par ʹ��ȫ�ֱ��� \n
* @note �� ִ��ʱ��: \n
*       �� ��������: �������� \n
*       �� �ɷ���: ���� \n
*
* @par  ע��:
*       �� �� \n
*/	  
void OledClear(void)  
{  
	int i;	
    for (i = 0; i < PAGE_SIZE; i += 9)
    {
        DispBuf[i] = 0x80;
        DispBuf[i+1] = 0x40;
        DispBuf[i+2] = 0x20;
        DispBuf[i+3] = 0x10;
        DispBuf[i+4] = 0x08;
        DispBuf[i+5] = 0x04;
        DispBuf[i+6] = 0x02;
        DispBuf[i+7] = 0x01;
        DispBuf[i+8] = 0x00;
    }
}

/**
* @brief  OLED��ʾһ��������
* @param  ������ ����˵��  
* @param  ��
* @retval ��
* @par ʹ��ȫ�ֱ��� \n
* @note �� ִ��ʱ��: \n
*       �� ��������: �������� \n
*       �� �ɷ���: ���� \n
*
* @par  ע��:
*       �� �� \n
*/
void OledDisp(void)
{
    static u8 i = 0;
    /* ��һ��DMA�Ѿ�������� */
    if (DMA_GetFlagStatus(DMA1_FLAG_TC3))
    {
        u8 *p = DispBuf + LINE_SIZE * i;
        OledSetPosition(0, i);
        OLED_CS_Clr();
        SPI_AF_PP();
        SpiSend(p, LINE_SIZE);
        i++;
        i &= 7;
    }
}

/**
* @brief  OLEDͼƬ��ʾ����
* @param  ������ ����˵��  
* @param  [in] x x����, 0~127
* @param  [in] y y����, 0~7
* @param  [in] width x���, ��λ:���� 0~127
* @param  [in] hight y���, ��λ:�� 0~7
* @param  [in] data ��Ҫ��ʾ����
* @param  [in] xOffset x���ƫ��, С��0��ʾ����ƫ��, ����0��ʾ����ƫ��
* @param  [in] inverse �Ƿ�ɫ
* @retval ��
* @par ʹ��ȫ�ֱ��� \n
* @note �� ִ��ʱ��: \n
*       �� ��������: �������� \n
*       �� �ɷ���: ���� \n
*
* @par  ע��:
*       �� �˴�ֻ�ǽ�Ҫ��ʾ�����ݼ��뵽��ʾ��������,������ʾ��OledDisp()���� \n
*/
void OledDisplay(u8 x, u8 y, char width, char hight, u8 *data, int xOffset, u8 inverse)
{
	int i;
	/* ���ұ߽� */
	int leftBound,rightBound;
	/* ��ʾ��x�����, ��ʾ�Ŀ��, ����ƫ�� */
	char startX = x + xOffset, widthShow = width, dataOffset = 0;
	
    /* ���ƫ��128 */
	if(xOffset < -128)	xOffset = -128;
	else if(xOffset > 128)	xOffset = 128;
	
	/* ����ƫ�ƺ�����ұ߽� */
	leftBound = (int)x + xOffset;
	rightBound = (int)x + (int)width + xOffset - 1;
	
    /* �����߽����127 �� �ұ߽�С��0, ������ͼƬ��������Ļ��, ����Ҫ��ʾ */
	if(leftBound > 127)	return;
	if(rightBound < 0)	return;
	
	/* ����߽�С��0 */
	if(leftBound < 0)
	{
		/* �������Ϊ0 */
		startX = 0;
		/* ������ʾ������ʼλ�� */
		dataOffset = -leftBound;
		/* ������ʾ�Ŀ��, x����С��0��������Ҫ��ʾ */
		widthShow = width + leftBound;
	}
    /* ���ұ߽����0 */
	else if(rightBound > 127)
	{
        /* ������ʾ�Ŀ��, x������127��������Ҫ��ʾ */
		widthShow = width - (rightBound - 127);
	}
	
    /* ��ÿһ�����ݲ��� */
	for(i = 0;i < hight; i++)
	{
        addDataToBuf(startX, y+i, &data[i * width + dataOffset], widthShow, inverse);
	}
}

/**
* @brief  OLED�ַ�����ʾ����
* @param  ������ ����˵��  
* @param  [in] x x����, 0~127
* @param  [in] y y����, 0~7
* @param  [in] chr ��Ҫ��ʾ���ַ���ָ��
* @param  [in] xOffset x���ƫ��, С��0��ʾ����ƫ��, ����0��ʾ����ƫ��
* @param  [in] inverse �Ƿ�ɫ
* @retval ��
* @par ʹ��ȫ�ֱ��� \n
* @note �� ִ��ʱ��: \n
*       �� ��������: �������� \n
*       �� �ɷ���: ���� \n
*
* @par  ע��:
*       �� �� \n
*/
void OledShowString(u8 x, u8 y, char *chr, int xOffset, u8 inverse)
{
	unsigned char j = 0;
	while (chr[j] != '\0')
	{	
        if (x <= 122)
        {
            OledShowChar(x, y, chr[j], xOffset, inverse);
        }
		x += 6;
		j++;
	}
}

/**
* @brief  OLED�����ַ���ʾ����
* @param  ������ ����˵��  
* @param  [in] x x����, 0~127
* @param  [in] y y����, 0~7
* @param  [in] chr ��Ҫ��ʾ���ַ�ASCII��
* @param  [in] xOffset x���ƫ��, С��0��ʾ����ƫ��, ����0��ʾ����ƫ��
* @param  [in] inverse �Ƿ�ɫ
* @retval ��
* @par ʹ��ȫ�ֱ��� \n
* @note �� ִ��ʱ��: \n
*       �� ��������: �������� \n
*       �� �ɷ���: ���� \n
*
* @par  ע��:
*       �� �� \n
*/
static void OledShowChar(u8 x, u8 y, char chr, int xOffset, u8 inverse)
{      	
	unsigned char c=0;
	/* �õ�ƫ�ƺ��ֵ, ����ģ�����и��ַ���λ�� */	
	c = chr - ' ';			
	if(x > Max_Column - 6)
    {   
        x = 0;
        y += 1;
    }
	OledDisplay(x, y, 6, 1, (u8*)&F6x8[c][0], xOffset, inverse);
}

/**
* @brief  ��Ҫ��ʾ��������9bit��ʽ�����Դ�
* @param  ������ ����˵��  
* @param  [in] x x����, 0~127
* @param  [in] y y����, 0~7
* @param  [in] data Ҫ��ʾ������
* @param  [in] num ���ݳ��� 
* @param  [in] inverse �Ƿ�ɫ
* @retval ��
* @par ʹ��ȫ�ֱ��� \n
* @note �� ִ��ʱ��: \n
*       �� ��������: �������� \n
*       �� �ɷ���: ���� \n
*
* @par  ע��:
*       �� �� \n
*/
static void addDataToBuf(u8 x, u8 y, u8 *data, u16 num, u8 inverse)
{             
    u16 offset = offsetLine[y] + (x >> 3) * 9 + (x & 0x07);     // �ֽ�ƫ��
    u16 bufSize = (num >> 3) * 9 + (num & 0x07);                // ת����9bit����Ҫ�޸ĵ����ֽ���
    u8 bitOffset = x & 0x07;                                    // λƫ�� 
    u8 dstOffset = bitOffset;                                   // ��offset % 9��Ч;  // Ŀ��λ�õ�ƫ��(9���ֽ�һ��)        
    u8 *pDst = &DispBuf[offset];                                // Ŀ��λ��ָ��
    u8 *pSrc = data;                                            // Դ����ָ��
    u8 i;        
    u8 tmp, dataSrc0, dataSrc1;        
    
    if ((num & 0x07) || (x & 0x07))     // ������Ȳ���8�ı��� �� ƫ�Ʋ���8�ı���
    {
        bufSize++;
    }
    if (((num & 0x07) + (x & 0x07)) > 8)
    {
        bufSize++;
    }
    
    /* �����һ���ֽ� */
    dataSrc0 = (inverse ? ~*pSrc : *pSrc);
    tmp = *pDst & (0xff << (8 - bitOffset));
    *pDst = tmp + (0x80 >> bitOffset) + (dataSrc0 >> (bitOffset + 1));
    dstOffset++;        // bitOffset <= 7, ��destOffset <= 8, �������������
    
    /* �����м����� */
    for (i = 1; i < (bufSize - 1); i++)
    {
        pDst++;
        dataSrc0 = (inverse ? ~*pSrc : *pSrc);
        dataSrc1 = (inverse ? ~*(pSrc + 1) : *(pSrc+1));
        /* ��ǰĿ��λ��ƫ�� */
        switch (dstOffset)
        {
            case 0:
                *pDst = 0x80 | (dataSrc1 >> 1);
                pSrc++;
                dstOffset++;
                break;
            case 1:
                *pDst = 0x40 | (dataSrc0 << 7) | (dataSrc1 >> 2);
                pSrc++;
                dstOffset++;
                break;
            case 2:
                *pDst = 0x20 | (dataSrc0 << 6) | (dataSrc1 >> 3);
                pSrc++;
                dstOffset++;
                break;
            case 3:
                *pDst = 0x10 | (dataSrc0 << 5) | (dataSrc1 >> 4);
                pSrc++;
                dstOffset++;
                break;
            case 4:
                *pDst = 0x08 | (dataSrc0 << 4) | (dataSrc1 >> 5);
                pSrc++;
                dstOffset++;
                break;
            case 5:
                *pDst = 0x04 | (dataSrc0 << 3) | (dataSrc1 >> 6);
                pSrc++;
                dstOffset++;
                break;
            case 6:
                *pDst = 0x02 | (dataSrc0 << 2) | (dataSrc1 >> 7);
                pSrc++;
                dstOffset++;
                break;
            case 7:
                *pDst = 0x01 | (dataSrc0 << 1);
                pSrc++;
                dstOffset++;
                break;
            case 8:
                *pDst = dataSrc0;         
                dstOffset = 0;
                break;
        }
    }
    
    pDst++;
    /* �������һ���ֽ� */
    dataSrc0 = (inverse ? ~*pSrc : *pSrc);
    tmp = *pDst & (0xff >> dstOffset);
    *pDst = (dataSrc0 << (8 - dstOffset)) + tmp;
}

/**
* @brief  OLED���û���λ�ú���
* @param  ������ ����˵��  
* @param  [in] x x����, 0~127
* @param  [in] y y����, 0~7
* @retval ��
* @par ʹ��ȫ�ֱ��� \n
* @note �� ִ��ʱ��: \n
*       �� ��������: �������� \n
*       �� �ɷ���: ���� \n
*
* @par  ע��:
*       �� �� \n
*/
static void OledSetPosition(u8 x, u8 y) 
{ 
	OledWriteCmd(0xb0 + y);
	OledWriteCmd(((x & 0xf0) >> 4)| 0x10);
	OledWriteCmd((x & 0x0f) | 0x01); 
}  

/**
* @brief  OLED���ų�ʼ������
* @param  ������ ����˵��  
* @param  ��  
* @retval ��
* @par ʹ��ȫ�ֱ��� \n
* @note �� ִ��ʱ��: \n
*       �� ��������: ��������, ��ʼ��ʱ���� \n
*       �� �ɷ���: ���� \n
*
* @par  ע��:
*       �� �� \n
*/
static void OledGpioInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
} 

/**
* @brief  ��OLED������д����
* @param  ������ ����˵��  
* @param  [in] dat ����
* @retval ��
* @par ʹ��ȫ�ֱ��� \n
* @note �� ִ��ʱ��: \n
*       �� ��������: �������� \n
*       �� �ɷ���: ���� \n
*
* @par  ע��:
*       �� �� \n
*/
static void OledWriteCmd(u8 cmd)
{	
	u8 i;			
    SPI_OUT_PP();
    
	OLED_CS_Clr();
	
	OLED_SCLK_Clr();
	OLED_SDA_Clr();
	OLED_SCLK_Set();
	
	for(i=0;i<8;i++)
	{			  
		OLED_SCLK_Clr();
		if(cmd&0x80)
			OLED_SDA_Set();
		else 
			OLED_SDA_Clr();
		OLED_SCLK_Set();
		cmd<<=1;   
	}				 		  
	OLED_CS_Set();  	  
} 

/**  @} */

