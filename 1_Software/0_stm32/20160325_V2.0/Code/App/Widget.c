/**
*************************************************************************************************
* @file    Widget.c
* @author  Ge Yue
* @version V1.0.0
* @date    2016.03.25
* @brief   人机显示控件
*************************************************************************************************
@verbatim 

- 版本号       日期      创建人        描述 
  V1.0      2016.03.25   Ge Yue      人机显示控件; 

- 版本号       日期      修改人        描述 
@endverbatim 
*/

/**
* @defgroup WIDGET Widget: APP Group
* @{
*/

#include "Widget.h" 
#include "string.h"

/** @brief 显示字符串缓存数组 */
static char tempShow[30], num;
/** @brief 将数字转换为可供显示的字符串 */
static u8 num2string (char *str,u32 num,char dot);
/** @brief 大时钟样式显示 */
static void DispClockBig(STR_Clock *clock, int xOffset);
/** @brief 日期样式显示 */
static void DispClockDat(STR_Clock *clock, int xOffset);
/** @brief 时间日期设置样式显示 */
static void DispClockSet(STR_Clock *clock, int xOffset);
/** @brief 时间日期样式显示 */
static void DispClockFul(STR_Clock *clock, int xOffset);

/**
 * @brief  初始化时钟控件
 * @param  参数名 参数说明  
 * @param  [in] x x轴坐标
 * @param  [in] y y轴坐标
 * @param  [in] style 显示样式
 * @param  [in] time 时间结构体指针
 * @param  [out] clock 时钟控件指针
 * @retval 无
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 触发调用, 初始化时调用  \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 无 \n
 */
void InitClock(u8 x, u8 y, u8 style, STR_Time *time, STR_Clock *clock)
{
    clock->x = x;
    clock->y = y;
    clock->style = style;
    clock->time = time;
}

/**
 * @brief  显示时钟控件
 * @param  参数名 参数说明  
 * @param  [in] clock 时钟控件指针
 * @param  [in] xOffset x轴偏移, 页面滑入滑出时用
 * @retval 无
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 触发调用  \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 无 \n
 */
void DispClock(STR_Clock *clock, int xOffset)
{    
    switch (clock->style)
    {
        case CLOCK_BIG:
            DispClockBig(clock, xOffset);
            break;
        case CLOCK_DAT:
            DispClockDat(clock, xOffset);
            break;
        case CLOCK_SET:
            DispClockSet(clock, xOffset);
            break;
        case CLOCK_FUL:
            DispClockFul(clock, xOffset);
            break;
    } 
}

/**
 * @brief  初始化图片组控件
 * @param  参数名 参数说明  
 * @param  [in] x x轴坐标
 * @param  [in] y y轴坐标
 * @param  [in] width 宽
 * @param  [in] height 高
 * @param  [in] picBase 图片指针
 * @param  [out] sttPic 图片组控件指针
 * @retval 无
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 触发调用, 初始化时调用  \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 无 \n
 */
void InitSttPic(u8 x, u8 y, u8 width, u8 height, u8 *picBase, STR_SttPic *sttPic)
{
    sttPic->x = x;
    sttPic->y = y;
    sttPic->width = width;
    sttPic->height = height;
    sttPic->picBase = picBase;
}

/**
 * @brief  图片组控件
 * @param  参数名 参数说明  
 * @param  [in] sttPic 控件指针
 * @param  [in] xOffset x轴偏移, 页面滑入滑出时用
 * @retval 无
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 触发调用  \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 该控件是指某个系统状态，不同的状态对应显示不同的图片，如电池电量、蓝牙状态、延时关灯状态等 \n
 */
void DispSttPic(STR_SttPic *sttPic, int xOffset)
{
    if (sttPic->stt != sttPic->preStt || sttPic->upd)
    {
        OledDisplay(sttPic->x, sttPic->y, sttPic->width, sttPic->height, &sttPic->picBase[sttPic->stt * sttPic->height * sttPic->width], xOffset, 0);
        sttPic->preStt = sttPic->stt;
        sttPic->upd = 0;
    }
}

/**
 * @brief  初始化数值控件
 * @param  参数名 参数说明  
 * @param  [in] x x轴坐标
 * @param  [in] y y轴坐标
 * @param  [in] dot 小数点位数
 * @param  [in] str 单位字符串
 * @param  [out] value 数值控件指针
 * @retval 无
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 触发调用, 初始化时调用  \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 无 \n
 */
void InitValue(u8 x, u8 y, u8 dot, char *str, STR_Value *value)
{
    value->x = x;
    value->y = y;
    value->dot = dot;
    value->str[0] = str[0];
    value->str[1] = str[1];
    value->str[2] = str[2];
    value->str[3] = str[3];
}

