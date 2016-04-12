/**
*************************************************************************************************
* @file    Widget.h
* @author  Ge Yue
* @version V1.0.0
* @date    2016.03.25
* @brief   人机显示控件
*************************************************************************************************
@verbatim 

- 版本号       日期      创建人        描述 
  V1.0      2016.03.25   Ge Yue      人机显示控件; 

- 版本号       日期      修改人        描述 
@endverbatim 
*/

/**
* @defgroup WIDGET Widget: APP Group
* @{
*/


#ifndef _WIDGET_H
#define _WIDGET_H

#include "HAL.h"

/** @brief 时间显示超大字体 */
extern const unsigned char fontTimeNum[][28];
/** @brief 电量显示图标 */
extern const unsigned char fontBattery[][10];
/** @brief 蓝牙图标 */
extern const unsigned char fontBLE[][10];
/** @brief 时间显示用的冒号 */
extern const unsigned char fontTimeColon[][16];
/** @brief 勾选框图标 */
extern const unsigned char fontEnable[][8];
/** @brief 时钟图标 */
extern const unsigned char fontDelay[][15];
/** @brief 进度条图标 */
extern const unsigned char fontSlide[100];

/** @brief 滑动条结构体 */
typedef struct
{
    u8 x;           ///< x坐标 0~127
    u8 y;           ///< y坐标 0~7   
    u8 len;         ///< 进度条长度  0~100
    u8 pos;         ///< 进度块位置
    u8 prePos;      ///< 之前的进度块位置
    u8 focus;       ///< 0 未选中  1 选中(会显示反色)
    u8 preFocus;    ///< 之前的状态
    u8 upd;         ///< 强制更新标志位
    u8 val;         ///< 值
    u8 str[7];      ///< 数值单位
}STR_Slider;

/** @brief 静态文本结构体 */
typedef struct
{
    u8 x;           ///< x坐标 0~127
    u8 y;           ///< y坐标 0~7
    u8 focus;       ///< 0 未选中  1 选中(会显示反色)
    u8 preFocus;    ///< 之前的状态
    u8 upd;         ///< 强制更新标志位
    char *str;      ///< 文本字符串指针
}STR_Text;

/** @brief 状态图片组结构体 */
typedef struct
{
    u8 x;           ///< x坐标 0~127
    u8 y;           ///< y坐标 0~7
    u8 width;       ///< 宽
    u8 height;      ///< 高
    u8 stt;         ///< 状态
    u8 preStt;      ///< 之前的状态
    u8 upd;         ///< 强制更新标志位
    u8 *picBase;    ///< 图片组首地址    
}STR_SttPic;

/** @brief 数值控件结构体 */
typedef struct
{
    u8 x;           ///< x坐标 0~127
    u8 y;           ///< y坐标 0~7
    u8 upd;         ///< 强制更新标志位
    u8 dot;         ///< 小数点位数
    u8 str[4];      ///< 数字后要显示的单位
    u8 focus;       ///< 0 未选中  1 选中(会显示反色)
    u8 preFocus;    ///< 之前的状态
    s16 val;        ///< 数值
    s16 preVal;     ///< 前值
}STR_Value;

/** @brief 时钟选中状态枚举 */
typedef enum
{
    FOCUS_NONE = 0,
    FOCUS_YEAR = 1,
    FOCUS_MONTH = 2,
    FOCUS_DATE = 3,
    FOCUS_HOUR = 4,
    FOCUS_MIN = 5,
    FOCUS_SEC = 6,
}ENUM_ClockFocus;

/** @brief 时钟样式枚举 */
typedef enum
{
    CLOCK_BIG = 0,  ///< 首页大时钟样式
    CLOCK_SET = 1,  ///< 日期时间设置样式
    CLOCK_DAT = 2,  ///< 仅显示日期样式
    CLOCK_FUL = 3,  ///< 完整显示样式
}ENUM_ClockStyle;

/** @brief 时钟控件结构体 */
typedef struct
{
    u8 x;               ///< x坐标 0~127
    u8 y;               ///< y坐标 0~7
    u8 upd;             ///< 强制更新标志位
    u8 style;           ///< 样式
    u8 focus;           ///< 选中状态 0: 未选中 1: 选中年 2: 选中月 3: 选中日 4: 选中时 5: 选中分 6: 选中秒 
    u8 preFocus;        ///< 上一次的选中状态
    STR_Time *time;     ///< 需要显示的时间指针
    STR_Time preTime;   ///< 上次显示的时间    
}STR_Clock;

/** @brief  初始化时钟控件 */
void InitClock(u8 x, u8 y, u8 style, STR_Time *time, STR_Clock *clock);
/** @brief  显示时钟控件 */
void DispClock(STR_Clock *clock, int xOffset);
/** @brief  初始化图片组控件 */
void InitSttPic(u8 x, u8 y, u8 width, u8 height, u8 *picBase, STR_SttPic *sttPic);
/** @brief  显示图片组控件 */
void DispSttPic(STR_SttPic *sttPic, int xOffset);
/** @brief  初始化数值控件 */
void InitValue(u8 x, u8 y, u8 dot, char *str, STR_Value *value);
/** @brief  显示数值控件 */
void DispValue(STR_Value *value, int xOffset);
/** @brief  初始化进度条控件 */
void InitSlider(u8 x, u8 y, u8 len, char *str, STR_Slider *slider);
/** @brief  进度条显示控件 */
void DispSlid(STR_Slider *slider, int xOffset);
/** @brief  初始化静态文本控件 */
void InitText(u8 x, u8 y, char *str, STR_Text *text);
/** @brief  静态文本显示控件 */
void DispText(STR_Text *text, int xOffset);

#endif

 /**  @} */
