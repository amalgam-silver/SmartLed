/**
 *************************************************************************************************
 * @file    BspRtc.c
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

#include "BspRtc.h"
#include "Bsp.h"

/** @brief ʵʱʱ�ӽṹ�� */
STR_Rtc Rtc;

/**
 * @brief  ʵʱʱ�ӳ�ʼ������
 * @param  ������ ����˵��  
 * @param  ��  
 * @retval ��
 * @par ʹ��ȫ�ֱ��� \n
 * @note �� ִ��ʱ��: \n
 *       �� ��������: ��������, ��ʼ��ʱ���� \n
 *       �� �ɷ���: ���� \n
 *
 * @par  ע��:
 *       �� �� \n
 */
void RtcInit(void)
{
	u8 registerRX8025[16] = {0x00,0x00,0x00,0x04,0x01,0x01,0x15,0x00,
							 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0};
	u8 temp[16];
	
    /* ��ʼ��I2C������ */                         
    I2CInit();
                             
    /* ��ȡRX8025��16���Ĵ��� */                         
	I2CReadRegister(RX8025_ADDR, (RX8025_ADDR_SECONDS & RX8025_WRITE_MODE), temp, 16); 
	
    /* �����־λ�Ĵ�����bit1(���ݶ�ʧλ)��λ������Ϊ���ݶ�ʧ���������üĴ��� */
	if(temp[14] & 0x02)
	{
		I2CWriteRegister(RX8025_ADDR, (RX8025_ADDR_SECONDS & RX8025_WRITE_MODE), registerRX8025, 16);	
	}
    
    /* ��ȡʱ�� */
    Rtc.en = 1;
}

/**
 * @brief  ����ʱ��
 * @param  ������ ����˵��  
 * @param  [in] time ʱ��ṹ��ָ��  
 * @retval 0-ʧ�� 1-�ɹ�
 * @par ʹ��ȫ�ֱ��� \n
 * @note �� ִ��ʱ��: \n
 *       �� ��������: �������� \n
 *       �� �ɷ���: ���� \n
 *
 * @par  ע��:
 *       ��  \n
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
 * @brief  ��ȡʱ��
 * @param  ������ ����˵��  
 * @param  [out] time ʱ��ṹ��ָ��  
 * @retval 0-ʧ�� 1-�ɹ�
 * @par ʹ��ȫ�ֱ��� \n
 * @note �� ִ��ʱ��: \n
 *       �� ��������: 100ms���� \n
 *       �� �ɷ���: ���� \n
 *
 * @par  ע��:
 *       �� �ȴ���ʽI2C��ȡ���ݣ���ʱ�ϳ���2.0���ϰ汾�Ѿ�����ʹ�� \n
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
 * @brief  ��ȡʱ��
 * @param  ������ ����˵��  
 * @param  [out] rtc ʱ��ṹ��ָ��  
 * @retval ��
 * @par ʹ��ȫ�ֱ��� \n
 * @note �� ִ��ʱ��: \n
 *       �� ��������: 100ms���� \n
 *       �� �ɷ���: ���� \n
 *
 * @par  ע��:
 *       �� ͨ��DMA I2C��ȡRX8025�ļĴ���������洢��TimeNow.i2cBuf�� \n
 */
void ReadTimeDMA(STR_Rtc *rtc)
{
    if ((rtc->en) && (!I2c.busy))
    {
        I2CReadRegDMA(RX8025_ADDR, (RX8025_ADDR_SECONDS & RX8025_READ_MODE), rtc->i2cBuf, 8); 
    }
}

/**
 * @brief  ��ȡʱ��
 * @param  ������ ����˵��  
 * @param  [out] rtc ʱ��ṹ��ָ��  
 * @retval ��
 * @par ʹ��ȫ�ֱ��� \n
 * @note �� ִ��ʱ��: \n
 *       �� ��������: 100ms���� \n
 *       �� �ɷ���: ���� \n
 *
 * @par  ע��:
 *       �� ����TimeNow.i2cBuf�ж����ļĴ���ֵ, ����ʱ��ṹ���еĸ������� \n
 */
void GetTimeDMA(STR_Rtc *rtc)
{
    /* ��BCD��תΪ10������ */
    rtc->time.sec   = (rtc->i2cBuf[0] >> 4) * 10 + (rtc->i2cBuf[0] & 0x0f);
    rtc->time.min   = (rtc->i2cBuf[1] >> 4) * 10 + (rtc->i2cBuf[1] & 0x0f);
    rtc->time.hour  = (rtc->i2cBuf[2] >> 4) * 10 + (rtc->i2cBuf[2] & 0x0f);
    rtc->time.week  = rtc->i2cBuf[3];
    rtc->time.date  = (rtc->i2cBuf[4] >> 4) * 10 + (rtc->i2cBuf[4] & 0x0f);
    rtc->time.month = (rtc->i2cBuf[5] >> 4) * 10 + (rtc->i2cBuf[5] & 0x0f);
    rtc->time.year  = (rtc->i2cBuf[6] >> 4) * 10 + (rtc->i2cBuf[6] & 0x0f);
}

/**
 * @brief  ��ȡʱ���ַ���������ʾ
 * @param  ������ ����˵��  
 * @param  [in] time ʱ��ṹ��ָ��
 * @param  [out] string �ַ���ָ��
 * @retval 0-ʧ�� 1-�ɹ�
 * @par ʹ��ȫ�ֱ��� \n
 * @note �� ִ��ʱ��: \n
 *       �� ��������: 100ms���� \n
 *       �� �ɷ���: ���� \n
 *
 * @par  ע��:
 *       �� �� \n
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
 * @brief  ��ȡ�����ַ���������ʾ
 * @param  ������ ����˵��  
 * @param  [in] time ʱ��ṹ��ָ��
 * @param  [out] string �ַ���ָ��
 * @retval 0-ʧ�� 1-�ɹ�
 * @par ʹ��ȫ�ֱ��� \n
 * @note �� ִ��ʱ��: \n
 *       �� ��������: 100ms���� \n
 *       �� �ɷ���: ���� \n
 *
 * @par  ע��:
 *       �� �� \n
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

