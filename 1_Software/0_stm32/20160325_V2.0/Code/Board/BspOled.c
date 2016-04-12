/**
*************************************************************************************************
* @file    BspOled.c
* @author  Ge Yue
* @version V1.0.0
* @date    2016.03.19
* @brief   OLED显示底层驱动
*************************************************************************************************
@verbatim 

- 版本号       日期      创建人        描述 
  V1.0      2016.03.19   Ge Yue    OLED显示底层驱动; 

- 版本号       日期      修改人        描述 
@endverbatim 
*/

/**
* @defgroup OLED BspOled: BSP Group
* @{
*/

#include "BspOled.h"
#include "Bsp.h"
#include "String.h"

/** @brief OLED引脚初始化函数 */
static void OledGpioInit(void);
/** @brief 向OLED控制器写命令 */
static void OledWriteCmd(u8 cmd);
/** @brief 设置画点位置 */
static void OledSetPosition(u8 x, u8 y);
/** @brief OLED单个字符显示函数 */
static void OledShowChar(u8 x, u8 y, char chr, int xOffset, u8 inverse);
/** @brief 将需要显示的内容以9bit的格式加入显存中 */
static void addDataToBuf(u8 x, u8 y, u8 *data, u16 num, u8 inverse);
/** @brief 每一行首列在显存中的偏移 */
const static u16 offsetLine[] = {LINE1, LINE2, LINE3, LINE4, LINE5, LINE6, LINE7, LINE8};

/** @brief 显存 */
u8 DispBuf[PAGE_SIZE];

/** 
* @brief  OLED初始化函数
* @param  参数名 参数说明  
* @param  无
* @retval 无
* @par 使用全局变量 \n
* @note ● 执行时间: \n
*       ● 调用周期: 触发调用, 初始化时调用 \n
*       ● 可否打断: 可以 \n
*
* @par  注意:
*       ● 无 \n
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
	OledWriteCmd(0xA1);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	OledWriteCmd(0xC8);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
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
* @brief  OLED使能显示函数
* @param  参数名 参数说明  
* @param  无
* @retval 无
* @par 使用全局变量 \n
* @note ● 执行时间: \n
*       ● 调用周期: 触发调用 \n
*       ● 可否打断: 可以 \n
*
* @par  注意:
*       ● 无 \n
*/	
void OledDisplayOn(void)
{
	OledWriteCmd(0X8D);  //SET DCDC命令
	OledWriteCmd(0X14);  //DCDC ON
	OledWriteCmd(0XAF);  //DISPLAY ON
}

/**
* @brief  OLED失能显示函数
* @param  参数名 参数说明  
* @param  无
* @retval 无
* @par 使用全局变量 \n
* @note ● 执行时间: \n
*       ● 调用周期: 触发调用 \n
*       ● 可否打断: 可以 \n
*
* @par  注意:
*       ● 无 \n
*/	
void OledDisplayOff(void)
{
	OledWriteCmd(0X8D);  //SET DCDC命令
	OledWriteCmd(0X10);  //DCDC OFF
	OledWriteCmd(0XAE);  //DISPLAY OFF
}

