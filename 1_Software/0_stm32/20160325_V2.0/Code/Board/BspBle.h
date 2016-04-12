/**
*************************************************************************************************
* @file    BspBle.h
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

#ifndef _BSP_BLE_H
#define _BSP_BLE_H

#include "DataType.h"

/** @brief 蓝牙连接中断优先级,这里蓝牙连接优先级和下键共用一个中断, 故优先级相同 */
#define BLE_NVIC_PRIO   15;
/** @brief 每条报文缓存的大小 */
#define BYTES_PER_MSG   20
/** @brief 发送缓冲区中报文数量 */
//#define MSG_BUF_NUM     16

/** @brief 蓝牙报文 */
typedef struct
{
    u8 data[BYTES_PER_MSG];     ///< 需要发送的数据指针
    u8 len;                     ///< 长度
}STR_BleMsg;

/** @brief 蓝牙发送状态 */
#define BLE_SEND_IDLE   0   ///< 空闲
#define BLE_SEND_START  1   ///< 等待发送引脚置低
#define BLE_SEND_PROC   2   ///< 正在发送
#define BLE_SEND_END    3   ///< 发送完成，等待发送引脚置高

/** @brief 低功耗蓝牙结构体 */
typedef struct
{
    u8 en;                      ///< 蓝牙是否使能
    u8 conStt;                  ///< 连接状态
    u8 sendStt;                 ///< 发送状态 0: 空闲 1:等待发送引脚置低 2: 正在发送 3: 发送完成, 等待发送引脚置高
    u8 txCurPtr;                ///< 当前发送报文
    u8 txNeedPtr;               ///< 需要发送的报文    
    STR_BleMsg msgTx[16];       ///< 蓝牙发送数据缓冲区
    void (*onConn)(void);       ///< 连接响应函数
    void (*onDisconn)(void);    ///< 断开链接响应函数
}STR_Ble;

/** @brief 低功耗蓝牙结构体 */
extern STR_Ble Ble;

/** @brief 蓝牙初始化函数 */
void BleInit(void);
/** @brief  初始化UART，使用uart1+DMA,按RTU格式收发数据 */
void BleCmd(u8 newState);
/** @brief  将需要发送的数据写入缓冲区 */
u8 PushSendBuf(u8 *data, u8 len);
/** @brief  蓝牙发送状态机 */
void BleSend(void);

#endif
/**  @} */

