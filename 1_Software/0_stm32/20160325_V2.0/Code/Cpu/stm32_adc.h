/**
 *************************************************************************************************
 * @file    stm32_adc.h
 * @author  Ge Yue
 * @version V1.0.0
 * @date    2016.03.18
 * @brief   stm32��ADC����
 *************************************************************************************************
 @verbatim 
 
  - �汾��       ����      ������        ���� 
    V1.0      2016.03.18   Ge Yue     stm32��ADC����; 
 
  - �汾��       ����      �޸���        ���� 
 @endverbatim 
 */

/**
 * @defgroup ADC stm32_adc: CPU Group
 * @{
 */
 
#ifndef _STM32_ADC_H
#define _STM32_ADC_H

#include "stm32f10x.h"

/** @brief ADC1����Ĵ�����ַ */
#define ADC1_DR_Address ((u32)0x4001244C)

/** @brief  ��ȡADC����ֵ */
u16 GetAdcVal(void);
/** @brief  ADC��ʼ������ */
void ADCInit(void);

#endif

/**  @} */
