#include "GUI.h"

u16 temperature = 0;
char tempShow[20];
char num,preSecond = 0;
timeStructure time;
screenState screenStateNow;
/********LED设置界面相关变量********/
ledSetOption optionLedSet;
u8 flagColorLedEnable = 0, flagBreathLedEnable = 0, flagDelayLedEnable = 0;
u8 modeBreathLed = breathOff;
u8 pwmWhite = 0;
u8 pwmRedSet = 0, pwmGreenSet = 0, pwmBlueSet = 0;
u8 ledDelayMin = 0;
u8 strDelayMin[7] = "  0min";
u32 ledDelaySecond = 0;
/********SETTING界面相关变量********/
settingOption optionSetting;
u8 timeScreenOff = 10;
timeStructure timeSetting;
u8 monthDay[12] = {31,29,31,30,31,30,31,31,30,31,30,31};
/********INFO界面相关变量********/
u8 infoPage = 0;

void lcdDisplay(void)
{
	switch(screenStateNow)
	{
		case displayMainTime:
			MainTime();
			break;
		case displayLed:
			break;
		case displaySettingPic:
			break;
		case displayInfoPic:
			break;
		case displayInfo:
			screenInfo();
			break;
		case displayLedSet:
			break;
		case displaySetting:
			break;
	}
}

void keyAction(u8 keyPressed)
{
	switch(screenStateNow)
	{
		case displayMainTime:
			keyActionMainTime(keyPressed);
			break;
		case displayLed:
			keyActionLed(keyPressed);
			break;
		case displaySettingPic:
			keyActionSettingPic(keyPressed);
			break;
		case displayInfoPic:
			keyActionInfoPic(keyPressed);
			break;
		case displayInfo:
			keyActionInfo(keyPressed);
			break;
		case displayLedSet:
			keyActionLedSet(keyPressed);
			break;		
		case displaySetting:
			keyActionSetting(keyPressed);
	}
}

