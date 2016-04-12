#ifndef LED_H
#define LED_H

#include "main.h"

extern u16 pwmRed, pwmGreen, pwmBlue;

void colorSet(u16 RGB565);
void pwmChange(u8 pwmRedSet,u8 pwmGreenSet,u8 pwmBlueSet);
void pwmChangeLow(u16 pwmRedSet,u16 pwmGreenSet,u16 pwmBlueSet);
void ledInit(void);

#endif
