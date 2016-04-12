#ifndef SYSTEM_STATE_TRANSFORM_H
#define SYSTEM_STATE_TRANSFORM_H

#include "main.h"

#define OLED_ON		0
#define OLED_OFF	1

#define MODE_HIGH	0
#define MODE_LOW	1
#define MODE_SLEEP	2

extern u8 oledState,systemState;

void modeSleepToLow(void);
void modeSleepToHigh(void);
void modeHighToSleep(void);
void modeHighToLow(void);
void modeLowToHigh(void);
void modeLowToSleep(void);





#endif
