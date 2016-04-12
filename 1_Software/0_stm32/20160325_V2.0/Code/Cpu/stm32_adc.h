/**
 *************************************************************************************************
 * @file    stm32_adc.h
 * @author  Ge Yue
 * @version V1.0.0
 * @date    2016.03.18
 * @brief   stm32的ADC采样
 *************************************************************************************************
 @verbatim 
 
  - 版本号       日期      创建人        描述 
    V1.0      2016.03.18   Ge Yue     stm32的ADC采样; 
 
  - 版本号       日期      修改人        描述 
 @endverbatim 
 */

/**
 * @defgroup ADC stm32_adc: CPU Group
 * @{
 */
 
#ifndef _STM32_ADC_H
#define _STM32_ADC_H

#include "stm32f10x.h"

/** @brief ADC1结果寄存器地址 */
#define ADC1_DR_Address ((u32)0x4001244C)

/** @brief  读取ADC采样值 */
u16 GetAdcVal(void);
/** @brief  ADC初始化函数 */
void ADCInit(void);

#endif

/**  @} */
