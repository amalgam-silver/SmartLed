/**
 *************************************************************************************************
 * @file    BspTemp.h
 * @author  Ge Yue
 * @version V1.0.0
 * @date    2016.03.18
 * @brief   LM75温度传感器驱动
 *************************************************************************************************
 @verbatim 
 
  - 版本号       日期      创建人        描述 
    V1.0      2016.03.18   Ge Yue     LM75温度传感器驱动; 
 
  - 版本号       日期      修改人        描述 
 @endverbatim 
 */

/**
 * @defgroup TEMP BspTemp: BSP Group
 * @{
 */

#ifndef _BSP_TEMP_H
#define _BSP_TEMP_H

#include "DataType.h"

/** @brief 设备地址 */
#define I2C_ADDRESS_LM75			0x90		
/** @brief 温度寄存器地址 */
#define REG_ADDRESS_TEMPERATURE		0x00

/** @brief 温度结构体 */
typedef struct
{
    u8 en;          ///< 采样使能
    u8 i2cBuf[2];   ///< I2C通讯缓存    
    s16 val;        ///< 温度值, 单位: 摄氏度, 2 dot 
}STR_Temperture;

/** @brief 初始化温度传感器 */
void TempInit(void);
/** @brief 读取温度 */
void ReadTemperature(STR_Temperture *temp);

void ReadTempDMA(STR_Temperture *temp);

void GetTempDMA(STR_Temperture *temp);

/** @brief 温度结构体 */
extern STR_Temperture Temp;
#endif

/**  @} */
