/**
*************************************************************************************************
* @file    BspBle.c
* @author  Ge Yue
* @version V1.0.0
* @date    2016.03.19
* @brief   蓝牙模块驱动
*************************************************************************************************
@verbatim 

- 版本号       日期      创建人        描述 
  V1.0      2016.03.19   Ge Yue     蓝牙模块驱动; 

- 版本号       日期      修改人        描述 
@endverbatim 
*/

/**
* @defgroup BLE BspBle: BSP Group
* @{
*/

#include "BspBle.h"
#include "Bsp.h"
#include "string.h"

/** @brief 低功耗蓝牙结构体 */
STR_Ble Ble = {0,};

/**
 * @brief  温度传感器初始化函数
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
void BleInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	
	/* PB10 -> BT_CON   PB11 -> BT_SLEEP */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* PB0 -> BT_EN     PB1 -> BT_BRTS */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_WriteBit(GPIOB,GPIO_Pin_1,Bit_SET);
	GPIO_WriteBit(GPIOB,GPIO_Pin_0,Bit_RESET);
	Ble.en = ENABLE;
	
	/* 配置蓝牙连接中断 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line10;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;  
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource10);
    
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = BLE_NVIC_PRIO;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
    
    /* 初始化串口 */
    UsartInit(9600);
}

/**
 * @brief  初始化UART，使用uart1+DMA,按RTU格式收发数据
 * @param  参数名 参数说明
 * @param  [in] newState 需要切换的状态 ENABLE 开启蓝牙 DISABLE 关闭蓝牙
 * @retval 无
 * @par 使用全局变量
 *         无 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 触发调用 \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 无
 *
 */
void BleCmd(u8 newState)
{
	if(newState == ENABLE)
	{
		GPIO_WriteBit(GPIOB,GPIO_Pin_0,Bit_RESET);
		Ble.en = ENABLE;
	}
	else
	{
		GPIO_WriteBit(GPIOB,GPIO_Pin_0,Bit_SET);
		Ble.en = DISABLE;
	}
}

/**
 * @brief  将需要发送的数据写入缓冲区
 * @param  参数名 参数说明
 * @param  [in] *data 发送的数据指针
 * @param  [in] len 发送的数据长度
 * @retval 无
 * @par 使用全局变量
 *         无 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 触发调用 \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 无
 *
 */
u8 PushSendBuf(u8 *data, u8 len)
{
    /* 报文长度超过限制 */
    if (len > BYTES_PER_MSG)
    {
        return 0;
    }
    
    /* 缓冲区已满 */
    if (((Ble.txCurPtr + 15) & 0x0f) == Ble.txNeedPtr)
    {
        return 0;
    }
    
    memcpy(Ble.msgTx[Ble.txNeedPtr].data, data, len);
    Ble.msgTx[Ble.txNeedPtr].len = len;
    Ble.txNeedPtr++;
    Ble.txNeedPtr &= 0x0f;
    
    return 1;
}

/**
 * @brief  蓝牙发送状态机
 * @param  参数名 参数说明  
 * @param  无  
 * @retval 无
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 1ms \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 由于该蓝牙模块在发送数据前需先将发送引脚拉低至少500us,发送完成后需至少等待200us再将发送引脚拉高，故采用此状态机取代延时等待 \n
 */
void BleSend(void)
{
    switch (Ble.sendStt)
    {
        /* 空闲状态 */
        case BLE_SEND_IDLE:
            /* 有待发送报文 */
            if (Ble.txCurPtr != Ble.txNeedPtr)
            {
                /* 将发送引脚置低 */
                GPIO_WriteBit(GPIOB,GPIO_Pin_1,Bit_RESET);
                Ble.sendStt = BLE_SEND_START;
            }
            break;
        
        /* 等待发送引脚置低 */
        case BLE_SEND_START:
            Uart.tPtr = Ble.msgTx[Ble.txCurPtr].len;
            Uart.txFlg = 0xff;
            memcpy(Uart.txBuf, Ble.msgTx[Ble.txCurPtr].data, Ble.msgTx[Ble.txCurPtr].len);
            TxUart();
            Ble.sendStt = BLE_SEND_PROC;
            break;
        
        /* 发送中 */
        case BLE_SEND_PROC:
            /* 发送完成 */
            if (GetSendStt())
            {
                Ble.sendStt = BLE_SEND_END;
            }
            break;
        
        /* 发送完成, 将发送引脚置高 */
        case BLE_SEND_END:
            GPIO_WriteBit(GPIOB,GPIO_Pin_1,Bit_SET);
            Ble.sendStt = BLE_SEND_IDLE;
            Ble.txCurPtr++;
            Ble.txCurPtr &= 0x0f;
            break;
    }  
}

/**
 * @brief  10~15通道外部中断处理函数
 * @param  参数名 参数说明  
 * @param  无  
 * @retval 无
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 触发调用 \n
 *       ● 可否打断: 不可以 \n
 *
 * @par  注意:
 *       ● 12通道用于下键中断，仅在睡眠前开启，用于按键将系统从睡眠中唤醒 \n
 *       ● 10通道用于蓝牙连接状态变化断 \n
 */
void EXTI15_10_IRQHandler(void)
{
    //按键下
	if(EXTI_GetITStatus(EXTI_Line12) != RESET)
	{	
		EXTI_ClearITPendingBit(EXTI_Line12);
	}
    
	//蓝牙connected信号有跳变
	if(EXTI_GetITStatus(EXTI_Line10) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line10);
		//连接断开
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10) == 1)
		{
			Ble.conStt = 0;
            if (Ble.onDisconn)
            {
                Ble.onDisconn();
            }
		}
		//建立连接
		else
		{
			Ble.conStt = 1;
            if (Ble.onConn)
            {
                Ble.onConn();
            }
		}	
	}
}



/**  @} */

