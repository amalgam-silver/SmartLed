/**
*************************************************************************************************
* @file    BspOled.h
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

#ifndef _BSP_OLED_H
#define _BSP_OLED_H

#include "DataType.h"

/** @brief 如果采用3线SPI, 每个字节除了8bit数据, 还需要发1bit的D/C信号;
       而stm32f10x不支持9bit硬件SPI, 故手动将8bit扩充为9bit, 缓冲区是原来的 9/8 倍 */
#ifdef OLED_SPI_3WIRE

/** @brief 一屏显示缓存大小 */
#define PAGE_SIZE 1152  ///< 1024 / 8 * 9 = 1152 
#define LINE_SIZE 144   ///< 1152 / 8 = 144
/** @brief 每一行的第一个列在缓冲区中的偏移 */
#define LINE1   0       ///< 第一行
#define LINE2   144     ///< 第二行
#define LINE3   288     ///< 第三行
#define LINE4   432     ///< 第四行
#define LINE5   576     ///< 第五行
#define LINE6   720     ///< 第六行
#define LINE7   864     ///< 第七行
#define LINE8   1008    ///< 第八行

/** @brief 写入数据的格式 */
typedef enum
{
    DATA_8BIT = 0,
    DATA_9BIT = 1,
}ENUM_DataFormat;

#else

/** @brief 一屏显示缓存大小, 128 * 64 * 1bit = 1024 byte */
#define PAGE_SIZE 1024
#define LINE_SIZE 128   ///< 1024 / 8 = 128;
/** @brief 每一行的第一个列在缓冲区中的偏移 */
#define LINE1   0       ///< 第一行
#define LINE2   128     ///< 第二行
#define LINE3   256     ///< 第三行
#define LINE4   384     ///< 第四行
#define LINE5   512     ///< 第五行
#define LINE6   640     ///< 第六行
#define LINE7   768     ///< 第七行
#define LINE8   896     ///< 第八行

#endif

/** @brief 最大列数 */
#define Max_Column	128
/** @brief 最大行数 */
#define Max_Row		64

/** @brief 小字体 */
extern const unsigned char F6x8[][6];

/** @brief 将PA4,PA5,PA7引脚设置为推挽输出 */
#define SPI_OUT_PP()  GPIOA->CRL &= 0x3f3fffff          
/** @brief 将PA5,PA7引脚设置为复用推挽输出 */
#define SPI_AF_PP()  do{GPIOA->CRL &= 0x3f3fffff; GPIOA->CRL |= 0x80800000;}while(0)

/** @brief OLED RST引脚 置低 */
#define OLED_RST_Clr() GPIOA->BRR = GPIO_Pin_3 
/** @brief OLED RST引脚 置高 */
#define OLED_RST_Set() GPIOA->BSRR = GPIO_Pin_3

/** @brief OLED CS引脚 置低 */
#define OLED_CS_Clr() GPIOA->BRR = GPIO_Pin_4
/** @brief OLED CS引脚 置高 */
#define OLED_CS_Set() GPIOA->BSRR = GPIO_Pin_4 

/** @brief OLED CLK引脚 置低 */
#define OLED_SCLK_Clr() GPIOA->BRR = GPIO_Pin_5
/** @brief OLED CLK引脚 置高 */
#define OLED_SCLK_Set() GPIOA->BSRR = GPIO_Pin_5

/** @brief OLED DATA引脚 置低 */
#define OLED_SDA_Clr() GPIOA->BRR = GPIO_Pin_7
/** @brief OLED DATA引脚 置高 */
#define OLED_SDA_Set() GPIOA->BSRR = GPIO_Pin_7 

/** @brief OLED初始化函数 */	
void OledInit(void);
/** @brief OLED使能显示函数 */	
void OledDisplayOn(void);
/** @brief OLED失能显示函数 */	
void OledDisplayOff(void);
/** @brief OLED清屏函数 */	  
void OledClear(void);  
/** @brief OLED显示整屏 */
void OledDisp(void);

/** @brief OLED图片显示函数 */
void OledDisplay(u8 x, u8 y, char width, char hight, u8 *data, int xOffset, u8 inverse);
/** @brief OLED字符串显示函数 */
void OledShowString(u8 x, u8 y, char *chr, int xOffset, u8 inverse);

/** 

关于 bit8 数据 -> bit9 数据
    bit   0      1       2       3       4       5       6       7
byte

0       D/C		BIT7	BIT6	BIT5	BIT4	BIT3	BIT2	BIT1
1 	    BIT0	D/C		BIT7	BIT6	BIT5	BIT4	BIT3	BIT2	
2 	    BIT1	BIT0	D/C		BIT7	BIT6	BIT5	BIT4	BIT3
3 	    BIT2	BIT1	BIT0	D/C		BIT7	BIT6	BIT5	BIT4
4 	    BIT3	BIT2	BIT1	BIT0	D/C		BIT7	BIT6	BIT5
5	    BIT4	BIT3	BIT2	BIT1	BIT0	D/C		BIT7	BIT6
6	    BIT5	BIT4	BIT3	BIT2	BIT1	BIT0	D/C		BIT7
7	    BIT6	BIT5	BIT4	BIT3	BIT2	BIT1	BIT0	D/C
8	    BIT7	BIT6	BIT5	BIT4	BIT3	BIT2	BIT1	BIT0
9       D/C		BIT7	BIT6	BIT5	BIT4	BIT3	BIT2	BIT1
10	    BIT0	D/C		BIT7	BIT6	BIT5	BIT4	BIT3	BIT2	
11	    BIT1	BIT0	D/C		BIT7	BIT6	BIT5	BIT4	BIT3
12	    BIT2	BIT1	BIT0	D/C		BIT7	BIT6	BIT5	BIT4
13	    BIT3	BIT2	BIT1	BIT0	D/C		BIT7	BIT6	BIT5
14      BIT4	BIT3	BIT2	BIT1	BIT0	D/C		BIT7	BIT6
15	    BIT5	BIT4	BIT3	BIT2	BIT1	BIT0	D/C		BIT7
16	    BIT6	BIT5	BIT4	BIT3	BIT2	BIT1	BIT0	D/C
17	    BIT7	BIT6	BIT5	BIT4	BIT3	BIT2	BIT1	BIT0
18      D/C		BIT7	BIT6	BIT5	BIT4	BIT3	BIT2	BIT1
19 	    BIT0	D/C		BIT7	BIT6	BIT5	BIT4	BIT3	BIT2	
20 	    BIT1	BIT0	D/C		BIT7	BIT6	BIT5	BIT4	BIT3

x为8bit数据 y为9bit数据

y[0] = 		  		 0x80 | (x[0] >> 1);
y[1] = (x[0] << 7) | 0x40 | (x[1] >> 2);
y[2] = (x[1] << 6) | 0x20 | (x[2] >> 3);
y[3] = (x[2] << 5) | 0x10 | (x[3] >> 4);
y[4] = (x[3] << 4) | 0x08 | (x[4] >> 5);
y[5] = (x[4] << 3) | 0x04 | (x[5] >> 6);
y[6] = (x[5] << 2) | 0x02 | (x[6] >> 7);
y[7] = (x[6] << 1) | 0x01;
y[8] = x[7];

*/

#endif 
/**  @} */


