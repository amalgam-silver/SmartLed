/**
*************************************************************************************************
* @file    BspKey.c
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

#include "BspKey.h"
#include "Bsp.h"

/** @brief ����״̬�ṹ�� */
STR_Key KeyStt;

/**
 * @brief  ������ʼ������
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
void KeyInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
    /* ʹ��GPIOA, GPIOB, AFIO */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	
    /* PA1: �Ҽ�    PB8: ���    PB12: �¼� */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_12;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
    /* �ж�����, ��������½��ð����ж�, ����˯��ģʽǰ����, �Ա㰴�����Խ�ϵͳ��˯���л��� */
	EXTI_InitStructure.EXTI_Line = EXTI_Line1 | EXTI_Line8 | EXTI_Line12;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
	EXTI_InitStructure.EXTI_LineCmd = DISABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource8);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource12);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = KEY_NVIC_PRIO;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = KEY_NVIC_PRIO;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = KEY_NVIC_PRIO;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/**
 * @brief  ���ð����жϺ���
 * @param  ������ ����˵��  
 * @param  [in] newStt ��Ҫ���õ��ж�״̬
 * @retval ��
 * @par ʹ��ȫ�ֱ��� \n
 * @note �� ִ��ʱ��: \n
 *       �� ��������: �������� \n
 *       �� �ɷ���: ���� \n
 *
 * @par  ע��:
 *       �� �� \n
 */
void SetKeyInt(u8 newStt)
{
    EXTI_InitTypeDef EXTI_InitStructure;

	EXTI_InitStructure.EXTI_Line = EXTI_Line1 | EXTI_Line8 | EXTI_Line12;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
	EXTI_InitStructure.EXTI_LineCmd =(newStt ? ENABLE : DISABLE);
	EXTI_Init(&EXTI_InitStructure);
}

/**
 * @brief  ����ɨ�躯��
 * @param  ������ ����˵��  
 * @param  [in] key �����ṹ��ָ��
 * @retval ��
 * @par ʹ��ȫ�ֱ��� \n
 * @note �� ִ��ʱ��: \n
 *       �� ��������: 1ms \n
 *       �� �ɷ���: ���� \n
 *
 * @par  ע��:
 *       �� �� \n
 */
void KeyScan(STR_Key *key)
{
    /* �����˲�������, �������������� */
    static u16 fltCnt = 0, longCnt = 0;
    static u8 lastKeyPress = 0; 
    u8 keyPress = 0;
    
    if(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1))            // �Ҽ�
    {
        keyPress = 2;
    }
    else if(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8))       // ���
    {
        keyPress = 1;
    }
    else if(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12))      // �¼�
    {
        keyPress = 4;
    }
    
    /* �а������� */
    if (keyPress)
    {
        /* ���ϴΰ��������ͬ */
        if (keyPress == lastKeyPress)
        {
            /* ���� KEY_DN_FLT ms, ȷ�ϰ������� */
            if (fltCnt >= KEY_DN_FLT)
            {
                key->keyPressing.all = keyPress;  
                if (longCnt <= KEY_LONG)
                {
                    longCnt++;
                }
            }
            else
            {
                fltCnt++;
            }
        }
        else
        {
            longCnt = 0;
            fltCnt = 0;
        }
    }
    /* �ް������� */
    else
    {
        /* �ϴΰ������Ҳ�ް������� */
        if (keyPress == lastKeyPress)
        {
            /* ���� KEY_UP_FLT ms, ȷ�ϰ������� */
            if (fltCnt >= KEY_UP_FLT)
            {
                /* �ϴ��а�������, �൱���жϰ������½��� */
                if (key->keyPressing.all)
                {
                    key->keyPressed.all = key->keyPressing.all;
                    if (longCnt > KEY_LONG)
                    {
                        key->keyLongPressed.all = key->keyPressing.all;
                        longCnt = 0;
                    }
                    key->keyPressing.all = 0;
                }
                else
                {
                    key->keyPressed.all = 0;
                    key->keyLongPressed.all = 0;
                }
            }
            else
            {
                fltCnt++;
            }
        }
        else
        {
            fltCnt = 0;
        }
    }
    
    lastKeyPress = keyPress;
}

/**
 * @brief  1ͨ���ⲿ�жϴ�����
 * @param  ������ ����˵��  
 * @param  ��  
 * @retval ��
 * @par ʹ��ȫ�ֱ��� \n
 * @note �� ִ��ʱ��: \n
 *       �� ��������: �������� \n
 *       �� �ɷ���: ������ \n
 *
 * @par  ע��:
 *       �� 1ͨ�������Ҽ��жϣ�����˯��ǰ���������ڰ�����ϵͳ��˯���л��� \n
 */
void EXTI1_IRQHandler(void)
{
	//������
	if(EXTI_GetITStatus(EXTI_Line1) != RESET)
	{  
		EXTI_ClearITPendingBit(EXTI_Line1);
	}

}

/**
 * @brief  5~9ͨ���ⲿ�жϴ�����
 * @param  ������ ����˵��  
 * @param  ��  
 * @retval ��
 * @par ʹ��ȫ�ֱ��� \n
 * @note �� ִ��ʱ��: \n
 *       �� ��������: �������� \n
 *       �� �ɷ���: ������ \n
 *
 * @par  ע��:
 *       �� 8ͨ����������жϣ�����˯��ǰ���������ڰ�����ϵͳ��˯���л��� \n
 */
void EXTI9_5_IRQHandler(void)
{	
	//������
	if(EXTI_GetITStatus(EXTI_Line8) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line8);
	}
}

/**  @} */


