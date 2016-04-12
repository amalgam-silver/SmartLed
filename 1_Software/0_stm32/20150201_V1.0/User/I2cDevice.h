#ifndef _I2C_DEVICE_H
#define _I2C_DEVICE_H

#include "main.h"

#define TIMEOUTI2C 5000

void I2CInit(void);
unsigned char I2CReadRegister(u8 SlaveAddress,u8 RegisterAddress,u8 *Value,u8 num);
unsigned char I2CWriteRegister(u8 SlaveAddress,u8 RegisterAddress,u8 *Value,u8 num);

#endif
