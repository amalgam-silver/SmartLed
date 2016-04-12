#include "OLED.h"

/************�ڲ����ú���Ԥ����*************/

void oledGpioInit(void);
void oledWriteData(unsigned char dat);
void oledWriteCmd(unsigned char dat);

/****************************************************************************
*	��������oledInit
	��  �룺void
	��  ����void
	��  �ܣ�OLED��ʼ������
	��  ע����
*	��  �ڣ�2015.02.01
****************************************************************************/	  
void oledInit(void)
{
	oledGpioInit();
	OLED_RST_Set();
	delayMs(100);
	OLED_RST_Clr();
	delayMs(100);
	OLED_RST_Set(); 
					  
	oledWriteCmd(0xAE);//--turn off oled panel
	oledWriteCmd(0x02);//---set low column address
	oledWriteCmd(0x10);//---set high column address
	oledWriteCmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	oledWriteCmd(0x81);//--set contrast control register
	oledWriteCmd(0xCF); // Set SEG Output Current Brightness
	oledWriteCmd(0xA1);//--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
	oledWriteCmd(0xC8);//Set COM/Row Scan Direction   0xc0���·��� 0xc8����
	oledWriteCmd(0xA6);//--set normal display
	oledWriteCmd(0xA8);//--set multiplex ratio(1 to 64)
	oledWriteCmd(0x3f);//--1/64 duty
	oledWriteCmd(0xD3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	oledWriteCmd(0x00);//-not offset
	oledWriteCmd(0xd5);//--set display clock divide ratio/oscillator frequency
	oledWriteCmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
	oledWriteCmd(0xD9);//--set pre-charge period
	oledWriteCmd(0xF1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	oledWriteCmd(0xDA);//--set com pins hardware configuration
	oledWriteCmd(0x12);
	oledWriteCmd(0xDB);//--set vcomh
	oledWriteCmd(0x40);//Set VCOM Deselect Level
	oledWriteCmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
	oledWriteCmd(0x02);//
	oledWriteCmd(0x8D);//--set Charge Pump enable/disable
	oledWriteCmd(0x14);//--set(0x10) disable
	oledWriteCmd(0xA4);// Disable Entire Display On (0xa4/0xa5)
	oledWriteCmd(0xA6);// Disable Inverse Display On (0xa6/a7) 
	oledWriteCmd(0xAF);//--turn on oled panel
	
	oledWriteCmd(0xAF); /*display ON*/ 
	oledClear();
	oledSetPosition(0,0); 	
}

/****************************************************************************
*	��������oledClear
	��  �룺void
	��  ����void
	��  �ܣ���������
	��  ע��������,������Ļ�Ǻ�ɫ��,��û����һ��.
*	��  �ڣ�2015.02.01
****************************************************************************/	  
void oledClear(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		oledWriteCmd (0xb0+i);    //����ҳ��ַ��0~7��
		oledWriteCmd (0x02);      //������ʾλ�á��е͵�ַ
		oledWriteCmd (0x10);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)	oledWriteData(0); 
	} 
}

/****************************************************************************
*	��������oledSetPosition
	��  �룺unsigned char x ������ unsigned char y ������
	��  ����void
	��  �ܣ����õ�ǰ����λ��
	��  ע��x��Χ:0-127		y��Χ:0-7
*	��  �ڣ�2015.02.01
****************************************************************************/
void oledSetPosition(unsigned char x, unsigned char y) 
{ 
	oledWriteCmd(0xb0+y);
	oledWriteCmd(((x&0xf0)>>4)|0x10);
	oledWriteCmd((x&0x0f)|0x01); 
}  

/****************************************************************************
*	��������oledDisplayOn
	��  �룺void
	��  ����void
	��  �ܣ�����OLED��ʾ
	��  ע����
*	��  �ڣ�2015.02.01
****************************************************************************/   
void oledDisplayOn(void)
{
	oledWriteCmd(0X8D);  //SET DCDC����
	oledWriteCmd(0X14);  //DCDC ON
	oledWriteCmd(0XAF);  //DISPLAY ON
}

/****************************************************************************
*	��������oledDisplayOff
	��  �룺void
	��  ����void
	��  �ܣ��ر�OLED��ʾ
	��  ע����
*	��  �ڣ�2015.02.01
****************************************************************************/
void oledDisplayOff(void)
{
	oledWriteCmd(0X8D);  //SET DCDC����
	oledWriteCmd(0X10);  //DCDC OFF
	oledWriteCmd(0XAE);  //DISPLAY OFF
}	

