#include "systemInit.h"

/**********�ڲ����ú���Ԥ����**************/
void clockConfiguration(void);
void buttonInit(void);
void breathTimerInit(void);

/****************************************************************************
*	��������systemInit
	��  �룺void
	��  ����void
	��  �ܣ�ϵͳ��ʼ��
	��  ע����
*	��  �ڣ�2015.02.01
****************************************************************************/
void systemInit(void)
{
	//ϵͳʱ�ӳ�ʼ��
	clockConfiguration();
		
	//��ʼ��OLED
	oledInit();
	
	//��ʼ������
	buttonInit();
	
	//��ʼ��I2C
	I2CInit();

	//��ʼ��RTCʱ��
	modeConfigurationRX8025();
	
	//��ʼ�������Ϣ�ɼ�
	batteryDetectorInit();
	
	//��ʼ���͹�������
	bluetoothInit();
	
	//��ʼ��LED����PWM
	ledInit();
	
	//�����Ƴ�ʼ��
	breathTimerInit();
}

/****************************************************************************
*	��������clockConfiguration
	��  �룺void
	��  ����void
	��  �ܣ�ϵͳʱ�ӳ�ʼ��
	��  ע������ʹ�����ڲ����񣬳�ʼ��ϵͳʱ��Ϊ32MHz
*	��  �ڣ�2015.02.01
****************************************************************************/
void clockConfiguration(void)
{
	//ʹ��оƬ�ڲ�RC����
	RCC_HSICmd(ENABLE);

	//����FLASH��ȡ�ȴ�ʱ��Ϊ1���ڣ�ϵͳƵ��Ϊ32MHZ��
	FLASH_SetLatency(FLASH_Latency_1);
	//����PLL����Ϊ�ڲ�����/2��8��Ƶ
	//PLLƵ�� 8MHZ / 2 * 8 = 32MHZ
	RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_8); 
	//ʹ��PLL 
	RCC_PLLCmd(ENABLE);
	//����ϵͳʱ����ԴΪPLL,32MHZ
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

	// AHB Ƶ�� = ϵͳƵ��    32MHZ
	RCC_HCLKConfig(RCC_SYSCLK_Div1);			
	// APB1 = AHB/16	     2MHZ 	
	RCC_PCLK1Config(RCC_HCLK_Div16);			
	// APB2 = AHB			 32MHZ		
	RCC_PCLK2Config(RCC_HCLK_Div1);		
	
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);

	//��ʼ����ʱ��
	delayInit(32);	
}

/****************************************************************************
*	��������buttonInit
	��  �룺void
	��  ����void
	��  �ܣ�������ʼ��
	��  ע����
*	��  �ڣ�2015.02.01
****************************************************************************/
void buttonInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line1 | EXTI_Line8 | EXTI_Line12;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
	EXTI_InitStructure.EXTI_LineCmd = DISABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource8);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource12);

//	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0E;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0D;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void breathTimerInit()
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//ʹ��TIM1
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	//ϵͳʱ��2M��4��Ƶ��ÿ0.5us ��ʱ��counter��1
	TIM_TimeBaseStructure.TIM_Prescaler = 4 - 1;
	//���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	//����40000��20ms,Ƶ��50Hz
	TIM_TimeBaseStructure.TIM_Period = 40000 - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	//������ʱ��1
	TIM_Cmd(TIM2, ENABLE);
	
	//��ʱ����������ж�
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	//��ʼ����
	TIM_Cmd(TIM2, ENABLE);
}

//��ʱ��2�жϣ�ÿ20ms�ж�һ��
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) != RESET)
	{
		static u16 pwmBreathRed = 0, pwmBreathGreen = 0, pwmBreathBlue = 0;
		static u8 pwmBreathLowBattery = 0;
		static u8 flagDirection = 1,breathCount = 0;
		u16 stepRed,stepGreen,stepBlue;
		
		switch(modeBreathLed)
		{
			case breathOff:
				flagDirection = 1;
				breathCount = 0;
				pwmBreathLowBattery = 0;
				pwmBreathRed = 0;
				pwmBreathGreen = 0;
				pwmBreathBlue = 0;
				break;
			case breathColor:
				if((pwmRedSet >= 3) || (pwmGreenSet >= 3) || (pwmBlueSet >= 3))
				{
					stepRed = pwmRedSet * 10 / 25;
					stepGreen = pwmGreenSet * 10 / 25;
					stepBlue = pwmBlueSet * 10 / 25;
					
					if(flagDirection)
					{
						pwmBreathRed += stepRed;
						pwmBreathGreen += stepGreen;
						pwmBreathBlue += stepBlue;
						
						breathCount ++;
						if(breathCount >= 25)
						{
							flagDirection = 0;
						}
					}
					else
					{
						pwmBreathRed -= stepRed;
						pwmBreathGreen -= stepGreen;
						pwmBreathBlue -= stepBlue;
						
						breathCount --;
						if(breathCount == 245)
						{
							pwmBreathRed = 0;
							pwmBreathGreen = 0;
							pwmBreathBlue = 0;
							breathCount = 0;
							flagDirection = 1;
						}
					}
                    pwmChangeLow(pwmBreathRed,pwmBreathGreen,pwmBreathBlue);
					
				}
                else 
                {
                    pwmChange(0, 0, 0);
                }
				break;
			case breathLowBattery:
				if(flagDirection)
				{
					pwmBreathLowBattery ++;
					if(pwmBreathLowBattery >= 25)
					{
						flagDirection = 0;
					}
				}
				else
				{
					pwmBreathLowBattery --;
					if(pwmBreathLowBattery == 245)
					{
						pwmBreathLowBattery = 0;
						flagDirection = 1;
					}
				}
				pwmChangeLow(pwmBreathLowBattery,0,0);
				break;
		}
		
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}
}

void EXTI9_5_IRQHandler(void)
{
	//RX8025T���ж�
	if(EXTI_GetITStatus(EXTI_Line9) != RESET)
	{

		systemSecond++;
		
		if(flagBLEConnected == 1)
		{
//			bleSend(send_temp,1);
		}
		
		if(flagDelayLedEnable)
		{
			if(ledDelaySecond == 0)
			{
				flagDelayLedEnable = DISABLE;
				pwmWhite = 0;
				pwmChange(pwmWhite,pwmWhite,pwmWhite);
			}
			else
				ledDelaySecond --;
		}
			
		EXTI_ClearITPendingBit(EXTI_Line9);
	}
	
	//������
	if(EXTI_GetITStatus(EXTI_Line8) != RESET)
	{
		modeSleepToHigh();
		
		EXTI_ClearITPendingBit(EXTI_Line8);
	}
}


void EXTI1_IRQHandler(void)
{
	//������
	if(EXTI_GetITStatus(EXTI_Line1) != RESET)
	{
		modeSleepToHigh();
	  
		EXTI_ClearITPendingBit(EXTI_Line1);
	}

}

void EXTI15_10_IRQHandler(void)
{
	//������
	if(EXTI_GetITStatus(EXTI_Line12) != RESET)
	{
		modeSleepToHigh();
		
		EXTI_ClearITPendingBit(EXTI_Line12);
	}
	
	//����connected�ź�������
	if(EXTI_GetITStatus(EXTI_Line10) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line10);
		//���ӶϿ�
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10) == 1)
		{
			flagBLEConnected = 0;
		}
		//��������
		else
		{
			flagBLEConnected = 1;
			if(systemState == MODE_SLEEP)
			{
				modeSleepToLow();
			}
		}	
	}

}

