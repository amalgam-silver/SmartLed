/**
*************************************************************************************************
* @file    BspKey.h
* @author  Ge Yue
* @version V1.0.0
* @date    2016.03.19
* @brief   按键驱动
*************************************************************************************************
@verbatim 

- 版本号       日期      创建人        描述 
  V1.0      2016.03.19   Ge Yue      按键驱动; 

- 版本号       日期      修改人        描述 
@endverbatim 
*/

/**
* @defgroup KEY BspKey: BSP Group
* @{
*/
#ifndef _BSP_KEY_H
#define _BSP_KEY_H

#include "DataType.h"

/** @brief 按键按下滤波次数,每次1ms */
#define KEY_DN_FLT     100      ///< 相当于 100ms
/** @brief 按键弹起滤波次数,每次1ms */
#define KEY_UP_FLT     50      ///< 相当于 50ms
/** @brief 按键长按滤波次数,每次1ms */
#define KEY_LONG       900     ///< 相当于 900ms

/** @brief 按键中断优先级 */
#define KEY_NVIC_PRIO   15;

/** @brief 按键状态联合体 */
typedef union
{
    u8 all;            ///< 按键状态
    struct
    {
        u8 left:1;     ///< 左键
        u8 right:1;    ///< 右键
        u8 down:1;     ///< 下键
        u8 rsv:5;      ///< 保留
    }bit;              ///< 1 bit
}UN_KeyStt;

/** @brief 按键状态结构体 */
typedef struct
{
    UN_KeyStt keyPressing;      ///< 按键正在按下
    UN_KeyStt keyPressed;       ///< 按键弹起
    UN_KeyStt keyLongPressed;   ///< 按键常按后弹起
}STR_Key;

/** @brief 按键状态结构体 */
extern STR_Key KeyStt;  

/** @brief  按键初始化函数 */
void KeyInit(void);
/** @brief  按键扫描函数 */
void KeyScan(STR_Key *key);
/** @brief  设置按键中断函数 */
void SetKeyInt(u8 newStt);

#endif

/**  @} */

