/**
 *************************************************************************************************
 * @file    stm32_uart.c
 * @author  Ge Yue
 * @version V1.0.0
 * @date    2016.03.18
 * @brief   stm32的串口读写
 *************************************************************************************************
 @verbatim 
 
  - 版本号       日期      创建人        描述 
    V1.0      2016.03.18   Ge Yue     stm32的串口读写; 
    
  - 版本号       日期      修改人        描述 
 @endverbatim 
 */

/**
 * @defgroup UART stm32_uart: CPU Group
 * @{
 */
 
#include "stm32_uart.h"

/** @brief 串口结构体 */
STR_Uart Uart;

/**
 * @brief  初始化UART，使用uart1+DMA,按RTU格式收发数据
 * @param  参数名 参数说明
 * @param  [in] baud usart1的波特率bps(2400~115200bps)
 * @retval 无
 * @par 使用全局变量
 *         无 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 上电初始化 \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ●  \n
 *
 */
void UsartInit(u16 baud)
{
    USART_InitTypeDef USART_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    DMA_InitTypeDef DMA_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

    /* 打开时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;        // 外设地址不自加
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                 // 内存地址自加
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; // 传输单位: 字节
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;         // 传输单位: 字节
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                            // 禁止M->M模式
    // DMA1 Channel4 (triggered by USART1 Tx event) Config
    DMA_DeInit(DMA1_Channel4);  
    DMA_InitStructure.DMA_PeripheralBaseAddr = USART1_DR_Base;              // DMA连接的外设寄存器地址
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)Uart.txBuf;	                // UART1发送缓冲器首地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;                      // 数据发送方向外部资源为目的地址
    DMA_InitStructure.DMA_BufferSize = UART_BUF_MAX;                        // 发送缓冲区大小
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                           // DMA模式为正常
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;      		        // 当前DMA通道优先级为低
    DMA_Init(DMA1_Channel4, &DMA_InitStructure);                	        // 初始化DMA通道  
   
    // DMA1 Channel5 (triggered by USART1 Rx event) Config
    DMA_DeInit(DMA1_Channel5);  
    DMA_InitStructure.DMA_PeripheralBaseAddr = USART1_DR_Base;
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)Uart.rxBuf;                 // UART1接收缓冲器首地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                      // 数据发送方向,外部资源为源地址
    DMA_InitStructure.DMA_BufferSize = UART_BUF_MAX;                        // 接收缓冲区大小
  	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                         // DMA模式为循环
    DMA_Init(DMA1_Channel5, &DMA_InitStructure);                            // 初始化DMA通道 
    
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   	
    
    /* PA9 -> UART1_TX, PA10 -> UART1_RX */ 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;          		
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    		
    GPIO_Init(GPIOA, &GPIO_InitStructure);  
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;              
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   
    GPIO_Init(GPIOA, &GPIO_InitStructure);	
   
    /* 初始化串口 */
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;                         //数据长度8bit
    USART_InitStructure.USART_StopBits = USART_StopBits_1;                              //停止位1bit
    USART_InitStructure.USART_Parity = USART_Parity_No;                                 //无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;     //无流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                     //发送接收
    USART_InitStructure.USART_BaudRate = baud;                                          //波特率
    USART_Init(USART1, &USART_InitStructure);
    
    // Uart1 通讯开通 DMA    
    USART_DMACmd(USART1, USART_DMAReq_Rx | USART_DMAReq_Tx, ENABLE);

    // Enable the USART1 
    USART_Cmd(USART1, ENABLE);		

    DMA_Cmd(DMA1_Channel4, ENABLE);
    DMA_Cmd(DMA1_Channel5, ENABLE);
    
}

/**
 * @brief  UART1数据接收程序，每20ms执行一次，检查数据接收进度，如果大于4ms没有新的数据被接收， \n
 *         则认为，当前数据包接收结束，等待处理进程处理数据.
 *         接收方式为DMA，数据包的分辨使用间隔时间分辨。
 * @param  参数名 参数说明
 * @param  [in] handle 串口数据处理函数指针
 * @retval 无
 * @par 使用全局变量
 *         Uart: 串口1结构体 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 4ms \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● \n
 */
void RxUart(UartHandler handle)
{
    u32 tmpRp = 0;
    
    tmpRp = UART_BUF_MAX - DMA1_Channel5->CNDTR;     	//计算串口接收BUF下标

	if(Uart.rPtr != tmpRp)				                //接收数据长度有变化，正在接收数据中
	{
	    Uart.rPtr = tmpRp;
	}
	else
	{
		if(Uart.rPtr > 0)
		{
            Uart.rxFin = 1;				                //接收完成
            
            (*handle)(&Uart);                           //用户处理函数
            
            DMA_Cmd(DMA1_Channel5, DISABLE);			//关闭当前通道，进行通道设置（如果不关闭，不能进行设置）。
	        DMA1_Channel5->CNDTR = UART_BUF_MAX;		//设定接收数据长度（接收缓冲空间）
            Uart.rPtr=0;
	        DMA_Cmd(DMA1_Channel5, ENABLE);				//使能DMA1通道5，接收数据
		}
	}
}

/**        
 * @brief  串口1发送数据。
 * @param  参数名 参数说明
 * @param  无
 * @retval 无
 * @par 使用全局变量
 *         无 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 触发调用 \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 发送前先将要发送的数据放入缓冲区 Uart[].txBuf，赋值发送长度 Uart[].tPtr，将发送标志位Uart[].txFlg置1。 \n
 */
void TxUart(void)
{
    if(Uart.txFlg)
	{
		DMA_Cmd(DMA1_Channel4, DISABLE);	    // 关闭当前通道，进行通道设置（如果不关闭，不能进行设置）		
		DMA1_Channel4->CNDTR = Uart.tPtr;	    // 设定发送数据长度
     	DMA1->IFCR |= (u32)0x0000F000;	        // 清除DMA_Cannel4中断标志(usart1)
     	DMA_Cmd(DMA1_Channel4, ENABLE);		    // 使能DMA通道4，发送数据
		Uart.txFlg = 0;
	}	
}

/**        
 * @brief  获取串口发送状态
 * @param  参数名 参数说明
 * @param  无
 * @retval 0 - 正在发送 非0 - 发送完成
 * @par 使用全局变量
 *         无 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 触发调用 \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ●  \n
 */
u8 GetSendStt(void)
{
    return USART_GetFlagStatus(USART1, USART_FLAG_TC);
}

/**  @} */

