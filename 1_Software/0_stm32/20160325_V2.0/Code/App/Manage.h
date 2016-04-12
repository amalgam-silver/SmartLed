/**
*************************************************************************************************
* @file    Manage.h
* @author  Ge Yue
* @version V1.0.0
* @date    2016.03.22
* @brief   应用层与硬件层之间、应用层各模块之间的数据传递
*************************************************************************************************
@verbatim 

- 版本号       日期      创建人        描述 
  V1.0      2016.03.22   Ge Yue    应用层与硬件层之间、应用层各模块之间的数据传递 

- 版本号       日期      修改人        描述 
@endverbatim 
*/

/**
* @defgroup MANAGE Manage: APP Group
* @{
*/

#ifndef _MANAGE_H
#define _MANAGE_H

#include "Power.h"
#include "Widget.h"
#include "Gui.h"
#include "Protocol.h"
#include "HAL.h"

/** @brief 软件版本 2.0 */
#define SW_VERSION 20
/** @brief 软件版本 2.0 */
#define STR_VERSION "SmartLED V2.0"

/** @brief 系统结构体 */
typedef struct{
    u8 secSleep;            ///< 关屏时间
    u32 secSys;             ///< 系统时间
    u32 secLastKey;         ///< 上次按键时间
    
    /* BSP层 */
    STR_Ble *ble;           ///< 蓝牙
    STR_BATTERY *bat;       ///< 电池
    STR_Key *key;           ///< 按键
    STR_Led *led;           ///< LED
    STR_Temperture *temp;   ///< 温度
    STR_Rtc *rtc;           ///< 时钟
    
    /* APP层 */
    STR_Power *power;       ///< 能耗管理
    STR_Gui *gui;           ///< 人机交互
}STR_Sys;


/** @brief 硬件初始化函数 */
void SystemInit(void);
/** @brief 软件初始化函数 */
void SoftwareInit(void);
/** @brief 任务分派函数 */
void Task(void);
/** @brief 更新系统状态 */
void SysSttUpd(STR_Sys *sys);
/** @brief 更新人机界面数据 */
void GuiDataUpd(STR_Sys *sys);

/** @brief 系统变量结构体 */
extern STR_Sys Sys;

#endif

/**  @} */
