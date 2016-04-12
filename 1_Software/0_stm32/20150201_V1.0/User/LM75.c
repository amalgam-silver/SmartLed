#include "main.h"

/****************************************************************************
*	��������readTemperature
	��  �룺void
	��  ����s16 temperature �¶�
	��  �ܣ���ȡ�¶�ֵ
	��  ע�����ص���ʵ�������¶�*1000
*	��  �ڣ�2015.02.02
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
	//��11λ��Ч
	//����ֵ(0.125 / 32 = 0.0039) ��	
    temperature = temperature / 100 * 39;
	
	return temperature;
}







