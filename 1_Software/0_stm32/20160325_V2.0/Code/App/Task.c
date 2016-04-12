/**
*************************************************************************************************
* @file    Task.c
* @author  Ge Yue
* @version V1.0.0
* @date    2016.03.22
* @brief   任务分派
*************************************************************************************************
@verbatim 

- 版本号       日期      创建人        描述 
  V1.0      2016.03.22   Ge Yue      任务分派; 

- 版本号       日期      修改人        描述 
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
 * @brief  1ms任务
 * @param  参数名 参数说明    
 * @param  无
 * @retval 无
 * @note ● 调用周期: 1ms调用\n *
 * @par  注意:
 *       ● \n
 */

static void Task1ms(void)
{     
    KeyScan(&KeyStt);           // 扫描按键
    KeyAction(&Gui, &KeyStt);   // 按键响应  
    BleSend();                  // 蓝牙发送数据
    if (Power.sysStt == NORMAL)
    {
        OledDisp();                 // OLED显示，通过DMA SPI将缓冲区的内容发送到OLED
    }
    if (KeyStt.keyPressing.all)
    {
        Sys.secLastKey = Sys.secSys;
    }
}   

 /**
 * @brief  4ms任务1
 * @param  参数名 参数说明    
 * @param  无
 * @retval 无
 * @note ● 调用周期: 4ms调用\n *
 * @par  注意:
 *       ● \n
 */
static void Task4ms_sub1(void)
{     
    RxUart(UartDecode);         // BLE接收处理数据
}

 /**
 * @brief  4ms任务2
 * @param  参数名 参数说明    
 * @param  无
 * @retval 无
 * @note ● 调用周期: 4ms调用\n *
 * @par  注意:
 *       ● \n
 */
static void Task4ms_sub2(void)
{
}

 /**
 * @brief  8ms任务1
 * @param  参数名 参数说明    
 * @param  无
 * @retval 无
 * @note ● 调用周期: 8ms调用\n *
 * @par  注意:
 *       ● \n
 */
static void Task8ms_sub1(void)
{     

}

 /**
 * @brief  8ms任务2
 * @param  参数名 参数说明    
 * @param  无
 * @retval 无
 * @note ● 调用周期: 8ms调用\n *
 * @par  注意:
 *       ● \n
 */
static void Task8ms_sub2(void)
{   
    if (Power.sysStt == NORMAL)
    {    
        GuiDataUpd(&Sys);
        Display(&Gui);              // 人机交互界面显示
    }
}

 /**
 * @brief  20ms任务
 * @param  参数名 参数说明    
 * @param  无
 * @retval 无
 * @note ● 调用周期: 20ms调用\n *
 * @par  注意:
 *       ● \n
 */
static void Task20ms(void)
{  
    LedUpdStt(&Led);            // LED控制
}

 /**
 * @brief  100ms任务
 * @param  参数名 参数说明    
 * @param  无
 * @retval 无
 * @note ● 调用周期: 100ms调用\n *
 * @par  注意:
 *       ● \n
 */
static void Task100ms(void)
{
    GetBatStt(&Bat);                // 更新电池状态
    ReadTimeDMA(&Rtc);              // 通过I2C读取RTC芯片    
    GetTimeDMA(&Rtc);               // 获取时间    
    SysSttUpd(&Sys);                // 更新系统状态
    PowerManage(&Power);            // 能耗管理
    
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
 * @brief  1s任务
 * @param  参数名 参数说明    
 * @param  无
 * @retval 无
 * @note ● 调用周期: 1s调用\n *
 * @par  注意:
 *       ● \n
 */
static void Task1s(void)
{   
    ReadTempDMA(&Temp);             // 通过I2C读取温度芯片
    GetTempDMA(&Temp);              // 获取温度
}

 /**
 * @brief  任务分派函数
 * @param  参数名 参数说明    
 * @param  无
 * @retval 无
 * @note ● 调用周期: 500us调用\n *
 * @par  注意:
 *       ● \n
 */
void Task(void)
{
    static u8 s4msReq = 0;   //用于触发4ms任务标志,每1ms累加
    static u8 s8msReq = 0;   //用于触发8ms任务标志,每4ms累加
    static u8 s20msReq = 0;  //用于触发20ms任务标志,每4ms累加
    static u8 s100msReq = 0; //用于触发100ms任务标志,每20ms累加
    static u8 s1sReq = 0;    //用于触发100ms任务标志,每100ms累加
        
    Task1ms();

    s4msReq++;
    switch (s4msReq)
    {
        case 1:     //触发4ms任务
        {
            Task4ms_sub1();
            break;
        }
        case 2:     //出发20ms\100ms\1s任务
        {
            s20msReq++;
            if (0x03 == s20msReq)    //两次20ms任务之间的中点时刻(5次时间片中间值为3次)
            {
                s100msReq++;
                if(0x03 == s100msReq)   //两次100ms任务之间的中点时刻(5次时间片中间值为3次)
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
        case 3:     //触发4ms任务
        {
            Task4ms_sub2();
            break;
        }
        default:    //触发2个8ms任务
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
