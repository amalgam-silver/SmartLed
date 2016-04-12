/**
*************************************************************************************************
* @file    Widget.h
* @author  Ge Yue
* @version V1.0.0
* @date    2016.03.25
* @brief   �˻���ʾ�ؼ�
*************************************************************************************************
@verbatim 

- �汾��       ����      ������        ���� 
  V1.0      2016.03.25   Ge Yue      �˻���ʾ�ؼ�; 

- �汾��       ����      �޸���        ���� 
@endverbatim 
*/

/**
* @defgroup WIDGET Widget: APP Group
* @{
*/


#ifndef _WIDGET_H
#define _WIDGET_H

#include "HAL.h"

/** @brief ʱ����ʾ�������� */
extern const unsigned char fontTimeNum[][28];
/** @brief ������ʾͼ�� */
extern const unsigned char fontBattery[][10];
/** @brief ����ͼ�� */
extern const unsigned char fontBLE[][10];
/** @brief ʱ����ʾ�õ�ð�� */
extern const unsigned char fontTimeColon[][16];
/** @brief ��ѡ��ͼ�� */
extern const unsigned char fontEnable[][8];
/** @brief ʱ��ͼ�� */
extern const unsigned char fontDelay[][15];
/** @brief ������ͼ�� */
extern const unsigned char fontSlide[100];

/** @brief �������ṹ�� */
typedef struct
{
    u8 x;           ///< x���� 0~127
    u8 y;           ///< y���� 0~7   
    u8 len;         ///< ����������  0~100
    u8 pos;         ///< ���ȿ�λ��
    u8 prePos;      ///< ֮ǰ�Ľ��ȿ�λ��
    u8 focus;       ///< 0 δѡ��  1 ѡ��(����ʾ��ɫ)
    u8 preFocus;    ///< ֮ǰ��״̬
    u8 upd;         ///< ǿ�Ƹ��±�־λ
    u8 val;         ///< ֵ
    u8 str[7];      ///< ��ֵ��λ
}STR_Slider;

/** @brief ��̬�ı��ṹ�� */
typedef struct
{
    u8 x;           ///< x���� 0~127
    u8 y;           ///< y���� 0~7
    u8 focus;       ///< 0 δѡ��  1 ѡ��(����ʾ��ɫ)
    u8 preFocus;    ///< ֮ǰ��״̬
    u8 upd;         ///< ǿ�Ƹ��±�־λ
    char *str;      ///< �ı��ַ���ָ��
}STR_Text;

/** @brief ״̬ͼƬ��ṹ�� */
typedef struct
{
    u8 x;           ///< x���� 0~127
    u8 y;           ///< y���� 0~7
    u8 width;       ///< ��
    u8 height;      ///< ��
    u8 stt;         ///< ״̬
    u8 preStt;      ///< ֮ǰ��״̬
    u8 upd;         ///< ǿ�Ƹ��±�־λ
    u8 *picBase;    ///< ͼƬ���׵�ַ    
}STR_SttPic;

/** @brief ��ֵ�ؼ��ṹ�� */
typedef struct
{
    u8 x;           ///< x���� 0~127
    u8 y;           ///< y���� 0~7
    u8 upd;         ///< ǿ�Ƹ��±�־λ
    u8 dot;         ///< С����λ��
    u8 str[4];      ///< ���ֺ�Ҫ��ʾ�ĵ�λ
    u8 focus;       ///< 0 δѡ��  1 ѡ��(����ʾ��ɫ)
    u8 preFocus;    ///< ֮ǰ��״̬
    s16 val;        ///< ��ֵ
    s16 preVal;     ///< ǰֵ
}STR_Value;

/** @brief ʱ��ѡ��״̬ö�� */
typedef enum
{
    FOCUS_NONE = 0,
    FOCUS_YEAR = 1,
    FOCUS_MONTH = 2,
    FOCUS_DATE = 3,
    FOCUS_HOUR = 4,
    FOCUS_MIN = 5,
    FOCUS_SEC = 6,
}ENUM_ClockFocus;

/** @brief ʱ����ʽö�� */
typedef enum
{
    CLOCK_BIG = 0,  ///< ��ҳ��ʱ����ʽ
    CLOCK_SET = 1,  ///< ����ʱ��������ʽ
    CLOCK_DAT = 2,  ///< ����ʾ������ʽ
    CLOCK_FUL = 3,  ///< ������ʾ��ʽ
}ENUM_ClockStyle;

/** @brief ʱ�ӿؼ��ṹ�� */
typedef struct
{
    u8 x;               ///< x���� 0~127
    u8 y;               ///< y���� 0~7
    u8 upd;             ///< ǿ�Ƹ��±�־λ
    u8 style;           ///< ��ʽ
    u8 focus;           ///< ѡ��״̬ 0: δѡ�� 1: ѡ���� 2: ѡ���� 3: ѡ���� 4: ѡ��ʱ 5: ѡ�з� 6: ѡ���� 
    u8 preFocus;        ///< ��һ�ε�ѡ��״̬
    STR_Time *time;     ///< ��Ҫ��ʾ��ʱ��ָ��
    STR_Time preTime;   ///< �ϴ���ʾ��ʱ��    
}STR_Clock;

/** @brief  ��ʼ��ʱ�ӿؼ� */
void InitClock(u8 x, u8 y, u8 style, STR_Time *time, STR_Clock *clock);
/** @brief  ��ʾʱ�ӿؼ� */
void DispClock(STR_Clock *clock, int xOffset);
/** @brief  ��ʼ��ͼƬ��ؼ� */
void InitSttPic(u8 x, u8 y, u8 width, u8 height, u8 *picBase, STR_SttPic *sttPic);
/** @brief  ��ʾͼƬ��ؼ� */
void DispSttPic(STR_SttPic *sttPic, int xOffset);
/** @brief  ��ʼ����ֵ�ؼ� */
void InitValue(u8 x, u8 y, u8 dot, char *str, STR_Value *value);
/** @brief  ��ʾ��ֵ�ؼ� */
void DispValue(STR_Value *value, int xOffset);
/** @brief  ��ʼ���������ؼ� */
void InitSlider(u8 x, u8 y, u8 len, char *str, STR_Slider *slider);
/** @brief  ��������ʾ�ؼ� */
void DispSlid(STR_Slider *slider, int xOffset);
/** @brief  ��ʼ����̬�ı��ؼ� */
void InitText(u8 x, u8 y, char *str, STR_Text *text);
/** @brief  ��̬�ı���ʾ�ؼ� */
void DispText(STR_Text *text, int xOffset);

#endif

 /**  @} */
