/**
 *************************************************************************************************
 * @file    stm32_pwm.c
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
 
#include "stm32_pwm.h"
 
 /**
 * @brief  PWM初始化函数
 * @param  参数名 参数说明  
 * @param  无  
 * @retval 无
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 触发调用, 初始化时调用 \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 无 \n
 */
 void PwmInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	//使能GPIOB
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);
	//使能TIM1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	
	//将PB13(TIM1_CH1N)、PB14(TIM1_CH2N)、PB15(TIM1_CH3N)设置为复用推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//系统时钟2M，8分频，每0.5us 定时器counter加1
	TIM_TimeBaseStructure.TIM_Prescaler = 8-1;
	//向上计数模式
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	//周期1000，频率1k
	TIM_TimeBaseStructure.TIM_Period = 1000 - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);
	
	//开启定时器1
	TIM_Cmd(TIM1, ENABLE);
	
	//PWM输出使能
	TIM_CtrlPWMOutputs(TIM1, ENABLE);	
}

 /**
 * @brief  PWM占空比设置
 * @param  参数名 参数说明  
 * @param  [in] pwmCh1 通道1占空比, 单位: ‰ 
 * @param  [in] pwmCh2 通道2占空比, 单位: ‰
 * @param  [in] pwmCh3 通道3占空比, 单位: ‰ 
 * @retval 无
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 触发调用, 初始化时调用 \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 无 \n
 */
void PwmSet(u16 pwmCh1, u16 pwmCh2, u16 pwmCh3)
{
    static u16 pwmCh1Old = 0, pwmCh2Old = 0, pwmCh3Old = 0;
    
    /* 各路占空比未变化 */
    if ((pwmCh1Old == pwmCh1) && (pwmCh2Old == pwmCh2) && (pwmCh3Old == pwmCh3))
    {
        return;
    }
    
	if(pwmCh1 >= 1000)	pwmCh1 = 1000;
	if(pwmCh2 >= 1000)	pwmCh2 = 1000;
	if(pwmCh3 >= 1000)	pwmCh3 = 1000;
	
	TIM_SetCompare1(TIM1, pwmCh1);
	TIM_SetCompare2(TIM1, pwmCh2);
	TIM_SetCompare3(TIM1, pwmCh3);
    
    pwmCh1Old = pwmCh1;
    pwmCh2Old = pwmCh2;
    pwmCh3Old = pwmCh3;
}

/**  @} */
