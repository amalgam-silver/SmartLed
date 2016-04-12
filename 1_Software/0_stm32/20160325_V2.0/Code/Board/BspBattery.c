/**
*************************************************************************************************
* @file    BspBattery.c
* @author  Ge Yue
* @version V1.0.0
* @date    2016.03.19
* @brief   通过ADC采集的电池电压估算电池剩余电量
*************************************************************************************************
@verbatim 

- 版本号       日期      创建人        描述 
  V1.0      2016.03.19   Ge Yue    通过ADC采集的电池电压估算电池剩余电量; 

- 版本号       日期      修改人        描述 
@endverbatim 
*/

/**
* @defgroup BETTERY BspBattery: BSP Group
* @{
*/

#include "BspBattery.h"
#include "Bsp.h"

/** @brief 电池信息结构体 */
STR_BATTERY Bat;

/** @brief 根据电池电压估算电池电量 */
static void GetBatLvl(STR_BATTERY *bat);

/**
 * @brief  电池检测初始化函数
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
void BatInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;  
    
    /* 初始化ADC, 采样电池电压 */
    ADCInit();
    
    /* 初始化充电检测引脚 PB3 和 充满检测引脚 PB4 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);       // 使能GPIOB
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);   // 禁用JTAG功能，将PB4作为普通引脚使用
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    /* 更新Bat结构体 */
    GetBatStt(&Bat);
}

/**
 * @brief  更新电池状态函数
 * @param  参数名 参数说明  
 * @param  [in] bat 电池状态结构体指针
 * @retval 无
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 100ms \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 无 \n
 */
void GetBatStt(STR_BATTERY *bat)
{
    u16 adcVal = 0, volTmp;
    
    /* 读取电池电压 */
    adcVal = GetAdcVal();   
    volTmp = (adcVal * 600) >> 12;
    bat->vol = (volTmp + bat->vol * 7) >> 3;
    /* 根据电池电压估算剩余电量 */
    GetBatLvl(bat);
    
    /* 读取充电状态 */
    if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3))	
    {
		bat->stt.bit.chrg = 1;
	}
    else
	{
        bat->stt.bit.chrg = 0;
    }
	
    /* 读取充满状态 */
	if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4))
    {
		bat->stt.bit.full = 1;
    }
	else
    {
		bat->stt.bit.full = 0;
    }
}

/**
 * @brief  根据电池电压估算电池电量
 * @param  参数名 参数说明  
 * @param  [in] bat 电池状态结构体指针
 * @retval 无
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 被GetBatStt调用, 100ms \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 无 \n
 */
static void GetBatLvl(STR_BATTERY *bat)
{
    // 根据电压分为7档
	// 0: 3.4V以下	
	// 1: 3.4V-3.55V		
    // 2: 3.55-3.65V	
    // 3: 3.65-3.75V
	// 4: 3.75-3.85V		
    // 5: 3.85-4.05	
	// 6: 4.05以上
	switch(Bat.lvl)
	{
		case 0:
			if(bat->vol > 355)
				bat->lvl++;
			break;
		case 1:
			if(bat->vol < 345)
				bat->lvl--;
			else if(bat->vol > 365)
				bat->lvl++;
			break;
		case 2:
			if(bat->vol < 355)
				bat->lvl--;
			else if(bat->vol > 375)
				bat->lvl++;
			break;
		case 3:
			if(bat->vol < 365)
				bat->lvl--;
			else if(bat->vol > 385)
				bat->lvl++;
			break;
		case 4:
			if(bat->vol < 375)
				bat->lvl--;
			else if(bat->vol > 395)
				bat->lvl++;
			break;
		case 5:
			if(bat->vol < 385)
				bat->lvl--;
			else if(bat->vol > 405)
				bat->lvl++;
			break;
		case 6:
			if(bat->vol < 395)
				bat->lvl--;
			break;	
	}
}

/**  @} */


