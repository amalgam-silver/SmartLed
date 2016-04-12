/**
*************************************************************************************************
* @file    BspBle.h
* @author  Ge Yue
* @version V1.0.0
* @date    2016.03.19
* @brief   ����ģ������
*************************************************************************************************
@verbatim 

- �汾��       ����      ������        ���� 
  V1.0      2016.03.19   Ge Yue     ����ģ������; 

- �汾��       ����      �޸���        ���� 
@endverbatim 
*/

/**
* @defgroup BLE BspBle: BSP Group
* @{
*/

#ifndef _BSP_BLE_H
#define _BSP_BLE_H

#include "DataType.h"

/** @brief ���������ж����ȼ�,���������������ȼ����¼�����һ���ж�, �����ȼ���ͬ */
#define BLE_NVIC_PRIO   15;
/** @brief ÿ�����Ļ���Ĵ�С */
#define BYTES_PER_MSG   20
/** @brief ���ͻ������б������� */
//#define MSG_BUF_NUM     16

/** @brief �������� */
typedef struct
{
    u8 data[BYTES_PER_MSG];     ///< ��Ҫ���͵�����ָ��
    u8 len;                     ///< ����
}STR_BleMsg;

/** @brief ��������״̬ */
#define BLE_SEND_IDLE   0   ///< ����
#define BLE_SEND_START  1   ///< �ȴ����������õ�
#define BLE_SEND_PROC   2   ///< ���ڷ���
#define BLE_SEND_END    3   ///< ������ɣ��ȴ����������ø�

/** @brief �͹��������ṹ�� */
typedef struct
{
    u8 en;                      ///< �����Ƿ�ʹ��
    u8 conStt;                  ///< ����״̬
    u8 sendStt;                 ///< ����״̬ 0: ���� 1:�ȴ����������õ� 2: ���ڷ��� 3: �������, �ȴ����������ø�
    u8 txCurPtr;                ///< ��ǰ���ͱ���
    u8 txNeedPtr;               ///< ��Ҫ���͵ı���    
    STR_BleMsg msgTx[16];       ///< �����������ݻ�����
    void (*onConn)(void);       ///< ������Ӧ����
    void (*onDisconn)(void);    ///< �Ͽ�������Ӧ����
}STR_Ble;

/** @brief �͹��������ṹ�� */
extern STR_Ble Ble;

/** @brief ������ʼ������ */
void BleInit(void);
/** @brief  ��ʼ��UART��ʹ��uart1+DMA,��RTU��ʽ�շ����� */
void BleCmd(u8 newState);
/** @brief  ����Ҫ���͵�����д�뻺���� */
u8 PushSendBuf(u8 *data, u8 len);
/** @brief  ��������״̬�� */
void BleSend(void);

#endif
/**  @} */