void keyActionSetting(u8 keyPressed)
{
	u8 rowData = 5,rowTime = 7,rowSlid = 2;
	
	switch(keyPressed)
	{
		case LEFT_KEY:
			switch(optionSetting)
			{
				case settingBluetooth:
					bluetoothCmd(!stateBLE);
					oledDisplay(0,0,8,1,(u8*)&fontEnable[stateBLE][0],0,0);
					break;
				case settingYear:	
					if(timeSetting.year == 0)	
						timeSetting.year = 99;
					else timeSetting.year --;
					tempShow[0] = '2';
					tempShow[1] = '0';
					tempShow[2] = timeSetting.year / 10 + '0';
					tempShow[3] = timeSetting.year % 10 + '0';
					tempShow[4] = '\0';	
					oledShowString(22, rowData, tempShow, 0, 1);
					
					if((timeSetting.year % 4 != 0) && (timeSetting.month == 2) 
						&& (timeSetting.data >= 29))
					{
						timeSetting.data = 28;
						tempShow[0] = timeSetting.data / 10 + '0';
						tempShow[1] = timeSetting.data % 10 + '0';
						tempShow[2] = '\0';			
						oledShowString(96, rowData, tempShow, 0, 0);
					}
					break;
				case settingMonth:
					if(timeSetting.month == 1)	
						timeSetting.month = 12;
					else timeSetting.month --;
					tempShow[0] = timeSetting.month / 10 + '0';
					tempShow[1] = timeSetting.month % 10 + '0';
					tempShow[2] = '\0';		
					oledShowString(66, rowData, tempShow, 0, 1);
					
					if(timeSetting.data > monthDay[timeSetting.month - 1])
					{
						timeSetting.data = monthDay[timeSetting.month - 1];
						
						if(timeSetting.month == 2)
						{
							if((timeSetting.year % 4 != 0) && (timeSetting.data > 28))
								timeSetting.data = 28;
						}
						tempShow[0] = timeSetting.data / 10 + '0';
						tempShow[1] = timeSetting.data % 10 + '0';
						tempShow[2] = '\0';			
						oledShowString(96, rowData, tempShow, 0, 0);
					}
					break;
				case settingData:
					if(timeSetting.data == 1)	
						timeSetting.data = monthDay[timeSetting.month-1];
					else timeSetting.data --;
					
					if((timeSetting.year % 4 != 0) && (timeSetting.month == 2) 
						&& (timeSetting.data >= 29))
						timeSetting.data = 28;
					
					tempShow[0] = timeSetting.data / 10 + '0';
					tempShow[1] = timeSetting.data % 10 + '0';
					tempShow[2] = '\0';			
					oledShowString(96, rowData, tempShow, 0, 1);
					break;
				case settingHour:
					if(timeSetting.hour == 0)
						timeSetting.hour = 23;
					else
						timeSetting.hour --;
					
					tempShow[0] = timeSetting.hour / 10 + '0';
					tempShow[1] = timeSetting.hour % 10 + '0';
					tempShow[2] = '\0';	
					oledShowString(28, rowTime, tempShow, 0, 1);
					break;
				case settingMinute:
					if(timeSetting.minute == 0)
						timeSetting.minute = 59;
					else
						timeSetting.minute --;
					
					tempShow[0] = timeSetting.minute / 10 + '0';
					tempShow[1] = timeSetting.minute % 10 + '0';
					tempShow[2] = '\0';		
					oledShowString(58, rowTime, tempShow, 0, 1);
					break;
				case settingSecond:
					if(timeSetting.second == 0)
						timeSetting.second = 59;
					else
						timeSetting.second --;
					
					tempShow[0] = timeSetting.second / 10 + '0';
					tempShow[1] = timeSetting.second % 10 + '0';
					tempShow[2] = '\0';			
					oledShowString(88, rowTime, tempShow, 0, 1);
					break;
				case settingTimeScreenOff:
					timeScreenOff -= 10;
					if(timeScreenOff <= 10)	timeScreenOff = 10;
					
					if(timeScreenOff <= 60)
					{
						oledSlid(4, rowSlid, 72, timeScreenOff, 1);
						num = num2string(tempShow,timeScreenOff,0);
						tempShow[num] = 's';
						tempShow[num+1] = '\0';
						oledShowString(84,rowSlid,"     ",0,0);
						oledShowString(84,rowSlid,tempShow,0,0);
					}
					else
					{
						oledSlid(4, rowSlid, 72, 70, 1);
						oledShowString(84,rowSlid,"Never",0,0);
					}
					break;
			}
			break;
		case RIGHT_KEY:
			switch(optionSetting)
			{
				case settingBluetooth:
					bluetoothCmd(!stateBLE);
					oledDisplay(0,0,8,1,(u8*)&fontEnable[stateBLE][0],0,0);
					break;
				case settingYear:	
					if(timeSetting.year == 99)	
						timeSetting.year = 0;
					else timeSetting.year ++;
					tempShow[0] = '2';
					tempShow[1] = '0';
					tempShow[2] = timeSetting.year / 10 + '0';
					tempShow[3] = timeSetting.year % 10 + '0';
					tempShow[4] = '\0';	
					oledShowString(22, rowData, tempShow, 0, 1);
					
					if((timeSetting.year % 4 != 0) && (timeSetting.month == 2) 
						&& (timeSetting.data >= 29))
					{
						timeSetting.data = 28;
						tempShow[0] = timeSetting.data / 10 + '0';
						tempShow[1] = timeSetting.data % 10 + '0';
						tempShow[2] = '\0';			
						oledShowString(96, rowData, tempShow, 0, 0);
					}
					break;
				case settingMonth:
					if(timeSetting.month == 12)	
						timeSetting.month = 1;
					else timeSetting.month ++;
					tempShow[0] = timeSetting.month / 10 + '0';
					tempShow[1] = timeSetting.month % 10 + '0';
					tempShow[2] = '\0';		
					oledShowString(66, rowData, tempShow, 0, 1);
					
					if(timeSetting.data > monthDay[timeSetting.month - 1])
					{
						timeSetting.data = monthDay[timeSetting.month - 1];
						
						if(timeSetting.month == 2)
						{
							if((timeSetting.year % 4 != 0) && (timeSetting.data > 28))
								timeSetting.data = 28;
						}
						tempShow[0] = timeSetting.data / 10 + '0';
						tempShow[1] = timeSetting.data % 10 + '0';
						tempShow[2] = '\0';			
						oledShowString(96, rowData, tempShow, 0, 0);
					}				
					break;
				case settingData:
					if(timeSetting.data == monthDay[timeSetting.month-1])	
						timeSetting.data = 1;
					else timeSetting.data ++;
					
					if((timeSetting.year % 4 != 0) && (timeSetting.month == 2) 
						&& (timeSetting.data >= 29))
						timeSetting.data = 28;
					
					tempShow[0] = timeSetting.data / 10 + '0';
					tempShow[1] = timeSetting.data % 10 + '0';
					tempShow[2] = '\0';			
					oledShowString(96, rowData, tempShow, 0, 1);
					break;
				case settingHour:
					if(timeSetting.hour == 23)
						timeSetting.hour = 0;
					else
						timeSetting.hour ++;
					
					tempShow[0] = timeSetting.hour / 10 + '0';
					tempShow[1] = timeSetting.hour % 10 + '0';
					tempShow[2] = '\0';	
					oledShowString(28, rowTime, tempShow, 0, 1);
					break;
				case settingMinute:
					if(timeSetting.minute == 59)
						timeSetting.minute = 0;
					else
						timeSetting.minute ++;
					
					tempShow[0] = timeSetting.minute / 10 + '0';
					tempShow[1] = timeSetting.minute % 10 + '0';
					tempShow[2] = '\0';		
					oledShowString(58, rowTime, tempShow, 0, 1);
					break;
				case settingSecond:
					if(timeSetting.second == 59)
						timeSetting.second = 0;
					else
						timeSetting.second ++;
					
					tempShow[0] = timeSetting.second / 10 + '0';
					tempShow[1] = timeSetting.second % 10 + '0';
					tempShow[2] = '\0';			
					oledShowString(88, rowTime, tempShow, 0, 1);
					break;
				case settingTimeScreenOff:
					timeScreenOff += 10;
					if(timeScreenOff >= 70)	timeScreenOff = 70;
					
					if(timeScreenOff <= 60)
					{
						oledSlid(4, rowSlid, 72, timeScreenOff, 1);
						num = num2string(tempShow,timeScreenOff,0);
						tempShow[num] = 's';
						tempShow[num+1] = '\0';
						oledShowString(84,rowSlid,"     ",0,0);
						oledShowString(84,rowSlid,tempShow,0,0);
					}
					else
					{
						oledSlid(4, rowSlid, 72, 70, 1);
						oledShowString(84,rowSlid,"Never",0,0);
					}
					break;
			}
			break;
		case DOWN_KEY:
			switch(optionSetting)
			{
				case settingBluetooth:
					optionSetting = settingTimeScreenOff;
					oledShowString(8,0,"Bluetooth",0,0);
					
					if(timeScreenOff <= 60)
					{
						oledSlid(4, rowSlid, 72, timeScreenOff, 1);
					}
					else
					{
						oledSlid(4, rowSlid, 72, 70, 1);
					}
					break;
				case settingTimeScreenOff:
					optionSetting = settingYear;
					if(timeScreenOff <= 60)
					{
						oledSlid(4, rowSlid, 72, timeScreenOff, 0);
					}
					else
					{
						oledSlid(4, rowSlid, 72, 70, 0);
					}
					tempShow[0] = '2';
					tempShow[1] = '0';
					tempShow[2] = timeSetting.year / 10 + '0';
					tempShow[3] = timeSetting.year % 10 + '0';
					tempShow[4] = '\0';	
					oledShowString(22, rowData, tempShow, 0, 1);
					break;
				case settingYear:
					optionSetting = settingMonth;
					tempShow[0] = '2';
					tempShow[1] = '0';
					tempShow[2] = timeSetting.year / 10 + '0';
					tempShow[3] = timeSetting.year % 10 + '0';
					tempShow[4] = '\0';	
					oledShowString(22, rowData, tempShow, 0, 0);
				
					tempShow[0] = timeSetting.month / 10 + '0';
					tempShow[1] = timeSetting.month % 10 + '0';
					tempShow[2] = '\0';		
					oledShowString(66, rowData, tempShow, 0, 1);
					break;
				case settingMonth:
					optionSetting = settingData;
					tempShow[0] = timeSetting.month / 10 + '0';
					tempShow[1] = timeSetting.month % 10 + '0';
					tempShow[2] = '\0';		
					oledShowString(66, rowData, tempShow, 0, 0);
					
					tempShow[0] = timeSetting.data / 10 + '0';
					tempShow[1] = timeSetting.data % 10 + '0';
					tempShow[2] = '\0';			
					oledShowString(96, rowData, tempShow, 0, 1);
					break;
				case settingData:
					optionSetting = settingHour;
					tempShow[0] = timeSetting.data / 10 + '0';
					tempShow[1] = timeSetting.data % 10 + '0';
					tempShow[2] = '\0';			
					oledShowString(96, rowData, tempShow, 0, 0);
					
					tempShow[0] = timeSetting.hour / 10 + '0';
					tempShow[1] = timeSetting.hour % 10 + '0';
					tempShow[2] = '\0';	
					oledShowString(28, rowTime, tempShow, 0, 1);
					break;
				case settingHour:
					optionSetting = settingMinute;
					tempShow[0] = timeSetting.hour / 10 + '0';
					tempShow[1] = timeSetting.hour % 10 + '0';
					tempShow[2] = '\0';	
					oledShowString(28, rowTime, tempShow, 0, 0);
					
					tempShow[0] = timeSetting.minute / 10 + '0';
					tempShow[1] = timeSetting.minute % 10 + '0';
					tempShow[2] = '\0';		
					oledShowString(58, rowTime, tempShow, 0, 1);
					break;
				case settingMinute:
					optionSetting = settingSecond;
					tempShow[0] = timeSetting.minute / 10 + '0';
					tempShow[1] = timeSetting.minute % 10 + '0';
					tempShow[2] = '\0';		
					oledShowString(58, rowTime, tempShow, 0, 0);
					
					tempShow[0] = timeSetting.second / 10 + '0';
					tempShow[1] = timeSetting.second % 10 + '0';
					tempShow[2] = '\0';			
					oledShowString(88, rowTime, tempShow, 0, 1);
					break;
				case settingSecond:
					optionSetting = settingBluetooth;
					tempShow[0] = timeSetting.second / 10 + '0';
					tempShow[1] = timeSetting.second % 10 + '0';
					tempShow[2] = '\0';			
					oledShowString(88, rowTime, tempShow, 0, 0);
					
					oledShowString(8,0,"Bluetooth",0,1);
					
					break;
			}
			break;
		case DOWN_KEY_LONG:
			slidSettingPic(ACTION_NONE);
			if((optionSetting != settingTimeScreenOff) && (optionSetting != settingBluetooth))
				timeSet(timeSetting);
			while(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12));
			screenStateNow = displaySettingPic;
			break;
	}
}

