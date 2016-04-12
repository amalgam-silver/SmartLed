#include "main.h"

u32 lastTimeKeyPressed = 0;
void updateSystemState(u8 key);

int main(void)
{
	u8 key;
	
	systemInit();	
	
	slidMainTimeScreen(COME_FROM_LEFT);
	
	screenStateNow = displayMainTime;
	
  	while (1)
  	{
		key = 0;
		if(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1))
		{
			delayMs(300);
			key += 1;
			lastTimeKeyPressed = systemSecond;
		}
		if(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8))
		{
			delayMs(300);
			key += 2;
			lastTimeKeyPressed = systemSecond;
		}
		if(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12))
		{
			delayMs(300);
			if(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12))
			{
				delayMs(700);
				if(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12))
					key += 8;
				else
					key += 4;
			}
			else
				key += 4;
			lastTimeKeyPressed = systemSecond;
		}
			
		if(oledState == OLED_ON)
		{		
			lcdDisplay();
			
			keyAction(key);	
		}
		
		updateSystemState(key);		
  	}
}

void updateSystemState(u8 key)
{
	static u32 preSystemSecond = 0;
	//����ʱ
	if(oledState == OLED_ON)
	{
		//����ϴΰ���ʱ��������ڳ��� ��ֵ��Ĭ��10s��
		if((timeScreenOff <= 60) && (systemSecond - lastTimeKeyPressed >= timeScreenOff))
		{
			//��ϵͳ״̬����Ϊ����״̬
			oledState = OLED_OFF;
			//�ر�OLED
			oledDisplayOff();
			if((!flagBLEConnected) && (!flagBreathLedEnable) && (!flagDelayLedEnable))
				//����˯��ģʽ
				modeHighToSleep();
			else
				//�������ģʽ
				modeHighToLow();
		}
	}
	//����ʱ
	else
	{
		//���а���
		if(key)
		{
			//��ϵͳ״̬����Ϊ����״̬
			oledState = OLED_ON;
			if(systemState == MODE_LOW)
			{
				modeLowToHigh();
			}			
			//����OLED
			oledDisplayOn();
			//����
			oledClear();
			//��ʾ����Ϊ������
			screenStateNow = displayMainTime;
		}
		if(systemState == MODE_LOW)
		{
			if((!flagBLEConnected) && (!flagBreathLedEnable) && (!flagDelayLedEnable))
				//����˯��ģʽ
				modeLowToSleep();
		}
	}
	
	if(preSystemSecond != systemSecond)
	{
		preSystemSecond = systemSecond;
		
		getBatteryState();
		
		if(batteryState <= 1)
		{
			modeBreathLed = breathLowBattery;
		}
		else if(modeBreathLed == breathLowBattery)
		{
			modeBreathLed = breathOff;
			pwmChange(0,0,0);
		}
	}
}


