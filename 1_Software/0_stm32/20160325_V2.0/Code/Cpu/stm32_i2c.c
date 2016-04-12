/**
 *************************************************************************************************
 * @file    stm32_i2c.c
 * @author  Ge Yue
 * @version V1.0.0
 * @date    2016.03.18
 * @brief   ͨ��stm32��i2c��������дI2C�豸
 *************************************************************************************************
 @verbatim 
 
  - �汾��       ����      ������        ���� 
    V1.0      2016.03.18   Ge Yue     ͨ��stm32��i2c��������дI2C�豸; 
 
  - �汾��       ����      �޸���        ���� 
 @endverbatim 
 */

/**
 * @defgroup I2C stm32_i2c: CPU Group
 * @{
 */

#include "stm32_i2c.h"
#include "string.h"

/** @brief I2C1���ݼĴ�����ַ */
#define I2C1_DR_Address        0x40005410

/** @brief I2C�շ�����״̬ */
typedef enum i2c_state
{
  COMM_DONE = 0,        ///< �ɹ�
  COMM_PRE = 1,         ///< ׼��
  COMM_IN_PROCESS = 2,  ///< 
  CHECK_IN_PROCESS = 3,
  COMM_EXIT = 4         ///< ʧ���˳� 
}I2C_STATE;

#define I2C_RD  0       ///< ���Ĵ���
#define I2C_WR  1       ///< д�Ĵ���

/** @brief I2C�շ���������м�����ṹ�� */
typedef struct
{
    u8 slaveAddr;   ///< ���豸��ַ
    u8 regAddr;     ///< �Ĵ�����ַ
    u8 dir;         ///< ���ڱ�־������д�Ĵ��� 0:�� 1:д
    u8 stt;         ///< I2C�����շ������еĺ�״̬
    u8 regAddrDone; ///< ���ڱ�־�Ƿ��ѷ��͹��Ĵ�����ַ
}STR_I2cProc;

/** @brief I2C�շ���������м�����ṹ�� */
static STR_I2cProc I2cProc = {0,};

/** @brief I2C�շ��ṹ�� */
STR_I2C I2c = {0,};

/**
 * @brief  ��ȡi2c�豸�ļĴ���
 * @param  ������ ����˵��  
 * @param  [in] slaveAddress ���豸��ַ  
 * @param  [in] registerAddress �Ĵ�����ַ
 * @param  [out] *buf ��ȡ��ֵ��ŵĻ����� 
 * @param  [in] num ��Ҫ��ȡ���ֽ���  
 * @retval 0-ʧ�� 1-�ɹ�
 * @par ʹ��ȫ�ֱ��� \n
 * @note �� ִ��ʱ��: \n
 *       �� ��������: ��������, ��ʼ��ʱ���� \n
 *       �� �ɷ���: ���� \n
 *
 * @par  ע��:
 *       �� �� \n
 */
u8 I2CReadRegister(u8 slaveAddress, u8 registerAddress, u8 *buf, u8 num)
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
	I2C_Send7bitAddress(I2C1, slaveAddress, I2C_Direction_Transmitter);
	timeout = TIMEOUTI2C;
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))		
	{
	 	timeout --;
		if(timeout == 0) return 0;
	}

	//���ͼĴ�����ַ
	I2C_SendData(I2C1, registerAddress); 
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
	I2C_Send7bitAddress(I2C1, slaveAddress, I2C_Direction_Receiver);
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
		buf[i] = RegisterValue;
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
	buf[i] = RegisterValue;
	return 1;
}

/**
 * @brief  дi2c�豸�ļĴ���
 * @param  ������ ����˵��  
 * @param  [in] slaveAddress ���豸��ַ  
 * @param  [in] registerAddress �Ĵ�����ַ
 * @param  [in] *buf д���ֵ��ŵĻ����� 
 * @param  [in] num ��Ҫ��ȡ���ֽ���  
 * @retval 0-ʧ�� 1-�ɹ�
 * @par ʹ��ȫ�ֱ��� \n
 * @note �� ִ��ʱ��: \n
 *       �� ��������: ��������, ��ʼ��ʱ���� \n
 *       �� �ɷ���: ���� \n
 *
 * @par  ע��:
 *       �� �� \n
 */
