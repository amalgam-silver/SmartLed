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
	//屏亮时
	if(oledState == OLED_ON)
	{
		//如果上次按键时间距离现在超过 阈值（默认10s）
		if((timeScreenOff <= 60) && (systemSecond - lastTimeKeyPressed >= timeScreenOff))
		{
			//将系统状态设置为关屏状态
			oledState = OLED_OFF;
			//关闭OLED
			oledDisplayOff();
			if((!flagBLEConnected) && (!flagBreathLedEnable) && (!flagDelayLedEnable))
				//进入睡眠模式
				modeHighToSleep();
			else
				//进入低速模式
				modeHighToLow();
		}
	}
	//关屏时
	else
	{
		//若有按键
		if(key)
		{
			//将系统状态设置为屏亮状态
			oledState = OLED_ON;
			if(systemState == MODE_LOW)
			{
				modeLowToHigh();
			}			
			//开启OLED
			oledDisplayOn();
			//清屏
			oledClear();
			//显示界面为主界面
			screenStateNow = displayMainTime;
		}
		if(systemState == MODE_LOW)
		{
			if((!flagBLEConnected) && (!flagBreathLedEnable) && (!flagDelayLedEnable))
				//进入睡眠模式
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


