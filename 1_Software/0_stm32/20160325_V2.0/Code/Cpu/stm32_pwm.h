/**
 *************************************************************************************************
 * @file    stm32_pwm.h
 * @author  Ge Yue
 * @version V1.0.0
 * @date    2016.03.18
 * @brief   通过Stm32的TIM1定时器产生3路PWM
 *************************************************************************************************
 @verbatim 
 
  - 版本号       日期      创建人        描述 
    V1.0      2016.03.18   Ge Yue     通过Stm32的TIM1定时器产生3路PWM; 
 
  - 版本号       日期      修改人        描述 
 @endverbatim 
 */

/**
 * @defgroup PWM stm32_pwm: CPU Group
 * @{
 */
 
#ifndef _STM32_PWM_H
#define _STM32_PWM_H

#include "stm32f10x.h"

void PwmInit(void);
void PwmSet(u16 pwmCh1, u16 pwmCh2, u16 pwmCh3);
 
 
#endif

/**  @} */

