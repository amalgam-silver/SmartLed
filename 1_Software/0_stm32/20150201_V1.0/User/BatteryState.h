#ifndef BATTERY_STATE_H
#define BATTERY_STATE_H

#include "main.h"

#define ADC1_DR_Address ((u32)0x4001244C)

extern int batteryState;
extern int voltageBattery;
extern char stateCharging,stateReady;

void getBatteryState(void);
void batteryDetectorInit(void);

#endif