/**
 * @brief  数值显示控件
 * @param  参数名 参数说明  
 * @param  [in] value 数值控件指针
 * @param  [in] xOffset x轴偏移, 页面滑入滑出时用
 * @retval 无
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 触发调用  \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 无 \n
 */
void DispValue(STR_Value *value, int xOffset)
{
    if (value->val != value->preVal || value->focus != value->preFocus || value->upd)
    {
        OledShowString(value->x, value->y, "          ", xOffset, value->focus);
        if (value->val < 0)
        {            
            tempShow[0] = '-';
            num = num2string(&tempShow[1], -value->val, value->dot);
            tempShow[num + 1] = value->str[0];
            tempShow[num + 2] = value->str[1];
            tempShow[num + 3] = value->str[2];
            tempShow[num + 4] = value->str[3];
            OledShowString(value->x, value->y, tempShow, xOffset, value->focus);
        }
        else
        {
            num = num2string(tempShow, value->val, value->dot);
            tempShow[num] = value->str[0];
            tempShow[num + 1] = value->str[1];
            tempShow[num + 2] = value->str[2];
            tempShow[num + 3] = value->str[3];
            OledShowString(value->x, value->y, tempShow, xOffset, value->focus);
        }
        value->preVal = value->val;
        value->upd = 0;
    }
}

/**
 * @brief  初始化进度条控件
 * @param  参数名 参数说明  
 * @param  [in] x x轴坐标
 * @param  [in] y y轴坐标
 * @param  [in] len 进度条长度
 * @param  [in] str 单位字符串
 * @param  [out] slider 进度条控件指针
 * @retval 无
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 触发调用, 初始化时调用  \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 无 \n
 */
void InitSlider(u8 x, u8 y, u8 len, char *str, STR_Slider *slider)
{
    slider->x = x;
    slider->y = y;
    slider->len = len;
    memcpy(slider->str, str, strlen(str));
}

/**
 * @brief  进度条显示控件
 * @param  参数名 参数说明  
 * @param  [in] slider 进度条控件指针
 * @param  [in] xOffset x轴偏移, 页面滑入滑出时用
 * @retval 无
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 触发调用  \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 无 \n
 */
void DispSlid(STR_Slider *slider, int xOffset)
{
    if (slider->pos != slider->prePos || slider->focus != slider->preFocus || slider->upd)
    {
        u8 xBlk;        // 滑块x坐标               
        u8 Blk[2];      // 滑块显示 

        Blk[0] = 0xff;
        Blk[1] = 0xff;        
        if (slider->pos > slider->len)
        {
            slider->pos = slider->len;
        }
        xBlk = slider->x + slider->pos; 
        OledDisplay(slider->x, slider->y, slider->len, 1, (u8*)&fontSlide, xOffset, slider->focus);   // 滑动轴 
        OledDisplay(xBlk, slider->y, 2, 1, Blk, xOffset, slider->focus);          // 滑块
        
        num = num2string(tempShow, slider->val, 0);
        memcpy(&tempShow[num], slider->str, strlen((char *)&slider->str));
        OledShowString(slider->x + slider->len + 8, slider->y, tempShow, xOffset, 0);
        
        slider->prePos = slider->pos;
        slider->preFocus = slider->focus;
        slider->upd = 0;
    } 
}

/**
 * @brief  初始化静态文本控件
 * @param  参数名 参数说明  
 * @param  [in] x x轴坐标
 * @param  [in] y y轴坐标
 * @param  [in] str 文本字符串
 * @param  [out] text 静态文本控件指针
 * @retval 无
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 触发调用, 初始化时调用  \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 无 \n
 */
void InitText(u8 x, u8 y, char *str, STR_Text *text)
{
    text->x = x;
    text->y = y;
    text->str = str;
}

/**
 * @brief  静态文本显示控件
 * @param  参数名 参数说明  
 * @param  [in] text 静态文本控件指针
 * @param  [in] xOffset x轴偏移, 页面滑入滑出时用
 * @retval 无
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 触发调用  \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 无 \n
 */
void DispText(STR_Text *text, int xOffset)
{
    if (text->focus != text->preFocus || text->upd)
    {
        OledShowString(text->x, text->y, text->str, xOffset, text->focus);
        
        text->preFocus = text->focus;
        text->upd = 0;
    }
}

/**
 * @brief  数字转换为字符串函数
 * @param  参数名 参数说明  
 * @param  [out] str 存放字符串的指针
 * @param  [in] num 要转换的数字
 * @param  [in] dot 小数点位数
 * @retval 字符串中'\0'所在位置的偏移
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 触发调用  \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 无 \n
 */
