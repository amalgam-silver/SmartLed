/**
 *************************************************************************************************
 * @file    stm32_rcc.c
 * @author  Ge Yue
 * @version V1.0.0
 * @date    2016.03.18
 * @brief   stm32的系统时钟设置
 *************************************************************************************************
 @verbatim 
 
  - 版本号       日期      创建人        描述 
    V1.0      2016.03.18   Ge Yue     stm32的系统时钟设置; 
 
  - 版本号       日期      修改人        描述 
 @endverbatim 
 */

/**
 * @defgroup RCC stm32_rcc: CPU Group
 * @{
 */
 
#include "stm32_rcc.h"

/**
 * @brief  Rcc初始函数
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
void RccInit(void)
{
	//使用芯片内部RC振荡器
	RCC_HSICmd(ENABLE);

	//设置FLASH读取等待时间为0周期（系统频率为8MHZ）
	FLASH_SetLatency(FLASH_Latency_0);
	//设置PLL输入为内部振荡器/2，8倍频
	//PLL频率 8MHZ / 2 * 8 = 8MHZ
	RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_2); 
	//使能PLL 
	RCC_PLLCmd(ENABLE);
	//设置系统时钟来源为PLL,8MHZ
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

	// AHB 频率 = 系统频率    8MHZ
	RCC_HCLKConfig(RCC_SYSCLK_Div1);			
	// APB1 = AHB/4	         2MHZ 	
	RCC_PCLK1Config(RCC_HCLK_Div4);			
	// APB2 = AHB/4			 2MHZ		
	RCC_PCLK2Config(RCC_HCLK_Div1);	
}

/**
 * @brief  stm32进入睡眠状态函数
 * @param  参数名 参数说明  
 * @param  无  
 * @retval 无
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 触发调用 \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 无 \n
 */
void Sleep(void)
{
    PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
}

/**  @} */
