/**
 *************************************************************************************************
 * @file    stm32_spi.c
 * @author  Ge Yue
 * @version V1.0.0
 * @date    2016.03.25
 * @brief   stm32通过DMA SPI驱动OLED显示屏底层函数
 *************************************************************************************************
 @verbatim 
 
  - 版本号       日期      创建人        描述 
    V1.0      2016.03.25   Ge Yue     stm32通过DMA SPI驱动OLED显示屏底层函数; 
 
  - 版本号       日期      修改人        描述 
 @endverbatim 
 */

/**
 * @defgroup SPI stm32_spi: CPU Group
 * @{
 */
 
#include "stm32_spi.h"

/**@brief 无效字节 */
u8 dummyByte = 0;

/**
 * @brief  SPI初始函数
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
void SpiInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;
	DMA_InitTypeDef  DMA_InitStructure;
    
    /* 使能GPIOA, AFIO, SPI, DMA1*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1 | RCC_APB2Periph_AFIO, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); 
    
    /* PA4 -> CS */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    /* PA5 -> CLK, PA7 -> MOSI */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    /* 配置SPI */
	SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;	        // 数据方向：仅发送 
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		                // STM32的SPI工作模式：主机模式 
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;	                // 数据位长度： 8位 
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;			                // 总线空闲是高电平 */
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;		                // 时钟的第1个边沿采样数据 */
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;			                // 片选控制方式：软件控制 */
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;  // 波特率预分频系数：2分频 */
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	                // 数据位传输次序：高位先传 */
	SPI_InitStructure.SPI_CRCPolynomial = 7;			                // CRC多项式寄存器，复位后为7。本例程不用 */
	SPI_Init(SPI1, &SPI_InitStructure);  
    SPI_Cmd(SPI1, ENABLE);                                              // 使能SPI
	SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);                    // SPI配置DMA发送 
    
    /* 配置DMA, DMA1_CH3 -> SPI1_TX */
	DMA_DeInit(DMA1_Channel3);
	DMA_InitStructure.DMA_PeripheralBaseAddr =(u32)&SPI1->DR;               // 外设地址: SPI1的DR寄存器地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&dummyByte;                 // 内存地址: 发送时指定    
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;                      // 数据流向: 内存->外设
	DMA_InitStructure.DMA_BufferSize = 1;                                   // 发送时指定, 此处无效
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;        // 外设地址不自加
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                 // 内存地址自动增加模式
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; // 传输单位: 字节
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;         // 传输单位: 字节
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                           // 非循环模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;                     // 优先级: 高
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                            // 非内存->内存方式 
	DMA_Init(DMA1_Channel3, &DMA_InitStructure);

	DMA_Cmd(DMA1_Channel3, ENABLE);
}

/**
 * @brief  SPI通过DMA方式发送数据
 * @param  参数名 参数说明  
 * @param  [in] buf 发送数据缓存指针  
 * @param  [in] len 发送长度
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 触发调用 \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 无 \n
 */
void SpiSend(u8 *buf, u16 len)
{
    DMA_Cmd(DMA1_Channel3, DISABLE);	    // 关闭当前通道，进行通道设置（如果不关闭，不能进行设置）		
    DMA1_Channel3->CNDTR = len;	            // 设定发送数据长度
    DMA1_Channel3->CMAR = (u32)buf;         // 设定发送缓存地址
    DMA1->IFCR |= (u32)0x00000F00;	        // 清除DMA_Cannel3中断标志(SPI1)
    DMA_Cmd(DMA1_Channel3, ENABLE);		    // 使能DMA通道3，发送数据
}

 /**  @} */

