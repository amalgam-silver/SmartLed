#include "main.h"


/****************************************************************************
*	��������I2CReadRegister
	��  �룺u8 SlaveAddress ���豸��ַ,u8 RegisterAddress �Ĵ�����ַ,
			u8 *Value ��ȡ��ֵ u8 num ��Ҫ��ȡ���ֽ���
	��  ����u8 Result 0-ʧ�� 1-�ɹ�
	��  �ܣ���ȡָ���豸��ָ���Ĵ�����ֵ
*	��  �ڣ�2015.02.01
****************************************************************************/
u8 I2CReadRegister(u8 SlaveAddress,u8 RegisterAddress,u8 *Value,u8 num)
{
	unsigned char RegisterValue = 0;
	unsigned int timeout = 0;
	unsigned char i = 0;

	//�ȴ�I2C���߿���
	timeout = TIMEOUTI2C;
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
	{
	 	timeout --;
		if(timeout == 0) return 0;
	}

	//����ACKӦ��
	I2C_AcknowledgeConfig(I2C1, ENABLE);
	
	//���Ϳ�ʼλ
	I2C_GenerateSTART(I2C1, ENABLE);
	timeout = TIMEOUTI2C;
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) 
	{
	 	timeout --;
		if(timeout == 0) return 0;
	}

	//�����豸��ַ��д��
	I2C_Send7bitAddress(I2C1, SlaveAddress, I2C_Direction_Transmitter);
	timeout = TIMEOUTI2C;
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))		
	{
	 	timeout --;
		if(timeout == 0) return 0;
	}

	//���ͼĴ�����ַ
	I2C_SendData(I2C1, RegisterAddress); 
	timeout = TIMEOUTI2C;
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
	 	timeout --;
		if(timeout == 0) return 0;
	}

	//���Ϳ�ʼλ
	I2C_GenerateSTART(I2C1, ENABLE);
	timeout = TIMEOUTI2C;
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
	{
	 	timeout --;
		if(timeout == 0) return 0;
	}

	//�����豸��ַ������
	I2C_Send7bitAddress(I2C1, SlaveAddress, I2C_Direction_Receiver);
	timeout = TIMEOUTI2C;
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
	{
	 	timeout --;
		if(timeout == 0) return 0;
	}

	for(i=0; i<(num - 1); i++)
	{
		//���Ĵ�����ֵ
		timeout = TIMEOUTI2C;
		while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED))
		{
			timeout --;
			if(timeout == 0) return 0;
		}	
		RegisterValue = I2C_ReceiveData(I2C1);
		Value[i] = RegisterValue;
	}
	
	//�ر�ACKӦ��
	I2C_AcknowledgeConfig(I2C1, DISABLE);
	
	//����ֹͣλ
	I2C_GenerateSTOP(I2C1, ENABLE);
	
   	//���Ĵ�����ֵ
 	timeout = TIMEOUTI2C;
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED))
	{
	 	timeout --;
		if(timeout == 0) return 0;
	}	
	RegisterValue = I2C_ReceiveData(I2C1);
	Value[i] = RegisterValue;
	return 1;
}

/****************************************************************************
*	��������I2CWriteRegister
	��  �룺u8 SlaveAddress ���豸��ַ,u8 RegisterAddress �Ĵ�����ַ,
			u8 *Value �Ĵ�����ֵָ��,u8 num ��Ҫд���ֵ����
	��  ����u8 Result 0-ʧ�� 1-�ɹ�
	��  �ܣ���ָ���豸��ָ���Ĵ���д��ָ��ֵ
*	��  �ڣ�2015.02.01
****************************************************************************/
u8 I2CWriteRegister(u8 SlaveAddress,u8 RegisterAddress,u8 *Value,u8 num)
{
	unsigned char i = 0;
	unsigned int timeout = 0;
	//�ȴ�I2C���߿���
	timeout = TIMEOUTI2C;
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
	{
	 	timeout --;
		if(timeout == 0) return 0;
	}

	//����ACKӦ��
	I2C_AcknowledgeConfig(I2C1, ENABLE);	

	//���Ϳ�ʼλ
	I2C_GenerateSTART(I2C1, ENABLE);
	timeout = TIMEOUTI2C;
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
	{
	 	timeout --;
		if(timeout == 0) return 0;
	} 

	//�����豸��ַ��д��
	I2C_Send7bitAddress(I2C1, SlaveAddress, I2C_Direction_Transmitter);
	timeout = TIMEOUTI2C;
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
	 	timeout --;
		if(timeout == 0) return 0;
	} 		

	//���ͼĴ�����ַ
	I2C_SendData(I2C1, RegisterAddress);												
	timeout = 5000;
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
	 	timeout --;
		if(timeout == 0) return 0;
	}   	 	
	for(i=0; i<num; i++)
	{
		//���ͼĴ���ֵ
		I2C_SendData(I2C1, Value[i]);								  		
		timeout = TIMEOUTI2C;
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		{
			timeout --;
			if(timeout == 0) return 0;
		}
	}

	//����ֹͣλ 	
	I2C_GenerateSTOP(I2C1, ENABLE);
	
	return 1;		
}

/****************************************************************************
*	��������I2CConfiguration
	��  �룺void
	��  ����void
	��  �ܣ�I2C��ʼ��
	��  ע����
*	��  �ڣ�2015.02.01
****************************************************************************/
void I2CInit(void)
{
	I2C_InitTypeDef  I2C_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//ʹ��I2C1��GPIOB
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

	//����PB6->SCL��PB7->SDA
  	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//����I2C
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  	I2C_InitStructure.I2C_OwnAddress1 = 0x11;
  	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  	I2C_InitStructure.I2C_ClockSpeed = 100000;
  	I2C_Init(I2C1, &I2C_InitStructure);
	
	//��I2C
	I2C_Cmd(I2C1, ENABLE);
}


