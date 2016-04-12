/**
 *************************************************************************************************
 * @file    stm32_i2c.c
 * @author  Ge Yue
 * @version V1.0.0
 * @date    2016.03.18
 * @brief   通过stm32的i2c控制器读写I2C设备
 *************************************************************************************************
 @verbatim 
 
  - 版本号       日期      创建人        描述 
    V1.0      2016.03.18   Ge Yue     通过stm32的i2c控制器读写I2C设备; 
 
  - 版本号       日期      修改人        描述 
 @endverbatim 
 */

/**
 * @defgroup I2C stm32_i2c: CPU Group
 * @{
 */

#include "stm32_i2c.h"
#include "string.h"

/** @brief I2C1数据寄存器地址 */
#define I2C1_DR_Address        0x40005410

/** @brief I2C收发过程状态 */
typedef enum i2c_state
{
  COMM_DONE = 0,        ///< 成功
  COMM_PRE = 1,         ///< 准备
  COMM_IN_PROCESS = 2,  ///< 
  CHECK_IN_PROCESS = 3,
  COMM_EXIT = 4         ///< 失败退出 
}I2C_STATE;

#define I2C_RD  0       ///< 读寄存器
#define I2C_WR  1       ///< 写寄存器

/** @brief I2C收发过程相关中间变量结构体 */
typedef struct
{
    u8 slaveAddr;   ///< 从设备地址
    u8 regAddr;     ///< 寄存器地址
    u8 dir;         ///< 用于标志读还是写寄存器 0:读 1:写
    u8 stt;         ///< I2C处于收发过程中的何状态
    u8 regAddrDone; ///< 用于标志是否已发送过寄存器地址
}STR_I2cProc;

/** @brief I2C收发过程相关中间变量结构体 */
static STR_I2cProc I2cProc = {0,};

/** @brief I2C收发结构体 */
STR_I2C I2c = {0,};

/**
 * @brief  读取i2c设备的寄存器
 * @param  参数名 参数说明  
 * @param  [in] slaveAddress 从设备地址  
 * @param  [in] registerAddress 寄存器地址
 * @param  [out] *buf 读取的值存放的缓存区 
 * @param  [in] num 需要读取的字节数  
 * @retval 0-失败 1-成功
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 触发调用, 初始化时调用 \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 无 \n
 */
u8 I2CReadRegister(u8 slaveAddress, u8 registerAddress, u8 *buf, u8 num)
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
	I2C_Send7bitAddress(I2C1, slaveAddress, I2C_Direction_Transmitter);
	timeout = TIMEOUTI2C;
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))		
	{
	 	timeout --;
		if(timeout == 0) return 0;
	}

	//发送寄存器地址
	I2C_SendData(I2C1, registerAddress); 
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
	I2C_Send7bitAddress(I2C1, slaveAddress, I2C_Direction_Receiver);
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
		buf[i] = RegisterValue;
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
	buf[i] = RegisterValue;
	return 1;
}

/**
 * @brief  写i2c设备的寄存器
 * @param  参数名 参数说明  
 * @param  [in] slaveAddress 从设备地址  
 * @param  [in] registerAddress 寄存器地址
 * @param  [in] *buf 写入的值存放的缓存区 
 * @param  [in] num 需要读取的字节数  
 * @retval 0-失败 1-成功
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 触发调用, 初始化时调用 \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 无 \n
 */
