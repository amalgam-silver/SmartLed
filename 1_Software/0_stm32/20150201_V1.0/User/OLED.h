#ifndef _OLED_H
#define _OLED_H

#include "main.h"

#define OLED_RST_Clr() GPIOA->BRR = GPIO_Pin_3 
#define OLED_RST_Set() GPIOA->BSRR = GPIO_Pin_3

#define OLED_CS_Clr() GPIOA->BRR = GPIO_Pin_4
#define OLED_CS_Set() GPIOA->BSRR = GPIO_Pin_4 

#define OLED_SCLK_Clr() GPIOA->BRR = GPIO_Pin_5
#define OLED_SCLK_Set() GPIOA->BSRR = GPIO_Pin_5

#define OLED_SDA_Clr() GPIOA->BRR = GPIO_Pin_7
#define OLED_SDA_Set() GPIOA->BSRR = GPIO_Pin_7 

#define SIZE 16
//#define XLevelL		0x00
//#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
//#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	

extern const unsigned char F8X16[];
extern const unsigned char F6x8[][6];
extern const unsigned char fontTimeNum[][28];
extern const unsigned char fontBattery[][10];
extern const unsigned char fontBLE[][10];
extern const unsigned char fontTimeColon[][16];

void oledInit(void);
void oledClear(void);
void oledSetPosition(unsigned char x, unsigned char y);
void oledDisplayOn(void);
void oledDisplayOff(void);
void oledShowChar(unsigned char x,unsigned char y, char chr,int xOffset,u8 colorInverse);
void oledShowString(unsigned char x,unsigned char y, char *chr,int xOffset,u8 colorInverse);
u8 num2string (char *str,u32 num,char dot);
void oledShowBattery(u8 x,u8 y,u8 state,int xOffset);
void oledShowBLE(u8 x,u8 y,u8 state,int xOffset);
void oledDisplay(char x, char y, char width, char hight, u8 *data, int xOffset,u8 colorInverse);
void oledSlid(u8 x,u8 y,u8 length,u8 point,u8 colorInverse);
void oledClearLind(u8 yStart,u8 yEnd);
//void oledShowBigTime(timeStructure time,u8 second);

#endif 
