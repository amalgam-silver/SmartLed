/**
*************************************************************************************************
* @file    Power.c
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

#include "Power.h"
#include "stm32_pwm.h"

/** @brief �ܺĿ��ƽṹ�� */
STR_Power Power;

/** @brief  ˯��ǰ����׼������ */
static void PrepareSleep(void);
/** @brief  ���Ѻ���Ҫ���Ĺ��� */
static void PrepareWake(void);

/**
 * @brief  �ܺĹ�����
 * @param  ������ ����˵��  
 * @param  [in] power �ܺĹ���ṹ��ָ��  
 * @retval ��
 * @par ʹ��ȫ�ֱ��� \n
 * @note �� ִ��ʱ��: \n
 *       �� ��������: 100ms \n
 *       �� �ɷ���: ���� \n
 *
 * @par  ע��:
 *       �� �� \n
 */
void PowerManage(STR_Power *power)
{        
    switch (power->sysStt)
    {
        case NORMAL:
            /* ���� -> ���� */ 
            if (power->tarStt == OLEDOFF)
            {
                OledDisplayOff();
                power->sysStt = OLEDOFF;
            }
            /* ���� -> ˯�� */
            else if (power->tarStt == SLEEP)
            {
                OledDisplayOff();
                PrepareSleep();
                power->sysStt = SLEEP;
            }
            break;       
        case OLEDOFF:
            /* ���� -> ���� */
            if (power->tarStt == NORMAL)
            {
                OledDisplayOn();
                power->sysStt = NORMAL;
            }
            /* ���� -> ˯�� */
            else if (power->tarStt == SLEEP)
            {
                PrepareSleep();
                power->sysStt = SLEEP;
            }
            break;      
        case SLEEP:
            /* ˯�� -> ���� */
            if (power->tarStt == NORMAL)
            {
                PrepareWake();
                OledDisplayOn();
                power->sysStt = NORMAL;
            }
            /* ˯�� -> ���� */
            else if (power->tarStt == OLEDOFF)
            {
                PrepareWake();
                power->sysStt = OLEDOFF;
            }
            break;
    }    
}

/**
 * @brief  ˯��ǰ����׼������
 * @param  ������ ����˵��  
 * @param  ��  
 * @retval ��
 * @par ʹ��ȫ�ֱ��� \n
 * @note �� ִ��ʱ��: \n
 *       �� ��������: �������� \n
 *       �� �ɷ���: ���� \n
 *
 * @par  ע��:
 *       �� �� \n
 */
static void PrepareSleep(void)
{
	/* ���������ж� */
	SetKeyInt(1);
    
	/* �ر�����PWM */
	PwmSet(0,0,0);
	
	/* ����STOP_MODE */
	Sleep();
}

/**
 * @brief  ���Ѻ���Ҫ���Ĺ���
 * @param  ������ ����˵��  
 * @param  ��  
 * @retval ��
 * @par ʹ��ȫ�ֱ��� \n
 * @note �� ִ��ʱ��: \n
 *       �� ��������: �������� \n
 *       �� �ɷ���: ���� \n
 *
 * @par  ע��:
 *       �� �� \n
 */
static void PrepareWake(void)
{    
    //�رհ����ж�
	SetKeyInt(0);
}

 /**  @} */
