/**
 *************************************************************************************************
 * @file    BspTemp.h
 * @author  Ge Yue
 * @version V1.0.0
 * @date    2016.03.18
 * @brief   LM75�¶ȴ���������
 *************************************************************************************************
 @verbatim 
 
  - �汾��       ����      ������        ���� 
    V1.0      2016.03.18   Ge Yue     LM75�¶ȴ���������; 
 
  - �汾��       ����      �޸���        ���� 
 @endverbatim 
 */

/**
 * @defgroup TEMP BspTemp: BSP Group
 * @{
 */

#ifndef _BSP_TEMP_H
#define _BSP_TEMP_H

#include "DataType.h"

/** @brief �豸��ַ */
#define I2C_ADDRESS_LM75			0x90		
/** @brief �¶ȼĴ�����ַ */
#define REG_ADDRESS_TEMPERATURE		0x00

/** @brief �¶Ƚṹ�� */
typedef struct
{
    u8 en;          ///< ����ʹ��
    u8 i2cBuf[2];   ///< I2CͨѶ����    
    s16 val;        ///< �¶�ֵ, ��λ: ���϶�, 2 dot 
}STR_Temperture;

/** @brief ��ʼ���¶ȴ����� */
void TempInit(void);
/** @brief ��ȡ�¶� */
void ReadTemperature(STR_Temperture *temp);

void ReadTempDMA(STR_Temperture *temp);

void GetTempDMA(STR_Temperture *temp);

/** @brief �¶Ƚṹ�� */
extern STR_Temperture Temp;
#endif

/**  @} */
