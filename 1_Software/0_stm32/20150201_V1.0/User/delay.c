#include "delay.h"
#include "stm32f10x.h"

//us��ʱ������	 
static unsigned char  fac_us=0;
//ms��ʱ������
static unsigned int fac_ms=0;

/****************************************************************************
*	��������delayInit
	��  �룺unsigned char SYSCLK��ϵͳʱ�ӣ���MHZΪ��λ
	��  ����void
	��  �ܣ���ʼ���ӳٺ���
	��  ע�������������HCLK���ٵ���
*	��  �ڣ�2013.05.10
****************************************************************************/
void delayInit(unsigned char SYSCLK)
{
	//bit2��0��ѡ��SYSTICK��ʱ��ԴΪHCLKʱ�ӵ�1/8
	SysTick->CTRL&=0Xfffb;
	
	//us��ʱ����������ÿ10΢��SysTick�ݼ�����
	fac_us=SYSCLK*10/8;		    

	//ms��ʱ����������ÿ����SysTick�ݼ�����
	fac_ms=(unsigned int)fac_us*100;
}

/****************************************************************************
*	��������delayMs
	��  �룺unsigned char SYSCLK��ϵͳʱ�ӣ���MHZΪ��λ
	��  ����void
	��  �ܣ���ʱnms
	��  ע��ע�������ʱʱ��,ϵͳƵ��72MHZʱ�������ʱ1864����
			SysTick->LOADΪ24λ�Ĵ���,����,�����ʱΪ:
			nms<=0xffffff*8*1000/SYSCLK
			SYSCLK��λΪHz,nms��λΪms
			��72M������,nms<=1864 
*	��  �ڣ�2013.05.10
****************************************************************************/
void delayMs(unsigned int nms)
{	 		  	  
	unsigned long temp;	
		   
	//ʱ�����(SysTick->LOADΪ24bit)
	SysTick->LOAD=(u32)nms*fac_ms;
	
	//��ռ�����
	SysTick->VAL =0x00;
	
	//��ʼ����            
	SysTick->CTRL=0x01 ;           
	
	//�ȴ�ʱ�䵽��
	//SysTick->VAL������0��SysTick->CTRL���λ�Զ���1����ȡ��λ���Զ���0 
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));  
	
	//�رռ�����
	SysTick->CTRL=0x00;       
	
	//��ռ�����
	SysTick->VAL =0X00;       	  	    
}  
 
/****************************************************************************
*	��������delayUs
	��  �룺unsigned int nus����ʱ������
	��  ����void
	��  �ܣ���ʱnus
	��  ע��72Mhzʱ�����ʱ1864000us 
*	��  �ڣ�2013.05.10
****************************************************************************/	    								   
void delayUs(unsigned long nus)
{		
	unsigned long temp;
	
	//ʱ�����(SysTick->LOADΪ24bit)	    	 
	SysTick->LOAD=nus*fac_us/10; 
	
	//��ռ�����	  		 
	SysTick->VAL=0x00;       
	
	//��ʼ����
	SysTick->CTRL=0x01 ;       	 
	
	//�ȴ�ʱ�䵽��
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));   
	
	//�رռ�����
	SysTick->CTRL=0x00;       
	
	//��ռ�����
	SysTick->VAL =0X00;       	 
}
