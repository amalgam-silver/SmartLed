/**
 *************************************************************************************************
 * @file    stm32_rcc.h
 * @author  Ge Yue
 * @version V1.0.0
 * @date    2016.03.18
 * @brief   stm32的系统时钟设置
 *************************************************************************************************
 @verbatim 
 
  - 版本号       日期      创建人        描述 
    V1.0      2016.03.18   Ge Yue     stm32的系统时钟设置; 
 
  - 版本号       日期      修改人        描述 
 @endverbatim  
 *
 */

/**
 * @defgroup RCC stm32_rcc: CPU Group
 * @{
 */
 
#ifndef _STM32_RCC_H
#define _STM32_RCC_H

#include "stm32f10x.h"

/** @brief  Rcc初始函数 */
void RccInit(void);
/** @brief  stm32进入睡眠状态函数 */
void Sleep(void);
#endif


/**  @} */