u8 I2CWriteRegister(u8 slaveAddress, u8 registerAddress, u8 *buf, u8 num)
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
	I2C_Send7bitAddress(I2C1, slaveAddress, I2C_Direction_Transmitter);
	timeout = TIMEOUTI2C;
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
	 	timeout --;
		if(timeout == 0) return 0;
	} 		

	//���ͼĴ�����ַ
	I2C_SendData(I2C1, registerAddress);												
	timeout = 5000;
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
	 	timeout --;
		if(timeout == 0) return 0;
	}   	 	
	for(i=0; i<num; i++)
	{
		//���ͼĴ���ֵ
		I2C_SendData(I2C1, buf[i]);								  		
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

/**
 * @brief  i2c��������ʼ������
 * @param  ������ ����˵��  
 * @param  ��
 * @retval ��
 * @par ʹ��ȫ�ֱ��� \n
 * @note �� ִ��ʱ��: \n
 *       �� ��������: ��������, ��ʼ��ʱ���� \n
 *       �� �ɷ���: ���� \n
 *
 * @par  ע��:
 *       �� �� \n
 */
void I2CInit(void)
{
	I2C_InitTypeDef  I2C_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
    DMA_InitTypeDef  DMA_InitStructure;
    
	/* ʹ��I2C1, GPIOB, DMA1 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	/* ����PB6->SCL��PB7->SDA */
  	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* ����I2C */
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  	I2C_InitStructure.I2C_OwnAddress1 = 0x11;
  	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  	I2C_InitStructure.I2C_ClockSpeed = 100000;  // 100kHz
  	I2C_Init(I2C1, &I2C_InitStructure);
	
	/* ��I2C */
	I2C_Cmd(I2C1, ENABLE);
    
    /* ����I2C1 �¼��ж� */
    NVIC_InitStructure.NVIC_IRQChannel = I2C1_EV_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    /* ����I2C1 �����ж� */
    NVIC_InitStructure.NVIC_IRQChannel = I2C1_ER_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    /* I2C1����ͨ�� ->  DMA1_CH6 */
    DMA_DeInit(DMA1_Channel6);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)I2C1_DR_Address;    // �����ַ: I2C1���ݼĴ���
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)I2c.bufTx;              // �ڴ��ַ: ���ͻ�����
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;                  // ��������: �ڴ� -> ����
    DMA_InitStructure.DMA_BufferSize = 1;                               // ���ݳ���: �˴���ֵ������, �ڷ��ͺ����л�������¸�ֵ
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;    // ��ֹ�����ַ�Լ�
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;             // ʹ���ڴ��ַ�Լ�    
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_MemoryDataSize_Byte; // �ڴ洫�䵥λ: �ֽ�
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;     // ���贫�䵥λ: �ֽ�
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                       // ��ѭ��ģʽ
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;                 // ���ȼ�: ��
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                        // ���ڴ�->�ڴ�ģʽ
    DMA_Init(DMA1_Channel6, &DMA_InitStructure);
    DMA_ITConfig(DMA1_Channel6, DMA_IT_TC, ENABLE);
    
    /* DMA1_CH6 �ж����ȼ����� */
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel6_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    /* I2C1����ͨ�� ->  DMA1_CH7 */
    DMA_DeInit(DMA1_Channel7);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)I2C1_DR_Address;    // �����ַ: I2C1���ݼĴ���
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)I2c.bufRx;              // �ڴ��ַ: ���ջ�����
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                  // ��������: ���� -> �ڴ�
    DMA_InitStructure.DMA_BufferSize = 1;                               // ���ݳ���: �˴���ֵ������, �ڽ��պ����л�������¸�ֵ
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;    // ��ֹ�����ַ�Լ�
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;             // ʹ���ڴ��ַ�Լ�
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_MemoryDataSize_Byte; // �ڴ洫�䵥λ: �ֽ�
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;     // ���贫�䵥λ: �ֽ�
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                       // ��ѭ��ģʽ
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;                 // ���ȼ�: ��
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                        // ���ڴ�->�ڴ�ģʽ
    DMA_Init(DMA1_Channel7, &DMA_InitStructure);
    DMA_ITConfig(DMA1_Channel7, DMA_IT_TC, ENABLE);

    /* DMA1_CH7 �ж����ȼ����� */
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel7_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

}

