/**
*************************************************************************************************
* @file    HAL.h
* @author  Ge Yue
* @version V1.0.0
* @date    2016.03.21
* @brief   Ӳ�������ӿ�
*************************************************************************************************
@verbatim 

- �汾��       ����      ������        ���� 
  V1.0      2016.03.21   Ge Yue    Ӳ�������ӿ�; 

- �汾��       ����      �޸���        ���� 
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

/** @brief  stm32����˯��״̬���� */
void Sleep(void);

#endif
/**  @} */