static u8 num2string (char *str, u32 num, char dot)
{
	char weishu = 0;
	u32 temp = 0;
	char i = 0, j = 0;
	if(!num)
	{
		str[0] = '0';
		return 1;
	}
	else
	{
		//计算num是几位数	
		temp = num;
		while (temp)
		{
			temp /= 10;
			weishu++;
		}
		temp = num;
		if (weishu <= dot)
		{
			str[0] = '0';
			str[1] = '.';
			for (i = 0; i < (dot - weishu); i++)
				str[2 + i] = '0';
			for (j = weishu; j > 0; j--)
			{	
				str[2 + i + j - 1] = (temp % 10) + 48;
				temp /= 10;
			}
			return (dot + 2);
		}
		else
		{
			if (dot)
			{
				for (i = weishu + 1;i > 0;i--)
				{
					if ((weishu + 1 - i) < dot)
					{
						str[i - 1] = (temp % 10) + 48;
						temp /= 10;
					}
					else if ((weishu + 1 - i) == dot)
					{
						str[i - 1] = '.';
					}
					else
					{
						str[i - 1] = (temp % 10) + 48;
						temp /= 10;
					}
				}
				return (weishu + 1);
			}
			else
			{
				for (i = weishu; i > 0; i--)
				{
					str[i - 1] = (temp % 10) + 48;
					temp /= 10;
				}
				return weishu;	
			}	
		}
	}	
}

/**
 * @brief  显示首页大时钟样式
 * @param  参数名 参数说明  
 * @param  [in] clock 时钟控件指针
 * @param  [in] xOffset x轴偏移, 页面滑入滑出时用
 * @retval 无
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 触发调用  \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 显示样式: 23:11, 占据128列，6行 \n
 */
static void DispClockBig(STR_Clock *clock, int xOffset)
{
    u8 tmp[4] = {0,}, preTmp[4] = {0,}, sec;
    tmp[0] = clock->time->hour / 10;
	tmp[1] = clock->time->hour % 10;
	tmp[2] = clock->time->min / 10;
	tmp[3] = clock->time->min % 10;
    preTmp[0] = clock->preTime.hour / 10;
	preTmp[1] = clock->preTime.hour % 10;
	preTmp[2] = clock->preTime.min / 10;
	preTmp[3] = clock->preTime.min % 10;
    sec = clock->time->sec;
    
    if (clock->time->sec != clock->preTime.sec || clock->upd)
    {       
        OledDisplay(56, clock->y, 16, 6, (u8*)(&fontTimeColon[(clock->time->sec & 0x01) * 6][0]), xOffset, 0);
        clock->preTime.sec = sec;
    }
    if (tmp[0] != preTmp[0] || clock->upd)
    {
        OledDisplay(  0, clock->y, 28, 6, (u8*)(&fontTimeNum[tmp[0]*6][0]), xOffset, 0);
        clock->preTime.hour = clock->time->hour;
    }
    if (tmp[1] != preTmp[1] || clock->upd)
    {
        OledDisplay( 28, clock->y, 28, 6, (u8*)(&fontTimeNum[tmp[1]*6][0]), xOffset, 0);
        clock->preTime.hour = clock->time->hour;
    }
    if (tmp[2] != preTmp[2] || clock->upd)
    {
        OledDisplay( 72, clock->y, 28, 6, (u8*)(&fontTimeNum[tmp[2]*6][0]), xOffset, 0);
        clock->preTime.min = clock->time->min;
    }
    if (tmp[3] != preTmp[3] || clock->upd)
    {
        OledDisplay(100, clock->y, 28, 6, (u8*)(&fontTimeNum[tmp[3]*6][0]), xOffset, 0);
        clock->preTime.min = clock->time->min;
    }   
    clock->upd = 0;
}

/**
 * @brief  显示日期样式
 * @param  参数名 参数说明  
 * @param  [in] clock 时钟控件指针
 * @param  [in] xOffset x轴偏移, 页面滑入滑出时用
 * @retval 无
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 触发调用  \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 显示样式: 04-01 \n
 */
static void DispClockDat(STR_Clock *clock, int xOffset)
{
    u8 month = clock->time->month, date = clock->time->date;
    if (month != clock->preTime.month || date != clock->preTime.date || clock->upd)
    {
        GetDateString(clock->time, tempShow);
		OledShowString(clock->x, clock->y, tempShow, xOffset, 0);
        clock->preTime.month = month;
        clock->preTime.date = date;
        clock->upd = 0;
    }  
}

/**
 * @brief  显示时间设置样式
 * @param  参数名 参数说明  
 * @param  [in] clock 时钟控件指针
 * @param  [in] xOffset x轴偏移, 页面滑入滑出时用
 * @retval 无
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 触发调用  \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 显示样式: 2015 - 04 - 04 \n   
 *                     23 : 28 : 01  \n 
 */
