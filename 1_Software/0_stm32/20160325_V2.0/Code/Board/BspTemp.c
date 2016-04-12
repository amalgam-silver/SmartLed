/**
 *************************************************************************************************
 * @file    BspTemp.c
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

#include "BspTemp.h"
#include "Bsp.h"

/** @brief 温度结构体 */
STR_Temperture Temp;

/**
 * @brief  温度传感器初始化函数
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
void TempInit(void)
{
    /* 初始化I2C控制器 */
    I2CInit();
    /* 使能读取温度 */
    Temp.en = 1;
}

/**
 * @brief  读取温度
 * @param  参数名 参数说明  
 * @param  [in] temp 温度结构体  
 * @retval 无
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 1s \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 通过等待方式I2C读取温度, 耗时较长, 2.0版本以上不再使用 \n
 */
void ReadTemperature(STR_Temperture *temp)
{
	u8 receiveTempLM75[2];
	u8 errorTime = 0;
	u16 temperature = 0;
    
    if (temp->en)
    {	
        /* 从I2C 读取温度 */
        while(I2CReadRegister(I2C_ADDRESS_LM75, REG_ADDRESS_TEMPERATURE, receiveTempLM75, 2) == 0)
        {
            errorTime ++;
            if(errorTime > 5)
            {
                temp->val = 0;
            }
        }	
        temperature = ((s16)receiveTempLM75[0] << 8) + receiveTempLM75[1];
        //高11位有效
        //步进值(0.125 / 32 = 0.0039) ℃	
        temperature = temperature / 100 * 39;
        
        temp->val = temperature;
    }
}

/**
 * @brief  读取温度
 * @param  参数名 参数说明  
 * @param  [in] temp 温度结构体  
 * @retval 无
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 1s \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 通过DMA方式I2C读取温度, 读取的寄存器值存放于 Temp.i2cBuf中 \n
 */
void ReadTempDMA(STR_Temperture *temp)
{
    if (temp->en && !I2c.busy)
    {
        I2CReadRegDMA(I2C_ADDRESS_LM75, REG_ADDRESS_TEMPERATURE, temp->i2cBuf, 2);
    }
}

/**
 * @brief  读取温度
 * @param  参数名 参数说明  
 * @param  [in] temp 温度结构体  
 * @retval 无
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 1s \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 根据i2cBuf中读取的寄存器值，更新温度值 \n
 */
void GetTempDMA(STR_Temperture *temp)
{
    temp->val = ((s16)temp->i2cBuf[0] << 8) + temp->i2cBuf[1];
    //高11位有效
    //步进值(0.125 / 32 = 0.0039) ℃	
    temp->val = temp->val / 100 * 39;
}

/**  @} */