void screenSetting(void)
{
	u8 rowData = 5,rowTime = 7,rowSlid = 2;
	timeRead(&time);
	
	timeSetting = time;
	//时间、日期设置
	oledShowString(0,rowData-1,"Time Set:",0,0);
	//年
	tempShow[0] = '2';
	tempShow[1] = '0';
	tempShow[2] = timeSetting.year / 10 + '0';
	tempShow[3] = timeSetting.year % 10 + '0';
	tempShow[4] = '\0';	
	oledShowString(22, rowData, tempShow, 0, 0);
	
	oledShowString(48, rowData, " - ", 0, 0);
	//月
	tempShow[0] = timeSetting.month / 10 + '0';
	tempShow[1] = timeSetting.month % 10 + '0';
	tempShow[2] = '\0';		
	oledShowString(66, rowData, tempShow, 0, 0);
	
	oledShowString(78, rowData, " - ", 0, 0);
	//日
	tempShow[0] = timeSetting.data / 10 + '0';
	tempShow[1] = timeSetting.data % 10 + '0';
	tempShow[2] = '\0';			
	oledShowString(96, rowData, tempShow, 0, 0);
	//时
	tempShow[0] = timeSetting.hour / 10 + '0';
	tempShow[1] = timeSetting.hour % 10 + '0';
	tempShow[2] = '\0';	
	oledShowString(28, rowTime, tempShow, 0, 0);
	
	oledShowString(40, rowTime, " : ", 0, 0);
	//分
	tempShow[0] = timeSetting.minute / 10 + '0';
	tempShow[1] = timeSetting.minute % 10 + '0';
	tempShow[2] = '\0';		
	oledShowString(58, rowTime, tempShow, 0, 0);
	
	oledShowString(70, rowTime, " : ", 0, 0);
	//秒
	tempShow[0] = timeSetting.second / 10 + '0';
	tempShow[1] = timeSetting.second % 10 + '0';
	tempShow[2] = '\0';			
	oledShowString(88, rowTime, tempShow, 0, 0);
	
	//关屏时间设置
	oledShowString(0, rowSlid-1, "Screen Off:", 0, 0);
	
	if(timeScreenOff <= 60)
	{
		oledSlid(4, rowSlid, 72, timeScreenOff, 0);
		num = num2string(tempShow,timeScreenOff,0);
		tempShow[num] = 's';
		tempShow[num+1] = '\0';
		oledShowString(84,rowSlid,tempShow,0,0);
	}
	else
	{
		oledSlid(4, rowSlid, 72, 70, 0);
		oledShowString(84,rowSlid,"Never",0,0);
	}
	
	//蓝牙开关设置
	oledDisplay(0,0,8,1,(u8*)&fontEnable[stateBLE][0],0,0);
	oledShowString(8,0,"Bluetooth",0,1);
}

