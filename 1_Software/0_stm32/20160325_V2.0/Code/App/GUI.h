/**
*************************************************************************************************
* @file    GUI.h
* @author  Ge Yue
* @version V1.0.0
* @date    2016.03.22
* @brief   人机交互界面
*************************************************************************************************
@verbatim 

- 版本号       日期      创建人        描述 
  V1.0      2016.03.22   Ge Yue    人机交互界面

- 版本号       日期      修改人        描述 
@endverbatim 
*/

/**
* @defgroup GUI GUI: APP Group
* @{
*/

#ifndef _GUI_H
#define _GUI_H

#include "HAL.h"
#include "Widget.h"

/** @brief 滑入滑出动画状态 */
typedef enum
{
    SLIP_STANDSTILL = 0,    ///< 静止状态
    SLIP_LEFT_OUT   = 1,    ///< 向左滑出
    SLIP_RIGHT_OUT  = 2,    ///< 向右滑出   
    SLIP_LEFT_IN    = 3,    ///< 向左划入
    SLIP_RIGHT_IN   = 4,    ///< 向右划入
}ENUM_SlipStt;

/** @brief 页面滑入滑出动画结构体 */
typedef struct
{
    u8 stt;     ///< 状态: 0: 静止 1: 向左滑出 2: 向右滑出 3: 向左滑入 4: 向右滑入
    u8 step;    ///< 每一帧的偏移步进, 最大为2
    u8 dest;    ///< 目标界面
    s16 offset; ///< 当前帧显示的x坐标偏移
    
}STR_Slip;

/** @brief 主界面显示结构体 */
typedef struct
{
    STR_SttPic ble;         ///< 蓝牙图标
    STR_SttPic dly;         ///< 延时关灯图标
    STR_SttPic bat;         ///< 电量图标
    STR_Value temp;         ///< 温度
    STR_Clock bigClock;     ///< 大时钟
    STR_Clock date;         ///< 日期
}STR_GuiMain;

/** @brief 设置界面编辑选项枚举 */
typedef enum
{
    SET_BLE = 0,    ///< 蓝牙使能
    SET_SLP = 1,    ///< 睡眠时间
    SET_YEA = 2,    ///< 年
    SET_MON = 3,    ///< 月
    SET_DAT = 4,    ///< 日
    SET_HOU = 5,    ///< 时
    SET_MIN = 6,    ///< 分
    SET_SEC = 7,    ///< 秒
    SET_SAV = 8,    ///< 保存
    SET_EXT = 9,    ///< 退出
}ENUM_SetEditPrt;

/** @brief 设置界面显示结构体 */
typedef struct
{
    u8 editPtr;             ///< 当前编辑项    
    STR_Text ble;           ///< "蓝牙" 
    STR_Text sleep;         ///< "关屏时间"
    STR_Text time;          ///< "时间设置"
    STR_Text save;          ///< "保存"
    STR_Text exit;          ///< "退出"
    STR_SttPic bleEn;       ///< 蓝牙使能勾选框
    STR_Slider secSleep;    ///< 关屏时间滑动条 
    STR_Clock timeSet;      ///< 时间设置 
}STR_GuiSetting;

/** @brief 信息界面显示结构体 */
typedef struct
{
    u8 pagePtr;             ///< 当前信息页
    STR_Clock time;         ///< 当前时间
    STR_Value temp;         ///< 温度
    STR_Value secDly;       ///< 延时关灯时间
    STR_Value batVol;       ///< 电池电压
    STR_SttPic bat;         ///< 电池电量    
    STR_Text batChrg;       ///< "电池充电状态"
    STR_Text batFull;       ///< "电池充满状态"
    STR_Text bleStt;        ///< "蓝牙使能状态"
    STR_Text bleConn;       ///< "蓝牙连接状态"
    STR_Text ledDly;        ///< "延时关灯"
    STR_Text ver;           ///< "版本信息"
    STR_Text author;        ///< "作者"
    
}STR_GuiInfo;

/** @brief LED设置界面编辑选项枚举 */
typedef enum
{
    LEDSET_MODE = 0,
    LEDSET_BRTH = 1,
    LEDSET_PWMR = 2,
    LEDSET_PWMG = 3,
    LEDSET_PWMB = 4,
    LEDSET_PWMW = 5,
    LEDSET_DELA = 6,
    LEDSET_SAVE = 7,
    LEDSET_EXIT = 8,
}ENUM_ledEditPrt;

/** @brief LED设置界面显示结构体 */
typedef struct
{
    u8 editPtr;             ///< 当前编辑项  
    STR_SttPic ledMode;     ///< 模式
    STR_Text colorLed;      ///< "彩灯模式"
    STR_Text dly;           ///< "延时关灯"
    STR_Slider minDly;      ///< 延时关灯时间
    STR_Text light;         ///< "亮度"
    STR_Slider pwmW;        ///< 亮度
    STR_SttPic breathEn;    ///< 呼吸灯使能    
    STR_Text breath;        ///< "呼吸灯"
    STR_Text red;           ///< "红色"
    STR_Slider pwmR;        ///< 红灯亮度
    STR_Text green;         ///< "绿色"
    STR_Slider pwmG;        ///< 绿灯亮度
    STR_Text blue;          ///< "蓝色"
    STR_Slider pwmB;        ///< 蓝灯亮度  
    STR_Text exit;          ///< "退出"
    STR_Text save;          ///< "保存"    
}STR_LedSet;

/** @brief GUI各显示界面 */
typedef enum
{
    GUI_MAIN = 0,       ///< 时间主界面
    GUI_LED_MENU = 1,   ///< LED设置菜单界面
    GUI_INF_MENU = 2,   ///< 系统信息菜单界面
    GUI_SET_MENU = 3,   ///< 系统设置菜单界面
    GUI_LED = 4,        ///< LED设置界面
    GUI_INF = 5,        ///< 系统信息界面
    GUI_SET = 6,        ///< 系统设置界面
}ENUM_GuiStt;

/** @brief GUI显示结构体 */
typedef struct
{
    u8 stt;                 ///< GUI页面状态
    u8 upd;                 ///< 强制更新整个页面标志位
    STR_Slip slip;          ///< 滑动动画结构体
    STR_GuiMain main;       ///< 主界面显示结构体
    STR_GuiSetting setting; ///< 设置界面显示结构体
    STR_GuiInfo info;       ///< 信息界面显示结构体
    STR_LedSet led;         ///< LED设置界面结构体
}STR_Gui;

/** @brief GUI显示结构体 */
extern STR_Gui Gui;

/** @brief  人机交互界面显示*/
void Display(STR_Gui *gui);
/** @brief  人机交互界面按键响应*/
void KeyAction(STR_Gui *gui, STR_Key *key);
void GuiInit(void);

#endif 

/**  @} */
