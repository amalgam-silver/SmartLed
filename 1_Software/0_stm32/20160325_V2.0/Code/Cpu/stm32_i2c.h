/**
 *************************************************************************************************
 * @file    stm32_i2c.h
 * @author  Ge Yue
 * @version V1.0.0
 * @date    2016.03.18
 * @brief   通过stm32的i2c控制器读写I2C设备
 *************************************************************************************************
 @verbatim 
 
  - 版本号       日期      创建人        描述 
    V1.0      2016.03.18   Ge Yue     通过stm32的i2c控制器读写I2C设备; 
 
  - 版本号       日期      修改人        描述 
 @endverbatim 
 */
 
/**
 * @defgroup I2C stm32_i2c: CPU Group
 * @{
 */
 
#ifndef _STM32_I2C_H
#define _STM32_I2C_H

#include "stm32f10x.h"

/** @brief i2c超时时间 */
#define TIMEOUTI2C 5000

/** @brief I2C收发结构体 */
typedef struct
{
    u8 busy;            ///< I2C处于忙碌状态    
    u8 bufTx[30];       ///< 发送缓冲区
    u8 bufRx[30];       ///< 接收缓冲区
}STR_I2C;

/** @brief I2C收发结构体 */
extern STR_I2C I2c;

/** @brief  i2c控制器初始化函数 */ 
void I2CInit(void);
/** @brief  写i2c设备的寄存器 */
u8 I2CWriteRegister(u8 slaveAddress, u8 registerAddress, u8 *buf, u8 num); 
/** @brief  读取i2c设备的寄存器 */
u8 I2CReadRegister(u8 slaveAddress, u8 registerAddress, u8 *buf, u8 num);
/** @brief  DMA方式读取i2c设备的寄存器 */
void I2CReadRegDMA(u8 slaveAddr, u8 regAddr, u8 *buf, u8 num);
/** @brief  DMA方式写i2c设备的寄存器 */
void I2CWriteRegDMA(u8 slaveAddr, u8 regAddr, u8 *buf, u8 num);

#endif

/**  @} */