/**
 * @brief  DMA��ʽ��ȡi2c�豸�ļĴ���
 * @param  ������ ����˵��  
 * @param  [in] slaveAddr ���豸��ַ  
 * @param  [in] regAddr �Ĵ�����ַ
 * @param  [out] *buf ��ȡ��ֵ��ŵĻ����� 
 * @param  [in] num ��Ҫ��ȡ���ֽ���  
 * @retval ��
 * @par ʹ��ȫ�ֱ��� \n
 * @note �� ִ��ʱ��: \n
 *       �� ��������: ��������, ��ʼ��ʱ���� \n
 *       �� �ɷ���: ���� \n
 *
 * @par  ע��:
 *       �� �� \n
 */
void I2CReadRegDMA(u8 slaveAddr, u8 regAddr, u8 *buf, u8 num)
{      
    I2c.busy = 0xff;
    I2cProc.dir = I2C_RD;
    I2cProc.slaveAddr = slaveAddr;
    I2cProc.regAddr = regAddr;
    I2cProc.regAddrDone = 0;
    I2cProc.stt = COMM_PRE;
    
    DMA_Cmd(DMA1_Channel7, DISABLE);	    // �رյ�ǰͨ��������ͨ�����ã�������رգ����ܽ������ã�	
    DMA1_Channel7->CNDTR = num;             // ���ó���
    DMA1_Channel7->CMAR = (u32)buf;         // ���û�����
    DMA1->IFCR |= (u32)0x0F000000;	        // ���DMA_Cannel7�жϱ�־
    
    I2C_AcknowledgeConfig(I2C1, ENABLE);
    I2C_ITConfig(I2C1, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR, ENABLE);
    
    /* Send START condition */
    if(I2C1->CR1 & 0x200)
        I2C1->CR1 &= 0xFDFF;
    I2C_GenerateSTART(I2C1, ENABLE);
  
}

/**
 * @brief  DMA��ʽдi2c�豸�ļĴ���
 * @param  ������ ����˵��  
 * @param  [in] slaveAddr ���豸��ַ  
 * @param  [in] regAddr �Ĵ�����ַ
 * @param  [in] *buf д���ֵ��ŵĻ����� 
 * @param  [in] num ��Ҫ��ȡ���ֽ���  
 * @retval 0-ʧ�� 1-�ɹ�
 * @par ʹ��ȫ�ֱ��� \n
 * @note �� ִ��ʱ��: \n
 *       �� ��������: ��������, ��ʼ��ʱ���� \n
 *       �� �ɷ���: ���� \n
 *
 * @par  ע��:
 *       �� �� \n
 */
