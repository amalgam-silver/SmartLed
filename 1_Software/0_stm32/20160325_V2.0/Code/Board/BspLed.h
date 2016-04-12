/**
*************************************************************************************************
* @file    BspLed.h
* @author  Ge Yue
* @version V1.0.0
* @date    2016.03.21
* @brief   Led����
*************************************************************************************************
@verbatim 

- �汾��       ����      ������        ���� 
  V1.0      2016.03.21   Ge Yue       Led����; 

- �汾��       ����      �޸���        ���� 
@endverbatim 
*/

/**
* @defgroup LED BspLed: BSP Group
* @{
*/

#ifndef _BSP_LED_H
#define _BSP_LED_H

#include "DataType.h"

/** @brief LED����ģʽ�ṹ�� */
typedef struct 
{
    u16 pwmW;       ///< ��������, ��λ: ��
    u16 delayEn;    ///< ��ʱ�ص�ʹ��
    u32 delaySec;   ///< ��ʱ�ص�ʣ������
}STR_Light;

/** @brief LED�ʵ�ģʽ�ṹ�� */
typedef struct 
{
    u8 breath;      ///< ������ʹ��
    u8 stepR;       ///< �����ƺ�����Ȳ���
    u8 stepG;       ///< �������̵����Ȳ���
    u8 stepB;       ///< �������������Ȳ���
    u16 pwmR;       ///< �������
    u16 pwmG;       ///< �̹�����
    u16 pwmB;       ///< ��������
}STR_Color;

#define LED_LIGHT   0   ///< ����ģʽ
#define LED_COLOR   1   ///< �ʵ�ģʽ
#define LED_LOPWR   2   ///< �͵�������   

/** @brief LED�ṹ�� */
typedef struct
{
    u8 mode;            ///< ģʽ     0: ����ģʽ     1: �ʵ�ģʽ    
    u8 lowPwr;          ///< �͵�������
    STR_Light light;    ///< ����ģʽ
    STR_Color color;    ///< �ʵ�ģʽ
}STR_Led;

extern STR_Led Led;

/** @brief  Led��ʼ������ */
void LedInit(void);
/** @brief  ��������ģʽ���� */
void SetLight(u16 light, u32 delay);
/** @brief  ��������ģʽ���� */
void SetColor(u16 r, u16 g, u16 b, u8 breath);
/** @brief  LED״̬���º��� */
void LedUpdStt(STR_Led *led);

#endif

/**  @} */

