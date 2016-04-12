/**
 *************************************************************************************************
 * @file    stm32_systick.c
 * @author  Ge Yue
 * @version V1.0.0
 * @date    2016.03.18
 * @brief   通过Stm32的滴答定时器实现精确延时
 *************************************************************************************************
 @verbatim 
 
  - 版本号       日期      创建人        描述 
    V1.0      2016.03.18   Ge Yue     通过Stm32的滴答定时器实现精确延时; 
 
  - 版本号       日期      修改人        描述 
 @endverbatim  
 */
 
/**
 * @defgroup SYSTICK stm32_systick: CPU Group
 * @{
 */
 
#include "stm32_systick.h"

/** @brief 系统滴答定时器结构体 */
STR_Systick Systick = {0,};

/**
 * @brief  SysTick初始函数
 * @param  参数名 参数说明  
 * @param  [in] sysClk 系统时钟, 单位: MHz  
 * @retval 无
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 触发调用, 初始化时调用 \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 无 \n
 */
void SystickInit(unsigned char sysClk)
{
	/* bit2置0，选择SYSTICK的时钟源为HCLK时钟的1/8 */
	SysTick->CTRL &= 0Xfffb;
	
	/* us延时倍乘数，即每10微秒SysTick递减多少 */
	Systick.facUs = sysClk * 10 / 8;		    

	/* ms延时倍乘数，即每毫秒SysTick递减多少 */
	Systick.facMs = (unsigned int)Systick.facUs * 100;
    
    /* ms延时最大时间 
        SysTick->LOAD为24位寄存器,所以,最大延时为: nms <= 0xffffff * 8 / (sysClk * 1000000) * 1000
    */
    Systick.maxMs = 134217 / sysClk;
    
    /* us延时最大时间 
        SysTick->LOAD为24位寄存器,所以,最大延时为: nms <= 0xffffff * 8 / (sysClk * 1000000) * 1000000
    */   
    Systick.maxUs = 134217728 / sysClk;
}

/**
 * @brief  毫秒级延时函数
 * @param  参数名 参数说明  
 * @param  [in] nms 延时的毫秒数, 单位: ms  
 * @retval 无
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 触发调用 \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 注意最大延时时间, 系统频率72MHZ时，最大延时1864毫秒	
 *          SysTick->LOAD为24位寄存器,所以,最大延时为: nms <= 0xffffff * 8 * 1000 / sysClk
 *          sysClk单位为 Hz, nms单位为 ms, 对72M条件下, nms <= 1864  \n
 */
void delayMs(unsigned long nms)
{	 		  	  
	unsigned long temp;	
		   
	//时间加载(SysTick->LOAD为24bit)
	SysTick->LOAD = (u32)nms * Systick.facMs;
	
	//清空计数器
	SysTick->VAL = 0x00;
	
	//开始倒数            
	SysTick->CTRL = 0x01 ;           
	
	//等待时间到达
	//SysTick->VAL倒数到0后，SysTick->CTRL最高位自动置1，读取该位将自动清0 
	do
	{
		temp = SysTick->CTRL;
	}
	while(temp & 0x01 && !(temp & (1 << 16)));  
	
	//关闭计数器
	SysTick->CTRL = 0x00;       
	
	//清空计数器
	SysTick->VAL = 0x00;       	  	    
}  

/**
 * @brief  微秒级延时函数
 * @param  参数名 参数说明  
 * @param  [in] nus 延时的微秒数, 单位: us  
 * @retval 无
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 触发调用 \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 注意最大延时时间, 系统频率72MHZ时，最大延时1864000微秒	
 */   								   
void delayUs(unsigned long nus)
{		
	unsigned long temp;
	
	//时间加载(SysTick->LOAD为24bit)	    	 
	SysTick->LOAD = nus * Systick.facUs / 10; 
	
	//清空计数器	  		 
	SysTick->VAL = 0x00;       
	
	//开始倒数
	SysTick->CTRL = 0x01 ;       	 
	
	//等待时间到达
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp & 0x01 && !(temp & (1 << 16)));   
	
	//关闭计数器
	SysTick->CTRL=0x00;       
	
	//清空计数器
	SysTick->VAL =0X00;       	 
}

/**  @} */