/**
* @brief  OLED清屏函数
* @param  参数名 参数说明  
* @param  无
* @retval 无
* @par 使用全局变量 \n
* @note ● 执行时间: \n
*       ● 调用周期: 触发调用 \n
*       ● 可否打断: 可以 \n
*
* @par  注意:
*       ● 无 \n
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
* @brief  OLED显示一整屏函数
* @param  参数名 参数说明  
* @param  无
* @retval 无
* @par 使用全局变量 \n
* @note ● 执行时间: \n
*       ● 调用周期: 触发调用 \n
*       ● 可否打断: 可以 \n
*
* @par  注意:
*       ● 无 \n
*/
void OledDisp(void)
{
    static u8 i = 0;
    /* 上一次DMA已经传输完成 */
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
* @brief  OLED图片显示函数
* @param  参数名 参数说明  
* @param  [in] x x坐标, 0~127
* @param  [in] y y行数, 0~7
* @param  [in] width x宽度, 单位:像素 0~127
* @param  [in] hight y宽度, 单位:行 0~7
* @param  [in] data 需要显示内容
* @param  [in] xOffset x轴的偏移, 小于0表示向左偏移, 大于0表示向右偏移
* @param  [in] inverse 是否反色
* @retval 无
* @par 使用全局变量 \n
* @note ● 执行时间: \n
*       ● 调用周期: 触发调用 \n
*       ● 可否打断: 可以 \n
*
* @par  注意:
*       ● 此处只是将要显示的内容加入到显示缓存区中,真正显示由OledDisp()来做 \n
*/
void OledDisplay(u8 x, u8 y, char width, char hight, u8 *data, int xOffset, u8 inverse)
{
	int i;
	/* 左右边界 */
	int leftBound,rightBound;
	/* 显示的x轴起点, 显示的宽度, 数组偏移 */
	char startX = x + xOffset, widthShow = width, dataOffset = 0;
	
    /* 最多偏移128 */
	if(xOffset < -128)	xOffset = -128;
	else if(xOffset > 128)	xOffset = 128;
	
	/* 计算偏移后的左、右边界 */
	leftBound = (int)x + xOffset;
	rightBound = (int)x + (int)width + xOffset - 1;
	
    /* 如果左边界大于127 或 右边界小于0, 则整张图片都不在屏幕上, 不需要显示 */
	if(leftBound > 127)	return;
	if(rightBound < 0)	return;
	
	/* 若左边界小于0 */
	if(leftBound < 0)
	{
		/* 起点修正为0 */
		startX = 0;
		/* 修正显示数据起始位置 */
		dataOffset = -leftBound;
		/* 修正显示的宽度, x坐标小于0的区域不需要显示 */
		widthShow = width + leftBound;
	}
    /* 若右边界大于0 */
	else if(rightBound > 127)
	{
        /* 修正显示的宽度, x左标大于127的区域不需要显示 */
		widthShow = width - (rightBound - 127);
	}
	
    /* 对每一行数据操作 */
	for(i = 0;i < hight; i++)
	{
        addDataToBuf(startX, y+i, &data[i * width + dataOffset], widthShow, inverse);
	}
}

/**
* @brief  OLED字符串显示函数
* @param  参数名 参数说明  
* @param  [in] x x坐标, 0~127
* @param  [in] y y行数, 0~7
* @param  [in] chr 需要显示的字符串指针
* @param  [in] xOffset x轴的偏移, 小于0表示向左偏移, 大于0表示向右偏移
* @param  [in] inverse 是否反色
* @retval 无
* @par 使用全局变量 \n
* @note ● 执行时间: \n
*       ● 调用周期: 触发调用 \n
*       ● 可否打断: 可以 \n
*
* @par  注意:
*       ● 无 \n
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
* @brief  OLED单个字符显示函数
* @param  参数名 参数说明  
* @param  [in] x x坐标, 0~127
* @param  [in] y y行数, 0~7
* @param  [in] chr 需要显示的字符ASCII码
* @param  [in] xOffset x轴的偏移, 小于0表示向左偏移, 大于0表示向右偏移
* @param  [in] inverse 是否反色
* @retval 无
* @par 使用全局变量 \n
* @note ● 执行时间: \n
*       ● 调用周期: 触发调用 \n
*       ● 可否打断: 可以 \n
*
* @par  注意:
*       ● 无 \n
*/
static void OledShowChar(u8 x, u8 y, char chr, int xOffset, u8 inverse)
{      	
	unsigned char c=0;
	/* 得到偏移后的值, 即字模数组中该字符的位置 */	
	c = chr - ' ';			
	if(x > Max_Column - 6)
    {   
        x = 0;
        y += 1;
    }
	OledDisplay(x, y, 6, 1, (u8*)&F6x8[c][0], xOffset, inverse);
}

/**
* @brief  将要显示的数据以9bit格式放入显存
* @param  参数名 参数说明  
* @param  [in] x x坐标, 0~127
* @param  [in] y y行数, 0~7
* @param  [in] data 要显示的数据
* @param  [in] num 数据长度 
* @param  [in] inverse 是否反色
* @retval 无
* @par 使用全局变量 \n
* @note ● 执行时间: \n
*       ● 调用周期: 触发调用 \n
*       ● 可否打断: 可以 \n
*
* @par  注意:
*       ● 无 \n
*/
static void addDataToBuf(u8 x, u8 y, u8 *data, u16 num, u8 inverse)
{             
    u16 offset = offsetLine[y] + (x >> 3) * 9 + (x & 0x07);     // 字节偏移
    u16 bufSize = (num >> 3) * 9 + (num & 0x07);                // 转换成9bit所需要修改的总字节数
    u8 bitOffset = x & 0x07;                                    // 位偏移 
    u8 dstOffset = bitOffset;                                   // 与offset % 9等效;  // 目标位置的偏移(9个字节一组)        
    u8 *pDst = &DispBuf[offset];                                // 目标位置指针
    u8 *pSrc = data;                                            // 源数据指针
    u8 i;        
    u8 tmp, dataSrc0, dataSrc1;        
    
    if ((num & 0x07) || (x & 0x07))     // 如果长度不是8的倍数 或 偏移不是8的倍数
    {
        bufSize++;
    }
    if (((num & 0x07) + (x & 0x07)) > 8)
    {
        bufSize++;
    }
    
    /* 处理第一个字节 */
    dataSrc0 = (inverse ? ~*pSrc : *pSrc);
    tmp = *pDst & (0xff << (8 - bitOffset));
    *pDst = tmp + (0x80 >> bitOffset) + (dataSrc0 >> (bitOffset + 1));
    dstOffset++;        // bitOffset <= 7, 故destOffset <= 8, 不需做溢出处理
    
    /* 处理中间数据 */
    for (i = 1; i < (bufSize - 1); i++)
    {
        pDst++;
        dataSrc0 = (inverse ? ~*pSrc : *pSrc);
        dataSrc1 = (inverse ? ~*(pSrc + 1) : *(pSrc+1));
        /* 当前目标位置偏移 */
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
    /* 处理最后一个字节 */
    dataSrc0 = (inverse ? ~*pSrc : *pSrc);
    tmp = *pDst & (0xff >> dstOffset);
    *pDst = (dataSrc0 << (8 - dstOffset)) + tmp;
}

/**
* @brief  OLED设置画点位置函数
* @param  参数名 参数说明  
* @param  [in] x x坐标, 0~127
* @param  [in] y y行数, 0~7
* @retval 无
* @par 使用全局变量 \n
* @note ● 执行时间: \n
*       ● 调用周期: 触发调用 \n
*       ● 可否打断: 可以 \n
*
* @par  注意:
*       ● 无 \n
*/
static void OledSetPosition(u8 x, u8 y) 
{ 
	OledWriteCmd(0xb0 + y);
	OledWriteCmd(((x & 0xf0) >> 4)| 0x10);
	OledWriteCmd((x & 0x0f) | 0x01); 
}  

/**
* @brief  OLED引脚初始化函数
* @param  参数名 参数说明  
* @param  无  
* @retval 无
* @par 使用全局变量 \n
* @note ● 执行时间: \n
*       ● 调用周期: 触发调用, 初始化时调用 \n
*       ● 可否打断: 可以 \n
*
* @par  注意:
*       ● 无 \n
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
* @brief  向OLED控制器写命令
* @param  参数名 参数说明  
* @param  [in] dat 命令
* @retval 无
* @par 使用全局变量 \n
* @note ● 执行时间: \n
*       ● 调用周期: 触发调用 \n
*       ● 可否打断: 可以 \n
*
* @par  注意:
*       ● 无 \n
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

