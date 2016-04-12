/**
 *************************************************************************************************
 * @file    stm32_systick.c
 * @author  Ge Yue
 * @version V1.0.0
 * @date    2016.03.18
 * @brief   ͨ��Stm32�ĵδ�ʱ��ʵ�־�ȷ��ʱ
 *************************************************************************************************
 @verbatim 
 
  - �汾��       ����      ������        ���� 
    V1.0      2016.03.18   Ge Yue     ͨ��Stm32�ĵδ�ʱ��ʵ�־�ȷ��ʱ; 
 
  - �汾��       ����      �޸���        ���� 
 @endverbatim  
 */
 
/**
 * @defgroup SYSTICK stm32_systick: CPU Group
 * @{
 */
 
#include "stm32_systick.h"

/** @brief ϵͳ�δ�ʱ���ṹ�� */
STR_Systick Systick = {0,};

/**
 * @brief  SysTick��ʼ����
 * @param  ������ ����˵��  
 * @param  [in] sysClk ϵͳʱ��, ��λ: MHz  
 * @retval ��
 * @par ʹ��ȫ�ֱ��� \n
 * @note �� ִ��ʱ��: \n
 *       �� ��������: ��������, ��ʼ��ʱ���� \n
 *       �� �ɷ���: ���� \n
 *
 * @par  ע��:
 *       �� �� \n
 */
void SystickInit(unsigned char sysClk)
{
	/* bit2��0��ѡ��SYSTICK��ʱ��ԴΪHCLKʱ�ӵ�1/8 */
	SysTick->CTRL &= 0Xfffb;
	
	/* us��ʱ����������ÿ10΢��SysTick�ݼ����� */
	Systick.facUs = sysClk * 10 / 8;		    

	/* ms��ʱ����������ÿ����SysTick�ݼ����� */
	Systick.facMs = (unsigned int)Systick.facUs * 100;
    
    /* ms��ʱ���ʱ�� 
        SysTick->LOADΪ24λ�Ĵ���,����,�����ʱΪ: nms <= 0xffffff * 8 / (sysClk * 1000000) * 1000
    */
    Systick.maxMs = 134217 / sysClk;
    
    /* us��ʱ���ʱ�� 
        SysTick->LOADΪ24λ�Ĵ���,����,�����ʱΪ: nms <= 0xffffff * 8 / (sysClk * 1000000) * 1000000
    */   
    Systick.maxUs = 134217728 / sysClk;
}

/**
 * @brief  ���뼶��ʱ����
 * @param  ������ ����˵��  
 * @param  [in] nms ��ʱ�ĺ�����, ��λ: ms  
 * @retval ��
 * @par ʹ��ȫ�ֱ��� \n
 * @note �� ִ��ʱ��: \n
 *       �� ��������: �������� \n
 *       �� �ɷ���: ���� \n
 *
 * @par  ע��:
 *       �� ע�������ʱʱ��, ϵͳƵ��72MHZʱ�������ʱ1864����	
 *          SysTick->LOADΪ24λ�Ĵ���,����,�����ʱΪ: nms <= 0xffffff * 8 * 1000 / sysClk
 *          sysClk��λΪ Hz, nms��λΪ ms, ��72M������, nms <= 1864  \n
 */
void delayMs(unsigned long nms)
{	 		  	  
	unsigned long temp;	
		   
	//ʱ�����(SysTick->LOADΪ24bit)
	SysTick->LOAD = (u32)nms * Systick.facMs;
	
	//��ռ�����
	SysTick->VAL = 0x00;
	
	//��ʼ����            
	SysTick->CTRL = 0x01 ;           
	
	//�ȴ�ʱ�䵽��
	//SysTick->VAL������0��SysTick->CTRL���λ�Զ���1����ȡ��λ���Զ���0 
	do
	{
		temp = SysTick->CTRL;
	}
	while(temp & 0x01 && !(temp & (1 << 16)));  
	
	//�رռ�����
	SysTick->CTRL = 0x00;       
	
	//��ռ�����
	SysTick->VAL = 0x00;       	  	    
}  

/**
 * @brief  ΢�뼶��ʱ����
 * @param  ������ ����˵��  
 * @param  [in] nus ��ʱ��΢����, ��λ: us  
 * @retval ��
 * @par ʹ��ȫ�ֱ��� \n
 * @note �� ִ��ʱ��: \n
 *       �� ��������: �������� \n
 *       �� �ɷ���: ���� \n
 *
 * @par  ע��:
 *       �� ע�������ʱʱ��, ϵͳƵ��72MHZʱ�������ʱ1864000΢��	
 */   								   
void delayUs(unsigned long nus)
{		
	unsigned long temp;
	
	//ʱ�����(SysTick->LOADΪ24bit)	    	 
	SysTick->LOAD = nus * Systick.facUs / 10; 
	
	//��ռ�����	  		 
	SysTick->VAL = 0x00;       
	
	//��ʼ����
	SysTick->CTRL = 0x01 ;       	 
	
	//�ȴ�ʱ�䵽��
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp & 0x01 && !(temp & (1 << 16)));   
	
	//�رռ�����
	SysTick->CTRL=0x00;       
	
	//��ռ�����
	SysTick->VAL =0X00;       	 
}

/**  @} */

