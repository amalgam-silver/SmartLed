/**
 *************************************************************************************************
 * @file    stm32_uart.h
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
 
#ifndef _STM32_UART_H
#define _STM32_UART_H

#include "stm32f10x.h"

/** @brief 串口1数据寄存器地址 */
#define USART1_DR_Base  0x40013804

/** @brief 串口缓存区大小 */
#define UART_BUF_MAX 200

/** @brief  串口收发数据结构体 */
typedef struct
{ 
    u8 rPtr;                    ///< 串口接收buf下标 
    u8 tPtr;                    ///< 串口发送buf下标 
    u8 rxFin;  	                ///< 串口接收完成 
	u8 txFlg;	                ///< 串口发送允许标志 
    u8 rxBuf[UART_BUF_MAX];     ///< 串口接收buf 
    u8 txBuf[UART_BUF_MAX];     ///< 串口发送buf 
}STR_Uart;

/** @brief 串口数据处理回调函数指针类型 */
typedef void (*UartHandler)(STR_Uart *uart);

/** @brief 串口结构体 */
extern STR_Uart Uart;

/** @brief  初始化串口 */
void UsartInit(u16 baud);
/** @brief  串口发送数据 */
void TxUart(void);
/** @brief  串口接收数据 */
void RxUart(UartHandler handle);
/** @brief  获取串口发送状态 */
u8 GetSendStt(void);

#endif

/**  @} */