void keyActionLedSet(u8 keyPressed)
{
	switch(keyPressed)
	{
		case LEFT_KEY:
			switch(optionLedSet)
			{
				//是否开启彩灯模式
				case ledSetColorLedEnable:
					//标志位翻转
					flagColorLedEnable = !flagColorLedEnable;
					//显示标志位
					oledDisplay(0,0,8,1,(u8*)&fontEnable[flagColorLedEnable][0],0,0);
					//如果是照明模式
					if(!flagColorLedEnable)
					{
						//关闭呼吸灯
						flagBreathLedEnable = 0;
						modeBreathLed = breathOff;
						
						//显示照明模式选项
						oledClearLind(1,7);
						//白灯PWM
						oledShowString(0,2,"Light:",0,0);
						oledSlid(0,3,102,pwmWhite,0);	
						num = num2string(tempShow,pwmWhite,0);
						tempShow[num] = ' ';
						tempShow[num] = ' ';
						tempShow[num] = '\0';
						oledShowString(104,3,tempShow,0,0);
						pwmChange(pwmWhite,pwmWhite,pwmWhite);
						//计算延时秒数
						oledShowString(0,5,"Delay:",0,0);
						ledDelayMin = ledDelaySecond / 60;
						oledSlid(0,6,62,ledDelayMin>>1,0);	
						strDelayMin[0] = ((ledDelayMin >= 100) ? (ledDelayMin / 100 + '0'): ' ');
						strDelayMin[1] = ((ledDelayMin >= 10) ? (ledDelayMin / 10 % 10 + '0') : ' ');
						strDelayMin[2] = ledDelayMin % 10 + '0';
						oledShowString(66,6,(char *)strDelayMin,0,0);
						
					}
					//如果是彩灯模式
					else
					{
						//显示彩灯模式选项
						oledClearLind(1,7);
						//呼吸灯模式
						oledDisplay(0,1,8,1,(u8*)&fontEnable[flagBreathLedEnable][0],0,0);
						oledShowString(10,1,"BreathMode",0,0);
						//红灯PWM
						oledShowString(0,2,"Red:",0,0);
						oledSlid(0,3,102,pwmRedSet,0);	
						num = num2string(tempShow,pwmRedSet,0);
						tempShow[num] = '\0';
						oledShowString(104,3,tempShow,0,0);
						//绿灯PWM
						oledShowString(0,4,"Green:",0,0);
						oledSlid(0,5,102,pwmGreenSet,0);	
						num = num2string(tempShow,pwmGreenSet,0);
						tempShow[num] = '\0';
						oledShowString(104,5,"   ",0,0);
						oledShowString(104,5,tempShow,0,0);
						//蓝灯PWM
						oledShowString(0,6,"Blue:",0,0);
						oledSlid(0,7,102,pwmBlueSet,0);	
						num = num2string(tempShow,pwmBlueSet,0);
						tempShow[num] = '\0';
						oledShowString(104,7,tempShow,0,0);
						pwmChange(pwmRedSet,pwmGreenSet,pwmBlueSet);
					}
					break;
				//是否开启呼吸灯
				case ledSetBreathLedEnable:
					//标志位取反
					flagBreathLedEnable = !flagBreathLedEnable;
					//显示标志位
					oledDisplay(0,1,8,1,(u8*)&fontEnable[flagBreathLedEnable][0],0,0);
					if(flagBreathLedEnable)
						modeBreathLed = breathColor;
					else
						modeBreathLed = breathOff;
					break;
				//设置白灯PWM
				case ledSetPwmWhiteLedSet:
					if(pwmWhite <= 10)	pwmWhite = 0;
					else
						pwmWhite -= 10;
					
					pwmChange(pwmWhite,pwmWhite,pwmWhite);
					
					oledSlid(0,3,102,pwmWhite,1);
			
					num = num2string(tempShow,pwmWhite,0);
					tempShow[num] = '\0';
					oledShowString(104,3,"   ",0,0);
					oledShowString(104,3,tempShow,0,0);
					break;
				//设置延时
				case ledSetDelay:
					if(ledDelayMin <= 10)	ledDelayMin = 0;
					else	ledDelayMin -= 10;
					
					oledSlid(0,6,62,ledDelayMin>>1,1);	
					strDelayMin[0] = ((ledDelayMin >= 100) ? (ledDelayMin / 100 + '0'): ' ');
					strDelayMin[1] = ((ledDelayMin >= 10) ? (ledDelayMin / 10 % 10 + '0') : ' ');
					strDelayMin[2] = ledDelayMin % 10 + '0';
					oledShowString(66,6,(char *)strDelayMin,0,0);
					break;
				//设置红灯PWM
				case ledSetPwmRedLedSet:
					if(pwmRedSet <= 1)	pwmRedSet = 0;
					else
						pwmRedSet -= 1;
					
					if(flagBreathLedEnable == DISABLE)
						pwmChange(pwmRedSet,pwmGreenSet,pwmBlueSet);
					
					oledSlid(0,3,102,pwmRedSet,1);
			
					num = num2string(tempShow,pwmRedSet,0);
					tempShow[num] = '\0';
					oledShowString(104,3,"   ",0,0);
					oledShowString(104,3,tempShow,0,0);
					break;
				//设置绿灯PWM
				case ledSetPwmGreenLedSet:
					if(pwmGreenSet <= 1)	pwmGreenSet = 0;
					else
						pwmGreenSet -= 1;
					
					if(flagBreathLedEnable == DISABLE)
						pwmChange(pwmRedSet,pwmGreenSet,pwmBlueSet);
					
					oledSlid(0,5,102,pwmGreenSet,1);
			
					num = num2string(tempShow,pwmGreenSet,0);
					tempShow[num] = '\0';
					oledShowString(104,5,"   ",0,0);
					oledShowString(104,5,tempShow,0,0);
					break;
				//设置蓝灯PWM
				case ledSetPwmBlueLedSet:
					if(pwmBlueSet <= 1)	pwmBlueSet = 0;
					else
						pwmBlueSet -= 1;
					
					if(flagBreathLedEnable == DISABLE)
						pwmChange(pwmRedSet,pwmGreenSet,pwmBlueSet);
					
					oledSlid(0,7,102,pwmBlueSet,1);
			
					num = num2string(tempShow,pwmBlueSet,0);
					tempShow[num] = '\0';
					oledShowString(104,7,"   ",0,0);
					oledShowString(104,7,tempShow,0,0);
					break;
			}
			break;
		case RIGHT_KEY:
			switch(optionLedSet)
			{
				//是否开启彩灯模式
				case ledSetColorLedEnable:
					//标志位翻转
					flagColorLedEnable = !flagColorLedEnable;
					//显示标志位
					oledDisplay(0,0,8,1,(u8*)&fontEnable[flagColorLedEnable][0],0,0);
					//如果是照明模式
					if(!flagColorLedEnable)
					{
						//关闭呼吸灯
						flagBreathLedEnable = 0;
						modeBreathLed = breathOff;
						
						//显示照明模式选项
						oledClearLind(1,7);
						//白灯PWM
						oledShowString(0,2,"Light:",0,0);
						oledSlid(0,3,102,pwmWhite,0);	
						num = num2string(tempShow,pwmWhite,0);
						tempShow[num] = ' ';
						tempShow[num] = ' ';
						tempShow[num] = '\0';
						oledShowString(104,3,tempShow,0,0);
						pwmChange(pwmWhite,pwmWhite,pwmWhite);
						//计算延时秒数
						ledDelayMin = ledDelaySecond / 60;
						oledSlid(0,6,62,ledDelayMin>>1,0);	
						strDelayMin[0] = ((ledDelayMin >= 100) ? (ledDelayMin / 100 + '0'): ' ');
						strDelayMin[1] = ((ledDelayMin >= 10) ? (ledDelayMin / 10 % 10 + '0') : ' ');
						strDelayMin[2] = ledDelayMin % 10 + '0';
						oledShowString(66,6,(char *)strDelayMin,0,0);
						
					}
					//如果是彩灯模式
					else
					{
						//显示彩灯模式选项
						oledClearLind(1,7);
						//呼吸灯模式
						oledDisplay(0,1,8,1,(u8*)&fontEnable[flagBreathLedEnable][0],0,0);
						oledShowString(10,1,"BreathMode",0,0);
						//红灯PWM
						oledShowString(0,2,"Red:",0,0);
						oledSlid(0,3,102,pwmRedSet,0);	
						num = num2string(tempShow,pwmRedSet,0);
						tempShow[num] = '\0';
						oledShowString(104,3,tempShow,0,0);
						//绿灯PWM
						oledShowString(0,4,"Green:",0,0);
						oledSlid(0,5,102,pwmGreenSet,0);	
						num = num2string(tempShow,pwmGreenSet,0);
						tempShow[num] = '\0';
						oledShowString(104,5,"   ",0,0);
						oledShowString(104,5,tempShow,0,0);
						//蓝灯PWM
						oledShowString(0,6,"Blue:",0,0);
						oledSlid(0,7,102,pwmBlueSet,0);	
						num = num2string(tempShow,pwmBlueSet,0);
						tempShow[num] = '\0';
						oledShowString(104,7,tempShow,0,0);
						pwmChange(pwmRedSet,pwmGreenSet,pwmBlueSet);
					}
					break;
				//是否开启呼吸灯
				case ledSetBreathLedEnable:
					//标志位取反
					flagBreathLedEnable = !flagBreathLedEnable;
					//显示标志位
					oledDisplay(0,1,8,1,(u8*)&fontEnable[flagBreathLedEnable][0],0,0);
					if(flagBreathLedEnable)
						modeBreathLed = breathColor;
					else
						modeBreathLed = breathOff;
					break;
				//设置白灯PWM
				case ledSetPwmWhiteLedSet:
					if(pwmWhite >= 90)	pwmWhite = 100;
					else
						pwmWhite += 10;
					
					pwmChange(pwmWhite,pwmWhite,pwmWhite);
					
					oledSlid(0,3,102,pwmWhite,1);
			
					num = num2string(tempShow,pwmWhite,0);
					tempShow[num] = '\0';
					oledShowString(104,3,"   ",0,0);
					oledShowString(104,3,tempShow,0,0);
					break;
				//设置延时
				case ledSetDelay:
					if(ledDelayMin >= 110)	ledDelayMin = 120;
					else	ledDelayMin += 10;
					
					oledSlid(0,6,62,ledDelayMin>>1,1);	
					strDelayMin[0] = ((ledDelayMin >= 100) ? (ledDelayMin / 100 + '0'): ' ');
					strDelayMin[1] = ((ledDelayMin >= 10) ? (ledDelayMin / 10 % 10 + '0') : ' ');
					strDelayMin[2] = ledDelayMin % 10 + '0';
					oledShowString(66,6,(char *)strDelayMin,0,0);
					break;
				//设置红灯PWM
				case ledSetPwmRedLedSet:
					if(pwmRedSet >= 99)	pwmRedSet = 100;
					else
						pwmRedSet += 1;
					
					if(flagBreathLedEnable == DISABLE)
						pwmChange(pwmRedSet,pwmGreenSet,pwmBlueSet);
					
					oledSlid(0,3,102,pwmRedSet,1);
			
					num = num2string(tempShow,pwmRedSet,0);
					tempShow[num] = '\0';
					oledShowString(104,3,"   ",0,0);
					oledShowString(104,3,tempShow,0,0);
					break;
				//设置绿灯PWM
				case ledSetPwmGreenLedSet:
					if(pwmGreenSet >= 99)	pwmGreenSet = 100;
					else
						pwmGreenSet += 1;
					
					if(flagBreathLedEnable == DISABLE)
						pwmChange(pwmRedSet,pwmGreenSet,pwmBlueSet);
					
					oledSlid(0,5,102,pwmGreenSet,1);
			
					num = num2string(tempShow,pwmGreenSet,0);
					tempShow[num] = '\0';
					oledShowString(104,5,"   ",0,0);
					oledShowString(104,5,tempShow,0,0);
					break;
				//设置蓝灯PWM
				case ledSetPwmBlueLedSet:
					if(pwmBlueSet >= 99)	pwmBlueSet = 100;
					else
						pwmBlueSet += 1;
					
					if(flagBreathLedEnable == DISABLE)
						pwmChange(pwmRedSet,pwmGreenSet,pwmBlueSet);
					
					oledSlid(0,7,102,pwmBlueSet,1);
			
					num = num2string(tempShow,pwmBlueSet,0);
					tempShow[num] = '\0';
					oledShowString(104,7,"   ",0,0);
					oledShowString(104,7,tempShow,0,0);
					break;
			}
			break;
		case DOWN_KEY:
			switch(optionLedSet)
			{
				case ledSetColorLedEnable:
					if(!flagColorLedEnable)
					{
						optionLedSet = ledSetPwmWhiteLedSet;	
						oledShowString(10,0,"ColorMode",0,0);
						oledShowString(0,2,"Light:",0,1);					
						oledSlid(0,3,102,pwmWhite,1);
					}
					else
					{
						optionLedSet = ledSetBreathLedEnable;
						oledShowString(10,0,"ColorMode",0,0);
						oledShowString(10,1,"BreathMode",0,1);
					}	
					break;
				
				case ledSetBreathLedEnable:
					optionLedSet = ledSetPwmRedLedSet;
					oledShowString(0,2,"Red:",0,1);
					oledSlid(0,3,102,pwmRedSet,1);
					oledShowString(10,1,"BreathMode",0,0);
					break;
					
				case ledSetPwmWhiteLedSet:
					optionLedSet = ledSetDelay;
					oledShowString(0,2,"Light:",0,0);
					oledSlid(0,3,102,pwmWhite,0);
					oledShowString(0,5,"Delay:",0,1);
					oledSlid(0,6,62,ledDelayMin>>1,1);		
					break;
				
				case ledSetDelay:
					optionLedSet = ledSetColorLedEnable;
					oledShowString(0,5,"Delay:",0,0);
					oledSlid(0,6,62,ledDelayMin>>1,0);	
					oledShowString(10,0,"ColorMode",0,1);
					break;
				
				case ledSetPwmRedLedSet:
					optionLedSet = ledSetPwmGreenLedSet;	
					oledShowString(0,2,"Red:",0,0);
					oledSlid(0,3,102,pwmRedSet,0);
					oledShowString(0,4,"Green:",0,1);
					oledSlid(0,5,102,pwmGreenSet,1);			
					break;
				
				case ledSetPwmGreenLedSet:
					optionLedSet = ledSetPwmBlueLedSet;
					oledShowString(0,4,"Green:",0,0);
					oledSlid(0,5,102,pwmGreenSet,0);
					oledShowString(0,6,"Blue:",0,1);
					oledSlid(0,7,102,pwmBlueSet,1);
					break;
				
				case ledSetPwmBlueLedSet:
					optionLedSet = ledSetColorLedEnable;
					oledShowString(0,6,"Blue:",0,0);
					oledSlid(0,7,102,pwmBlueSet,0);
					oledShowString(10,0,"ColorMode",0,1);
					break;
			}
			break;
		case DOWN_KEY_LONG:
			//处于照明模式，设置了延时时间，并且灯光打开
			if((ledDelayMin > 0) && (pwmWhite > 0) && (flagColorLedEnable == DISABLE))
			{
				//开始计时
				flagDelayLedEnable = ENABLE;
				//计算延时秒数
				ledDelaySecond = ledDelayMin * 60;
			}			
			else
			{
				flagDelayLedEnable = DISABLE;
				ledDelaySecond = 0;
			}
			slidLed(ACTION_NONE);
			while(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12));
			screenStateNow = displayLed;
			break;
	}
}

