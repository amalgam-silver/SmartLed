/**
*************************************************************************************************
* @file    Task.c
* @author  Ge Yue
* @version V1.0.0
* @date    2016.03.22
* @brief   �������
*************************************************************************************************
@verbatim 

- �汾��       ����      ������        ���� 
  V1.0      2016.03.22   Ge Yue      �������; 

- �汾��       ����      �޸���        ���� 
@endverbatim 
*/

/**
* @defgroup TASK Task: APP Group
* @{
*/

#include "Manage.h"
#include "Bsp.h"
#include "HAL.h"

 /**
 * @brief  1ms����
 * @param  ������ ����˵��    
 * @param  ��
 * @retval ��
 * @note �� ��������: 1ms����\n *
 * @par  ע��:
 *       �� \n
 */

static void Task1ms(void)
{     
    KeyScan(&KeyStt);           // ɨ�谴��
    KeyAction(&Gui, &KeyStt);   // ������Ӧ  
    BleSend();                  // ������������
    if (Power.sysStt == NORMAL)
    {
        OledDisp();                 // OLED��ʾ��ͨ��DMA SPI�������������ݷ��͵�OLED
    }
    if (KeyStt.keyPressing.all)
    {
        Sys.secLastKey = Sys.secSys;
    }
}   

 /**
 * @brief  4ms����1
 * @param  ������ ����˵��    
 * @param  ��
 * @retval ��
 * @note �� ��������: 4ms����\n *
 * @par  ע��:
 *       �� \n
 */
static void Task4ms_sub1(void)
{     
    RxUart(UartDecode);         // BLE���մ�������
}

 /**
 * @brief  4ms����2
 * @param  ������ ����˵��    
 * @param  ��
 * @retval ��
 * @note �� ��������: 4ms����\n *
 * @par  ע��:
 *       �� \n
 */
static void Task4ms_sub2(void)
{
}

 /**
 * @brief  8ms����1
 * @param  ������ ����˵��    
 * @param  ��
 * @retval ��
 * @note �� ��������: 8ms����\n *
 * @par  ע��:
 *       �� \n
 */
static void Task8ms_sub1(void)
{     

}

 /**
 * @brief  8ms����2
 * @param  ������ ����˵��    
 * @param  ��
 * @retval ��
 * @note �� ��������: 8ms����\n *
 * @par  ע��:
 *       �� \n
 */
static void Task8ms_sub2(void)
{   
    if (Power.sysStt == NORMAL)
    {    
        GuiDataUpd(&Sys);
        Display(&Gui);              // �˻�����������ʾ
    }
}

 /**
 * @brief  20ms����
 * @param  ������ ����˵��    
 * @param  ��
 * @retval ��
 * @note �� ��������: 20ms����\n *
 * @par  ע��:
 *       �� \n
 */
static void Task20ms(void)
{  
    LedUpdStt(&Led);            // LED����
}

 /**
 * @brief  100ms����
 * @param  ������ ����˵��    
 * @param  ��
 * @retval ��
 * @note �� ��������: 100ms����\n *
 * @par  ע��:
 *       �� \n
 */
static void Task100ms(void)
{
    GetBatStt(&Bat);                // ���µ��״̬
    ReadTimeDMA(&Rtc);              // ͨ��I2C��ȡRTCоƬ    
    GetTimeDMA(&Rtc);               // ��ȡʱ��    
    SysSttUpd(&Sys);                // ����ϵͳ״̬
    PowerManage(&Power);            // �ܺĹ���
    
    if (Bat.lvl <= 1)
    {
        Led.lowPwr = 1;
    }
    else
    {
        Led.lowPwr = 0;
    }
}

 /**
 * @brief  1s����
 * @param  ������ ����˵��    
 * @param  ��
 * @retval ��
 * @note �� ��������: 1s����\n *
 * @par  ע��:
 *       �� \n
 */
static void Task1s(void)
{   
    ReadTempDMA(&Temp);             // ͨ��I2C��ȡ�¶�оƬ
    GetTempDMA(&Temp);              // ��ȡ�¶�
}

 /**
 * @brief  ������ɺ���
 * @param  ������ ����˵��    
 * @param  ��
 * @retval ��
 * @note �� ��������: 500us����\n *
 * @par  ע��:
 *       �� \n
 */
void Task(void)
{
    static u8 s4msReq = 0;   //���ڴ���4ms�����־,ÿ1ms�ۼ�
    static u8 s8msReq = 0;   //���ڴ���8ms�����־,ÿ4ms�ۼ�
    static u8 s20msReq = 0;  //���ڴ���20ms�����־,ÿ4ms�ۼ�
    static u8 s100msReq = 0; //���ڴ���100ms�����־,ÿ20ms�ۼ�
    static u8 s1sReq = 0;    //���ڴ���100ms�����־,ÿ100ms�ۼ�
        
    Task1ms();

    s4msReq++;
    switch (s4msReq)
    {
        case 1:     //����4ms����
        {
            Task4ms_sub1();
            break;
        }
        case 2:     //����20ms\100ms\1s����
        {
            s20msReq++;
            if (0x03 == s20msReq)    //����20ms����֮����е�ʱ��(5��ʱ��Ƭ�м�ֵΪ3��)
            {
                s100msReq++;
                if(0x03 == s100msReq)   //����100ms����֮����е�ʱ��(5��ʱ��Ƭ�м�ֵΪ3��)
                {
                    s1sReq++;
                    if(s1sReq >= 10)
                    {
                        Task1s();
                        s1sReq = 0x00;
                    }
                }
                
                if(s100msReq >= 5)
                {
                    Task100ms();
                    s100msReq = 0x00;
                }
            }
            if (s20msReq >= 5)
            {
                Task20ms();
                s20msReq = 0x00;
            }
            break;
        }
        case 3:     //����4ms����
        {
            Task4ms_sub2();
            break;
        }
        default:    //����2��8ms����
        {
            if (s8msReq++ & 0x01)
            {
                Task8ms_sub1();
            }
            else
            {
                Task8ms_sub2();
            }
            s4msReq = 0;
            break;
        }
    }
}

 /**  @} */
