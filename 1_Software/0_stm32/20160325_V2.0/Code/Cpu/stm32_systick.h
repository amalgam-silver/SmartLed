/**
 *************************************************************************************************
 * @file    stm32_systick.h
 * @author  Ge Yue
 * @version V1.0.0
 * @date    2016.03.18
 * @brief   通过Stm32的滴答定时器实现精确延时
 *************************************************************************************************
 @verbatim 
 
  - 版本号       日期      创建人        描述
    V1.0      2016.03.18   Ge Yue     通过Stm32的滴答定时器实现精确延时;
 
  - 版本号       日期      修改人        描述
 @endverbatim 
 */
 
/**
 * @defgroup SYSTICK stm32_systick: CPU Group
 * @{
 */
 
#ifndef _STM32_SYSTICK_H
#define _STM32_SYSTICK_H

#include "stm32f10x.h"

/** @brief 系统滴答定时器结构体 */
typedef struct
{
    unsigned short facUs;   ///< us延时倍乘数，每10微秒SysTick递减多少
    unsigned short facMs;   ///< ms延时倍乘数，每1毫秒SysTick递减多少
    unsigned long maxUs;    ///< 微秒最长延时    
    unsigned long maxMs;    ///< 毫秒最长延时
}STR_Systick;

/** @brief 系统滴答定时器结构体 */
extern STR_Systick Systick;

/** @brief Systick定时器初始化函数 */
void SystickInit(unsigned char SYSCLK);
/** @brief Systick毫秒级延时函数 */
void delayMs(unsigned long nms);
/** @brief Systick微秒级延时函数 */
void delayUs(unsigned long nus);

#endif

/**  @} */
