/**
*************************************************************************************************
* @file    BspOled.h
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

#ifndef _BSP_OLED_H
#define _BSP_OLED_H

#include "DataType.h"

/** @brief �������3��SPI, ÿ���ֽڳ���8bit����, ����Ҫ��1bit��D/C�ź�;
       ��stm32f10x��֧��9bitӲ��SPI, ���ֶ���8bit����Ϊ9bit, ��������ԭ���� 9/8 �� */
#ifdef OLED_SPI_3WIRE

/** @brief һ����ʾ�����С */
#define PAGE_SIZE 1152  ///< 1024 / 8 * 9 = 1152 
#define LINE_SIZE 144   ///< 1152 / 8 = 144
/** @brief ÿһ�еĵ�һ�����ڻ������е�ƫ�� */
#define LINE1   0       ///< ��һ��
#define LINE2   144     ///< �ڶ���
#define LINE3   288     ///< ������
#define LINE4   432     ///< ������
#define LINE5   576     ///< ������
#define LINE6   720     ///< ������
#define LINE7   864     ///< ������
#define LINE8   1008    ///< �ڰ���

/** @brief д�����ݵĸ�ʽ */
typedef enum
{
    DATA_8BIT = 0,
    DATA_9BIT = 1,
}ENUM_DataFormat;

#else

/** @brief һ����ʾ�����С, 128 * 64 * 1bit = 1024 byte */
#define PAGE_SIZE 1024
#define LINE_SIZE 128   ///< 1024 / 8 = 128;
/** @brief ÿһ�еĵ�һ�����ڻ������е�ƫ�� */
#define LINE1   0       ///< ��һ��
#define LINE2   128     ///< �ڶ���
#define LINE3   256     ///< ������
#define LINE4   384     ///< ������
#define LINE5   512     ///< ������
#define LINE6   640     ///< ������
#define LINE7   768     ///< ������
#define LINE8   896     ///< �ڰ���

#endif

/** @brief ������� */
#define Max_Column	128
/** @brief ������� */
#define Max_Row		64

/** @brief С���� */
extern const unsigned char F6x8[][6];

/** @brief ��PA4,PA5,PA7��������Ϊ������� */
#define SPI_OUT_PP()  GPIOA->CRL &= 0x3f3fffff          
/** @brief ��PA5,PA7��������Ϊ����������� */
#define SPI_AF_PP()  do{GPIOA->CRL &= 0x3f3fffff; GPIOA->CRL |= 0x80800000;}while(0)

/** @brief OLED RST���� �õ� */
#define OLED_RST_Clr() GPIOA->BRR = GPIO_Pin_3 
/** @brief OLED RST���� �ø� */
#define OLED_RST_Set() GPIOA->BSRR = GPIO_Pin_3

/** @brief OLED CS���� �õ� */
#define OLED_CS_Clr() GPIOA->BRR = GPIO_Pin_4
/** @brief OLED CS���� �ø� */
#define OLED_CS_Set() GPIOA->BSRR = GPIO_Pin_4 

/** @brief OLED CLK���� �õ� */
#define OLED_SCLK_Clr() GPIOA->BRR = GPIO_Pin_5
/** @brief OLED CLK���� �ø� */
#define OLED_SCLK_Set() GPIOA->BSRR = GPIO_Pin_5

/** @brief OLED DATA���� �õ� */
#define OLED_SDA_Clr() GPIOA->BRR = GPIO_Pin_7
/** @brief OLED DATA���� �ø� */
#define OLED_SDA_Set() GPIOA->BSRR = GPIO_Pin_7 

/** @brief OLED��ʼ������ */	
void OledInit(void);
/** @brief OLEDʹ����ʾ���� */	
void OledDisplayOn(void);
/** @brief OLEDʧ����ʾ���� */	
void OledDisplayOff(void);
/** @brief OLED�������� */	  
void OledClear(void);  
/** @brief OLED��ʾ���� */
void OledDisp(void);

/** @brief OLEDͼƬ��ʾ���� */
void OledDisplay(u8 x, u8 y, char width, char hight, u8 *data, int xOffset, u8 inverse);
/** @brief OLED�ַ�����ʾ���� */
void OledShowString(u8 x, u8 y, char *chr, int xOffset, u8 inverse);

/** 

���� bit8 ���� -> bit9 ����
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

xΪ8bit���� yΪ9bit����

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


