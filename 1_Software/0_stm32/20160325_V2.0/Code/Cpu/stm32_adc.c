/**
 *************************************************************************************************
 * @file    stm32_adc.c
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
 
#include "stm32_adc.h"

/** @brief ADC采样结果缓存 */
static u16 adcBuffer[16];

/**
 * @brief  读取ADC采样值
 * @param  参数名 参数说明  
 * @param  无
 * @retval ADC采样值 0~4096
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 触发调用 \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 无 \n
 */
u16 GetAdcVal(void)
{
    u8 i = 0;
    u16 ret = 0;
	for(i = 0; i < 16; i++)
	{
		ret += adcBuffer[i];
	}
    ret >>= 4;
    return ret;
}

/**
 * @brief  ADC初始化函数
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
void ADCInit(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//使能ADC1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB,ENABLE);
	//使能DMA
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);
	
	// 设置PA2作为ADC输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//DMA配置
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;							//外设地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&adcBuffer;								//内存地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;									//dma传输方向单向
	DMA_InitStructure.DMA_BufferSize = 16;												//设置DMA在传输时缓冲区的长度
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;					//设置DMA的外设递增模式，一个外设
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;								//设置DMA的内存递增模式，
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;			//外设数据字长
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;					//内存数据字长
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;										//设置DMA的传输模式：连续不断的循环模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;									//设置DMA的优先级别
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;										//设置DMA的2个memory中的变量互相访问
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);	
	DMA_Cmd(DMA1_Channel1, ENABLE);														//使能DMA

	//ADC1配置
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;									//独立工作模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;										//扫描方式
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;									//连续转换
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;					//无外部触发
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;								//数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;												//用于转换的通道数
	ADC_Init(ADC1, &ADC_InitStructure);
	
	//规则模式通道配置
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 1, ADC_SampleTime_239Cycles5);

	
	//使能ADC1 DMA
  	ADC_DMACmd(ADC1, ENABLE);
  																		
	//使能ADC1
	ADC_Cmd(ADC1, ENABLE);

   	//ADC校准
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));
	
	//ADC开始采样
	ADC_Cmd(ADC1,ENABLE);
	
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
}

/**  @} */

