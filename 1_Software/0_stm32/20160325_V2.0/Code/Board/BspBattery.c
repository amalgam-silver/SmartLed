/**
*************************************************************************************************
* @file    BspBattery.c
* @author  Ge Yue
* @version V1.0.0
* @date    2016.03.19
* @brief   ͨ��ADC�ɼ��ĵ�ص�ѹ������ʣ�����
*************************************************************************************************
@verbatim 

- �汾��       ����      ������        ���� 
  V1.0      2016.03.19   Ge Yue    ͨ��ADC�ɼ��ĵ�ص�ѹ������ʣ�����; 

- �汾��       ����      �޸���        ���� 
@endverbatim 
*/

/**
* @defgroup BETTERY BspBattery: BSP Group
* @{
*/

#include "BspBattery.h"
#include "Bsp.h"

/** @brief �����Ϣ�ṹ�� */
STR_BATTERY Bat;

/** @brief ���ݵ�ص�ѹ�����ص��� */
static void GetBatLvl(STR_BATTERY *bat);

/**
 * @brief  ��ؼ���ʼ������
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
void BatInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;  
    
    /* ��ʼ��ADC, ������ص�ѹ */
    ADCInit();
    
    /* ��ʼ����������� PB3 �� ����������� PB4 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);       // ʹ��GPIOB
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);   // ����JTAG���ܣ���PB4��Ϊ��ͨ����ʹ��
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    /* ����Bat�ṹ�� */
    GetBatStt(&Bat);
}

/**
 * @brief  ���µ��״̬����
 * @param  ������ ����˵��  
 * @param  [in] bat ���״̬�ṹ��ָ��
 * @retval ��
 * @par ʹ��ȫ�ֱ��� \n
 * @note �� ִ��ʱ��: \n
 *       �� ��������: 100ms \n
 *       �� �ɷ���: ���� \n
 *
 * @par  ע��:
 *       �� �� \n
 */
void GetBatStt(STR_BATTERY *bat)
{
    u16 adcVal = 0, volTmp;
    
    /* ��ȡ��ص�ѹ */
    adcVal = GetAdcVal();   
    volTmp = (adcVal * 600) >> 12;
    bat->vol = (volTmp + bat->vol * 7) >> 3;
    /* ���ݵ�ص�ѹ����ʣ����� */
    GetBatLvl(bat);
    
    /* ��ȡ���״̬ */
    if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3))	
    {
		bat->stt.bit.chrg = 1;
	}
    else
	{
        bat->stt.bit.chrg = 0;
    }
	
    /* ��ȡ����״̬ */
	if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4))
    {
		bat->stt.bit.full = 1;
    }
	else
    {
		bat->stt.bit.full = 0;
    }
}

/**
 * @brief  ���ݵ�ص�ѹ�����ص���
 * @param  ������ ����˵��  
 * @param  [in] bat ���״̬�ṹ��ָ��
 * @retval ��
 * @par ʹ��ȫ�ֱ��� \n
 * @note �� ִ��ʱ��: \n
 *       �� ��������: ��GetBatStt����, 100ms \n
 *       �� �ɷ���: ���� \n
 *
 * @par  ע��:
 *       �� �� \n
 */
static void GetBatLvl(STR_BATTERY *bat)
{
    // ���ݵ�ѹ��Ϊ7��
	// 0: 3.4V����	
	// 1: 3.4V-3.55V		
    // 2: 3.55-3.65V	
    // 3: 3.65-3.75V
	// 4: 3.75-3.85V		
    // 5: 3.85-4.05	
	// 6: 4.05����
	switch(Bat.lvl)
	{
		case 0:
			if(bat->vol > 355)
				bat->lvl++;
			break;
		case 1:
			if(bat->vol < 345)
				bat->lvl--;
			else if(bat->vol > 365)
				bat->lvl++;
			break;
		case 2:
			if(bat->vol < 355)
				bat->lvl--;
			else if(bat->vol > 375)
				bat->lvl++;
			break;
		case 3:
			if(bat->vol < 365)
				bat->lvl--;
			else if(bat->vol > 385)
				bat->lvl++;
			break;
		case 4:
			if(bat->vol < 375)
				bat->lvl--;
			else if(bat->vol > 395)
				bat->lvl++;
			break;
		case 5:
			if(bat->vol < 385)
				bat->lvl--;
			else if(bat->vol > 405)
				bat->lvl++;
			break;
		case 6:
			if(bat->vol < 395)
				bat->lvl--;
			break;	
	}
}

/**  @} */


