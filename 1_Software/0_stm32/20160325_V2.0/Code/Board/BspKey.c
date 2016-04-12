/**
*************************************************************************************************
* @file    BspKey.c
* @author  Ge Yue
* @version V1.0.0
* @date    2016.03.19
* @brief   按键驱动
*************************************************************************************************
@verbatim 

- 版本号       日期      创建人        描述 
  V1.0      2016.03.19   Ge Yue      按键驱动; 

- 版本号       日期      修改人        描述 
@endverbatim 
*/

/**
* @defgroup KEY BspKey: BSP Group
* @{
*/

#include "BspKey.h"
#include "Bsp.h"

/** @brief 按键状态结构体 */
STR_Key KeyStt;

/**
 * @brief  按键初始化函数
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
void KeyInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
    /* 使能GPIOA, GPIOB, AFIO */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	
    /* PA1: 右键    PB8: 左键    PB12: 下键 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_12;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
    /* 中断配置, 正常情况下禁用按键中断, 进入睡眠模式前启用, 以便按键可以将系统从睡眠中唤醒 */
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
 * @brief  设置按键中断函数
 * @param  参数名 参数说明  
 * @param  [in] newStt 需要设置的中断状态
 * @retval 无
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 触发调用 \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 无 \n
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
 * @brief  按键扫描函数
 * @param  参数名 参数说明  
 * @param  [in] key 按键结构体指针
 * @retval 无
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 1ms \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 无 \n
 */
void KeyScan(STR_Key *key)
{
    /* 按键滤波计数器, 按键长按计数器 */
    static u16 fltCnt = 0, longCnt = 0;
    static u8 lastKeyPress = 0; 
    u8 keyPress = 0;
    
    if(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1))            // 右键
    {
        keyPress = 2;
    }
    else if(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8))       // 左键
    {
        keyPress = 1;
    }
    else if(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12))      // 下键
    {
        keyPress = 4;
    }
    
    /* 有按键按下 */
    if (keyPress)
    {
        /* 与上次按键检测相同 */
        if (keyPress == lastKeyPress)
        {
            /* 超过 KEY_DN_FLT ms, 确认按键按下 */
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
    /* 无按键按下 */
    else
    {
        /* 上次按键检测也无按键按下 */
        if (keyPress == lastKeyPress)
        {
            /* 超过 KEY_UP_FLT ms, 确认按键弹起 */
            if (fltCnt >= KEY_UP_FLT)
            {
                /* 上次有按键按下, 相当于判断按键的下降沿 */
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
 * @brief  1通道外部中断处理函数
 * @param  参数名 参数说明  
 * @param  无  
 * @retval 无
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 触发调用 \n
 *       ● 可否打断: 不可以 \n
 *
 * @par  注意:
 *       ● 1通道用于右键中断，仅在睡眠前开启，用于按键将系统从睡眠中唤醒 \n
 */
void EXTI1_IRQHandler(void)
{
	//按键右
	if(EXTI_GetITStatus(EXTI_Line1) != RESET)
	{  
		EXTI_ClearITPendingBit(EXTI_Line1);
	}

}

/**
 * @brief  5~9通道外部中断处理函数
 * @param  参数名 参数说明  
 * @param  无  
 * @retval 无
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 触发调用 \n
 *       ● 可否打断: 不可以 \n
 *
 * @par  注意:
 *       ● 8通道用于左键中断，仅在睡眠前开启，用于按键将系统从睡眠中唤醒 \n
 */
void EXTI9_5_IRQHandler(void)
{	
	//按键左
	if(EXTI_GetITStatus(EXTI_Line8) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line8);
	}
}

/**  @} */


