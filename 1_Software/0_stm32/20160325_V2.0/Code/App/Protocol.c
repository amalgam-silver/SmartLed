/**
*************************************************************************************************
* @file    Protocol.c
* @author  Ge Yue
* @version V1.0.0
* @date    2016.03.27
* @brief   蓝牙协议解析
*************************************************************************************************
@verbatim 

- 版本号       日期      创建人        描述 
  V1.0      2016.03.27   Ge Yue      蓝牙协议解析; 

- 版本号       日期      修改人        描述 
@endverbatim 
*/

/**
* @defgroup PROTOCOL Protocol: APP Group
* @{
*/

#include "Protocol.h"
#include "Manage.h"

/** @brief 解析一帧报文 */
static u8 msgDec(u8 *rxBuf);

/**
 * @brief  串口数据解析函数
 * @param  参数名 参数说明  
 * @param  [in] uart 串口结构体指针
 * @retval 无
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 4ms \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 无 \n
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
 * @brief  解析一帧报文
 * @param  参数名 参数说明  
 * @param  [in] rxBuf 串口接收缓冲数据指针
 * @retval 该帧报文的长度
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 4ms, 被UartDecode调用 \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 无 \n
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
		//时间
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
		//关屏时间
		case 0x02:
			//写
			if(readOrWrite == 1)
			{
				Sys.secSleep = rxBuf[4];
			}
			//读
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
		//系统状态
		case 0x03:
            if (readOrWrite == 0)
            {
                data[0] = 0x7f;
                data[1] = 0x03;
                data[2] = 0x00;
                data[3] = 0x0C;
                data[4] = Bat.stt.bit.chrg;         // 是否充电中
                data[5] = Bat.vol - 200;            // 电池电压
                data[6] = Bat.lvl;                  // 电池电量等级
                data[7] = Led.light.delaySec / 60;  // 照明延时时间
                data[8] = SW_VERSION;               // 固件版本
                data[9] = Temp.val >> 8;            // 温度
                data[10] = Temp.val;                // 温度
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
			//写
			if(readOrWrite == 1)
			{				
                u8 delay = rxBuf[5];
                u8 mode = rxBuf[4];
                u16 pwmW = rxBuf[6] * 10;             // 照明亮度
                u8 breath = rxBuf[7];                 // 呼吸灯使能
                u16 pwmR = rxBuf[8] * 10;             // 红灯亮度
                u16 pwmG = rxBuf[9] * 10;             // 绿灯亮度
                u16 pwmB = rxBuf[10] * 10;            // 蓝灯亮度
               
                // 照明模式
                if (0 == mode)
                {
                    SetLight(pwmW, delay * 60);
                }
                // 彩灯模式
                else
                {
                    SetColor(pwmR, pwmG, pwmB, breath);
                }                
			}
			
            //读
			else if(readOrWrite == 0)
			{
				data[0] = 0x7f;
                data[1] = 0x04;
                data[2] = 0x00;
                data[3] = 0x0c;
                data[4] = Led.mode;                         // 模式
                data[5] = (Led.light.delaySec + 59) / 60;   // 延时时间
                data[6] = Led.light.pwmW / 10;                   // 照明亮度
                data[7] = Led.color.breath;                 // 呼吸灯
                data[8] = Led.color.pwmR / 10;                   // 红灯亮度
                data[9] = Led.color.pwmG / 10;                   // 绿灯亮度
                data[10] = Led.color.pwmB / 10;                  // 蓝灯亮度
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
