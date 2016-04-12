/**
 *************************************************************************************************
 * @file    BspTemp.c
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

#include "BspTemp.h"
#include "Bsp.h"

/** @brief �¶Ƚṹ�� */
STR_Temperture Temp;

/**
 * @brief  �¶ȴ�������ʼ������
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
void TempInit(void)
{
    /* ��ʼ��I2C������ */
    I2CInit();
    /* ʹ�ܶ�ȡ�¶� */
    Temp.en = 1;
}

/**
 * @brief  ��ȡ�¶�
 * @param  ������ ����˵��  
 * @param  [in] temp �¶Ƚṹ��  
 * @retval ��
 * @par ʹ��ȫ�ֱ��� \n
 * @note �� ִ��ʱ��: \n
 *       �� ��������: 1s \n
 *       �� �ɷ���: ���� \n
 *
 * @par  ע��:
 *       �� ͨ���ȴ���ʽI2C��ȡ�¶�, ��ʱ�ϳ�, 2.0�汾���ϲ���ʹ�� \n
 */
void ReadTemperature(STR_Temperture *temp)
{
	u8 receiveTempLM75[2];
	u8 errorTime = 0;
	u16 temperature = 0;
    
    if (temp->en)
    {	
        /* ��I2C ��ȡ�¶� */
        while(I2CReadRegister(I2C_ADDRESS_LM75, REG_ADDRESS_TEMPERATURE, receiveTempLM75, 2) == 0)
        {
            errorTime ++;
            if(errorTime > 5)
            {
                temp->val = 0;
            }
        }	
        temperature = ((s16)receiveTempLM75[0] << 8) + receiveTempLM75[1];
        //��11λ��Ч
        //����ֵ(0.125 / 32 = 0.0039) ��	
        temperature = temperature / 100 * 39;
        
        temp->val = temperature;
    }
}

/**
 * @brief  ��ȡ�¶�
 * @param  ������ ����˵��  
 * @param  [in] temp �¶Ƚṹ��  
 * @retval ��
 * @par ʹ��ȫ�ֱ��� \n
 * @note �� ִ��ʱ��: \n
 *       �� ��������: 1s \n
 *       �� �ɷ���: ���� \n
 *
 * @par  ע��:
 *       �� ͨ��DMA��ʽI2C��ȡ�¶�, ��ȡ�ļĴ���ֵ����� Temp.i2cBuf�� \n
 */
void ReadTempDMA(STR_Temperture *temp)
{
    if (temp->en && !I2c.busy)
    {
        I2CReadRegDMA(I2C_ADDRESS_LM75, REG_ADDRESS_TEMPERATURE, temp->i2cBuf, 2);
    }
}

/**
 * @brief  ��ȡ�¶�
 * @param  ������ ����˵��  
 * @param  [in] temp �¶Ƚṹ��  
 * @retval ��
 * @par ʹ��ȫ�ֱ��� \n
 * @note �� ִ��ʱ��: \n
 *       �� ��������: 1s \n
 *       �� �ɷ���: ���� \n
 *
 * @par  ע��:
 *       �� ����i2cBuf�ж�ȡ�ļĴ���ֵ�������¶�ֵ \n
 */
void GetTempDMA(STR_Temperture *temp)
{
    temp->val = ((s16)temp->i2cBuf[0] << 8) + temp->i2cBuf[1];
    //��11λ��Ч
    //����ֵ(0.125 / 32 = 0.0039) ��	
    temp->val = temp->val / 100 * 39;
}

/**  @} */
