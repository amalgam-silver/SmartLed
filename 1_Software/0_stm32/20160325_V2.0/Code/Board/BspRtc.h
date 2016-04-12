/**
 *************************************************************************************************
 * @file    BspRtc.h
 * @author  Ge Yue
 * @version V1.0.0
 * @date    2016.03.18
 * @brief   RX8025ʵʱʱ��оƬ����
 *************************************************************************************************
 @verbatim 
 
  - �汾��       ����      ������        ���� 
    V1.0      2016.03.18   Ge Yue     RX8025ʵʱʱ��оƬ����; 
 
  - �汾��       ����      �޸���        ���� 
 @endverbatim 
 */

/**
 * @defgroup RTC BspRtc: BSP Group
 * @{
 */

#ifndef _BSP_RTC_H
#define _BSP_RTC_H

#include "DataType.h"

/** @brief I2C�豸��ַ */
#define	RX8025_ADDR		0x64
/** @brief �豸�Ĵ�����ַ */
#define	RX8025_ADDR_SECONDS		0x00        ///< ��Ĵ���
#define	RX8025_ADDR_WEEK		0x30        ///< ���ڼĴ���
#define	RX8025_ADDR_DATES		0x40        ///< ���ڼĴ���
#define	RX8025_ADDR_MONTH		0x50        ///< �¼Ĵ���
#define	RX8025_ADDR_MINUTES		0x80        ///< ���ӼĴ���
#define RX8025_ADDR_EXTENSION	0xD0        ///< ��չ�Ĵ���
#define	RX8025_ADDR_FLAG		0xE0        ///< ��־λ�Ĵ���
#define	RX8025_ADDR_CONTROL		0xF0        ///< ���ƼĴ���
/** @brief �豸����ģʽ */
#define	RX8025_WRITE_MODE		0xF0        ///< дģʽ
#define	RX8025_READ_MODE		0xF0        ///< ��ģʽ

/** @brief �������� */
#define SUNDAY 		0x01    ///< ����       
#define MONDAY 		0x02    ///< ��һ
#define TUESDAY		0x04    ///< �ܶ�
#define WEDNESDAY	0x08    ///< ����
#define THURSDAY	0x10    ///< ����
#define FRIDAY 		0x20    ///< ����
#define SATURDAY	0x40    ///< ����

/** @brief ʱ��ṹ�� */
typedef struct
{
	u8 sec;         ///< ��
	u8 min;         ///< ��
	u8 hour;        ///< ʱ
	u8 week;        ///< ����
	u8 date;        ///< ��
	u8 month;       ///< ��
	u8 year;        ///< ��, ʵ������-2000
}STR_Time;

/** @brief ʵʱʱ�ӽṹ�� */
typedef struct
{
    u8 en;          ///< ʹ��
    u8 i2cBuf[16];  ///< I2cͨѶ����buf
    STR_Time time;  ///< ʵʱʱ��
}STR_Rtc;

/** @brief ʵʱʱ�ӽṹ�� */
extern STR_Rtc Rtc;

/** @brief  ʵʱʱ�ӳ�ʼ���� */
void RtcInit(void);
/** @brief  ����ʱ�� */
u8 TimeSet(STR_Time *time);
/** @brief  ��ȡʱ�� */
u8 TimeRead(STR_Time *time);
/** @brief  ��ȡ�����ַ���������ʾ */
void GetDateString(STR_Time *time, char *string);
/** @brief  ��ȡʱ���ַ���������ʾ */
void GetTimeString(STR_Time *time, char *string);
/** @brief  ��ȡʱ�� */
void ReadTimeDMA(STR_Rtc *rtc);
/** @brief  ��ȡʱ�� */
void GetTimeDMA(STR_Rtc *rtc);

#endif

/**  @} */


