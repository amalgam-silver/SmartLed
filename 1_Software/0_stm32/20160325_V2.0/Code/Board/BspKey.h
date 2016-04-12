/**
*************************************************************************************************
* @file    BspKey.h
* @author  Ge Yue
* @version V1.0.0
* @date    2016.03.19
* @brief   ��������
*************************************************************************************************
@verbatim 

- �汾��       ����      ������        ���� 
  V1.0      2016.03.19   Ge Yue      ��������; 

- �汾��       ����      �޸���        ���� 
@endverbatim 
*/

/**
* @defgroup KEY BspKey: BSP Group
* @{
*/
#ifndef _BSP_KEY_H
#define _BSP_KEY_H

#include "DataType.h"

/** @brief ���������˲�����,ÿ��1ms */
#define KEY_DN_FLT     100      ///< �൱�� 100ms
/** @brief ���������˲�����,ÿ��1ms */
#define KEY_UP_FLT     50      ///< �൱�� 50ms
/** @brief ���������˲�����,ÿ��1ms */
#define KEY_LONG       900     ///< �൱�� 900ms

/** @brief �����ж����ȼ� */
#define KEY_NVIC_PRIO   15;

/** @brief ����״̬������ */
typedef union
{
    u8 all;            ///< ����״̬
    struct
    {
        u8 left:1;     ///< ���
        u8 right:1;    ///< �Ҽ�
        u8 down:1;     ///< �¼�
        u8 rsv:5;      ///< ����
    }bit;              ///< 1 bit
}UN_KeyStt;

/** @brief ����״̬�ṹ�� */
typedef struct
{
    UN_KeyStt keyPressing;      ///< �������ڰ���
    UN_KeyStt keyPressed;       ///< ��������
    UN_KeyStt keyLongPressed;   ///< ������������
}STR_Key;

/** @brief ����״̬�ṹ�� */
extern STR_Key KeyStt;  

/** @brief  ������ʼ������ */
void KeyInit(void);
/** @brief  ����ɨ�躯�� */
void KeyScan(STR_Key *key);
/** @brief  ���ð����жϺ��� */
void SetKeyInt(u8 newStt);

#endif

/**  @} */