void screenLedSet(void)
{
	//彩灯选项	
	oledDisplay(0,0,8,1,(u8*)&fontEnable[flagColorLedEnable][0],0,0);
	oledShowString(10,0,"ColorMode",0,1);
	
	//照明模式
	if(!flagColorLedEnable)
	{
		//白灯PWM
		oledShowString(0,2,"Light:",0,0);
		oledSlid(0,3,102,pwmWhite,0);	
		num = num2string(tempShow,pwmWhite,0);
		tempShow[num] = '\0';
		oledShowString(104,3,tempShow,0,0);
		pwmChange(pwmWhite,pwmWhite,pwmWhite);
		
		//延时时间
		oledShowString(0,5,"Delay:",0,0);
		//计算延时秒数
		ledDelayMin = ledDelaySecond / 60;
		oledSlid(0,6,62,ledDelayMin>>1,0);	
		strDelayMin[0] = ((ledDelayMin >= 100) ? (ledDelayMin / 100 + '0'): ' ');
		strDelayMin[1] = ((ledDelayMin >= 10) ? (ledDelayMin / 10 % 10 + '0') : ' ');
		strDelayMin[2] = ledDelayMin % 10 + '0';
		oledShowString(66,6,(char *)strDelayMin,0,0);
	}
	//彩灯模式
	else
	{
		//呼吸灯模式
		oledDisplay(0,1,8,1,(u8*)&fontEnable[flagBreathLedEnable][0],0,0);
		oledShowString(10,1,"BreathMode",0,0);
		
		//红灯PWM
		oledShowString(0,2,"Red:",0,0);
		oledSlid(0,3,102,pwmRedSet,0);	
		num = num2string(tempShow,pwmRedSet,0);
		tempShow[num] = '\0';
		oledShowString(104,3,tempShow,0,0);
		
		//绿灯PWM
		oledShowString(0,4,"Green:",0,0);
		oledSlid(0,5,102,pwmGreenSet,0);	
		num = num2string(tempShow,pwmGreenSet,0);
		tempShow[num] = '\0';
		oledShowString(104,5,tempShow,0,0);
		
		//蓝灯PWM
		oledShowString(0,6,"Blue:",0,0);
		oledSlid(0,7,102,pwmBlueSet,0);	
		num = num2string(tempShow,pwmBlueSet,0);
		tempShow[num] = '\0';
		oledShowString(104,7,tempShow,0,0);
		pwmChange(pwmRedSet,pwmGreenSet,pwmBlueSet);
	}
}

