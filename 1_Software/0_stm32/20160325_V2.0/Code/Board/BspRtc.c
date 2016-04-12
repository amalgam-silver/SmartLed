/**
 *************************************************************************************************
 * @file    BspRtc.c
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

#include "BspRtc.h"
#include "Bsp.h"

/** @brief 实时时钟结构体 */
STR_Rtc Rtc;

/**
 * @brief  实时时钟初始化函数
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
void RtcInit(void)
{
	u8 registerRX8025[16] = {0x00,0x00,0x00,0x04,0x01,0x01,0x15,0x00,
							 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0};
	u8 temp[16];
	
    /* 初始化I2C控制器 */                         
    I2CInit();
                             
    /* 读取RX8025的16个寄存器 */                         
	I2CReadRegister(RX8025_ADDR, (RX8025_ADDR_SECONDS & RX8025_WRITE_MODE), temp, 16); 
	
    /* 如果标志位寄存器的bit1(数据丢失位)置位，则认为数据丢失，重新设置寄存器 */
	if(temp[14] & 0x02)
	{
		I2CWriteRegister(RX8025_ADDR, (RX8025_ADDR_SECONDS & RX8025_WRITE_MODE), registerRX8025, 16);	
	}
    
    /* 读取时间 */
    Rtc.en = 1;
}

/**
 * @brief  设置时间
 * @param  参数名 参数说明  
 * @param  [in] time 时间结构体指针  
 * @retval 0-失败 1-成功
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 触发调用 \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ●  \n
 */
u8 TimeSet(STR_Time *time)
{
	u8 timeSetTemp[8];
	
	timeSetTemp[0] = time->sec / 10;
	timeSetTemp[0] <<= 4;
	timeSetTemp[0] += time->sec % 10;
	
	timeSetTemp[1] = time->min / 10;
	timeSetTemp[1] <<= 4;
	timeSetTemp[1] += time->min % 10;
	
	timeSetTemp[2] = time->hour / 10;
	timeSetTemp[2] <<= 4;
	timeSetTemp[2] += time->hour % 10;
	
	timeSetTemp[3] = time->week;
	
	timeSetTemp[4] = time->date / 10;
	timeSetTemp[4] <<= 4;
	timeSetTemp[4] += time->date % 10;
	
	timeSetTemp[5] = time->month / 10;
	timeSetTemp[5] <<= 4;
	timeSetTemp[5] += time->month % 10;
	
	timeSetTemp[6] = time->year / 10;
	timeSetTemp[6] <<= 4;
	timeSetTemp[6] += time->year % 10;
	
	return I2CWriteRegister(RX8025_ADDR,(RX8025_ADDR_SECONDS&RX8025_WRITE_MODE),timeSetTemp,8);
}

/**
 * @brief  读取时间
 * @param  参数名 参数说明  
 * @param  [out] time 时间结构体指针  
 * @retval 0-失败 1-成功
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 100ms调用 \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 等待方式I2C读取数据，耗时较长，2.0以上版本已经不再使用 \n
 */
u8 GetTime(STR_Time *time)
{    
	u8 timeSetTemp[8];
	
	if(!I2CReadRegister(RX8025_ADDR,(RX8025_ADDR_SECONDS&RX8025_READ_MODE),timeSetTemp,8))
		return 0;
	
	time->sec = (timeSetTemp[0] >> 4) * 10 + (timeSetTemp[0] & 0x0f);
	time->min = (timeSetTemp[1] >> 4) * 10 + (timeSetTemp[1] & 0x0f);
	time->hour = (timeSetTemp[2] >> 4) * 10 + (timeSetTemp[2] & 0x0f);
	time->week = timeSetTemp[3];
	time->date = (timeSetTemp[4] >> 4) * 10 + (timeSetTemp[4] & 0x0f);
	time->month = (timeSetTemp[5] >> 4) * 10 + (timeSetTemp[5] & 0x0f);
	time->year = (timeSetTemp[6] >> 4) * 10 + (timeSetTemp[6] & 0x0f);
		
	return 1;
}

/**
 * @brief  读取时间
 * @param  参数名 参数说明  
 * @param  [out] rtc 时间结构体指针  
 * @retval 无
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 100ms调用 \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 通过DMA I2C读取RX8025的寄存器，结果存储在TimeNow.i2cBuf中 \n
 */
void ReadTimeDMA(STR_Rtc *rtc)
{
    if ((rtc->en) && (!I2c.busy))
    {
        I2CReadRegDMA(RX8025_ADDR, (RX8025_ADDR_SECONDS & RX8025_READ_MODE), rtc->i2cBuf, 8); 
    }
}

/**
 * @brief  读取时间
 * @param  参数名 参数说明  
 * @param  [out] rtc 时间结构体指针  
 * @retval 无
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 100ms调用 \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 根据TimeNow.i2cBuf中读到的寄存器值, 更新时间结构体中的各个变量 \n
 */
void GetTimeDMA(STR_Rtc *rtc)
{
    /* 将BCD码转为10进制数 */
    rtc->time.sec   = (rtc->i2cBuf[0] >> 4) * 10 + (rtc->i2cBuf[0] & 0x0f);
    rtc->time.min   = (rtc->i2cBuf[1] >> 4) * 10 + (rtc->i2cBuf[1] & 0x0f);
    rtc->time.hour  = (rtc->i2cBuf[2] >> 4) * 10 + (rtc->i2cBuf[2] & 0x0f);
    rtc->time.week  = rtc->i2cBuf[3];
    rtc->time.date  = (rtc->i2cBuf[4] >> 4) * 10 + (rtc->i2cBuf[4] & 0x0f);
    rtc->time.month = (rtc->i2cBuf[5] >> 4) * 10 + (rtc->i2cBuf[5] & 0x0f);
    rtc->time.year  = (rtc->i2cBuf[6] >> 4) * 10 + (rtc->i2cBuf[6] & 0x0f);
}

/**
 * @brief  获取时间字符串用于显示
 * @param  参数名 参数说明  
 * @param  [in] time 时间结构体指针
 * @param  [out] string 字符串指针
 * @retval 0-失败 1-成功
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 100ms调用 \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 无 \n
 */
void GetTimeString(STR_Time *time, char *string)
{
	string[0] = time->hour / 10 + '0';
	string[1] = time->hour % 10 + '0';
	string[2] = ':';
	
	string[3] = time->min / 10 + '0';
	string[4] = time->min % 10 + '0';
	string[5] = ':';
	
	string[6] = time->sec / 10 + '0';
	string[7] = time->sec % 10 + '0';
	string[8] = ' ';
	string[9] = ' ';
	string[10] = ' ';	
	
	string[11] = '2';
	string[12] = '0';
	string[13] = time->year / 10 + '0';
	string[14] = time->year % 10 + '0';
	string[15] = '-';
	
	string[16] = time->month / 10 + '0';
	string[17] = time->month % 10 + '0';
	string[18] = '-';
	
	string[19] = time->date / 10 + '0';
	string[20] = time->date % 10 + '0';
	string[21] = '\0';
}

/**
 * @brief  获取日期字符串用于显示
 * @param  参数名 参数说明  
 * @param  [in] time 时间结构体指针
 * @param  [out] string 字符串指针
 * @retval 0-失败 1-成功
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 100ms调用 \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 无 \n
 */
void GetDateString(STR_Time *time, char *string)
{
	string[0] = time->month / 10 + '0';
	string[1] = time->month % 10 + '0';
	string[2] = '-';
	
	string[3] = time->date / 10 + '0';
	string[4] = time->date % 10 + '0';
	string[5] = '\0';
}

/**  @} */

