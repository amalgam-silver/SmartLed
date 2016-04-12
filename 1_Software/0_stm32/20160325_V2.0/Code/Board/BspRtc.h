/**
 *************************************************************************************************
 * @file    BspRtc.h
 * @author  Ge Yue
 * @version V1.0.0
 * @date    2016.03.18
 * @brief   RX8025实时时钟芯片驱动
 *************************************************************************************************
 @verbatim 
 
  - 版本号       日期      创建人        描述 
    V1.0      2016.03.18   Ge Yue     RX8025实时时钟芯片驱动; 
 
  - 版本号       日期      修改人        描述 
 @endverbatim 
 */

/**
 * @defgroup RTC BspRtc: BSP Group
 * @{
 */

#ifndef _BSP_RTC_H
#define _BSP_RTC_H

#include "DataType.h"

/** @brief I2C设备地址 */
#define	RX8025_ADDR		0x64
/** @brief 设备寄存器地址 */
#define	RX8025_ADDR_SECONDS		0x00        ///< 秒寄存器
#define	RX8025_ADDR_WEEK		0x30        ///< 星期寄存器
#define	RX8025_ADDR_DATES		0x40        ///< 日期寄存器
#define	RX8025_ADDR_MONTH		0x50        ///< 月寄存器
#define	RX8025_ADDR_MINUTES		0x80        ///< 分钟寄存器
#define RX8025_ADDR_EXTENSION	0xD0        ///< 扩展寄存器
#define	RX8025_ADDR_FLAG		0xE0        ///< 标志位寄存器
#define	RX8025_ADDR_CONTROL		0xF0        ///< 控制寄存器
/** @brief 设备操作模式 */
#define	RX8025_WRITE_MODE		0xF0        ///< 写模式
#define	RX8025_READ_MODE		0xF0        ///< 读模式

/** @brief 星期设置 */
#define SUNDAY 		0x01    ///< 周日       
#define MONDAY 		0x02    ///< 周一
#define TUESDAY		0x04    ///< 周二
#define WEDNESDAY	0x08    ///< 周三
#define THURSDAY	0x10    ///< 周四
#define FRIDAY 		0x20    ///< 周五
#define SATURDAY	0x40    ///< 周六

/** @brief 时间结构体 */
typedef struct
{
	u8 sec;         ///< 秒
	u8 min;         ///< 分
	u8 hour;        ///< 时
	u8 week;        ///< 星期
	u8 date;        ///< 日
	u8 month;       ///< 月
	u8 year;        ///< 年, 实际年数-2000
}STR_Time;

/** @brief 实时时钟结构体 */
typedef struct
{
    u8 en;          ///< 使能
    u8 i2cBuf[16];  ///< I2c通讯接收buf
    STR_Time time;  ///< 实时时间
}STR_Rtc;

/** @brief 实时时钟结构体 */
extern STR_Rtc Rtc;

/** @brief  实时时钟初始函数 */
void RtcInit(void);
/** @brief  设置时间 */
u8 TimeSet(STR_Time *time);
/** @brief  读取时间 */
u8 TimeRead(STR_Time *time);
/** @brief  获取日期字符串用于显示 */
void GetDateString(STR_Time *time, char *string);
/** @brief  获取时间字符串用于显示 */
void GetTimeString(STR_Time *time, char *string);
/** @brief  读取时间 */
void ReadTimeDMA(STR_Rtc *rtc);
/** @brief  读取时间 */
void GetTimeDMA(STR_Rtc *rtc);

#endif

/**  @} */