void keyActionInfo(u8 keyPressed)
{
	switch(keyPressed)
	{
		case LEFT_KEY:
			infoPage = !infoPage;
			if(infoPage)
			{
				oledClear();
				//显示版本信息
				oledShowString(40,6,"SmartLed V1.0",0,0);
                num = num2string(tempShow,SW_VERSION,2);
                tempShow[num] = '\0';
                oledShowString(102,6,tempShow,0,0);
				oledShowString(40,7,"Designed by HG",0,0);
			}
			else
			{
				oledClearLind(6,7);
			}
			break;
		case RIGHT_KEY:
			infoPage = !infoPage;
			if(infoPage)
			{
				oledClear();
				//显示版本信息
				oledShowString(40,6,"SmartLed V1.0",0,0);
                num = num2string(tempShow,SW_VERSION,2);
                tempShow[num] = '\0';
                oledShowString(102,6,tempShow,0,0);
				oledShowString(40,7,"Designed by HG",0,0);
			}
			else
			{
				oledClearLind(6,7);
			}
			break;
		case DOWN_KEY:
			slidInfoPic(ACTION_NONE);
			screenStateNow = displayInfoPic;
			break;
		case DOWN_KEY_LONG:
			break;
	}
}

void screenInfo(void)
{
	static u32 preSecond = 0;
	
	if(!infoPage)
	{
		if(preSecond != systemSecond)
		{
			preSecond = systemSecond;
			//读取时间
			timeRead(&time);
			//读取温度
			temperature = readTemperature();
			//获得电池状态
			getBatteryState();
			
			//显示温度
			num = num2string(tempShow,temperature,2);
			tempShow[num] = 'z'+2;
			tempShow[num+1] = 'C';
			tempShow[num+2] = '\0';		
			oledShowString(0, 1, tempShow, 0, 0);

			//显示时间日期
			time2String(time,tempShow);
			oledShowString(0, 0, tempShow, 0, 0);
			
			//显示电池状态
			oledShowBattery(118,1,batteryState, 0);
			num = num2string(tempShow,voltageBattery,2);
			tempShow[num] = 'V';
			tempShow[num+1] = '\0';	
			oledShowString(0, 2, tempShow, 0, 0);
			
			if(stateCharging == 1)
				oledShowString(0,3,"Charging:Y",0,0);
			else
				oledShowString(0,3,"Charging:N",0,0);
			
			if(stateReady == 1)
				oledShowString(72,3,"Full:Y",0,0);
			else
				oledShowString(72,3,"Full:N",0,0);
			
			//显示蓝牙信息
			if(stateBLE == ENABLE)
				oledShowString(0,5,"BLE: On,",0,0);
			else
				oledShowString(0,5,"BLE: Off,",0,0);
			
			if(flagBLEConnected == 1)
				oledShowString(54,5,"Connected   ",0,0);
			else
				oledShowString(54,5,"Disconnected",0,0);
				
			//显示定时信息
			if(flagDelayLedEnable == ENABLE)
			{
				oledShowString(0,7,"Led Delay:",0,0);
				num = num2string(tempShow,ledDelaySecond,0);
				tempShow[num] = 's';
                tempShow[num+1] = ' ';
                tempShow[num+2] = ' ';
                tempShow[num+3] = ' ';
				tempShow[num+4] = '\0';
				oledShowString(60,7,tempShow,0,0);
			}
			else
			{
				oledShowString(0,7,"Led Delay:Off    ",0,0);
			}
		}
	}
	else
	{
		
	}

}