void I2CWriteRegDMA(u8 slaveAddr, u8 regAddr, u8 *buf, u8 num)
{
    I2c.busy = 0xff;
    I2cProc.dir = I2C_WR;
    I2cProc.slaveAddr = slaveAddr;
    I2cProc.regAddr = regAddr;
    I2cProc.regAddrDone = 0;
    I2cProc.stt = COMM_PRE;
    
    memcpy(I2c.bufTx, buf, num);
    
    DMA_Cmd(DMA1_Channel6, DISABLE);	    // �رյ�ǰͨ��������ͨ�����ã�������رգ����ܽ������ã�	
    DMA1_Channel6->CNDTR = num;             // ���ó���
    DMA1->IFCR |= (u32)0x00F00000;	        // ���DMA_Cannel7�жϱ�־
        
    I2C_AcknowledgeConfig(I2C1, ENABLE);
    I2C_ITConfig(I2C1, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR, ENABLE);
    
    /* Send START condition */
    if(I2C1->CR1 & 0x200)
        I2C1->CR1 &= 0xFDFF;
    I2C_GenerateSTART(I2C1, ENABLE);
}

/**
 * @brief  I2C1�¼��жϴ�����
 * @param  ������ ����˵��  
 * @param  ��
 * @retval ��
 * @par ʹ��ȫ�ֱ��� \n
 * @note �� ִ��ʱ��: \n
 *       �� ��������: I2C1״̬�����仯ʱ���� \n
 *       �� �ɷ���: ���� \n
 *
 * @par  ע��:
 *       �� �� \n
 */
void I2C1_EV_IRQHandler(void)
{
    u32 i2cEvent = I2C_GetLastEvent(I2C1);
    switch (i2cEvent)
    {
        /************************** Master Invoke**************************************/
        case I2C_EVENT_MASTER_MODE_SELECT:                      // MSL SB BUSY 0x30001
            I2cProc.stt = COMM_IN_PROCESS;

            /* ���Ĵ��� */
            if (I2cProc.dir == I2C_RD)
            {
                /* ����δ���͹��Ĵ�����ַ, ����д���豸��ַ */
                if (!I2cProc.regAddrDone)
                    I2C_Send7bitAddress(I2C1, I2cProc.slaveAddr, I2C_Direction_Transmitter);    
                /* ���ѷ��͹��Ĵ�����ַ, ���Ͷ����豸��ַ */
                else
                {
                    I2C_Send7bitAddress(I2C1, I2cProc.slaveAddr, I2C_Direction_Receiver);      
                    I2cProc.regAddrDone = 0;
                }
            }
            /* д�Ĵ��� */
            else
            {
                /* Send slave Address for write */
                I2C_Send7bitAddress(I2C1, I2cProc.slaveAddr, I2C_Direction_Transmitter);
            }
            I2C_ITConfig(I2C1, I2C_IT_BUF , ENABLE);//also TxE int allowed
            break;
              
        /********************** Master Receiver events ********************************/
        case I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED:           // MSL BUSY ADDR 0x30002
            I2C_ITConfig(I2C1, I2C_IT_EVT | I2C_IT_BUF, DISABLE);

            /* ���ڲ������һ���ֽڵ�NACK */
            I2C_DMALastTransferCmd(I2C1, ENABLE);
            /* ��ʼDMA���� */
            I2C_DMACmd(I2C1, ENABLE);
            DMA_Cmd(DMA1_Channel7, ENABLE);            
            break;
      
        case I2C_EVENT_MASTER_BYTE_RECEIVED:                    // MSL BUSY RXNE 0x30040
            break;
           
        /************************* Master Transmitter events **************************/
        case I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED:        // BUSY, MSL, ADDR, TXE and TRA 0x70082
            I2C_SendData(I2C1, I2cProc.regAddr);
            I2cProc.regAddrDone = 0xff;           
            break;
              
        case I2C_EVENT_MASTER_BYTE_TRANSMITTING:                // TRA, BUSY, MSL, TXE 0x70080
            if (I2cProc.dir == I2C_RD)
            {
                I2C_ITConfig(I2C1, I2C_IT_BUF , DISABLE);
                while ((I2C1->CR1 & 0x200) == 0x200); 
                    I2C_GenerateSTART(I2C1, ENABLE);
                break;
            }
            else
            {
                I2C_ITConfig(I2C1, I2C_IT_EVT | I2C_IT_BUF, DISABLE);
                I2C_DMACmd(I2C1, ENABLE);
                DMA_Cmd(DMA1_Channel6, ENABLE);
                break;
            }
      
        case I2C_EVENT_MASTER_BYTE_TRANSMITTED:                 // TRA, BUSY, MSL, TXE and BTF 0x70084
            break;
    }
}

