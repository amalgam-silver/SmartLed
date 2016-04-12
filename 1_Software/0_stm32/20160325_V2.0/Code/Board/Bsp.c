/**
 *************************************************************************************************
 * @file    Bsp.c
 * @author  Ge Yue
 * @version V1.0.0
 * @date    2016.03.18
 * @brief   硬件板级接口
 *************************************************************************************************
 @verbatim 
 
  - 版本号       日期      创建人        描述 
    V1.0      2016.03.18   Ge Yue     硬件板级接口; 
 
  - 版本号       日期      修改人        描述 
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
 * @brief  硬件初始函数
 * @param  参数名 参数说明  
 * @param  无  
 * @retval 无
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 触发调用, 初始化时调用 \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 无 \n
 */
void SystemInit(void)
{
    /* 初始化系统时钟 */
    RccInit();
    SystickInit(8);
    
    /* 设置中断: 0bit用于抢占优先级, 4bit用于响应优先级, 即: 不允许用户中断嵌套 */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    
    /* 初始化定时器 */
    TimerInit();
    
    /* 初始化实时时钟 */
    RtcInit();
    
    /* 初始化温度传感器 */
    TempInit();
    
    /* 蓝牙初始化 */
    BleInit();
    
    /* 电池状态初始化 */
    BatInit();
    
    /* OLED初始化 */
    OledInit();
    
    /* Led初始化 */
    LedInit();
    
    /* 按键初始化 */   
    KeyInit();
}
 
 
 /**  @} */

