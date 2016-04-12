/**
*************************************************************************************************
* @file    Manage.h
* @author  Ge Yue
* @version V1.0.0
* @date    2016.03.22
* @brief   Ӧ�ò���Ӳ����֮�䡢Ӧ�ò��ģ��֮������ݴ���
*************************************************************************************************
@verbatim 

- �汾��       ����      ������        ���� 
  V1.0      2016.03.22   Ge Yue    Ӧ�ò���Ӳ����֮�䡢Ӧ�ò��ģ��֮������ݴ��� 

- �汾��       ����      �޸���        ���� 
@endverbatim 
*/

/**
* @defgroup MANAGE Manage: APP Group
* @{
*/

#ifndef _MANAGE_H
#define _MANAGE_H

#include "Power.h"
#include "Widget.h"
#include "Gui.h"
#include "Protocol.h"
#include "HAL.h"

/** @brief ����汾 2.0 */
#define SW_VERSION 20
/** @brief ����汾 2.0 */
#define STR_VERSION "SmartLED V2.0"

/** @brief ϵͳ�ṹ�� */
typedef struct{
    u8 secSleep;            ///< ����ʱ��
    u32 secSys;             ///< ϵͳʱ��
    u32 secLastKey;         ///< �ϴΰ���ʱ��
    
    /* BSP�� */
    STR_Ble *ble;           ///< ����
    STR_BATTERY *bat;       ///< ���
    STR_Key *key;           ///< ����
    STR_Led *led;           ///< LED
    STR_Temperture *temp;   ///< �¶�
    STR_Rtc *rtc;           ///< ʱ��
    
    /* APP�� */
    STR_Power *power;       ///< �ܺĹ���
    STR_Gui *gui;           ///< �˻�����
}STR_Sys;


/** @brief Ӳ����ʼ������ */
void SystemInit(void);
/** @brief �����ʼ������ */
void SoftwareInit(void);
/** @brief ������ɺ��� */
void Task(void);
/** @brief ����ϵͳ״̬ */
void SysSttUpd(STR_Sys *sys);
/** @brief �����˻��������� */
void GuiDataUpd(STR_Sys *sys);

/** @brief ϵͳ�����ṹ�� */
extern STR_Sys Sys;

#endif

/**  @} */
