/**
*************************************************************************************************
* @file    Power.c
* @author  Ge Yue
* @version V1.0.0
* @date    2016.04.02
* @brief   能耗管理
*************************************************************************************************
@verbatim 

- 版本号       日期      创建人        描述 
  V1.0      2016.04.02   Ge Yue      能耗管理; 

- 版本号       日期      修改人        描述 
@endverbatim 
*/

/**
* @defgroup POWER Power: APP Group
* @{
*/

#include "Power.h"
#include "stm32_pwm.h"

/** @brief 能耗控制结构体 */
STR_Power Power;

/** @brief  睡眠前做的准备工作 */
static void PrepareSleep(void);
/** @brief  唤醒后需要做的工作 */
static void PrepareWake(void);

/**
 * @brief  能耗管理函数
 * @param  参数名 参数说明  
 * @param  [in] power 能耗管理结构体指针  
 * @retval 无
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 100ms \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 无 \n
 */
void PowerManage(STR_Power *power)
{        
    switch (power->sysStt)
    {
        case NORMAL:
            /* 正常 -> 关屏 */ 
            if (power->tarStt == OLEDOFF)
            {
                OledDisplayOff();
                power->sysStt = OLEDOFF;
            }
            /* 正常 -> 睡眠 */
            else if (power->tarStt == SLEEP)
            {
                OledDisplayOff();
                PrepareSleep();
                power->sysStt = SLEEP;
            }
            break;       
        case OLEDOFF:
            /* 关屏 -> 正常 */
            if (power->tarStt == NORMAL)
            {
                OledDisplayOn();
                power->sysStt = NORMAL;
            }
            /* 关屏 -> 睡眠 */
            else if (power->tarStt == SLEEP)
            {
                PrepareSleep();
                power->sysStt = SLEEP;
            }
            break;      
        case SLEEP:
            /* 睡眠 -> 正常 */
            if (power->tarStt == NORMAL)
            {
                PrepareWake();
                OledDisplayOn();
                power->sysStt = NORMAL;
            }
            /* 睡眠 -> 关屏 */
            else if (power->tarStt == OLEDOFF)
            {
                PrepareWake();
                power->sysStt = OLEDOFF;
            }
            break;
    }    
}

/**
 * @brief  睡眠前做的准备工作
 * @param  参数名 参数说明  
 * @param  无  
 * @retval 无
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 触发调用 \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 无 \n
 */
static void PrepareSleep(void)
{
	/* 开启按键中断 */
	SetKeyInt(1);
    
	/* 关闭所有PWM */
	PwmSet(0,0,0);
	
	/* 进入STOP_MODE */
	Sleep();
}

/**
 * @brief  唤醒后需要做的工作
 * @param  参数名 参数说明  
 * @param  无  
 * @retval 无
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 触发调用 \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 无 \n
 */
static void PrepareWake(void)
{    
    //关闭按键中断
	SetKeyInt(0);
}

 /**  @} */