u8 I2CWriteRegister(u8 slaveAddress, u8 registerAddress, u8 *buf, u8 num)
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
	I2C_Send7bitAddress(I2C1, slaveAddress, I2C_Direction_Transmitter);
	timeout = TIMEOUTI2C;
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
	 	timeout --;
		if(timeout == 0) return 0;
	} 		

	//发送寄存器地址
	I2C_SendData(I2C1, registerAddress);												
	timeout = 5000;
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
	 	timeout --;
		if(timeout == 0) return 0;
	}   	 	
	for(i=0; i<num; i++)
	{
		//发送寄存器值
		I2C_SendData(I2C1, buf[i]);								  		
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

/**
 * @brief  i2c控制器初始化函数
 * @param  参数名 参数说明  
 * @param  无
 * @retval 无
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 触发调用, 初始化时调用 \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 无 \n
 */
void I2CInit(void)
{
	I2C_InitTypeDef  I2C_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
    DMA_InitTypeDef  DMA_InitStructure;
    
	/* 使能I2C1, GPIOB, DMA1 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	/* 配置PB6->SCL，PB7->SDA */
  	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* 配置I2C */
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  	I2C_InitStructure.I2C_OwnAddress1 = 0x11;
  	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  	I2C_InitStructure.I2C_ClockSpeed = 100000;  // 100kHz
  	I2C_Init(I2C1, &I2C_InitStructure);
	
	/* 打开I2C */
	I2C_Cmd(I2C1, ENABLE);
    
    /* 配置I2C1 事件中断 */
    NVIC_InitStructure.NVIC_IRQChannel = I2C1_EV_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    /* 配置I2C1 错误中断 */
    NVIC_InitStructure.NVIC_IRQChannel = I2C1_ER_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    /* I2C1发送通道 ->  DMA1_CH6 */
    DMA_DeInit(DMA1_Channel6);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)I2C1_DR_Address;    // 外设地址: I2C1数据寄存器
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)I2c.bufTx;              // 内存地址: 发送缓存区
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;                  // 数据流向: 内存 -> 外设
    DMA_InitStructure.DMA_BufferSize = 1;                               // 数据长度: 此处的值无意义, 在发送函数中会对其重新赋值
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;    // 禁止外设地址自加
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;             // 使能内存地址自加    
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_MemoryDataSize_Byte; // 内存传输单位: 字节
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;     // 外设传输单位: 字节
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                       // 非循环模式
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;                 // 优先级: 高
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                        // 非内存->内存模式
    DMA_Init(DMA1_Channel6, &DMA_InitStructure);
    DMA_ITConfig(DMA1_Channel6, DMA_IT_TC, ENABLE);
    
    /* DMA1_CH6 中断优先级配置 */
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel6_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    /* I2C1接收通道 ->  DMA1_CH7 */
    DMA_DeInit(DMA1_Channel7);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)I2C1_DR_Address;    // 外设地址: I2C1数据寄存器
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)I2c.bufRx;              // 内存地址: 接收缓存区
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                  // 数据流向: 外设 -> 内存
    DMA_InitStructure.DMA_BufferSize = 1;                               // 数据长度: 此处的值无意义, 在接收函数中会对其重新赋值
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;    // 禁止外设地址自加
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;             // 使能内存地址自加
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_MemoryDataSize_Byte; // 内存传输单位: 字节
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;     // 外设传输单位: 字节
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                       // 非循环模式
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;                 // 优先级: 高
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                        // 非内存->内存模式
    DMA_Init(DMA1_Channel7, &DMA_InitStructure);
    DMA_ITConfig(DMA1_Channel7, DMA_IT_TC, ENABLE);

    /* DMA1_CH7 中断优先级配置 */
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel7_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

}

/**
 * @brief  DMA方式读取i2c设备的寄存器
 * @param  参数名 参数说明  
 * @param  [in] slaveAddr 从设备地址  
 * @param  [in] regAddr 寄存器地址
 * @param  [out] *buf 读取的值存放的缓存区 
 * @param  [in] num 需要读取的字节数  
 * @retval 无
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 触发调用, 初始化时调用 \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 无 \n
 */
