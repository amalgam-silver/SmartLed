#ifndef LM75_H
#define LM75_H

#include "main.h"

#define I2C_ADDRESS_LM75			0x90		
#define REG_ADDRESS_TEMPERATURE		0x00

s16 readTemperature(void);

#endif
