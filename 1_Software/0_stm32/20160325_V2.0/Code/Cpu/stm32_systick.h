/**
 *************************************************************************************************
 * @file    stm32_systick.h
 * @author  Ge Yue
 * @version V1.0.0
 * @date    2016.03.18
 * @brief   ͨ��Stm32�ĵδ�ʱ��ʵ�־�ȷ��ʱ
 *************************************************************************************************
 @verbatim 
 
  - �汾��       ����      ������        ����
    V1.0      2016.03.18   Ge Yue     ͨ��Stm32�ĵδ�ʱ��ʵ�־�ȷ��ʱ;
 
  - �汾��       ����      �޸���        ����
 @endverbatim 
 */
 
/**
 * @defgroup SYSTICK stm32_systick: CPU Group
 * @{
 */
 
#ifndef _STM32_SYSTICK_H
#define _STM32_SYSTICK_H

#include "stm32f10x.h"

/** @brief ϵͳ�δ�ʱ���ṹ�� */
typedef struct
{
    unsigned short facUs;   ///< us��ʱ��������ÿ10΢��SysTick�ݼ�����
    unsigned short facMs;   ///< ms��ʱ��������ÿ1����SysTick�ݼ�����
    unsigned long maxUs;    ///< ΢�����ʱ    
    unsigned long maxMs;    ///< �������ʱ
}STR_Systick;

/** @brief ϵͳ�δ�ʱ���ṹ�� */
extern STR_Systick Systick;

/** @brief Systick��ʱ����ʼ������ */
void SystickInit(unsigned char SYSCLK);
/** @brief Systick���뼶��ʱ���� */
void delayMs(unsigned long nms);
/** @brief Systick΢�뼶��ʱ���� */
void delayUs(unsigned long nus);

#endif

/**  @} */
