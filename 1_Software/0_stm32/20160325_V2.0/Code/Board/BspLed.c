/**
*************************************************************************************************
* @file    BspLed.c
* @author  Ge Yue
* @version V1.0.0
* @date    2016.03.21
* @brief   Led驱动
*************************************************************************************************
@verbatim 

- 版本号       日期      创建人        描述 
  V1.0      2016.03.21   Ge Yue       Led驱动; 

- 版本号       日期      修改人        描述 
@endverbatim 
*/

/**
* @defgroup LED BspLed: BSP Group
* @{
*/

#include "BspLed.h"
#include "Bsp.h"

/** @brief LED结构体 */
STR_Led Led = {0,};

/**
 * @brief  Led初始化函数
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
void LedInit(void)
{
    PwmInit();
}

/**
 * @brief  设置照明模式函数
 * @param  参数名 参数说明  
 * @param  [in] light 亮度
 * @param  [in] delay 延时, 单位: 秒
 * @retval 无
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 500us \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 无 \n
 */
void SetLight(u16 light, u32 delay)
{
    if(light > 1000)    light = 1000;
    Led.light.pwmW = light;
    Led.light.delaySec = delay;
    Led.mode = LED_LIGHT; 
    if (Led.light.delayEn == 0)
    {
        if((delay > 0) && (light > 0))
        {
            //开始计时
            Led.light.delayEn = 1;
        }			
        else
        {
            Led.light.delayEn = 0;
        }
    }
}

/**
 * @brief  设置照明模式函数
 * @param  参数名 参数说明  
 * @param  [in] r 红光亮度
 * @param  [in] g 绿光亮度
 * @param  [in] b 蓝光亮度
 * @param  [in] breath 呼吸灯使能
 * @retval 无
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 500us \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 无 \n
 *
 */
void SetColor(u16 r, u16 g, u16 b, u8 breath)
{    
    if(r > 1000)    r = 1000;
    if(g > 1000)    g = 1000;
    if(b > 1000)    b = 1000;
    Led.color.pwmR = r;
    Led.color.pwmG = g;
    Led.color.pwmB = b;
    Led.color.breath = breath;
    Led.color.stepR = r >> 5;
    Led.color.stepG = g >> 5;
    Led.color.stepB = b >> 5;
    Led.mode = LED_COLOR;
}

/**
 * @brief  LED状态更新函数
 * @param  参数名 参数说明  
 * @param  [in] led LED结构体指针
 * @retval 无
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 20ms \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 无 \n
 */
void LedUpdStt(STR_Led *led)
{
    /* 计秒计数器, 呼吸灯计数器, 呼吸灯计数器方向 */
    static u8 secCnt = 0, breathCnt = 0, breathDir = 0;
    
    /* 用于呼吸灯 */
    if (!breathDir)
    {
        breathCnt++;
        if (128 == breathCnt)
        {
            breathDir = !breathDir;
        }
    }
    else
    {
        breathCnt--;
        if (0 == breathCnt)
        {
            breathDir = !breathDir;
        }
    }
    
    /* 用于计秒 */
    if (secCnt >= 50)
    {
        secCnt = 0;
    }
    else 
    {
        secCnt++;
    }
    
    if (led->lowPwr)
    {
        PwmSet(0, 0, (breathCnt >> 4));
    }
    else 
    {
        switch (led->mode)
        {
            case LED_LIGHT:
                /* 正在延时关灯计时 */
                if (led->light.delayEn)
                {
                    if (secCnt == 25)
                    {
                        led->light.delaySec--;
                        /* 计时时间到, 关灯 */
                        if ((s32)led->light.delaySec <= 0)
                        {
                            led->light.delayEn = 0;
                            led->light.delaySec = 0;
                            led->light.pwmW = 0;
                        }
                    }
                }
                breathCnt = 0;
                breathDir = 0;
                PwmSet(led->light.pwmW, led->light.pwmW, led->light.pwmW);
                break;
            case LED_COLOR:
                /* 呼吸灯使能 */
                if (led->color.breath)
                {
                    PwmSet(led->color.stepB * breathCnt >> 2, led->color.stepG * breathCnt >> 2, led->color.stepR * breathCnt >> 2);
                }
                else
                {
                    breathCnt = 0;
                    breathDir = 0;
                    PwmSet(led->color.pwmB, led->color.pwmG, led->color.pwmR);
                }
                break;
        }
    }
}

/**  @} */

