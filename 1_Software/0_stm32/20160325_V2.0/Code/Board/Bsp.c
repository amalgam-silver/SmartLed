/**
 *************************************************************************************************
 * @file    Bsp.c
 * @author  Ge Yue
 * @version V1.0.0
 * @date    2016.03.18
 * @brief   Ӳ���弶�ӿ�
 *************************************************************************************************
 @verbatim 
 
  - �汾��       ����      ������        ���� 
    V1.0      2016.03.18   Ge Yue     Ӳ���弶�ӿ�; 
 
  - �汾��       ����      �޸���        ���� 
 @endverbatim 
 */

/**
 * @defgroup BSP Bsp: BSP Group
 * @{
 */

#include "Bsp.h"

#include "BspRtc.h"
#include "BspTemp.h"
#include "BspBle.h"
#include "BspBattery.h"
#include "BspKey.h"
#include "BspOled.h"
#include "BspLed.h"
#include "BspTimer.h"

 /**
 * @brief  Ӳ����ʼ����
 * @param  ������ ����˵��  
 * @param  ��  
 * @retval ��
 * @par ʹ��ȫ�ֱ��� \n
 * @note �� ִ��ʱ��: \n
 *       �� ��������: ��������, ��ʼ��ʱ���� \n
 *       �� �ɷ���: ���� \n
 *
 * @par  ע��:
 *       �� �� \n
 */
void SystemInit(void)
{
    /* ��ʼ��ϵͳʱ�� */
    RccInit();
    SystickInit(8);
    
    /* �����ж�: 0bit������ռ���ȼ�, 4bit������Ӧ���ȼ�, ��: �������û��ж�Ƕ�� */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    
    /* ��ʼ����ʱ�� */
    TimerInit();
    
    /* ��ʼ��ʵʱʱ�� */
    RtcInit();
    
    /* ��ʼ���¶ȴ����� */
    TempInit();
    
    /* ������ʼ�� */
    BleInit();
    
    /* ���״̬��ʼ�� */
    BatInit();
    
    /* OLED��ʼ�� */
    OledInit();
    
    /* Led��ʼ�� */
    LedInit();
    
    /* ������ʼ�� */   
    KeyInit();
}
 
 
 /**  @} */