void I2CReadRegDMA(u8 slaveAddr, u8 regAddr, u8 *buf, u8 num)
{      
    I2c.busy = 0xff;
    I2cProc.dir = I2C_RD;
    I2cProc.slaveAddr = slaveAddr;
    I2cProc.regAddr = regAddr;
    I2cProc.regAddrDone = 0;
    I2cProc.stt = COMM_PRE;
    
    DMA_Cmd(DMA1_Channel7, DISABLE);	    // 关闭当前通道，进行通道设置（如果不关闭，不能进行设置）	
    DMA1_Channel7->CNDTR = num;             // 设置长度
    DMA1_Channel7->CMAR = (u32)buf;         // 设置缓存区
    DMA1->IFCR |= (u32)0x0F000000;	        // 清除DMA_Cannel7中断标志
    
    I2C_AcknowledgeConfig(I2C1, ENABLE);
    I2C_ITConfig(I2C1, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR, ENABLE);
    
    /* Send START condition */
    if(I2C1->CR1 & 0x200)
        I2C1->CR1 &= 0xFDFF;
    I2C_GenerateSTART(I2C1, ENABLE);
  
}

/**
 * @brief  DMA方式写i2c设备的寄存器
 * @param  参数名 参数说明  
 * @param  [in] slaveAddr 从设备地址  
 * @param  [in] regAddr 寄存器地址
 * @param  [in] *buf 写入的值存放的缓存区 
 * @param  [in] num 需要读取的字节数  
 * @retval 0-失败 1-成功
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 触发调用, 初始化时调用 \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 无 \n
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
    
    DMA_Cmd(DMA1_Channel6, DISABLE);	    // 关闭当前通道，进行通道设置（如果不关闭，不能进行设置）	
    DMA1_Channel6->CNDTR = num;             // 设置长度
    DMA1->IFCR |= (u32)0x00F00000;	        // 清除DMA_Cannel7中断标志
        
    I2C_AcknowledgeConfig(I2C1, ENABLE);
    I2C_ITConfig(I2C1, I2C_IT_EVT | I2C_IT_BUF | I2C_IT_ERR, ENABLE);
    
    /* Send START condition */
    if(I2C1->CR1 & 0x200)
        I2C1->CR1 &= 0xFDFF;
    I2C_GenerateSTART(I2C1, ENABLE);
}

/**
 * @brief  I2C1事件中断处理函数
 * @param  参数名 参数说明  
 * @param  无
 * @retval 无
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: I2C1状态发生变化时调用 \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 无 \n
 */
void I2C1_EV_IRQHandler(void)
{
    u32 i2cEvent = I2C_GetLastEvent(I2C1);
    switch (i2cEvent)
    {
        /************************** Master Invoke**************************************/
        case I2C_EVENT_MASTER_MODE_SELECT:                      // MSL SB BUSY 0x30001
            I2cProc.stt = COMM_IN_PROCESS;

            /* 读寄存器 */
            if (I2cProc.dir == I2C_RD)
            {
                /* 若还未发送过寄存器地址, 发送写从设备地址 */
                if (!I2cProc.regAddrDone)
                    I2C_Send7bitAddress(I2C1, I2cProc.slaveAddr, I2C_Direction_Transmitter);    
                /* 若已发送过寄存器地址, 发送读从设备地址 */
                else
                {
                    I2C_Send7bitAddress(I2C1, I2cProc.slaveAddr, I2C_Direction_Receiver);      
                    I2cProc.regAddrDone = 0;
                }
            }
            /* 写寄存器 */
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

            /* 用于产生最后一个字节的NACK */
            I2C_DMALastTransferCmd(I2C1, ENABLE);
            /* 开始DMA传输 */
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
 * @brief  I2C1错误中断处理函数
 * @param  参数名 参数说明  
 * @param  无
 * @retval 无
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: I2C1发生错误时调用 \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 无 \n
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
 * @brief  DMA1_CH6中断处理函数
 * @param  参数名 参数说明  
 * @param  无
 * @retval 无
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: i2c1发送完成后，DMA1_CH6传输完成中断时调用 \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 无 \n
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
 * @brief  DMA1_CH7中断处理函数
 * @param  参数名 参数说明  
 * @param  无
 * @retval 无
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: i2c1接收完成后，DMA1_CH7传输完成中断时调用 \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 无 \n
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

