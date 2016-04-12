#include "systemStateTransform.h"

u8 oledState = OLED_ON;
u8 systemState = MODE_HIGH;

void rccHighSpeed(void);
void rccLowSpeed(void);

//系统处于睡眠模式，被蓝牙连接中断唤醒
void modeSleepToLow(void)
{
	USART_InitTypeDef USART_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	//开启RX8025秒中断
	EXTI_InitStructure.EXTI_Line = EXTI_Line9;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	rccLowSpeed();
	
	//波特率9600，1位停止位，无奇偶校验位
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1,&USART_InitStructure);
	
	systemState = MODE_LOW;
}

//系统处于睡眠模式，被按键中断唤醒
void modeSleepToHigh(void)
{
	USART_InitTypeDef USART_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	//开启RX8025秒中断
	EXTI_InitStructure.EXTI_Line = EXTI_Line9;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	//关闭按键中断
	EXTI_InitStructure.EXTI_Line = EXTI_Line1 | EXTI_Line8 | EXTI_Line12;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
	EXTI_InitStructure.EXTI_LineCmd = DISABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	rccHighSpeed();
	
	//波特率9600，1位停止位，无奇偶校验位
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1,&USART_InitStructure);
	
	systemState = MODE_HIGH;
}

void modeHighToSleep(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	
	systemState = MODE_SLEEP;
	
	//关断RX8025 秒中断
	EXTI_InitStructure.EXTI_Line = EXTI_Line9;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
	EXTI_InitStructure.EXTI_LineCmd = DISABLE;
	EXTI_Init(&EXTI_InitStructure);

	//开启按键中断
	EXTI_InitStructure.EXTI_Line = EXTI_Line1 | EXTI_Line8 | EXTI_Line12;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	//关闭所有PWM
	pwmChange(0,0,0);
	
	//进入STOP_MODE
	PWR_EnterSTOPMode(PWR_Regulator_LowPower,PWR_STOPEntry_WFI);
}

void modeHighToLow(void)
{
	USART_InitTypeDef USART_InitStructure;
	rccLowSpeed();
	
	//波特率9600，1位停止位，无奇偶校验位
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1,&USART_InitStructure);
	
	systemState = MODE_LOW;
}

void modeLowToHigh(void)
{
	USART_InitTypeDef USART_InitStructure;
	rccHighSpeed();
	
	//波特率9600，1位停止位，无奇偶校验位
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1,&USART_InitStructure);
	
	systemState = MODE_HIGH;
}

void modeLowToSleep(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	
	systemState = MODE_SLEEP;

	//关断RX8025 秒中断
	EXTI_InitStructure.EXTI_Line = EXTI_Line9;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
	EXTI_InitStructure.EXTI_LineCmd = DISABLE;
	EXTI_Init(&EXTI_InitStructure);

	//开启按键中断
	EXTI_InitStructure.EXTI_Line = EXTI_Line1 | EXTI_Line8 | EXTI_Line12;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	//关闭所有PWM
	pwmChange(0,0,0);
	
	//进入STOP_MODE
	PWR_EnterSTOPMode(PWR_Regulator_LowPower,PWR_STOPEntry_WFI);

}

void rccLowSpeed(void)
{
	//使用芯片内部RC振荡器
	RCC_HSICmd(ENABLE);

	//设置FLASH读取等待时间为1周期（系统频率为8MHz）
	FLASH_SetLatency(FLASH_Latency_0);
	
	//系统时钟设置为内部振荡器
	RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
	//关闭PLL
	RCC_PLLCmd(DISABLE);
	
	//设置PLL输入为内部振荡器/2，2倍频
	//PLL频率 8MHZ / 2 * 2 = 8MHZ
	RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_2); 
	//使能PLL 
	RCC_PLLCmd(ENABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
	//设置系统时钟来源为PLL,8MHZ
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	while(RCC_GetSYSCLKSource() != 0x08);
	
	// AHB 频率 = 系统频率 / 1   8MHZ
	RCC_HCLKConfig(RCC_SYSCLK_Div1);			
	// APB1 = AHB/4			     2MHZ 	
	RCC_PCLK1Config(RCC_HCLK_Div4);			
	// APB2 = AHB/4			 	 2MHZ		
	RCC_PCLK2Config(RCC_HCLK_Div4);	
	
	//初始化延时器
	delayInit(8);
}

void rccHighSpeed(void)
{
	//使用芯片内部RC振荡器
	RCC_HSICmd(ENABLE);

	//设置FLASH读取等待时间为1周期（系统频率为32MHZ）
	FLASH_SetLatency(FLASH_Latency_1);
	
	//系统时钟设置为内部振荡器
	RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
	//关闭PLL
	RCC_PLLCmd(DISABLE);
	
	//设置PLL输入为内部振荡器/2，8倍频
	//PLL频率 8MHZ / 2 * 8 = 32MHZ
	RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_8); 
	//使能PLL 
	RCC_PLLCmd(ENABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
	//设置系统时钟来源为PLL,32MHZ
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	while(RCC_GetSYSCLKSource() != 0x08)
		
	// AHB 频率 = 系统频率    32MHZ
	RCC_HCLKConfig(RCC_SYSCLK_Div1);			
	// APB1 = AHB/16	     2MHZ 	
	RCC_PCLK1Config(RCC_HCLK_Div16);			
	// APB2 = AHB			 32MHZ		
	RCC_PCLK2Config(RCC_HCLK_Div1);	
	
	//初始化延时器
	delayInit(32);	
}
