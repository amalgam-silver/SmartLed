/**
*************************************************************************************************
* @file    BspBattery.h
* @author  Ge Yue
* @version V1.0.0
* @date    2016.03.19
* @brief   ͨ��ADC�ɼ��ĵ�ص�ѹ������ʣ�����
*************************************************************************************************
@verbatim 

- �汾��       ����      ������        ���� 
  V1.0      2016.03.19   Ge Yue    ͨ��ADC�ɼ��ĵ�ص�ѹ������ʣ�����; 

- �汾��       ����      �޸���        ���� 
@endverbatim 
*/

/**
* @defgroup BETTERY BspBattery: BSP Group
* @{
*/

#ifndef _BSP_BAT_H
#define _BSP_BAT_H

#include "DataType.h"

/** @brief ���״̬������ */
typedef union
{
    u8 all;             ///< ���״̬
    struct
    {
        u8 chrg:1;      ///< �Ƿ��ڳ����
        u8 full:1;      ///< �Ƿ������
        u8 rsv:6;       ///< ����
    }bit;               ///< 1 bit
}UN_BatStt;

/** @brief �����Ϣ�ṹ�� */
typedef struct
{
    UN_BatStt stt;      ///< ���״̬   bit0: ���״̬  bit1: �Ƿ����
    u8 lvl;             ///< �����ȼ�   0~6
    u16 vol;            ///< ��ص�ѹ   ��λ: V   2dot
}STR_BATTERY;

/** @brief �����Ϣ�ṹ�� */
extern STR_BATTERY Bat;

/** @brief ���״̬����ʼ�� */
void BatInit(void);
/** @brief ���µ��״̬ */
void GetBatStt(STR_BATTERY *bat);

#endif
/**  @} */


