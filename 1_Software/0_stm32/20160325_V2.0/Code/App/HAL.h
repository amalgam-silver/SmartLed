/**
*************************************************************************************************
* @file    HAL.h
* @author  Ge Yue
* @version V1.0.0
* @date    2016.03.21
* @brief   硬件抽象层接口
*************************************************************************************************
@verbatim 

- 版本号       日期      创建人        描述 
  V1.0      2016.03.21   Ge Yue    硬件抽象层接口; 

- 版本号       日期      修改人        描述 
@endverbatim 
*/

/**
* @defgroup MANAGE Manage: APP Group
* @{
*/

#ifndef _HAL_H
#define _HAL_H

#include "BspRtc.h"
#include "BspTemp.h"
#include "BspBle.h"
#include "BspBattery.h"
#include "BspKey.h"
#include "BspOled.h"
#include "BspLed.h"
#include "BspTimer.h"

#include "stm32_uart.h"

/** @brief  stm32进入睡眠状态函数 */
void Sleep(void);

#endif
/**  @} */
