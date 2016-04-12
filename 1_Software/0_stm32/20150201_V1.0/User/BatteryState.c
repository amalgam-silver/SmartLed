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
	
	//���ݵ�ѹ��Ϊ6��
	//0: 3.4V����	
	//1: 3.4V-3.55V		2: 3.55-3.65V	3: 3.65-3.75V
	//4: 3.75-3.85V		5: 3.85-4.05	
	//6: 4.05����
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
	
	//ʹ��ADC1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB,ENABLE);
	//ʹ��DMA
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);
	
	// ����PA2��ΪADC����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//PB3 -> CHARGE		PB3 -> READY
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//DMA����
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;							//�����ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&adcBuffer;								//�ڴ��ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;									//dma���䷽����
	DMA_InitStructure.DMA_BufferSize = 16;												//����DMA�ڴ���ʱ�������ĳ���
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;					//����DMA���������ģʽ��һ������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;								//����DMA���ڴ����ģʽ��
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;			//���������ֳ�
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;					//�ڴ������ֳ�
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;										//����DMA�Ĵ���ģʽ���������ϵ�ѭ��ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;									//����DMA�����ȼ���
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;										//����DMA��2��memory�еı����������
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	
	DMA_Cmd(DMA1_Channel1, ENABLE);														//ʹ��DMA

	//ADC1����
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;									//��������ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;										//ɨ�跽ʽ
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;									//����ת��
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;					//���ⲿ����
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;								//�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;												//����ת����ͨ����
	ADC_Init(ADC1, &ADC_InitStructure);
	
	//����ģʽͨ������
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 1, ADC_SampleTime_239Cycles5);

	
	//ʹ��ADC1 DMA
  	ADC_DMACmd(ADC1, ENABLE);
  																		
	//ʹ��ADC1
	ADC_Cmd(ADC1, ENABLE);

   	//ADCУ׼
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));
	
	//ADC��ʼ����
	ADC_Cmd(ADC1,ENABLE);
	
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	
}

