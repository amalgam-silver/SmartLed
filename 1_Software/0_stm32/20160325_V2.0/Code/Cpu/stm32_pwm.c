/**
 *************************************************************************************************
 * @file    stm32_pwm.c
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
 
#include "stm32_pwm.h"
 
 /**
 * @brief  PWM��ʼ������
 * @param  ������ ����˵��  
 * @param  ��  
 * @retval ��
 * @par ʹ��ȫ�ֱ��� \n
 * @note �� ִ��ʱ��: \n
 *       �� ��������: ��������, ��ʼ��ʱ���� \n
 *       �� �ɷ���: ���� \n
 *
 * @par  ע��:
 *       �� �� \n
 */
 void PwmInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	//ʹ��GPIOB
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);
	//ʹ��TIM1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	
	//��PB13(TIM1_CH1N)��PB14(TIM1_CH2N)��PB15(TIM1_CH3N)����Ϊ�����������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//ϵͳʱ��2M��8��Ƶ��ÿ0.5us ��ʱ��counter��1
	TIM_TimeBaseStructure.TIM_Prescaler = 8-1;
	//���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	//����1000��Ƶ��1k
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
	
	//������ʱ��1
	TIM_Cmd(TIM1, ENABLE);
	
	//PWM���ʹ��
	TIM_CtrlPWMOutputs(TIM1, ENABLE);	
}

 /**
 * @brief  PWMռ�ձ�����
 * @param  ������ ����˵��  
 * @param  [in] pwmCh1 ͨ��1ռ�ձ�, ��λ: �� 
 * @param  [in] pwmCh2 ͨ��2ռ�ձ�, ��λ: ��
 * @param  [in] pwmCh3 ͨ��3ռ�ձ�, ��λ: �� 
 * @retval ��
 * @par ʹ��ȫ�ֱ��� \n
 * @note �� ִ��ʱ��: \n
 *       �� ��������: ��������, ��ʼ��ʱ���� \n
 *       �� �ɷ���: ���� \n
 *
 * @par  ע��:
 *       �� �� \n
 */
void PwmSet(u16 pwmCh1, u16 pwmCh2, u16 pwmCh3)
{
    static u16 pwmCh1Old = 0, pwmCh2Old = 0, pwmCh3Old = 0;
    
    /* ��·ռ�ձ�δ�仯 */
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
