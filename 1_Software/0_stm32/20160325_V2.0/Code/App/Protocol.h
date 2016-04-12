/**
*************************************************************************************************
* @file    Protocol.h
* @author  Ge Yue
* @version V1.0.0
* @date    2016.03.27
* @brief   蓝牙协议解析
*************************************************************************************************
@verbatim 

- 版本号       日期      创建人        描述 
  V1.0      2016.03.27   Ge Yue      蓝牙协议解析; 

- 版本号       日期      修改人        描述 
@endverbatim 
*/

/**
* @defgroup PROTOCOL Protocol: APP Group
* @{
*/

#ifndef _PROTOCOL_H
#define _PROTOCOL_H

#include "HAL.h"

/** @brief 串口数据解析函数 */
void UartDecode(STR_Uart *uart);

#endif

/**  @} */
