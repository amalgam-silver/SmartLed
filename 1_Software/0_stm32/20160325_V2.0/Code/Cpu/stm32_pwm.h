/**
 *************************************************************************************************
 * @file    stm32_pwm.h
 * @author  Ge Yue
 * @version V1.0.0
 * @date    2016.03.18
 * @brief   ͨ��Stm32��TIM1��ʱ������3·PWM
 *************************************************************************************************
 @verbatim 
 
  - �汾��       ����      ������        ���� 
    V1.0      2016.03.18   Ge Yue     ͨ��Stm32��TIM1��ʱ������3·PWM; 
 
  - �汾��       ����      �޸���        ���� 
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

