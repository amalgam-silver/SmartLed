/**
*************************************************************************************************
* @file    BspBle.c
* @author  Ge Yue
* @version V1.0.0
* @date    2016.03.19
* @brief   ����ģ������
*************************************************************************************************
@verbatim 

- �汾��       ����      ������        ���� 
  V1.0      2016.03.19   Ge Yue     ����ģ������; 

- �汾��       ����      �޸���        ���� 
@endverbatim 
*/

/**
* @defgroup BLE BspBle: BSP Group
* @{
*/

#include "BspBle.h"
#include "Bsp.h"
#include "string.h"

/** @brief �͹��������ṹ�� */
STR_Ble Ble = {0,};

/**
 * @brief  �¶ȴ�������ʼ������
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
void BleInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	
	/* PB10 -> BT_CON   PB11 -> BT_SLEEP */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* PB0 -> BT_EN     PB1 -> BT_BRTS */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_WriteBit(GPIOB,GPIO_Pin_1,Bit_SET);
	GPIO_WriteBit(GPIOB,GPIO_Pin_0,Bit_RESET);
	Ble.en = ENABLE;
	
	/* �������������ж� */
	EXTI_InitStructure.EXTI_Line = EXTI_Line10;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;  
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource10);
    
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = BLE_NVIC_PRIO;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
    
    /* ��ʼ������ */
    UsartInit(9600);
}

/**
 * @brief  ��ʼ��UART��ʹ��uart1+DMA,��RTU��ʽ�շ�����
 * @param  ������ ����˵��
 * @param  [in] newState ��Ҫ�л���״̬ ENABLE �������� DISABLE �ر�����
 * @retval ��
 * @par ʹ��ȫ�ֱ���
 *         �� \n
 * @note �� ִ��ʱ��: \n
 *       �� ��������: �������� \n
 *       �� �ɷ���: ���� \n
 *
 * @par  ע��:
 *       �� ��
 *
 */
void BleCmd(u8 newState)
{
	if(newState == ENABLE)
	{
		GPIO_WriteBit(GPIOB,GPIO_Pin_0,Bit_RESET);
		Ble.en = ENABLE;
	}
	else
	{
		GPIO_WriteBit(GPIOB,GPIO_Pin_0,Bit_SET);
		Ble.en = DISABLE;
	}
}

/**
 * @brief  ����Ҫ���͵�����д�뻺����
 * @param  ������ ����˵��
 * @param  [in] *data ���͵�����ָ��
 * @param  [in] len ���͵����ݳ���
 * @retval ��
 * @par ʹ��ȫ�ֱ���
 *         �� \n
 * @note �� ִ��ʱ��: \n
 *       �� ��������: �������� \n
 *       �� �ɷ���: ���� \n
 *
 * @par  ע��:
 *       �� ��
 *
 */
u8 PushSendBuf(u8 *data, u8 len)
{
    /* ���ĳ��ȳ������� */
    if (len > BYTES_PER_MSG)
    {
        return 0;
    }
    
    /* ���������� */
    if (((Ble.txCurPtr + 15) & 0x0f) == Ble.txNeedPtr)
    {
        return 0;
    }
    
    memcpy(Ble.msgTx[Ble.txNeedPtr].data, data, len);
    Ble.msgTx[Ble.txNeedPtr].len = len;
    Ble.txNeedPtr++;
    Ble.txNeedPtr &= 0x0f;
    
    return 1;
}

/**
 * @brief  ��������״̬��
 * @param  ������ ����˵��  
 * @param  ��  
 * @retval ��
 * @par ʹ��ȫ�ֱ��� \n
 * @note �� ִ��ʱ��: \n
 *       �� ��������: 1ms \n
 *       �� �ɷ���: ���� \n
 *
 * @par  ע��:
 *       �� ���ڸ�����ģ���ڷ�������ǰ���Ƚ�����������������500us,������ɺ������ٵȴ�200us�ٽ������������ߣ��ʲ��ô�״̬��ȡ����ʱ�ȴ� \n
 */
void BleSend(void)
{
    switch (Ble.sendStt)
    {
        /* ����״̬ */
        case BLE_SEND_IDLE:
            /* �д����ͱ��� */
            if (Ble.txCurPtr != Ble.txNeedPtr)
            {
                /* �����������õ� */
                GPIO_WriteBit(GPIOB,GPIO_Pin_1,Bit_RESET);
                Ble.sendStt = BLE_SEND_START;
            }
            break;
        
        /* �ȴ����������õ� */
        case BLE_SEND_START:
            Uart.tPtr = Ble.msgTx[Ble.txCurPtr].len;
            Uart.txFlg = 0xff;
            memcpy(Uart.txBuf, Ble.msgTx[Ble.txCurPtr].data, Ble.msgTx[Ble.txCurPtr].len);
            TxUart();
            Ble.sendStt = BLE_SEND_PROC;
            break;
        
        /* ������ */
        case BLE_SEND_PROC:
            /* ������� */
            if (GetSendStt())
            {
                Ble.sendStt = BLE_SEND_END;
            }
            break;
        
        /* �������, �����������ø� */
        case BLE_SEND_END:
            GPIO_WriteBit(GPIOB,GPIO_Pin_1,Bit_SET);
            Ble.sendStt = BLE_SEND_IDLE;
            Ble.txCurPtr++;
            Ble.txCurPtr &= 0x0f;
            break;
    }  
}

/**
 * @brief  10~15ͨ���ⲿ�жϴ�����
 * @param  ������ ����˵��  
 * @param  ��  
 * @retval ��
 * @par ʹ��ȫ�ֱ��� \n
 * @note �� ִ��ʱ��: \n
 *       �� ��������: �������� \n
 *       �� �ɷ���: ������ \n
 *
 * @par  ע��:
 *       �� 12ͨ�������¼��жϣ�����˯��ǰ���������ڰ�����ϵͳ��˯���л��� \n
 *       �� 10ͨ��������������״̬�仯�� \n
 */
void EXTI15_10_IRQHandler(void)
{
    //������
	if(EXTI_GetITStatus(EXTI_Line12) != RESET)
	{	
		EXTI_ClearITPendingBit(EXTI_Line12);
	}
    
	//����connected�ź�������
	if(EXTI_GetITStatus(EXTI_Line10) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line10);
		//���ӶϿ�
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10) == 1)
		{
			Ble.conStt = 0;
            if (Ble.onDisconn)
            {
                Ble.onDisconn();
            }
		}
		//��������
		else
		{
			Ble.conStt = 1;
            if (Ble.onConn)
            {
                Ble.onConn();
            }
		}	
	}
}



/**  @} */

