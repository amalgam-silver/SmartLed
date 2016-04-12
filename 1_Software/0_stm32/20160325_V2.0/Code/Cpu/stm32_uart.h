/**
 *************************************************************************************************
 * @file    stm32_uart.h
 * @author  Ge Yue
 * @version V1.0.0
 * @date    2016.03.18
 * @brief   stm32�Ĵ��ڶ�д
 *************************************************************************************************
 @verbatim 
 
  - �汾��       ����      ������        ���� 
    V1.0      2016.03.18   Ge Yue     stm32�Ĵ��ڶ�д; 
 
  - �汾��       ����      �޸���        ���� 
 @endverbatim 
 */

/**
 * @defgroup UART stm32_uart: CPU Group
 * @{
 */
 
#ifndef _STM32_UART_H
#define _STM32_UART_H

#include "stm32f10x.h"

/** @brief ����1���ݼĴ�����ַ */
#define USART1_DR_Base  0x40013804

/** @brief ���ڻ�������С */
#define UART_BUF_MAX 200

/** @brief  �����շ����ݽṹ�� */
typedef struct
{ 
    u8 rPtr;                    ///< ���ڽ���buf�±� 
    u8 tPtr;                    ///< ���ڷ���buf�±� 
    u8 rxFin;  	                ///< ���ڽ������ 
	u8 txFlg;	                ///< ���ڷ��������־ 
    u8 rxBuf[UART_BUF_MAX];     ///< ���ڽ���buf 
    u8 txBuf[UART_BUF_MAX];     ///< ���ڷ���buf 
}STR_Uart;

/** @brief �������ݴ���ص�����ָ������ */
typedef void (*UartHandler)(STR_Uart *uart);

/** @brief ���ڽṹ�� */
extern STR_Uart Uart;

/** @brief  ��ʼ������ */
void UsartInit(u16 baud);
/** @brief  ���ڷ������� */
void TxUart(void);
/** @brief  ���ڽ������� */
void RxUart(UartHandler handle);
/** @brief  ��ȡ���ڷ���״̬ */
u8 GetSendStt(void);

#endif

/**  @} */
