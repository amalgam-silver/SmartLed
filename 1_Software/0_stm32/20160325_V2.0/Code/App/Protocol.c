/**
*************************************************************************************************
* @file    Protocol.c
* @author  Ge Yue
* @version V1.0.0
* @date    2016.03.27
* @brief   ����Э�����
*************************************************************************************************
@verbatim 

- �汾��       ����      ������        ���� 
  V1.0      2016.03.27   Ge Yue      ����Э�����; 

- �汾��       ����      �޸���        ���� 
@endverbatim 
*/

/**
* @defgroup PROTOCOL Protocol: APP Group
* @{
*/

#include "Protocol.h"
#include "Manage.h"

/** @brief ����һ֡���� */
static u8 msgDec(u8 *rxBuf);

/**
 * @brief  �������ݽ�������
 * @param  ������ ����˵��  
 * @param  [in] uart ���ڽṹ��ָ��
 * @retval ��
 * @par ʹ��ȫ�ֱ��� \n
 * @note �� ִ��ʱ��: \n
 *       �� ��������: 4ms \n
 *       �� �ɷ���: ���� \n
 *
 * @par  ע��:
 *       �� �� \n
 */
void UartDecode(STR_Uart *uart)
{   
	u8 *p = uart->rxBuf;
    u8 len = 0, offset = 0;
    do
    {
        len = msgDec(p);
        offset += len;
        if (offset >= uart->rPtr)
        {
            break;
        }
        p = uart->rxBuf + len;
    }while(len);
    msgDec(uart->rxBuf);
    uart->rxFin = 0;
}

/**
 * @brief  ����һ֡����
 * @param  ������ ����˵��  
 * @param  [in] rxBuf ���ڽ��ջ�������ָ��
 * @retval ��֡���ĵĳ���
 * @par ʹ��ȫ�ֱ��� \n
 * @note �� ִ��ʱ��: \n
 *       �� ��������: 4ms, ��UartDecode���� \n
 *       �� �ɷ���: ���� \n
 *
 * @par  ע��:
 *       �� �� \n
 */
static u8 msgDec(u8 *rxBuf)
{
    u8 length = 0, command = 0, verify = 0, readOrWrite = 0;
	u8 i;
	STR_Time timeSetBLE;
    u8 data[20];
    extern u16 temperature;
	
	command = rxBuf[1];
	readOrWrite = rxBuf[2];
	length = rxBuf[3];
	
	for(i = 1; i < (length - 1); i++)
	{
		verify += rxBuf[i];
	}
	
	if(verify != rxBuf[length - 1])
		return 0;
	
	switch(command)
	{
		//ʱ��
		case 0x01:
			if(readOrWrite == 1)
			{
				timeSetBLE.year 	=	rxBuf[4];
				timeSetBLE.month 	= 	rxBuf[5];
				timeSetBLE.date 	=	rxBuf[6];
				timeSetBLE.hour 	=	rxBuf[7];
				timeSetBLE.min 	    = 	rxBuf[8];
				timeSetBLE.sec 	    = 	rxBuf[9];
				timeSetBLE.week     =	rxBuf[10];
				TimeSet(&timeSetBLE);
			}
			break;
		//����ʱ��
		case 0x02:
			//д
			if(readOrWrite == 1)
			{
				Sys.secSleep = rxBuf[4];
			}
			//��
			else if(readOrWrite == 0)
			{
				data[0] = 0x7f;
                data[1] = 0x02;
                data[2] = 0x00;
                data[3] = 0x06;
                data[4] = Sys.secSleep;
                data[5] = data[1] + data[2] + data[3] + data[4];
                PushSendBuf(data, 6);
			}
			break;
		//ϵͳ״̬
		case 0x03:
            if (readOrWrite == 0)
            {
                data[0] = 0x7f;
                data[1] = 0x03;
                data[2] = 0x00;
                data[3] = 0x0C;
                data[4] = Bat.stt.bit.chrg;         // �Ƿ�����
                data[5] = Bat.vol - 200;            // ��ص�ѹ
                data[6] = Bat.lvl;                  // ��ص����ȼ�
                data[7] = Led.light.delaySec / 60;  // ������ʱʱ��
                data[8] = SW_VERSION;               // �̼��汾
                data[9] = Temp.val >> 8;            // �¶�
                data[10] = Temp.val;                // �¶�
                data[11] = 0;
                for (i = 1; i < 11; i++)
                {
                    data[11] += data[i];
                }
                PushSendBuf(data, 12);
            }
			break;
		//LED
		case 0x04:
			//д
			if(readOrWrite == 1)
			{				
                u8 delay = rxBuf[5];
                u8 mode = rxBuf[4];
                u16 pwmW = rxBuf[6] * 10;             // ��������
                u8 breath = rxBuf[7];                 // ������ʹ��
                u16 pwmR = rxBuf[8] * 10;             // �������
                u16 pwmG = rxBuf[9] * 10;             // �̵�����
                u16 pwmB = rxBuf[10] * 10;            // ��������
               
                // ����ģʽ
                if (0 == mode)
                {
                    SetLight(pwmW, delay * 60);
                }
                // �ʵ�ģʽ
                else
                {
                    SetColor(pwmR, pwmG, pwmB, breath);
                }                
			}
			
            //��
			else if(readOrWrite == 0)
			{
				data[0] = 0x7f;
                data[1] = 0x04;
                data[2] = 0x00;
                data[3] = 0x0c;
                data[4] = Led.mode;                         // ģʽ
                data[5] = (Led.light.delaySec + 59) / 60;   // ��ʱʱ��
                data[6] = Led.light.pwmW / 10;                   // ��������
                data[7] = Led.color.breath;                 // ������
                data[8] = Led.color.pwmR / 10;                   // �������
                data[9] = Led.color.pwmG / 10;                   // �̵�����
                data[10] = Led.color.pwmB / 10;                  // ��������
                data[11] = 0;
                for (i = 1; i < 11; i++)
                {
                    data[11] += data[i];
                }
                PushSendBuf(data, 12);
			}
			break;
		
		default:
			break;
	}
    
    return length;
}

 /**  @} */
