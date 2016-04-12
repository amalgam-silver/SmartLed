/**
*************************************************************************************************
* @file    BspTimer.h
* @author  Ge Yue
* @version V1.0.0
* @date    2016.03.21
* @brief   1ms定时器, 用于产生时间片
*************************************************************************************************
@verbatim 

- 版本号       日期      创建人        描述 
  V1.0      2016.03.21   Ge Yue       1ms定时器, 用于产生时间片; 

- 版本号       日期      修改人        描述 
@endverbatim 
*/

/**
* @defgroup TIMER BspTimer: BSP Group
* @{
*/

#include "DataType.h"

#ifndef _BSP_TIMER_H
#define _BSP_TIMER_H

/** @brief 定时器中断优先级 */
#define TIM_NVIC_PRIO   0;

/** @brief  定时器初始化函数 */
void TimerInit(void);

/** @brief 1ms心跳定时标志位 */
extern unsigned char ReqHeartBeat;


#endif
/**  @} */

