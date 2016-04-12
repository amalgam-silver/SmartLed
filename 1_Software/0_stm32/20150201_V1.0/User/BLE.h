#ifndef BLE_H
#define BLE_H

#include "main.h"

extern u8 stateBLE;
extern u8 flagBLEConnected;
extern u8 receiveBuffer[30];
extern u8 sendBuffer[200];

void bluetoothInit(void);
void USART1_IRQHandler(void);
void bluetoothCmd(u8 newState);
void bleSend(u8 *Data, u16 LengthTX);

#endif