/****************************************************************************
*	��������OLED_ShowChar
	��  �룺u8 x ������,u8 y������,u8 chr Ҫ��ʾ���ַ�,char x��ƫ����
	��  ����void
	��  �ܣ���ָ��λ����ʾһ���ַ�
	��  ע��x��Χ:0-127		y��Χ:0-7
*	��  �ڣ�2015.02.01
****************************************************************************/
void oledShowChar(unsigned char x,unsigned char y,char chr,int xOffset,u8 colorInverse)
{      	
	unsigned char c=0;
	//�õ�ƫ�ƺ��ֵ	
	c=chr-' ';			
	if(x>Max_Column-1){x=0;y=y+1;}
	
	oledDisplay(x,y,6,1,(u8*)&F6x8[c][0],xOffset,colorInverse);
//	oledSetPosition(x,y);
//	for(i=0;i<6;i++)
//	{
//		oledWriteData(F6x8[c][i]);
//	//	delayMs(500);
//	}
}

/****************************************************************************
*	��������oledShowString
	��  �룺u8 x ������,u8 y������,u8 *chr �ַ���ָ��
	��  ����void
	��  �ܣ���ָ��λ����ʾһ���ַ���
	��  ע��x��Χ:0-127		y��Χ:0-7
*	��  �ڣ�2015.02.01
****************************************************************************/
void oledShowString(unsigned char x,unsigned char y,char *chr,int xOffset,u8 colorInverse)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{	
		oledShowChar(x,y,chr[j],xOffset,colorInverse);
		x+=6;
		if(x>120)
		{
			x=0;y+=1;
		}
		j++;
	}
}

/****************************************************************************
	
	�������ܣ�	������ת��Ϊ�ַ���
	���룺		char *str �����洢�ַ���������,
				u32 num ��Ҫת��������,
				char dot С������м�λ
	���أ�		��\0����str���ǵ�ƫ����
	��������:	2014-4-2
	��ע��		����ʵ��:
					char DCvoltage1[20];
					u32 temp16 = 3312;
					char temp;
					temp = num2string(DCvoltage1,temp16,1);
					DCvoltage1[temp] = ' ';
					DCvoltage1[temp+1] = 'V';
					DCvoltage1[temp+2] = '\0';

					���   DCvoltage1 = ��331.2 V��

****************************************************************************/
u8 num2string (char *str,u32 num,char dot)
{
	char weishu=0;
	u32 temp=0;
	char i=0,j=0;
	if(!num)
	{
		str[0] = '0';
		return 1;
	}
	else
	{
		//����num�Ǽ�λ��	
		temp = num;
		while(temp)
		{
			temp /= 10;
			weishu++;
		}
		temp = num;
		if(weishu<=dot)
		{
			str[0] = '0';
			str[1] = '.';
			for(i=0;i<(dot-weishu);i++)
				str[2+i] = '0';
			for(j=weishu;j>0;j--)
			{	
				str[2+i+j-1] = (temp%10) + 48;
				temp /= 10;
			}
			return (dot + 2);
		}
		else
		{
			if(dot)
			{
				for(i=weishu+1;i>0;i--)
				{
					if((weishu + 1 - i)<dot)
					{
						str[i - 1] = (temp%10) + 48;
						temp /= 10;
					}
					else if((weishu + 1 - i) == dot)
					{
						str[i - 1] = '.';
					}
					else
					{
						str[i - 1] = (temp%10) + 48;
						temp /= 10;
					}
				}
				return (weishu+1);
			}
			else
			{
				for(i=weishu;i>0;i--)
				{
					str[i-1] = (temp%10) + 48;
					temp /= 10;
				}
				return weishu;	
			}	
		}
	}	
}

void oledClearLind(u8 yStart,u8 yEnd)  
{  
	u8 i,n;	
	
	for(i=0;i<(yEnd-yStart+1);i++)  
	{  
		oledSetPosition(0,yStart+i);
		for(n=0;n<128;n++)	oledWriteData(0); 
	} 
}

void oledShowBigNum(u8 x,u8 y,u8 num, int xOffset)
{  			    	
	oledDisplay(x, y, 28, 6, (u8*)(&fontTimeNum[num*6][0]), xOffset, 0);				
}

void oledShowBigTime(timeStructure time,u8 second, int xOffset)
{
	u8 temp[4];
	
	temp[0] = time.hour / 10;
	temp[1] = time.hour % 10;
	temp[2] = time.minute / 10;
	temp[3] = time.minute % 10;
	
	if(second & 0x01)
	{
		
		oledDisplay(56, 2, 16, 6, (u8*)(&fontTimeColon[0][0]), xOffset, 0);
	}
	else
	{
		oledDisplay(56, 2, 16, 6, (u8*)(&fontTimeColon[6][0]), xOffset, 0);
	}
	oledShowBigNum(0,2,temp[0],xOffset);
	oledShowBigNum(28,2,temp[1],xOffset);
	oledShowBigNum(72,2,temp[2],xOffset);
	oledShowBigNum(100,2,temp[3],xOffset);

}

void oledShowBattery(u8 x,u8 y,u8 state,int xOffset)
{
	oledDisplay(x,y,10,2,(u8*)&fontBattery[state*2][0],xOffset,0);
}

void oledShowBLE(u8 x,u8 y,u8 state,int xOffset)
{
	
	oledDisplay(x,y,10,2,(u8*)&fontBLE[state*2][0],xOffset,0);
}



