#include "main.h"


/****************************************************************************
*	函数名：I2CReadRegister
	输  入：u8 SlaveAddress 从设备地址,u8 RegisterAddress 寄存器地址,
			u8 *Value 读取的值 u8 num 需要读取的字节数
	输  出：u8 Result 0-失败 1-成功
	功  能：读取指定设备的指定寄存器的值
*	日  期：2015.02.01
****************************************************************************/
u8 I2CReadRegister(u8 SlaveAddress,u8 RegisterAddress,u8 *Value,u8 num)
{
	unsigned char RegisterValue = 0;
	unsigned int timeout = 0;
	unsigned char i = 0;

	//等待I2C总线空闲
	timeout = TIMEOUTI2C;
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
	{
	 	timeout --;
		if(timeout == 0) return 0;
	}

	//开启ACK应答
	I2C_AcknowledgeConfig(I2C1, ENABLE);
	
	//发送开始位
	I2C_GenerateSTART(I2C1, ENABLE);
	timeout = TIMEOUTI2C;
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) 
	{
	 	timeout --;
		if(timeout == 0) return 0;
	}

	//发送设备地址（写）
	I2C_Send7bitAddress(I2C1, SlaveAddress, I2C_Direction_Transmitter);
	timeout = TIMEOUTI2C;
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))		
	{
	 	timeout --;
		if(timeout == 0) return 0;
	}

	//发送寄存器地址
	I2C_SendData(I2C1, RegisterAddress); 
	timeout = TIMEOUTI2C;
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
	 	timeout --;
		if(timeout == 0) return 0;
	}

	//发送开始位
	I2C_GenerateSTART(I2C1, ENABLE);
	timeout = TIMEOUTI2C;
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
	{
	 	timeout --;
		if(timeout == 0) return 0;
	}

	//发送设备地址（读）
	I2C_Send7bitAddress(I2C1, SlaveAddress, I2C_Direction_Receiver);
	timeout = TIMEOUTI2C;
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
	{
	 	timeout --;
		if(timeout == 0) return 0;
	}

	for(i=0; i<(num - 1); i++)
	{
		//读寄存器的值
		timeout = TIMEOUTI2C;
		while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED))
		{
			timeout --;
			if(timeout == 0) return 0;
		}	
		RegisterValue = I2C_ReceiveData(I2C1);
		Value[i] = RegisterValue;
	}
	
	//关闭ACK应答
	I2C_AcknowledgeConfig(I2C1, DISABLE);
	
	//发送停止位
	I2C_GenerateSTOP(I2C1, ENABLE);
	
   	//读寄存器的值
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
*	函数名：I2CWriteRegister
	输  入：u8 SlaveAddress 从设备地址,u8 RegisterAddress 寄存器地址,
			u8 *Value 寄存器的值指针,u8 num 需要写入的值数量
	输  出：u8 Result 0-失败 1-成功
	功  能：向指定设备的指定寄存器写入指定值
*	日  期：2015.02.01
****************************************************************************/
u8 I2CWriteRegister(u8 SlaveAddress,u8 RegisterAddress,u8 *Value,u8 num)
{
	unsigned char i = 0;
	unsigned int timeout = 0;
	//等待I2C总线空闲
	timeout = TIMEOUTI2C;
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
	{
	 	timeout --;
		if(timeout == 0) return 0;
	}

	//开启ACK应答
	I2C_AcknowledgeConfig(I2C1, ENABLE);	

	//发送开始位
	I2C_GenerateSTART(I2C1, ENABLE);
	timeout = TIMEOUTI2C;
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
	{
	 	timeout --;
		if(timeout == 0) return 0;
	} 

	//发送设备地址（写）
	I2C_Send7bitAddress(I2C1, SlaveAddress, I2C_Direction_Transmitter);
	timeout = TIMEOUTI2C;
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
	 	timeout --;
		if(timeout == 0) return 0;
	} 		

	//发送寄存器地址
	I2C_SendData(I2C1, RegisterAddress);												
	timeout = 5000;
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
	 	timeout --;
		if(timeout == 0) return 0;
	}   	 	
	for(i=0; i<num; i++)
	{
		//发送寄存器值
		I2C_SendData(I2C1, Value[i]);								  		
		timeout = TIMEOUTI2C;
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		{
			timeout --;
			if(timeout == 0) return 0;
		}
	}

	//发送停止位 	
	I2C_GenerateSTOP(I2C1, ENABLE);
	
	return 1;		
}

/****************************************************************************
*	函数名：I2CConfiguration
	输  入：void
	输  出：void
	功  能：I2C初始化
	备  注：无
*	日  期：2015.02.01
****************************************************************************/
void I2CInit(void)
{
	I2C_InitTypeDef  I2C_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//使能I2C1，GPIOB
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

	//配置PB6->SCL，PB7->SDA
  	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//配置I2C
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  	I2C_InitStructure.I2C_OwnAddress1 = 0x11;
  	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  	I2C_InitStructure.I2C_ClockSpeed = 100000;
  	I2C_Init(I2C1, &I2C_InitStructure);
	
	//打开I2C
	I2C_Cmd(I2C1, ENABLE);
}


