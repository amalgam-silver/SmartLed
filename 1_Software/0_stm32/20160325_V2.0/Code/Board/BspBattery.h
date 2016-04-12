/**
*************************************************************************************************
* @file    BspBattery.h
* @author  Ge Yue
* @version V1.0.0
* @date    2016.03.19
* @brief   通过ADC采集的电池电压估算电池剩余电量
*************************************************************************************************
@verbatim 

- 版本号       日期      创建人        描述 
  V1.0      2016.03.19   Ge Yue    通过ADC采集的电池电压估算电池剩余电量; 

- 版本号       日期      修改人        描述 
@endverbatim 
*/

/**
* @defgroup BETTERY BspBattery: BSP Group
* @{
*/

#ifndef _BSP_BAT_H
#define _BSP_BAT_H

#include "DataType.h"

/** @brief 电池状态联合体 */
typedef union
{
    u8 all;             ///< 电池状态
    struct
    {
        u8 chrg:1;      ///< 是否处于充电中
        u8 full:1;      ///< 是否电量满
        u8 rsv:6;       ///< 保留
    }bit;               ///< 1 bit
}UN_BatStt;

/** @brief 电池信息结构体 */
typedef struct
{
    UN_BatStt stt;      ///< 电池状态   bit0: 充电状态  bit1: 是否充满
    u8 lvl;             ///< 电量等级   0~6
    u16 vol;            ///< 电池电压   单位: V   2dot
}STR_BATTERY;

/** @brief 电池信息结构体 */
extern STR_BATTERY Bat;

/** @brief 电池状态检测初始化 */
void BatInit(void);
/** @brief 更新电池状态 */
void GetBatStt(STR_BATTERY *bat);

#endif
/**  @} */


