/**
 *************************************************************************************************
 * @file    stm32_i2c.h
 * @author  Ge Yue
 * @version V1.0.0
 * @date    2016.03.18
 * @brief   ͨ��stm32��i2c��������дI2C�豸
 *************************************************************************************************
 @verbatim 
 
  - �汾��       ����      ������        ���� 
    V1.0      2016.03.18   Ge Yue     ͨ��stm32��i2c��������дI2C�豸; 
 
  - �汾��       ����      �޸���        ���� 
 @endverbatim 
 */
 
/**
 * @defgroup I2C stm32_i2c: CPU Group
 * @{
 */
 
#ifndef _STM32_I2C_H
#define _STM32_I2C_H

#include "stm32f10x.h"

/** @brief i2c��ʱʱ�� */
#define TIMEOUTI2C 5000

/** @brief I2C�շ��ṹ�� */
typedef struct
{
    u8 busy;            ///< I2C����æµ״̬    
    u8 bufTx[30];       ///< ���ͻ�����
    u8 bufRx[30];       ///< ���ջ�����
}STR_I2C;

/** @brief I2C�շ��ṹ�� */
extern STR_I2C I2c;

/** @brief  i2c��������ʼ������ */ 
void I2CInit(void);
/** @brief  дi2c�豸�ļĴ��� */
u8 I2CWriteRegister(u8 slaveAddress, u8 registerAddress, u8 *buf, u8 num); 
/** @brief  ��ȡi2c�豸�ļĴ��� */
u8 I2CReadRegister(u8 slaveAddress, u8 registerAddress, u8 *buf, u8 num);
/** @brief  DMA��ʽ��ȡi2c�豸�ļĴ��� */
void I2CReadRegDMA(u8 slaveAddr, u8 regAddr, u8 *buf, u8 num);
/** @brief  DMA��ʽдi2c�豸�ļĴ��� */
void I2CWriteRegDMA(u8 slaveAddr, u8 regAddr, u8 *buf, u8 num);

#endif

/**  @} */

