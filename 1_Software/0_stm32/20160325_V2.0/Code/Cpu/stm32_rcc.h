/**
 *************************************************************************************************
 * @file    stm32_rcc.h
 * @author  Ge Yue
 * @version V1.0.0
 * @date    2016.03.18
 * @brief   stm32��ϵͳʱ������
 *************************************************************************************************
 @verbatim 
 
  - �汾��       ����      ������        ���� 
    V1.0      2016.03.18   Ge Yue     stm32��ϵͳʱ������; 
 
  - �汾��       ����      �޸���        ���� 
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

/** @brief  Rcc��ʼ���� */
void RccInit(void);
/** @brief  stm32����˯��״̬���� */
void Sleep(void);
#endif


/**  @} */