void keyActionInfoPic(u8 keyPressed)
{
	switch(keyPressed)
	{
		case LEFT_KEY:
			slidInfoPic(GO_TO_LEFT);
			slidSettingPic(COME_FROM_RIGHT);
			screenStateNow = displaySettingPic;
			break;
		case RIGHT_KEY:
			slidInfoPic(GO_TO_RIGHT);
			slidLed(COME_FROM_LEFT);
			screenStateNow = displayLed;	
			break;
		case DOWN_KEY:
			infoPage = 0;
			oledClear();
			screenStateNow = displayInfo;
			break;
		case DOWN_KEY_LONG:
			break;
	}
}

void slidInfoPic(u8 state)
{
	extern const unsigned char gImage_INFO[][64];
		int i,iFinal;
	
	switch(state)
	{
		case COME_FROM_LEFT:
			i = -128;
			iFinal = 0;
			break;
		case COME_FROM_RIGHT:
			i = 128;
			iFinal = 0;
			break;
		case GO_TO_LEFT:
			i = 0;
			iFinal = -128;
			break;
		case GO_TO_RIGHT:
			i = 0;
			iFinal = 128;
			break;
		default:
			oledClear();
			oledDisplay(32,0,64,8,(u8*)gImage_INFO,0,0);
			return;
	}
	
	while(i!=iFinal)
	{
		if(state == COME_FROM_LEFT || state == GO_TO_RIGHT)
			i += 2;
		else
			i -= 2;
		
		oledDisplay(32,0,64,8,(u8*)gImage_INFO,i,0);		
	}
}

void keyActionSettingPic(u8 keyPressed)
{
	switch(keyPressed)
	{
		case LEFT_KEY:
			slidSettingPic(GO_TO_LEFT);
			slidMainTimeScreen(COME_FROM_RIGHT);
			screenStateNow = displayMainTime;
			break;
		case RIGHT_KEY:
			slidSettingPic(GO_TO_RIGHT);
			slidInfoPic(COME_FROM_LEFT);
			screenStateNow = displayInfoPic;
			break;
		case DOWN_KEY:
			oledClear();
			screenSetting();
			optionSetting = settingBluetooth;
			screenStateNow = displaySetting;
			break;
		case DOWN_KEY_LONG:
			break;
	}
}