/**
 * @brief  I2C1�����жϴ�����
 * @param  ������ ����˵��  
 * @param  ��
 * @retval ��
 * @par ʹ��ȫ�ֱ��� \n
 * @note �� ִ��ʱ��: \n
 *       �� ��������: I2C1��������ʱ���� \n
 *       �� �ɷ���: ���� \n
 *
 * @par  ע��:
 *       �� �� \n
 */
void I2C1_ER_IRQHandler(void)
{
    if (I2C_GetFlagStatus(I2C1, I2C_FLAG_AF))
    {
        if (I2C1->SR2 &0x01)
        {
            I2C_GenerateSTOP(I2C1, ENABLE);
            I2cProc.stt = COMM_EXIT;
            I2c.busy = 0;
        }

        I2C_ClearFlag(I2C1, I2C_FLAG_AF);
    }

    if (I2C_GetFlagStatus(I2C1, I2C_FLAG_BERR))
    {
        if (I2C1->SR2 &0x01)
        {
            I2C_GenerateSTOP(I2C1, ENABLE);
            I2cProc.stt = COMM_EXIT;
            I2c.busy = 0;
        }

        I2C_ClearFlag(I2C1, I2C_FLAG_BERR);
    }
}

/**
 * @brief  DMA1_CH6�жϴ�����
 * @param  ������ ����˵��  
 * @param  ��
 * @retval ��
 * @par ʹ��ȫ�ֱ��� \n
 * @note �� ִ��ʱ��: \n
 *       �� ��������: i2c1������ɺ�DMA1_CH6��������ж�ʱ���� \n
 *       �� �ɷ���: ���� \n
 *
 * @par  ע��:
 *       �� �� \n
 */
void DMA1_Channel6_IRQHandler(void)
{
    if (DMA_GetFlagStatus(DMA1_FLAG_TC6))
    {
        if (I2C1->SR2 & 0x01) // master send DMA finish, check process later
        {
            // DMA1-6 (I2C1 Tx DMA)transfer complete ISR
            I2C_DMACmd(I2C1, DISABLE);
            DMA_Cmd(DMA1_Channel6, DISABLE);
            // wait until BTF
            while (!(I2C1->SR1 & 0x04));
            I2C_GenerateSTOP(I2C1, ENABLE);
            // wait until BUSY clear
            while (I2C1->SR2 & 0x02);
            I2cProc.stt = COMM_DONE;
            I2c.busy = 0;
        }    
        DMA_ClearFlag(DMA1_FLAG_TC6);
    }
}

/**
 * @brief  DMA1_CH7�жϴ�����
 * @param  ������ ����˵��  
 * @param  ��
 * @retval ��
 * @par ʹ��ȫ�ֱ��� \n
 * @note �� ִ��ʱ��: \n
 *       �� ��������: i2c1������ɺ�DMA1_CH7��������ж�ʱ���� \n
 *       �� �ɷ���: ���� \n
 *
 * @par  ע��:
 *       �� �� \n
 */
void DMA1_Channel7_IRQHandler(void)
{
    if (DMA_GetFlagStatus(DMA1_FLAG_TC7))
    {
        if (I2C1->SR2 & 0x01) // master receive DMA finish
        {
            I2C_DMACmd(I2C1, DISABLE);
            I2C_GenerateSTOP(I2C1, ENABLE);
            I2cProc.stt = COMM_DONE;
            I2c.busy = 0;
        }
        DMA_ClearFlag(DMA1_FLAG_TC7);
    }
}
/**  @} */

