/**
*************************************************************************************************
* @file    BspTimer.c
* @author  Ge Yue
* @version V1.0.0
* @date    2016.03.21
* @brief   1ms定时器, 用于产生时间片
*************************************************************************************************
@verbatim 

- 版本号       日期      创建人        描述 
  V1.0      2016.03.21   Ge Yue       1ms定时器, 用于产生时间片; 

- 版本号       日期      修改人        描述 
@endverbatim 
*/

/**
* @defgroup TIMER BspTimer: BSP Group
* @{
*/

#include "BspTimer.h"
#include "Bsp.h"

/** @brief 1ms心跳定时标志位 */
u8 ReqHeartBeat = 0;

 /**
 * @brief  定时器初始化函数
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
void TimerInit(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//使能TIM1
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	//APB1时钟2M, TIM时钟4M, 16分频, 每4us 定时器counter加1
	TIM_TimeBaseStructure.TIM_Prescaler = 16 - 1;
	//向上计数模式
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	//周期250, 1ms, 频率1kHz
	TIM_TimeBaseStructure.TIM_Period = 250 - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	//定时器向上溢出中断
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = TIM_NVIC_PRIO;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	//开始计数
	TIM_Cmd(TIM2, ENABLE);
}

/**
 * @brief  TIM2中断处理函数
 * @param  参数名 参数说明  
 * @param  无
 * @retval 无
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 1ms, TIM2中断时调用 \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 无 \n
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

