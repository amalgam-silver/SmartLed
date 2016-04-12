/**
*************************************************************************************************
* @file    Power.h
* @author  Ge Yue
* @version V1.0.0
* @date    2016.04.02
* @brief   �ܺĹ���
*************************************************************************************************
@verbatim 

- �汾��       ����      ������        ���� 
  V1.0      2016.04.02   Ge Yue      �ܺĹ���; 

- �汾��       ����      �޸���        ���� 
@endverbatim 
*/

/**
* @defgroup POWER Power: APP Group
* @{
*/

#ifndef _POWER_H
#define _POWER_H

#include "HAL.H"

/** @brief ϵͳ���Ŀ���״̬ */
typedef enum
{
    NORMAL = 0,     ///< ����ģʽ
    OLEDOFF = 1,    ///< Ϩ��ģʽ
    SLEEP = 2,      ///< ˯��ģʽ
}ENUM_SysStt;

/** @brief �ܺĿ��ƽṹ�� */
typedef struct
{
    u8 sysStt;          ///< ϵͳ״̬
    u8 tarStt;          ///< Ŀ��״̬
}STR_Power;

/** @brief �ܺĿ��ƽṹ�� */
extern STR_Power Power;

/** @brief �ܺĹ����� */
void PowerManage(STR_Power *power);

#endif

 /**  @} */
