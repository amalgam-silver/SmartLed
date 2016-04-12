#include "RTC_RX8025.h"

u32 systemSecond = 0;

void modeConfigurationRX8025(void)
{
	u8 registerRX8025[16] = {0x00,0x00,0x00,0x04,0x01,0x01,0x15,0x00,
							 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0};
	u8 temp[16];
	
	EXTI_InitTypeDef   EXTI_InitStructure;
	GPIO_InitTypeDef   GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	I2CReadRegister(RX8025_ADDR,(RX8025_ADDR_SECONDS&RX8025_WRITE_MODE),temp,16);
	
	if(temp[14] & 0x02)
	{
		I2CWriteRegister(RX8025_ADDR,(RX8025_ADDR_SECONDS&RX8025_WRITE_MODE),registerRX8025,16);	
	}

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource9);

	EXTI_InitStructure.EXTI_Line = EXTI_Line9;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

u8 timeSet(timeStructure time)
{
	u8 timeSetTemp[8];
	
	timeSetTemp[0] = time.second / 10;
	timeSetTemp[0] <<= 4;
	timeSetTemp[0] += time.second % 10;
	
	timeSetTemp[1] = time.minute / 10;
	timeSetTemp[1] <<= 4;
	timeSetTemp[1] += time.minute % 10;
	
	timeSetTemp[2] = time.hour / 10;
	timeSetTemp[2] <<= 4;
	timeSetTemp[2] += time.hour % 10;
	
	timeSetTemp[3] = time.weekday;
	
	timeSetTemp[4] = time.data / 10;
	timeSetTemp[4] <<= 4;
	timeSetTemp[4] += time.data % 10;
	
	timeSetTemp[5] = time.month / 10;
	timeSetTemp[5] <<= 4;
	timeSetTemp[5] += time.month % 10;
	
	timeSetTemp[6] = time.year / 10;
	timeSetTemp[6] <<= 4;
	timeSetTemp[6] += time.year % 10;
	
	return I2CWriteRegister(RX8025_ADDR,(RX8025_ADDR_SECONDS&RX8025_WRITE_MODE),timeSetTemp,8);
}

u8 timeRead(timeStructure *time)
{
	u8 timeSetTemp[8];
	
	if(!I2CReadRegister(RX8025_ADDR,(RX8025_ADDR_SECONDS&RX8025_READ_MODE),timeSetTemp,8))
		return 0;
	
	time->second = (timeSetTemp[0] >> 4) * 10 + (timeSetTemp[0] & 0x0f);
	time->minute = (timeSetTemp[1] >> 4) * 10 + (timeSetTemp[1] & 0x0f);
	time->hour = (timeSetTemp[2] >> 4) * 10 + (timeSetTemp[2] & 0x0f);
	time->weekday = timeSetTemp[3];
	time->data = (timeSetTemp[4] >> 4) * 10 + (timeSetTemp[4] & 0x0f);
	time->month = (timeSetTemp[5] >> 4) * 10 + (timeSetTemp[5] & 0x0f);
	time->year = (timeSetTemp[6] >> 4) * 10 + (timeSetTemp[6] & 0x0f);
		
	return 1;
	
}

void time2String(timeStructure time, char *string)
{
	string[0] = time.hour / 10 + '0';
	string[1] = time.hour % 10 + '0';
	string[2] = ':';
	
	string[3] = time.minute / 10 + '0';
	string[4] = time.minute % 10 + '0';
	string[5] = ':';
	
	string[6] = time.second / 10 + '0';
	string[7] = time.second % 10 + '0';
	string[8] = ' ';
	string[9] = ' ';
	string[10] = ' ';	
	
	string[11] = '2';
	string[12] = '0';
	string[13] = time.year / 10 + '0';
	string[14] = time.year % 10 + '0';
	string[15] = '-';
	
	string[16] = time.month / 10 + '0';
	string[17] = time.month % 10 + '0';
	string[18] = '-';
	
	string[19] = time.data / 10 + '0';
	string[20] = time.data % 10 + '0';
	string[21] = '\0';
}

void data2String(timeStructure time, char *string)
{
	string[0] = time.month / 10 + '0';
	string[1] = time.month % 10 + '0';
	string[2] = '-';
	
	string[3] = time.data / 10 + '0';
	string[4] = time.data % 10 + '0';
	string[5] = '\0';
}