void slidSettingPic(u8 state)
{
	extern const unsigned char gImage_SETTING[][64];
	int i,iFinal;
	
	switch(state)
	{
		case COME_FROM_LEFT:
			i = -128;
			iFinal = 0;
			break;
		case COME_FROM_RIGHT:
			i = 128;
			iFinal = 0;
			break;
		case GO_TO_LEFT:
			i = 0;
			iFinal = -128;
			break;
		case GO_TO_RIGHT:
			i = 0;
			iFinal = 128;
			break;
		default:
			oledClear();
			oledDisplay(32,0,64,8,(u8*)gImage_SETTING,0,0);
			return;
	}
	
	while(i!=iFinal)
	{
		if(state == COME_FROM_LEFT || state == GO_TO_RIGHT)
			i += 2;
		else
			i -= 2;
		
		oledDisplay(32,0,64,8,(u8*)gImage_SETTING,i,0);		
	}
}

void keyActionMainTime(u8 keyPressed)
{
	switch(keyPressed)
	{
		case LEFT_KEY:
			slidMainTimeScreen(GO_TO_LEFT);
			slidLed(COME_FROM_RIGHT);
			screenStateNow = displayLed;
			break;
		case RIGHT_KEY:
			slidMainTimeScreen(GO_TO_RIGHT);
			slidSettingPic(COME_FROM_LEFT);
			screenStateNow = displaySettingPic;
			break;
		case DOWN_KEY:
			
			break;
		case DOWN_KEY_LONG:
			break;
	}
}

void MainTime(void)
{
	static u32 preSecond = 0;
	
	
	if(preSecond != systemSecond)
	{
		preSecond = systemSecond;
		//读取时间
		timeRead(&time);
		//读取温度
		temperature = readTemperature();
		//获得电池状态
		getBatteryState();
		//显示温度
		num = num2string(tempShow,temperature,2);
		tempShow[num-1] = 'z'+2;
		tempShow[num] = 'C';
		tempShow[num+1] = '\0';		
		oledShowString(0, 1, tempShow, 0, 0);

		//显示日期
		data2String(time,tempShow);
		oledShowString(0, 0, tempShow, 0, 0);

		//显示蓝牙状态
		if(stateBLE)
			oledShowBLE(106, 0, 0, 0);
		//显示电池状态
		oledShowBattery(118,0,batteryState, 0);
		//显示LED延时状态
		oledDisplay(86,0,15,2,(u8*)&fontDelay[flagDelayLedEnable*2][0],0,0);	
		//显示时间
		oledShowBigTime(time,time.second,0);
	}
}

void slidMainTimeScreen(u8 state)
{
	int i,iFinal;
	//读取温度
	temperature = readTemperature();
	//获得电池状态
	getBatteryState();
	//读取时间
	timeRead(&time);
	
	switch(state)
	{
		case COME_FROM_LEFT:
			i = -128;
			iFinal = 0;
			break;
		case COME_FROM_RIGHT:
			i = 128;
			iFinal = 0;
			break;
		case GO_TO_LEFT:
			i = 0;
			iFinal = -128;
			break;
		case GO_TO_RIGHT:
			i = 0;
			iFinal = 128;
			break;
		default:
			return;
	}
	
	while(i!=iFinal)
	{
		if(state == COME_FROM_LEFT || state == GO_TO_RIGHT)
			i += 2;
		else
			i -= 2;
		
		if(i == 0)	oledClearLind(0,1);
		
		//显示温度
		num = num2string(tempShow,temperature,2);
		tempShow[num-1] = 'z'+2;
		tempShow[num] = 'C';
		tempShow[num+1] = '\0';		
		oledShowString(0, 1, tempShow, i, 0);

		//显示日期
		data2String(time,tempShow);
		oledShowString(0, 0, tempShow, i, 0);

		//显示蓝牙状态
		if(stateBLE == ENABLE)
			oledShowBLE(106, 0, 0, i);
		//显示电池状态
		oledShowBattery(118,0,batteryState, i);
		//显示LED延时状态
		oledDisplay(86,0,15,2,(u8*)&fontDelay[flagDelayLedEnable*2][0],i,0);
		//显示时间
		oledShowBigTime(time,time.second,i);
	}
}

void keyActionLed(u8 keyPressed)
{
	switch(keyPressed)
	{
		case LEFT_KEY:
			slidLed(GO_TO_LEFT);
			slidInfoPic(COME_FROM_RIGHT);
			screenStateNow = displayInfoPic;
			break;
		case RIGHT_KEY:
			slidLed(GO_TO_RIGHT);
			slidMainTimeScreen(COME_FROM_LEFT);
			screenStateNow = displayMainTime;
			break;
		case DOWN_KEY:
			if(modeBreathLed != breathLowBattery)
			{
				oledClear();
				screenLedSet();
				optionLedSet = ledSetColorLedEnable;
				screenStateNow = displayLedSet;
			}
			break;
		case DOWN_KEY_LONG:
			break;
	}
}

void slidLed(u8 state)
{
	extern const unsigned char gImage_LED[][64];
	extern const unsigned char gImage_LED_DISABLE[][64];
	int i,iFinal;
	
	switch(state)
	{
		case COME_FROM_LEFT:
			i = -128;
			iFinal = 0;
			break;
		case COME_FROM_RIGHT:
			i = 128;
			iFinal = 0;
			break;
		case GO_TO_LEFT:
			i = 0;
			iFinal = -128;
			break;
		case GO_TO_RIGHT:
			i = 0;
			iFinal = 128;
			break;
		default:
			oledClear();
			if(modeBreathLed == breathLowBattery)	oledDisplay(32,0,64,8,(u8*)gImage_LED_DISABLE,0,0);
			else oledDisplay(32,0,64,8,(u8*)gImage_LED,0,0);
			return;
	}
	if(modeBreathLed == breathLowBattery)
	{
		while(i!=iFinal)
		{
			if(state == COME_FROM_LEFT || state == GO_TO_RIGHT)
				i += 2;
			else
				i -= 2;
			oledDisplay(32,0,64,8,(u8*)gImage_LED_DISABLE,i,0);
		}
	}
	else
	{
		while(i!=iFinal)
		{
			if(state == COME_FROM_LEFT || state == GO_TO_RIGHT)
				i += 2;
			else
				i -= 2;
			oledDisplay(32,0,64,8,(u8*)gImage_LED,i,0);
		}
	}
}
