/**
*************************************************************************************************
* @file    BspTimer.c
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

#include "BspTimer.h"
#include "Bsp.h"

/** @brief 1ms������ʱ��־λ */
u8 ReqHeartBeat = 0;

 /**
 * @brief  ��ʱ����ʼ������
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
void TimerInit(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//ʹ��TIM1
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	//APB1ʱ��2M, TIMʱ��4M, 16��Ƶ, ÿ4us ��ʱ��counter��1
	TIM_TimeBaseStructure.TIM_Prescaler = 16 - 1;
	//���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	//����250, 1ms, Ƶ��1kHz
	TIM_TimeBaseStructure.TIM_Period = 250 - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	//��ʱ����������ж�
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = TIM_NVIC_PRIO;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	//��ʼ����
	TIM_Cmd(TIM2, ENABLE);
}

/**
 * @brief  TIM2�жϴ�����
 * @param  ������ ����˵��  
 * @param  ��
 * @retval ��
 * @par ʹ��ȫ�ֱ��� \n
 * @note �� ִ��ʱ��: \n
 *       �� ��������: 1ms, TIM2�ж�ʱ���� \n
 *       �� �ɷ���: ���� \n
 *
 * @par  ע��:
 *       �� �� \n
 */
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) != RESET)
	{
        ReqHeartBeat = 1;
        TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
}

/**  @} */

