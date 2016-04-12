#include "BatteryState.h"
int batteryState = 0;
int voltageBattery = 0;
char stateCharging = 0,stateReady = 0;
u16 adcBuffer[16];

void getBatteryState(void)
{
	char i = 0;
	voltageBattery = 0;
	for(i=0; i<16; i++)
	{
		voltageBattery += adcBuffer[i];
	}
	voltageBattery >>= 4;
	
	//voltageBattery = voltageBattery * 3000 / 4096 * 2;

	voltageBattery = voltageBattery * 600 / 4096; 	
	
	//根据电压分为6档
	//0: 3.4V以下	
	//1: 3.4V-3.55V		2: 3.55-3.65V	3: 3.65-3.75V
	//4: 3.75-3.85V		5: 3.85-4.05	
	//6: 4.05以上
	switch(batteryState)
	{
		case 0:
			if(voltageBattery > 355)
				batteryState ++;
			break;
		case 1:
			if(voltageBattery < 345)
				batteryState --;
			else if(voltageBattery > 365)
				batteryState ++;
			break;
		case 2:
			if(voltageBattery < 355)
				batteryState --;
			else if(voltageBattery > 375)
				batteryState ++;
			break;
		case 3:
			if(voltageBattery < 365)
				batteryState --;
			else if(voltageBattery > 385)
				batteryState ++;
			break;
		case 4:
			if(voltageBattery < 375)
				batteryState --;
			else if(voltageBattery > 395)
				batteryState ++;
			break;
		case 5:
			if(voltageBattery < 385)
				batteryState --;
			else if(voltageBattery > 405)
				batteryState ++;
			break;
		case 6:
			if(voltageBattery < 395)
				batteryState --;
			break;	
	}
	
	if(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3))	
		stateCharging = 1;
	else
		stateCharging = 0;
	
	if(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4))
		stateReady = 1;
	else
		stateReady = 0;
}

void batteryDetectorInit(void)
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
	
	//PB3 -> CHARGE		PB3 -> READY
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
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

