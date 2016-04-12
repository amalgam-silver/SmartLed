/**
*************************************************************************************************
* @file    GUI.h
* @author  Ge Yue
* @version V1.0.0
* @date    2016.03.22
* @brief   �˻���������
*************************************************************************************************
@verbatim 

- �汾��       ����      ������        ���� 
  V1.0      2016.03.22   Ge Yue    �˻���������

- �汾��       ����      �޸���        ���� 
@endverbatim 
*/

/**
* @defgroup GUI GUI: APP Group
* @{
*/

#ifndef _GUI_H
#define _GUI_H

#include "HAL.h"
#include "Widget.h"

/** @brief ���뻬������״̬ */
typedef enum
{
    SLIP_STANDSTILL = 0,    ///< ��ֹ״̬
    SLIP_LEFT_OUT   = 1,    ///< ���󻬳�
    SLIP_RIGHT_OUT  = 2,    ///< ���һ���   
    SLIP_LEFT_IN    = 3,    ///< ������
    SLIP_RIGHT_IN   = 4,    ///< ���һ���
}ENUM_SlipStt;

/** @brief ҳ�滬�뻬�������ṹ�� */
typedef struct
{
    u8 stt;     ///< ״̬: 0: ��ֹ 1: ���󻬳� 2: ���һ��� 3: ������ 4: ���һ���
    u8 step;    ///< ÿһ֡��ƫ�Ʋ���, ���Ϊ2
    u8 dest;    ///< Ŀ�����
    s16 offset; ///< ��ǰ֡��ʾ��x����ƫ��
    
}STR_Slip;

/** @brief ��������ʾ�ṹ�� */
typedef struct
{
    STR_SttPic ble;         ///< ����ͼ��
    STR_SttPic dly;         ///< ��ʱ�ص�ͼ��
    STR_SttPic bat;         ///< ����ͼ��
    STR_Value temp;         ///< �¶�
    STR_Clock bigClock;     ///< ��ʱ��
    STR_Clock date;         ///< ����
}STR_GuiMain;

/** @brief ���ý���༭ѡ��ö�� */
typedef enum
{
    SET_BLE = 0,    ///< ����ʹ��
    SET_SLP = 1,    ///< ˯��ʱ��
    SET_YEA = 2,    ///< ��
    SET_MON = 3,    ///< ��
    SET_DAT = 4,    ///< ��
    SET_HOU = 5,    ///< ʱ
    SET_MIN = 6,    ///< ��
    SET_SEC = 7,    ///< ��
    SET_SAV = 8,    ///< ����
    SET_EXT = 9,    ///< �˳�
}ENUM_SetEditPrt;

/** @brief ���ý�����ʾ�ṹ�� */
typedef struct
{
    u8 editPtr;             ///< ��ǰ�༭��    
    STR_Text ble;           ///< "����" 
    STR_Text sleep;         ///< "����ʱ��"
    STR_Text time;          ///< "ʱ������"
    STR_Text save;          ///< "����"
    STR_Text exit;          ///< "�˳�"
    STR_SttPic bleEn;       ///< ����ʹ�ܹ�ѡ��
    STR_Slider secSleep;    ///< ����ʱ�们���� 
    STR_Clock timeSet;      ///< ʱ������ 
}STR_GuiSetting;

/** @brief ��Ϣ������ʾ�ṹ�� */
typedef struct
{
    u8 pagePtr;             ///< ��ǰ��Ϣҳ
    STR_Clock time;         ///< ��ǰʱ��
    STR_Value temp;         ///< �¶�
    STR_Value secDly;       ///< ��ʱ�ص�ʱ��
    STR_Value batVol;       ///< ��ص�ѹ
    STR_SttPic bat;         ///< ��ص���    
    STR_Text batChrg;       ///< "��س��״̬"
    STR_Text batFull;       ///< "��س���״̬"
    STR_Text bleStt;        ///< "����ʹ��״̬"
    STR_Text bleConn;       ///< "��������״̬"
    STR_Text ledDly;        ///< "��ʱ�ص�"
    STR_Text ver;           ///< "�汾��Ϣ"
    STR_Text author;        ///< "����"
    
}STR_GuiInfo;

/** @brief LED���ý���༭ѡ��ö�� */
typedef enum
{
    LEDSET_MODE = 0,
    LEDSET_BRTH = 1,
    LEDSET_PWMR = 2,
    LEDSET_PWMG = 3,
    LEDSET_PWMB = 4,
    LEDSET_PWMW = 5,
    LEDSET_DELA = 6,
    LEDSET_SAVE = 7,
    LEDSET_EXIT = 8,
}ENUM_ledEditPrt;

/** @brief LED���ý�����ʾ�ṹ�� */
typedef struct
{
    u8 editPtr;             ///< ��ǰ�༭��  
    STR_SttPic ledMode;     ///< ģʽ
    STR_Text colorLed;      ///< "�ʵ�ģʽ"
    STR_Text dly;           ///< "��ʱ�ص�"
    STR_Slider minDly;      ///< ��ʱ�ص�ʱ��
    STR_Text light;         ///< "����"
    STR_Slider pwmW;        ///< ����
    STR_SttPic breathEn;    ///< ������ʹ��    
    STR_Text breath;        ///< "������"
    STR_Text red;           ///< "��ɫ"
    STR_Slider pwmR;        ///< �������
    STR_Text green;         ///< "��ɫ"
    STR_Slider pwmG;        ///< �̵�����
    STR_Text blue;          ///< "��ɫ"
    STR_Slider pwmB;        ///< ��������  
    STR_Text exit;          ///< "�˳�"
    STR_Text save;          ///< "����"    
}STR_LedSet;

/** @brief GUI����ʾ���� */
typedef enum
{
    GUI_MAIN = 0,       ///< ʱ��������
    GUI_LED_MENU = 1,   ///< LED���ò˵�����
    GUI_INF_MENU = 2,   ///< ϵͳ��Ϣ�˵�����
    GUI_SET_MENU = 3,   ///< ϵͳ���ò˵�����
    GUI_LED = 4,        ///< LED���ý���
    GUI_INF = 5,        ///< ϵͳ��Ϣ����
    GUI_SET = 6,        ///< ϵͳ���ý���
}ENUM_GuiStt;

/** @brief GUI��ʾ�ṹ�� */
typedef struct
{
    u8 stt;                 ///< GUIҳ��״̬
    u8 upd;                 ///< ǿ�Ƹ�������ҳ���־λ
    STR_Slip slip;          ///< ���������ṹ��
    STR_GuiMain main;       ///< ��������ʾ�ṹ��
    STR_GuiSetting setting; ///< ���ý�����ʾ�ṹ��
    STR_GuiInfo info;       ///< ��Ϣ������ʾ�ṹ��
    STR_LedSet led;         ///< LED���ý���ṹ��
}STR_Gui;

/** @brief GUI��ʾ�ṹ�� */
extern STR_Gui Gui;

/** @brief  �˻�����������ʾ*/
void Display(STR_Gui *gui);
/** @brief  �˻��������水����Ӧ*/
void KeyAction(STR_Gui *gui, STR_Key *key);
void GuiInit(void);

#endif 

/**  @} */
