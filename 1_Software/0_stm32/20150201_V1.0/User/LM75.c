#include "main.h"

/****************************************************************************
*	函数名：readTemperature
	输  入：void
	输  出：s16 temperature 温度
	功  能：读取温度值
	备  注：返回的是实际摄氏温度*1000
*	日  期：2015.02.02
****************************************************************************/
s16 readTemperature(void)
{
	u8 receiveTempLM75[2];
	u8 errorTime = 0;
	s16 temperature = 0;
	
	while(I2CReadRegister(I2C_ADDRESS_LM75,REG_ADDRESS_TEMPERATURE,receiveTempLM75,2) == 0)
	{
		errorTime ++;
		if(errorTime > 5)
		{
			return 0xFFFF;
		}
	}
	
	temperature = ((s16)receiveTempLM75[0] << 8) + receiveTempLM75[1];
	//高11位有效
	//步进值(0.125 / 32 = 0.0039) ℃	
    temperature = temperature / 100 * 39;
	
	return temperature;
}







