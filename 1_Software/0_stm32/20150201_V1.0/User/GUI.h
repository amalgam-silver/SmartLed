#ifndef	GUI_H
#define	GUI_H

#include "main.h"

typedef enum{
	displayMainTime,
	displayLed,
	displayLedSet,
	displaySettingPic,
	displaySetting,
	displayInfoPic,
	displayInfo,
}screenState;

typedef enum{
	ledSetColorLedEnable,
	ledSetBreathLedEnable,
	ledSetPwmWhiteLedSet,
	ledSetPwmRedLedSet,
	ledSetPwmGreenLedSet,
	ledSetPwmBlueLedSet,
	ledSetDelay,
}ledSetOption;

typedef enum{
	settingYear,
	settingMonth,
	settingData,
	settingHour,
	settingMinute,
	settingSecond,
	settingTimeScreenOff,
	settingBluetooth,
}settingOption;

typedef enum
{
	breathOff,
	breathColor,
	breathLowBattery,
}modesBreathLed;

#define COME_FROM_LEFT 	0
#define COME_FROM_RIGHT 1
#define GO_TO_LEFT		2
#define GO_TO_RIGHT		3
#define ACTION_NONE		4

#define LEFT_KEY		2
#define RIGHT_KEY		1
#define DOWN_KEY		4
#define DOWN_KEY_LONG	8

extern const unsigned char fontEnable[][8];
extern const unsigned char fontDelay[][15];

extern screenState screenStateNow;
extern u8 timeScreenOff;
extern u8 flagColorLedEnable,flagBreathLedEnable,flagDelayLedEnable;
extern u8 modeBreathLed;
extern u8 pwmRedSet, pwmGreenSet, pwmBlueSet, pwmWhite;
extern u32 ledDelaySecond;

void lcdDisplay(void);
void keyAction(u8 keyPressed);

void keyActionMainTime(u8 keyPressed);
void MainTime(void);
void slidMainTimeScreen(u8 state);

void keyActionLed(u8 keyPressed);
void slidLed(u8 state);

void keyActionSettingPic(u8 keyAction);
void slidSettingPic(u8 state);

void keyActionInfoPic(u8 keyPressed);
void slidInfoPic(u8 state);

void keyActionInfo(u8 keyPressed);
void screenInfo(void);

void keyActionLedSet(u8 keyAction);
void screenLedSet(void);

void keyActionSetting(u8 keyAction);
void screenSetting(void);
#endif
