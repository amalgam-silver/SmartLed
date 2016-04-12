/**
*************************************************************************************************
* @file    Power.h
* @author  Ge Yue
* @version V1.0.0
* @date    2016.04.02
* @brief   能耗管理
*************************************************************************************************
@verbatim 

- 版本号       日期      创建人        描述 
  V1.0      2016.04.02   Ge Yue      能耗管理; 

- 版本号       日期      修改人        描述 
@endverbatim 
*/

/**
* @defgroup POWER Power: APP Group
* @{
*/

#ifndef _POWER_H
#define _POWER_H

#include "HAL.H"

/** @brief 系统功耗控制状态 */
typedef enum
{
    NORMAL = 0,     ///< 正常模式
    OLEDOFF = 1,    ///< 熄屏模式
    SLEEP = 2,      ///< 睡眠模式
}ENUM_SysStt;

/** @brief 能耗控制结构体 */
typedef struct
{
    u8 sysStt;          ///< 系统状态
    u8 tarStt;          ///< 目标状态
}STR_Power;

/** @brief 能耗控制结构体 */
extern STR_Power Power;

/** @brief 能耗管理函数 */
void PowerManage(STR_Power *power);

#endif

 /**  @} */