static void DispClockSet(STR_Clock *clock, int xOffset)
{
    u8 tmp[5], focus = 0, preFocus = 0;
    
    focus = ((clock->focus == FOCUS_YEAR) ? 1 : 0);
    preFocus = ((clock->preFocus == FOCUS_YEAR) ? 1 : 0);
    if (clock->time->year != clock->preTime.year ||  focus != preFocus || clock->upd)
    {
        tmp[0] = '2';
        tmp[1] = '0';
        tmp[2] = '0' + clock->time->year / 10;
        tmp[3] = '0' + clock->time->year % 10;
        tmp[4] = '\0';
        OledShowString(clock->x, clock->y, (char *)tmp, xOffset, focus);
        
        clock->preTime.year = clock->time->year;
    }
    
    focus = ((clock->focus == FOCUS_MONTH) ? 1 : 0);
    preFocus = ((clock->preFocus == FOCUS_MONTH) ? 1 : 0);
    if (clock->time->month != clock->preTime.month ||  focus != preFocus || clock->upd)
    {
        tmp[0] = '0' + clock->time->month / 10;
        tmp[1] = '0' + clock->time->month % 10;
        tmp[2] = '\0';
        OledShowString(clock->x + 42, clock->y, (char *)tmp, xOffset, focus);
        
        clock->preTime.month = clock->time->month;
    }
    
    focus = ((clock->focus == FOCUS_DATE) ? 1 : 0);
    preFocus = ((clock->preFocus == FOCUS_DATE) ? 1 : 0);
    if (clock->time->date != clock->preTime.date ||  focus != preFocus || clock->upd)
    {
        tmp[0] = '0' + clock->time->date / 10;
        tmp[1] = '0' + clock->time->date % 10;
        tmp[2] = '\0';
        OledShowString(clock->x + 72, clock->y, (char *)tmp, xOffset, focus);
        
        clock->preTime.date = clock->time->date;
    }
    
    focus = ((clock->focus == FOCUS_HOUR) ? 1 : 0);
    preFocus = ((clock->preFocus == FOCUS_HOUR) ? 1 : 0);
    if (clock->time->hour != clock->preTime.hour ||  focus != preFocus || clock->upd)
    {
        tmp[0] = '0' + clock->time->hour / 10;
        tmp[1] = '0' + clock->time->hour % 10;
        tmp[2] = '\0';
        OledShowString(clock->x + 6, clock->y + 1, (char *)tmp, xOffset, focus);
        
        clock->preTime.hour = clock->time->hour;
    }
    
    focus = ((clock->focus == FOCUS_MIN) ? 1 : 0);
    preFocus = ((clock->preFocus == FOCUS_MIN) ? 1 : 0);
    if (clock->time->min != clock->preTime.min ||  focus != preFocus || clock->upd)
    {
        tmp[0] = '0' + clock->time->min / 10;
        tmp[1] = '0' + clock->time->min % 10;
        tmp[2] = '\0';
        OledShowString(clock->x + 36, clock->y + 1, (char *)tmp, xOffset, focus);
        
        clock->preTime.min = clock->time->min;
    }
    
    focus = ((clock->focus == FOCUS_SEC) ? 1 : 0);
    preFocus = ((clock->preFocus == FOCUS_SEC) ? 1 : 0);
    if (clock->time->sec != clock->preTime.sec ||  focus != preFocus || clock->upd)
    {
        tmp[0] = '0' + clock->time->sec / 10;
        tmp[1] = '0' + clock->time->sec % 10;
        tmp[2] = '\0';
        OledShowString(clock->x + 66, clock->y + 1, (char *)tmp, xOffset, focus);
        
        clock->preTime.sec = clock->time->sec;
    }
    
    if (clock->upd)
    {
        tmp[0] = '-';
        tmp[1] = '\0';
        OledShowString(clock->x + 30, clock->y, (char *)tmp, xOffset, 0);
        OledShowString(clock->x + 60, clock->y, (char *)tmp, xOffset, 0);
        tmp[0] = ':';
        tmp[1] = '\0';
        OledShowString(clock->x + 24, clock->y + 1, (char *)tmp, xOffset, 0);
        OledShowString(clock->x + 54, clock->y + 1, (char *)tmp, xOffset, 0);
    }
    
    clock->preFocus = clock->focus;
    clock->upd = 0;
}

/**
 * @brief  显示完整时间日期样式
 * @param  参数名 参数说明  
 * @param  [in] clock 时钟控件指针
 * @param  [in] xOffset x轴偏移, 页面滑入滑出时用
 * @retval 无
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 触发调用  \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 显示样式: 2015-04-04 23:28:01  \n 
 */
static void DispClockFul(STR_Clock *clock, int xOffset)
{
    u8 sec = clock->time->sec;
    if (sec != clock->preTime.sec || clock->upd)
    {
        GetTimeString(clock->time, tempShow);
		OledShowString(clock->x, clock->y, tempShow, xOffset, 0);
        clock->preTime.sec = sec;
        clock->upd = 0;
    }  
}

 /**  @} */

