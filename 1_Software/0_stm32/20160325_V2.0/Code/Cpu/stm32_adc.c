/**
 *************************************************************************************************
 * @file    stm32_adc.c
 * @author  Ge Yue
 * @version V1.0.0
 * @date    2016.03.18
 * @brief   stm32��ADC����
 *************************************************************************************************
 @verbatim 
 
  - �汾��       ����      ������        ���� 
    V1.0      2016.03.18   Ge Yue     stm32��ADC����; 
 
  - �汾��       ����      �޸���        ���� 
 @endverbatim 
 */

/**
 * @defgroup ADC stm32_adc: CPU Group
 * @{
 */
 
#include "stm32_adc.h"

/** @brief ADC����������� */
static u16 adcBuffer[16];

/**
 * @brief  ��ȡADC����ֵ
 * @param  ������ ����˵��  
 * @param  ��
 * @retval ADC����ֵ 0~4096
 * @par ʹ��ȫ�ֱ��� \n
 * @note �� ִ��ʱ��: \n
 *       �� ��������: �������� \n
 *       �� �ɷ���: ���� \n
 *
 * @par  ע��:
 *       �� �� \n
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
 * @brief  ADC��ʼ������
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
void ADCInit(void)
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

/**  @} */

