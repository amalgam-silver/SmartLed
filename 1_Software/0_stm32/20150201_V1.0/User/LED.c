#include "LED.h"

u16 pwmRed = 0, pwmGreen = 0, pwmBlue = 0;

void colorSet(u16 RGB565)
{
	u8 pwmR = RGB565 >> 11;
	u8 pwmG = (RGB565 & 0x07FF) >> 6;
	u8 pwmB = RGB565 & 0x001F;
	
	pwmChange(pwmR,pwmG,pwmB);	
}

void pwmChange(u8 pwmRedSetS,u8 pwmGreenSetS,u8 pwmBlueSetS)
{
	if(pwmRedSetS <= 100)	pwmRed = pwmRedSetS;
	if(pwmGreenSetS <= 100)	pwmGreen = pwmGreenSetS;
	if(pwmBlueSetS <= 100)	pwmBlue = pwmBlueSetS;
	
	TIM_SetCompare1(TIM1, pwmBlue*10);
	TIM_SetCompare2(TIM1, pwmGreen*10);
	TIM_SetCompare3(TIM1, pwmRed*10);
}

void pwmChangeLow(u16 pwmRedSetL,u16 pwmGreenSetL,u16 pwmBlueSetL)
{
	if(pwmRedSetL <= 1000)	pwmRed = pwmRedSetL;
	if(pwmGreenSetL <= 1000)	pwmGreen = pwmGreenSetL;
	if(pwmBlueSetL <= 1000)	pwmBlue = pwmBlueSetL;
	
	TIM_SetCompare1(TIM1, pwmBlue);
	TIM_SetCompare2(TIM1, pwmGreen);
	TIM_SetCompare3(TIM1, pwmRed);
}

void ledInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	//使能GPIOB
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);
	//使能TIM1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	
	//将PB13(TIM1_CH1N)、PB14(TIM1_CH2N)、PB15(TIM1_CH3N)设置为复用推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//系统时钟2M，8分频，每0.5us 定时器counter加1
	TIM_TimeBaseStructure.TIM_Prescaler = 8-1;
	//向上计数模式
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	//周期1000，频率1k
	TIM_TimeBaseStructure.TIM_Period = 1000 - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);
	
	//开启定时器1
	TIM_Cmd(TIM1, ENABLE);
	
	//PWM输出使能
	TIM_CtrlPWMOutputs(TIM1, ENABLE);	
}

