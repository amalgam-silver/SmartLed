#include "systemStateTransform.h"

u8 oledState = OLED_ON;
u8 systemState = MODE_HIGH;

void rccHighSpeed(void);
void rccLowSpeed(void);

//ϵͳ����˯��ģʽ�������������жϻ���
void modeSleepToLow(void)
{
	USART_InitTypeDef USART_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	//����RX8025���ж�
	EXTI_InitStructure.EXTI_Line = EXTI_Line9;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	rccLowSpeed();
	
	//������9600��1λֹͣλ������żУ��λ
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1,&USART_InitStructure);
	
	systemState = MODE_LOW;
}

//ϵͳ����˯��ģʽ���������жϻ���
void modeSleepToHigh(void)
{
	USART_InitTypeDef USART_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	//����RX8025���ж�
	EXTI_InitStructure.EXTI_Line = EXTI_Line9;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	//�رհ����ж�
	EXTI_InitStructure.EXTI_Line = EXTI_Line1 | EXTI_Line8 | EXTI_Line12;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
	EXTI_InitStructure.EXTI_LineCmd = DISABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	rccHighSpeed();
	
	//������9600��1λֹͣλ������żУ��λ
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
	
	//�ض�RX8025 ���ж�
	EXTI_InitStructure.EXTI_Line = EXTI_Line9;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
	EXTI_InitStructure.EXTI_LineCmd = DISABLE;
	EXTI_Init(&EXTI_InitStructure);

	//���������ж�
	EXTI_InitStructure.EXTI_Line = EXTI_Line1 | EXTI_Line8 | EXTI_Line12;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	//�ر�����PWM
	pwmChange(0,0,0);
	
	//����STOP_MODE
	PWR_EnterSTOPMode(PWR_Regulator_LowPower,PWR_STOPEntry_WFI);
}

void modeHighToLow(void)
{
	USART_InitTypeDef USART_InitStructure;
	rccLowSpeed();
	
	//������9600��1λֹͣλ������żУ��λ
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
	
	//������9600��1λֹͣλ������żУ��λ
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

	//�ض�RX8025 ���ж�
	EXTI_InitStructure.EXTI_Line = EXTI_Line9;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
	EXTI_InitStructure.EXTI_LineCmd = DISABLE;
	EXTI_Init(&EXTI_InitStructure);

	//���������ж�
	EXTI_InitStructure.EXTI_Line = EXTI_Line1 | EXTI_Line8 | EXTI_Line12;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	//�ر�����PWM
	pwmChange(0,0,0);
	
	//����STOP_MODE
	PWR_EnterSTOPMode(PWR_Regulator_LowPower,PWR_STOPEntry_WFI);

}

void rccLowSpeed(void)
{
	//ʹ��оƬ�ڲ�RC����
	RCC_HSICmd(ENABLE);

	//����FLASH��ȡ�ȴ�ʱ��Ϊ1���ڣ�ϵͳƵ��Ϊ8MHz��
	FLASH_SetLatency(FLASH_Latency_0);
	
	//ϵͳʱ������Ϊ�ڲ�����
	RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
	//�ر�PLL
	RCC_PLLCmd(DISABLE);
	
	//����PLL����Ϊ�ڲ�����/2��2��Ƶ
	//PLLƵ�� 8MHZ / 2 * 2 = 8MHZ
	RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_2); 
	//ʹ��PLL 
	RCC_PLLCmd(ENABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
	//����ϵͳʱ����ԴΪPLL,8MHZ
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	while(RCC_GetSYSCLKSource() != 0x08);
	
	// AHB Ƶ�� = ϵͳƵ�� / 1   8MHZ
	RCC_HCLKConfig(RCC_SYSCLK_Div1);			
	// APB1 = AHB/4			     2MHZ 	
	RCC_PCLK1Config(RCC_HCLK_Div4);			
	// APB2 = AHB/4			 	 2MHZ		
	RCC_PCLK2Config(RCC_HCLK_Div4);	
	
	//��ʼ����ʱ��
	delayInit(8);
}

void rccHighSpeed(void)
{
	//ʹ��оƬ�ڲ�RC����
	RCC_HSICmd(ENABLE);

	//����FLASH��ȡ�ȴ�ʱ��Ϊ1���ڣ�ϵͳƵ��Ϊ32MHZ��
	FLASH_SetLatency(FLASH_Latency_1);
	
	//ϵͳʱ������Ϊ�ڲ�����
	RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
	//�ر�PLL
	RCC_PLLCmd(DISABLE);
	
	//����PLL����Ϊ�ڲ�����/2��8��Ƶ
	//PLLƵ�� 8MHZ / 2 * 8 = 32MHZ
	RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_8); 
	//ʹ��PLL 
	RCC_PLLCmd(ENABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
	//����ϵͳʱ����ԴΪPLL,32MHZ
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	while(RCC_GetSYSCLKSource() != 0x08)
		
	// AHB Ƶ�� = ϵͳƵ��    32MHZ
	RCC_HCLKConfig(RCC_SYSCLK_Div1);			
	// APB1 = AHB/16	     2MHZ 	
	RCC_PCLK1Config(RCC_HCLK_Div16);			
	// APB2 = AHB			 32MHZ		
	RCC_PCLK2Config(RCC_HCLK_Div1);	
	
	//��ʼ����ʱ��
	delayInit(32);	
}
