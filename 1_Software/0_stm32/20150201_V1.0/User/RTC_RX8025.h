#ifndef RTC_RX8025_H
#define RTC_RX8025_H

#include "main.h"

//设备地址
#define	RX8025_ADDR		0x64
// 设备寄存器地址
#define	RX8025_ADDR_SECONDS		0x00
#define	RX8025_ADDR_WEEK		0x30
#define	RX8025_ADDR_DATES		0x40
#define	RX8025_ADDR_MONTH		0x50
#define	RX8025_ADDR_MINUTES		0x80
#define RX8025_ADDR_EXTENSION	0xD0
#define	RX8025_ADDR_FLAG		0xE0
#define	RX8025_ADDR_CONTROL		0xF0
// 设备操作模式
#define	RX8025_WRITE_MODE		0xF0
#define	RX8025_READ_MODE		0xF0 
#define	RX8025_SIMP_READ_MODE	0x04

#define SUNDAY 		0x01
#define MONDAY 		0x02
#define TUESDAY		0x04
#define WEDNESDAY	0x08
#define THURSDAY	0x10
#define FRIDAY 		0x20
#define SATURDAY	0x40

typedef struct
{
	u8 second;
	u8 minute;
	u8 hour;
	u8 weekday;
	u8 data;
	u8 month;
	u8 year;
}timeStructure;

extern u32 systemSecond;

void modeConfigurationRX8025(void);
u8 timeRead(timeStructure *time);
u8 timeSet(timeStructure time);
void time2String(timeStructure time, char *string);
void data2String(timeStructure time, char *string);
void EXTI9_5_IRQHandler(void);

#endif
