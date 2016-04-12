/**
*************************************************************************************************
* @file    BspTimer.h
* @author  Ge Yue
* @version V1.0.0
* @date    2016.03.21
* @brief   1ms��ʱ��, ���ڲ���ʱ��Ƭ
*************************************************************************************************
@verbatim 

- �汾��       ����      ������        ���� 
  V1.0      2016.03.21   Ge Yue       1ms��ʱ��, ���ڲ���ʱ��Ƭ; 

- �汾��       ����      �޸���        ���� 
@endverbatim 
*/

/**
* @defgroup TIMER BspTimer: BSP Group
* @{
*/

#include "DataType.h"

#ifndef _BSP_TIMER_H
#define _BSP_TIMER_H

/** @brief ��ʱ���ж����ȼ� */
#define TIM_NVIC_PRIO   0;

/** @brief  ��ʱ����ʼ������ */
void TimerInit(void);

/** @brief 1ms������ʱ��־λ */
extern unsigned char ReqHeartBeat;


#endif
/**  @} */