void oledDisplay(char x, char y, char width, char hight, u8 *data, int xOffset, u8 colorInverse)
{
	int i,j;
	//���ұ߽�
	int leftBound,rightBound;
	//��ʾ��x�����, ��ʾ�Ŀ��, ����ƫ��
	char startX = x + xOffset, widthShow = width, dataOffset = 0;
	
	if(xOffset < -128)	xOffset = -128;
	else if(xOffset > 128)	xOffset = 128;
	
	//���ұ߽�
	leftBound = (int)x + xOffset;
	rightBound = (int)x + (int)width + xOffset;
	
	if(leftBound > 127)	return;
	if(rightBound < 0)	return;
	
	//����߽�С��0
	if(leftBound < 0)
	{
		//�������Ϊ0
		startX = 0;
		//������ʾ������ʼλ��
		dataOffset = -leftBound;
		//������ʾ�Ŀ��
		widthShow = width + leftBound;
	}
	else if(rightBound > 127)
	{
		widthShow = width - (rightBound - 127);
	}
	
	for(i=0;i<hight;i++)
	{
		if(xOffset == 0)
		{
			oledSetPosition(startX,y+i);
		}
		else if(startX >= 2)	
		{
			oledSetPosition(startX-2,y+i);
			if(!colorInverse)
			{
				oledWriteData(0x00);
				oledWriteData(0x00);
			}
			else
			{
				oledWriteData(0xff);
				oledWriteData(0xff);
			}
		}
		else if(startX == 1)
		{
			oledSetPosition(startX-1,y+i);
			if(!colorInverse)
			{
				oledWriteData(0x00);
			}
			else
			{
				oledWriteData(0xff);
			}
		}
		else
			oledSetPosition(startX,y+i);
		for(j=0;j<widthShow;j++)
		{	
			if(!colorInverse)
			{
				oledWriteData(data[i * width + j + dataOffset]);
			}
			else
			{
				oledWriteData(~data[i * width + j + dataOffset]);
			}
			
		}
		if(xOffset == 0);
		else if((127 - rightBound) >= 2)
		{
			if(!colorInverse)
			{
				oledWriteData(0x00);
				oledWriteData(0x00);
			}
			else
			{
				oledWriteData(0xff);
				oledWriteData(0xff);
			}
		}
		else if((127 - rightBound) == 1)
		{
			if(!colorInverse)
			{
				oledWriteData(0x00);
			}
			else
			{
				oledWriteData(0xff);
			}	
		}
	}
}

void oledSlid(u8 x,u8 y,u8 length,u8 point,u8 colorInverse)
{
	int i;
	oledSetPosition(x,y);
	for(i=0;i<length;i++)
	{
		if(!colorInverse)
			oledWriteData(0x18);
		else
			oledWriteData(0xe7);
	}
	oledSetPosition(x+point,y);	
	if(colorInverse)
	{
		oledWriteData(0x81);
		oledWriteData(0x81);
	}
	else
	{
		oledWriteData(0xff);
		oledWriteData(0xff);
	}
}
	
	
	
/****************************************************************************
*	��������oledGpioInit
	��  �룺void
	��  ����void
	��  �ܣ�OLEDʹ�õ�IO�ڳ�ʼ��
	��  ע����
*	��  �ڣ�2015.02.01
****************************************************************************/
void oledGpioInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/****************************************************************************
*	��������oledWriteData
	��  �룺void
	��  ����u8 dat	����
	��  �ܣ���OLEDд����
	��  ע����
*	��  �ڣ�2015.02.01
****************************************************************************/
void oledWriteData(unsigned char dat)
{	
	u8 i;			  		  
	OLED_CS_Clr();
	
	OLED_SCLK_Clr();
	OLED_SDA_Set();
	OLED_SCLK_Set();
	
	for(i=0;i<8;i++)
	{			  
		OLED_SCLK_Clr();
		if(dat&0x80)
			OLED_SDA_Set();
		else 
			OLED_SDA_Clr();
		OLED_SCLK_Set();
		dat<<=1;   
	}				 		  
	OLED_CS_Set();  	  
} 

/****************************************************************************
*	��������oledWriteCmd
	��  �룺void
	��  ����u8 dat	ָ��
	��  �ܣ���OLEDдָ��
	��  ע����
*	��  �ڣ�2015.02.01
****************************************************************************/
void oledWriteCmd(unsigned char dat)
{	
	u8 i;			  		  
	OLED_CS_Clr();
	
	OLED_SCLK_Clr();
	OLED_SDA_Clr();
	OLED_SCLK_Set();
	
	for(i=0;i<8;i++)
	{			  
		OLED_SCLK_Clr();
		if(dat&0x80)
			OLED_SDA_Set();
		else 
			OLED_SDA_Clr();
		OLED_SCLK_Set();
		dat<<=1;   
	}				 		  
	OLED_CS_Set();  	  
} 
