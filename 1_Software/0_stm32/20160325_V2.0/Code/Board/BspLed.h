/**
*************************************************************************************************
* @file    BspLed.h
* @author  Ge Yue
* @version V1.0.0
* @date    2016.03.21
* @brief   Led驱动
*************************************************************************************************
@verbatim 

- 版本号       日期      创建人        描述 
  V1.0      2016.03.21   Ge Yue       Led驱动; 

- 版本号       日期      修改人        描述 
@endverbatim 
*/

/**
* @defgroup LED BspLed: BSP Group
* @{
*/

#ifndef _BSP_LED_H
#define _BSP_LED_H

#include "DataType.h"

/** @brief LED照明模式结构体 */
typedef struct 
{
    u16 pwmW;       ///< 照明亮度, 单位: ‰
    u16 delayEn;    ///< 延时关灯使能
    u32 delaySec;   ///< 延时关灯剩余秒数
}STR_Light;

/** @brief LED彩灯模式结构体 */
typedef struct 
{
    u8 breath;      ///< 呼吸灯使能
    u8 stepR;       ///< 呼吸灯红灯亮度步进
    u8 stepG;       ///< 呼吸灯绿灯亮度步进
    u8 stepB;       ///< 呼吸灯蓝灯亮度步进
    u16 pwmR;       ///< 红光亮度
    u16 pwmG;       ///< 绿光亮度
    u16 pwmB;       ///< 蓝光亮度
}STR_Color;

#define LED_LIGHT   0   ///< 照明模式
#define LED_COLOR   1   ///< 彩灯模式
#define LED_LOPWR   2   ///< 低电量警告   

/** @brief LED结构体 */
typedef struct
{
    u8 mode;            ///< 模式     0: 照明模式     1: 彩灯模式    
    u8 lowPwr;          ///< 低电量警告
    STR_Light light;    ///< 照明模式
    STR_Color color;    ///< 彩灯模式
}STR_Led;

extern STR_Led Led;

/** @brief  Led初始化函数 */
void LedInit(void);
/** @brief  设置照明模式函数 */
void SetLight(u16 light, u32 delay);
/** @brief  设置照明模式函数 */
void SetColor(u16 r, u16 g, u16 b, u8 breath);
/** @brief  LED状态更新函数 */
void LedUpdStt(STR_Led *led);

#endif

/**  @} */

