#include "delay.h"
#include "stm32f10x.h"

//us延时倍乘数	 
static unsigned char  fac_us=0;
//ms延时倍乘数
static unsigned int fac_ms=0;

/****************************************************************************
*	函数名：delayInit
	输  入：unsigned char SYSCLK：系统时钟，以MHZ为单位
	输  出：void
	功  能：初始化延迟函数
	备  注：务必在设置完HCLK后再调用
*	日  期：2013.05.10
****************************************************************************/
void delayInit(unsigned char SYSCLK)
{
	//bit2置0，选择SYSTICK的时钟源为HCLK时钟的1/8
	SysTick->CTRL&=0Xfffb;
	
	//us延时倍乘数，即每10微秒SysTick递减多少
	fac_us=SYSCLK*10/8;		    

	//ms延时倍乘数，即每毫秒SysTick递减多少
	fac_ms=(unsigned int)fac_us*100;
}

/****************************************************************************
*	函数名：delayMs
	输  入：unsigned char SYSCLK：系统时钟，以MHZ为单位
	输  出：void
	功  能：延时nms
	备  注：注意最大延时时间,系统频率72MHZ时，最大延时1864毫秒
			SysTick->LOAD为24位寄存器,所以,最大延时为:
			nms<=0xffffff*8*1000/SYSCLK
			SYSCLK单位为Hz,nms单位为ms
			对72M条件下,nms<=1864 
*	日  期：2013.05.10
****************************************************************************/
void delayMs(unsigned int nms)
{	 		  	  
	unsigned long temp;	
		   
	//时间加载(SysTick->LOAD为24bit)
	SysTick->LOAD=(u32)nms*fac_ms;
	
	//清空计数器
	SysTick->VAL =0x00;
	
	//开始倒数            
	SysTick->CTRL=0x01 ;           
	
	//等待时间到达
	//SysTick->VAL倒数到0后，SysTick->CTRL最高位自动置1，读取该位将自动清0 
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));  
	
	//关闭计数器
	SysTick->CTRL=0x00;       
	
	//清空计数器
	SysTick->VAL =0X00;       	  	    
}  
 
/****************************************************************************
*	函数名：delayUs
	输  入：unsigned int nus：延时毫秒数
	输  出：void
	功  能：延时nus
	备  注：72Mhz时最大延时1864000us 
*	日  期：2013.05.10
****************************************************************************/	    								   
void delayUs(unsigned long nus)
{		
	unsigned long temp;
	
	//时间加载(SysTick->LOAD为24bit)	    	 
	SysTick->LOAD=nus*fac_us/10; 
	
	//清空计数器	  		 
	SysTick->VAL=0x00;       
	
	//开始倒数
	SysTick->CTRL=0x01 ;       	 
	
	//等待时间到达
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));   
	
	//关闭计数器
	SysTick->CTRL=0x00;       
	
	//清空计数器
	SysTick->VAL =0X00;       	 
}
