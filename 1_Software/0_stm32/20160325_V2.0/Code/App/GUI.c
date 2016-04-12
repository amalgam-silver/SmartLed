/**
*************************************************************************************************
* @file    GUI.c
* @author  Ge Yue
* @version V1.0.0
* @date    2016.03.22
* @brief   人机交互界面
*************************************************************************************************
@verbatim 

- 版本号       日期      创建人        描述 
  V1.0      2016.03.22   Ge Yue    人机交互界面

- 版本号       日期      修改人        描述 
@endverbatim 
*/

/**
* @defgroup GUI GUI: APP Group
* @{
*/

#include "GUI.h"
#include "Manage.h"
#include "string.h"

/** @brief GUI显示结构体 */
STR_Gui Gui = {0,};
/** @brief 时间设置 */
static STR_Time timeSet = {0,};
/** @brief 之前的LED参数 */
static STR_Led preLedSet = {0,};

/** @brief  处理界面的滑动切换动画 */
static void Slip(STR_Gui *gui);
/** @brief  主界面显示 */
static void DispMain(STR_Gui *gui);
/** @brief  主界面按键响应 */
static void KeyMain(STR_Gui *gui, STR_Key *key);
/** @brief  LED菜单界面显示 */
static void DispLedMenu(STR_Gui *gui);
/** @brief  LED菜单界面按键响应 */
static void KeyLedMenu(STR_Gui *gui, STR_Key *key);
/** @brief  信息菜单界面显示 */
static void DispInfMenu(STR_Gui *gui);
/** @brief  信息菜单界面按键响应 */
static void KeyInfMenu(STR_Gui *gui, STR_Key *key);
/** @brief  设置菜单界面显示 */
static void DispSetMenu(STR_Gui *gui);
/** @brief  设置菜单界面按键响应 */
static void KeySetMenu(STR_Gui *gui, STR_Key *key);
/** @brief  LED界面显示 */
static void DispLed(STR_Gui *gui);
/** @brief  LED界面按键响应 */
static void KeyLed(STR_Gui *gui, STR_Key *key);
/** @brief  信息界面显示 */
static void DispInf(STR_Gui *gui);
/** @brief  信息界面按键响应 */
static void KeyInf(STR_Gui *gui, STR_Key *key);
/** @brief  设置界面显示 */
static void DispSet(STR_Gui *gui);
/** @brief  设置界面按键响应 */
static void KeySet(STR_Gui *gui, STR_Key *key);


/**
 * @brief  人机交互界面显示
 * @param  参数名 参数说明  
 * @param  [in] gui GUI结构体指针  
 * @retval 无
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 8ms \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 无 \n
 */
void Display(STR_Gui *gui)
{
    Slip(gui);
    switch (gui->stt)
    {
        case GUI_MAIN:        // 时间主界面
            DispMain(gui);
            break; 
        case GUI_LED_MENU:    // LED设置菜单界面
            DispLedMenu(gui);
            break; 
        case GUI_INF_MENU:    // 系统信息菜单界面
            DispInfMenu(gui);
            break; 
        case GUI_SET_MENU:    // 系统设置菜单界面
            DispSetMenu(gui);
            break; 
        case GUI_LED:         // LED设置界面
            DispLed(gui);
            break; 
        case GUI_INF:         // 系统信息界面
            DispInf(gui);
            break; 
        case GUI_SET:         // 系统设置界面
            DispSet(gui);
            break;
    }
}

/**
 * @brief  人机交互界面按键响应
 * @param  参数名 参数说明  
 * @param  [out] gui GUI结构体指针
 * @param  [in] key 按键状态结构体指针
 * @retval 无
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 1ms \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 无 \n
 */
void KeyAction(STR_Gui *gui, STR_Key *key)
{
    switch (gui->stt)
    {
        case GUI_MAIN:        // 时间主界面
            KeyMain(gui, key);
            break; 
        case GUI_LED_MENU:    // LED设置菜单界面
            KeyLedMenu(gui, key);
            break; 
        case GUI_INF_MENU:    // 系统信息菜单界面
            KeyInfMenu(gui, key);
            break; 
        case GUI_SET_MENU:    // 系统设置菜单界面
            KeySetMenu(gui, key);
            break; 
        case GUI_LED:         // LED设置界面
            KeyLed(gui, key);
            break; 
        case GUI_INF:         // 系统信息界面
            KeyInf(gui, key);
            break; 
        case GUI_SET:         // 系统设置界面
            KeySet(gui, key);
            break;
    }
}

/**
 * @brief  人机交互界面初始化函数
 * @param  参数名 参数说明  
 * @param  无
 * @retval 无
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 触发调用，初始化时调用 \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 无 \n
 */
void GuiInit(void)
{
    char tmp[4] = " C ";
    tmp[0] = 'z' + 2;
    /* 主界面 */
    InitSttPic( 86, 0, 15, 2, (u8*)&fontDelay, &Gui.main.dly);
    InitSttPic(106, 0, 10, 2, (u8*)&fontBLE, &Gui.main.ble);
    InitSttPic(118, 0, 10, 2, (u8*)&fontBattery, &Gui.main.bat);
    InitValue(0, 1, 2, tmp, &Gui.main.temp);
    InitClock(0, 2, CLOCK_BIG, &Rtc.time, &Gui.main.bigClock);
    InitClock(0, 0, CLOCK_DAT, &Rtc.time, &Gui.main.date);
    
    /* 设置界面 */
    InitText(8, 0, "Bluetooth", &Gui.setting.ble);
    InitText(0, 1, "Screen Off:", &Gui.setting.sleep);
    InitText(0, 3, "Time Set:", &Gui.setting.time);
    InitText(0, 7, "Save&Exit", &Gui.setting.save);
    InitText(100, 7, "Exit", &Gui.setting.exit);
    InitSttPic(0, 0, 8, 1, (u8*)&fontEnable, &Gui.setting.bleEn);
    InitSlider(0, 2, 73, "s  ", &Gui.setting.secSleep);
    InitClock(25, 4, CLOCK_SET, &timeSet, &Gui.setting.timeSet);
  
    /* 信息界面 */
    InitClock(0, 0, CLOCK_FUL, &Rtc.time, &Gui.info.time);
    InitValue(0, 1, 2, tmp, &Gui.info.temp);
    InitSttPic(118, 1, 10, 2, (u8*)&fontBattery, &Gui.info.bat);
    InitValue(0, 2, 2, "V  ", &Gui.info.batVol);
    InitText( 0, 3, "Charging:N", &Gui.info.batChrg);
    InitText(72, 3, "Full:N", &Gui.info.batFull);
    InitText( 0, 5, "BLE:Off,", &Gui.info.bleStt);
    InitText(54, 5, "Disconnected", &Gui.info.bleConn);
    InitText( 0, 7, "Led Delay: Off   ", &Gui.info.ledDly);
    InitValue(60, 7, 0, "s  ", &Gui.info.secDly);
    InitText(46, 6, STR_VERSION, &Gui.info.ver);
    InitText(40,7,"Designed by HG", &Gui.info.author);
    
    /* LED设置界面 */
    InitSttPic(0, 0, 8, 1, (u8*)&fontEnable, &Gui.led.ledMode);
    InitText(10, 0, "Color", &Gui.led.colorLed);
    InitText(0, 2, "Light:", &Gui.led.light);
    InitSlider(0, 3, 100, "   ", &Gui.led.pwmW);
    InitText(0, 4, "Delay:", &Gui.led.dly);
    InitSlider(0, 5, 60, "min   ", &Gui.led.minDly);
    InitSttPic(64, 0, 8, 1, (u8*)&fontEnable, &Gui.led.breathEn);
    InitText(74, 0, "Breath", &Gui.led.breath);
    InitText(0, 1, "Red:", &Gui.led.red);
    InitSlider(0, 2, 100, "   ", &Gui.led.pwmR);
    InitText(0, 3, "Green:", &Gui.led.green);
    InitSlider(0, 4, 100, "   ", &Gui.led.pwmG);
    InitText(0, 5, "Blue:", &Gui.led.blue);
    InitSlider(0, 6, 100, "   ", &Gui.led.pwmB);
    InitText(0, 7, "Save&Exit", &Gui.led.save);
    InitText(100, 7, "Exit", &Gui.led.exit);
}

/**
 * @brief  处理界面的滑动切换动画
 * @param  参数名 参数说明  
 * @param  [in] gui GUI结构体指针  
 * @retval 无
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 触发调用 \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 无 \n
 */
static void Slip(STR_Gui *gui)
{
    switch (gui->slip.stt)
    {
         /* 静止状态 */
         case SLIP_STANDSTILL:
            gui->slip.offset = 0;
            break; 
         
         /* 向左滑出 */
         case SLIP_LEFT_OUT:                 
            gui->slip.offset -= gui->slip.step; // 每次左移step个像素
            /* 完全移出屏幕后 */
            if (gui->slip.offset < -128)
            {                
                gui->stt = gui->slip.dest;      // 切换到目标界面 
                gui->slip.stt = SLIP_LEFT_IN;   // 新界面向左滑入                
                gui->slip.offset = 128;         // offset 从 128 -> 0
            }
            break; 
         
         /* 向右滑出 */   
         case SLIP_RIGHT_OUT:
            gui->slip.offset += gui->slip.step; // 每次右移step个像素
            /* 完全移出屏幕后 */
            if (gui->slip.offset > 128)
            {                
                gui->stt = gui->slip.dest;      // 切换到目标界面 
                gui->slip.stt = SLIP_RIGHT_IN;  // 新界面向左滑入                
                gui->slip.offset = -128;        // offset 从 128 -> 0
            }
            break; 
         
         /* 向左滑入 */
         case SLIP_LEFT_IN:           
            gui->slip.offset -= gui->slip.step; // 每次左移step个像素
            /* 完全进入屏幕后 */
            if (gui->slip.offset < 0)
            {
                gui->slip.stt = SLIP_STANDSTILL;// 变为静止状态                
                gui->slip.offset = 0;           // offset变为0
            }
            break; 
            
         /* 向右滑入 */    
         case SLIP_RIGHT_IN: 
            gui->slip.offset += gui->slip.step; // 每次右移step个像素
            /* 完全进入屏幕后 */
            if (gui->slip.offset > 0)
            {
                gui->slip.stt = SLIP_STANDSTILL;// 变为静止状态                
                gui->slip.offset = 0;           // offset变为0
            }             
            break;
    }
}

/**
 * @brief  滑向下一个界面
 * @param  参数名 参数说明  
 * @param  [in] gui GUI结构体指针  
 * @param  [in] dir 滑动方向 1: 左 2: 右
 * @param  [in] step 滑动步长
 * @param  [in] dest 目标界面
 * @retval 无
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 被 Display() 调用  \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 无 \n
 */
static void SlipTo(STR_Gui *gui, u8 dir, u8 step, u8 dest)
{
    if (step > 2)
    {        
        step = 2;
    }    
    gui->slip.dest = dest;
    gui->slip.stt = dir;
    gui->slip.step = step;
}

/**
 * @brief  直接显示下一个界面
 * @param  参数名 参数说明  
 * @param  [in] gui GUI结构体指针  
 * @param  [in] dest 目标界面
 * @retval 无s
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 被 Display() 调用  \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 无 \n
 */
static void DirectTo(STR_Gui *gui, u8 dest)
{
    gui->slip.stt = SLIP_STANDSTILL;
    gui->stt = dest;
    gui->upd = 1;
    OledClear();
}

/**
 * @brief  主界面显示
 * @param  参数名 参数说明  
 * @param  [in] gui GUI结构体指针  
 * @retval 无
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 被 Display() 调用  \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 无 \n
 */
static void DispMain(STR_Gui *gui)
{
    s16 offset = gui->slip.offset;
    static s16 preOffset = 1;
    
    if(offset || preOffset || gui->upd)
	{
        gui->main.dly.upd = 1;
        gui->main.ble.upd = 1;
        gui->main.bat.upd = 1;
        gui->main.temp.upd = 1;
        gui->main.bigClock.upd = 1;
        gui->main.date.upd = 1;
        gui->upd = 0;
        OledClear();
    }

    preOffset = offset;
    
    DispValue(&gui->main.temp, offset);                 // 显示温度
    DispSttPic(&gui->main.dly, offset);                 // 显示LED延时关灯状态
    DispSttPic(&gui->main.ble, offset);                 // 显示蓝牙状态
    DispSttPic(&gui->main.bat, offset);                 // 显示电池状态             
    DispClock(&gui->main.date, offset);                 // 显示日期
    DispClock(&gui->main.bigClock, offset);             // 显示时钟
}

/**
 * @brief  主界面按键响应
 * @param  参数名 参数说明  
 * @param  [in] gui GUI结构体指针  
 * @param  [in] key 按键状态结构体指针
 * @retval 无
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 被 KeyAction() 调用  \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 无 \n
 */
static void KeyMain(STR_Gui *gui, STR_Key *key)
{
    if (key->keyPressed.bit.left)
    {
        SlipTo(gui, SLIP_LEFT_OUT, 2, GUI_LED_MENU);
    }
    else if (key->keyPressed.bit.right)
    {
        SlipTo(gui, SLIP_RIGHT_OUT, 2, GUI_SET_MENU);
    }
}

/**
 * @brief  LED菜单界面显示
 * @param  参数名 参数说明  
 * @param  [in] gui GUI结构体指针  
 * @retval 无
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 被 Display() 调用  \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 无 \n
 */
static void DispLedMenu(STR_Gui *gui)
{
    extern const unsigned char gImage_LED[][64];
	extern const unsigned char gImage_LED_DISABLE[][64];
    
    if (gui->slip.stt != SLIP_STANDSTILL  || gui->upd)
    {
        gui->upd = 0;
        
        if(Bat.lvl <= 1)
        {
            OledDisplay(32, 0, 64, 8, (u8*)gImage_LED_DISABLE, gui->slip.offset, 0);   
        }
        else 
        {
            OledDisplay(32, 0, 64, 8, (u8*)gImage_LED,  gui->slip.offset, 0);
        }
    }
}

/**
 * @brief  LED菜单界面按键响应
 * @param  参数名 参数说明  
 * @param  [in] gui GUI结构体指针  
 * @param  [in] key 按键状态结构体指针
 * @retval 无
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 被 KeyAction() 调用  \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 无 \n
 */
static void KeyLedMenu(STR_Gui *gui, STR_Key *key)
{
    if (key->keyPressed.bit.right)
    {
        SlipTo(gui, SLIP_RIGHT_OUT, 2, GUI_MAIN);
    }
    else if (key->keyPressed.bit.left)
    {
        SlipTo(gui, SLIP_LEFT_OUT, 2, GUI_INF_MENU);
    }
    if (key->keyPressed.bit.down)
    {
        gui->led.editPtr = 0;
        gui->led.breathEn.stt = Led.mode;
        gui->led.minDly.pos = (Led.light.delaySec + 59) / 120;
        gui->led.minDly.val = gui->led.minDly.pos << 1;
        gui->led.pwmW.pos = Led.light.pwmW / 10;
        gui->led.pwmW.val = gui->led.pwmW.pos;
        gui->led.breathEn.stt = Led.color.breath;
        gui->led.pwmR.pos = Led.color.pwmR / 10;
        gui->led.pwmR.val = gui->led.pwmR.pos;
        gui->led.pwmG.pos = Led.color.pwmG / 10;
        gui->led.pwmG.val = gui->led.pwmG.pos;
        gui->led.pwmB.pos = Led.color.pwmB / 10;
        gui->led.pwmB.val = gui->led.pwmB.pos;
        memcpy((void*)&preLedSet, (void*)&Led, sizeof(Led));
        DirectTo(gui, GUI_LED);
    }
}

/**
 * @brief  信息菜单界面显示
 * @param  参数名 参数说明  
 * @param  [in] gui GUI结构体指针  
 * @retval 无
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 被 Display() 调用  \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 无 \n
 */
static void DispInfMenu(STR_Gui *gui)
{
    extern const unsigned char gImage_INFO[][64];
    if (gui->slip.stt != SLIP_STANDSTILL || gui->upd)
    {
        gui->upd = 0;
        OledDisplay(32, 0, 64, 8, (u8*)gImage_INFO, gui->slip.offset, 0);
    }
}

/**
 * @brief  信息菜单界面按键响应
 * @param  参数名 参数说明  
 * @param  [in] gui GUI结构体指针  
 * @param  [in] key 按键状态结构体指针
 * @retval 无
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 被 KeyAction() 调用  \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 无 \n
 */
static void KeyInfMenu(STR_Gui *gui, STR_Key *key)
{
    if (key->keyPressed.bit.right)
    {
        SlipTo(gui, SLIP_RIGHT_OUT, 2, GUI_LED_MENU);
    }
    else if (key->keyPressed.bit.left)
    {
        SlipTo(gui, SLIP_LEFT_OUT, 2, GUI_SET_MENU);
    }
    else if (key->keyPressed.bit.down)
    {
        gui->info.pagePtr = 0;
        DirectTo(gui, GUI_INF);
    }
}

/**
 * @brief  设置菜单界面显示
 * @param  参数名 参数说明  
 * @param  [in] gui GUI结构体指针  
 * @retval 无
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 被 Display() 调用  \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 无 \n
 */
static void DispSetMenu(STR_Gui *gui)
{
    extern const unsigned char gImage_SETTING[][64];
    
    if (gui->slip.stt != SLIP_STANDSTILL || gui->upd)
    {
        gui->upd = 0;
        OledDisplay(32, 0, 64, 8, (u8*)gImage_SETTING, gui->slip.offset, 0);
    }
}

/**
 * @brief  设置菜单界面按键响应
 * @param  参数名 参数说明  
 * @param  [in] gui GUI结构体指针  
 * @param  [in] key 按键状态结构体指针
 * @retval 无
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 被 KeyAction() 调用  \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 无 \n
 */
static void KeySetMenu(STR_Gui *gui, STR_Key *key)
{
    if (key->keyPressed.bit.right)
    {
        SlipTo(gui, SLIP_RIGHT_OUT, 2, GUI_INF_MENU);
    }
    else if (key->keyPressed.bit.left)
    {
        SlipTo(gui, SLIP_LEFT_OUT, 2, GUI_MAIN);
    }
    else if (key->keyPressed.bit.down)
    {
        gui->setting.bleEn.stt = Ble.en;
        gui->setting.editPtr = 0;
        gui->setting.secSleep.pos = Sys.secSleep;
        gui->setting.secSleep.val = Sys.secSleep;
        timeSet.year = Rtc.time.year;
        timeSet.month = Rtc.time.month;
        timeSet.date = Rtc.time.date;
        timeSet.hour = Rtc.time.hour;
        timeSet.min = Rtc.time.min;
        timeSet.sec = Rtc.time.sec;
        DirectTo(gui, GUI_SET);
    }
}

/**
 * @brief  LED界面显示
 * @param  参数名 参数说明  
 * @param  [in] gui GUI结构体指针  
 * @retval 无
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 被 Display() 调用  \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 无 \n
 */
static void DispLed(STR_Gui *gui)
{
    static u8 preLedStt = 0;
    /* 如果修改了LED模式 或 要求整页更新，更新所有控件 */
    if (preLedStt != gui->led.ledMode.stt || gui->upd) 
    {
        gui->led.colorLed.upd = 1;
        gui->led.ledMode.upd = 1;
        gui->led.breath.upd = 1;
        gui->led.breathEn.upd = 1;
        gui->led.red.upd = 1;
        gui->led.pwmR.upd = 1;
        gui->led.green.upd = 1;
        gui->led.pwmG.upd = 1;
        gui->led.blue.upd = 1;
        gui->led.pwmB.upd = 1;
        gui->led.dly.upd = 1;
        gui->led.minDly.upd = 1;
        gui->led.light.upd = 1;
        gui->led.pwmW.upd = 1;
        gui->led.save.upd = 1;
        gui->led.exit.upd = 1;
        
        gui->upd = 0;
        preLedStt = gui->led.ledMode.stt; 
    }
    
    /* 选中的控件反色显示 */
    switch (gui->led.editPtr)
    {
        case LEDSET_MODE:
            gui->led.exit.focus = 0;
            gui->led.colorLed.focus = 1;
            break;
        case LEDSET_BRTH:
            gui->led.colorLed.focus = 0;
            gui->led.breath.focus = 1;
            break;
        case LEDSET_PWMR:
            gui->led.breath.focus = 0;
            gui->led.pwmR.focus = 1;
            break;
        case LEDSET_PWMG:
            gui->led.pwmR.focus = 0;
            gui->led.pwmG.focus = 1;
            break;
        case LEDSET_PWMB:
            gui->led.pwmG.focus = 0;
            gui->led.pwmB.focus = 1;
            break;
        case LEDSET_PWMW:
            gui->led.colorLed.focus = 0;
            gui->led.pwmW.focus = 1;
            break;
        case LEDSET_DELA:
            gui->led.pwmW.focus = 0;
            gui->led.minDly.focus = 1;
            break;
        case LEDSET_SAVE:
            gui->led.pwmB.focus = 0;
            gui->led.minDly.focus = 0;
            gui->led.save.focus = 1;
            break;
        case LEDSET_EXIT:
            gui->led.save.focus = 0;
            gui->led.exit.focus = 1;
            break;
    }
    
    DispSttPic(&gui->led.ledMode, 0);
    DispText(&gui->led.colorLed, 0);
    DispText(&gui->led.save, 0);
    DispText(&gui->led.exit, 0);
    if (gui->led.ledMode.stt)   // 彩灯模式
    {
        DispSttPic(&gui->led.breathEn, 0);
        DispText(&gui->led.breath, 0);
        DispText(&gui->led.red, 0);
        DispSlid(&gui->led.pwmR, 0);
        DispText(&gui->led.green, 0);
        DispSlid(&gui->led.pwmG, 0);
        DispText(&gui->led.blue, 0);
        DispSlid(&gui->led.pwmB, 0);      
    }
    else                        // 照明模式
    {
        DispText(&gui->led.dly, 0);
        DispSlid(&gui->led.minDly, 0);
        DispText(&gui->led.light, 0);
        DispSlid(&gui->led.pwmW, 0);
    }
}

/**
 * @brief  LED界面按键响应
 * @param  参数名 参数说明  
 * @param  [in] gui GUI结构体指针  
 * @param  [in] key 按键状态结构体指针
 * @retval 无
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 被 KeyAction() 调用  \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 无 \n
 */
static void KeyLed(STR_Gui *gui, STR_Key *key)
{
    if (key->keyLongPressed.bit.down)
    {       
        gui->led.colorLed.focus = 0;
        gui->led.breath.focus = 0;
        gui->led.pwmR.focus = 0; 
        gui->led.pwmG.focus = 0;
        gui->led.pwmB.focus = 0;
        gui->led.minDly.focus = 0;        
        gui->led.pwmW.focus = 0;
        gui->led.save.focus = 0;
        gui->led.exit.focus = 0;
        memcpy((void*)&Led, (void*)&preLedSet, sizeof(Led));
        DirectTo(gui, GUI_LED_MENU);
    }
    else if (key->keyPressed.bit.down)
    {
        switch (gui->led.editPtr)
        {
            case LEDSET_MODE:
                if (gui->led.ledMode.stt)
                {
                    gui->led.editPtr = LEDSET_BRTH;
                }
                else 
                {
                    gui->led.editPtr = LEDSET_PWMW;
                }
                break;
            case LEDSET_PWMB:
                gui->led.editPtr = LEDSET_SAVE;
                break;
            case LEDSET_EXIT:
                gui->led.editPtr = LEDSET_MODE;
                break;
            case LEDSET_PWMW:
            case LEDSET_DELA:
            case LEDSET_SAVE:
            case LEDSET_BRTH:
            case LEDSET_PWMR:
            case LEDSET_PWMG:
                gui->led.editPtr++;
                break;
        }
    }
    else if (key->keyPressed.bit.left)
    {
        switch (gui->led.editPtr)
        {
            case LEDSET_MODE:
                gui->led.ledMode.stt ^= 0x01;
                Led.mode = gui->led.ledMode.stt;
                OledClear();
                break;                      
            case LEDSET_BRTH:
                gui->led.breathEn.stt ^= 0x01;
                Led.color.breath = gui->led.breathEn.stt;
                break;
            case LEDSET_PWMR:
                if (gui->led.pwmR.pos >= 5)
                {
                    gui->led.pwmR.pos -= 5;
                    gui->led.pwmR.val = gui->led.pwmR.pos;
                }
                else
                {
                    gui->led.pwmR.pos = 0;
                    gui->led.pwmR.val = gui->led.pwmR.pos;
                }
                Led.color.pwmR = gui->led.pwmR.val * 10;
                SetColor(Led.color.pwmR, Led.color.pwmG, Led.color.pwmB, Led.color.breath);
                break;
            case LEDSET_PWMG:
                if (gui->led.pwmG.pos >= 5)
                {
                    gui->led.pwmG.pos -= 5;
                    gui->led.pwmG.val = gui->led.pwmG.pos;
                }
                else
                {
                    gui->led.pwmG.pos = 0;
                    gui->led.pwmG.val = gui->led.pwmG.pos;
                }
                Led.color.pwmG = gui->led.pwmG.val * 10;
                SetColor(Led.color.pwmR, Led.color.pwmG, Led.color.pwmB, Led.color.breath);
                break;
            case LEDSET_PWMB:
                if (gui->led.pwmB.pos >= 5)
                {
                    gui->led.pwmB.pos -= 5;
                    gui->led.pwmB.val = gui->led.pwmB.pos;
                }
                else
                {
                    gui->led.pwmB.pos = 0;
                    gui->led.pwmB.val = gui->led.pwmB.pos;
                }
                Led.color.pwmB = gui->led.pwmB.val * 10;
                SetColor(Led.color.pwmR, Led.color.pwmG, Led.color.pwmB, Led.color.breath);
                break;         
            case LEDSET_PWMW:
                if (gui->led.pwmW.pos >= 5)
                {
                    gui->led.pwmW.pos -= 5;
                    gui->led.pwmW.val = gui->led.pwmW.pos;
                }
                else
                {
                    gui->led.pwmW.pos = 0;
                    gui->led.pwmW.val = gui->led.pwmW.pos;
                }
                Led.light.pwmW = gui->led.pwmW.val * 10;
                SetLight(Led.light.pwmW, Led.light.delaySec);
                break;
            case LEDSET_DELA:
                if (gui->led.minDly.pos >= 5)
                {
                    gui->led.minDly.pos -= 5;
                    gui->led.minDly.val = gui->led.minDly.pos << 1;
                }
                else
                {
                    gui->led.minDly.pos = 0;
                    gui->led.minDly.val = gui->led.minDly.pos << 1;
                }
                Led.light.delaySec = gui->led.minDly.val * 60;
                SetLight(Led.light.pwmW, Led.light.delaySec);
                break;  
            case LEDSET_SAVE:
                gui->led.save.focus = 0;
                DirectTo(gui, GUI_LED_MENU);
                break;
            case LEDSET_EXIT:
                gui->led.exit.focus = 0;
                memcpy((void*)&Led, (void*)&preLedSet, sizeof(Led));
                DirectTo(gui, GUI_LED_MENU);
                break;
        }
    }
    else if (key->keyPressed.bit.right)
    {
        switch (gui->led.editPtr)
        {
            case LEDSET_MODE:
                gui->led.ledMode.stt ^= 0x01;
                Led.mode = gui->led.ledMode.stt;
                OledClear();
                break;                      
            case LEDSET_BRTH:
                gui->led.breathEn.stt ^= 0x01;
                Led.color.breath = gui->led.breathEn.stt;
                break;
            case LEDSET_PWMR:
                if (gui->led.pwmR.pos <= 95)
                {
                    gui->led.pwmR.pos += 5;
                    gui->led.pwmR.val = gui->led.pwmR.pos;
                }
                else
                {
                    gui->led.pwmR.pos = 100;
                    gui->led.pwmR.val = gui->led.pwmR.pos;
                }
                Led.color.pwmR = gui->led.pwmR.val * 10;
                SetColor(Led.color.pwmR, Led.color.pwmG, Led.color.pwmB, Led.color.breath);
                break;
            case LEDSET_PWMG:
                if (gui->led.pwmG.pos <= 95)
                {
                    gui->led.pwmG.pos += 5;
                    gui->led.pwmG.val = gui->led.pwmG.pos;
                }
                else
                {
                    gui->led.pwmG.pos = 100;
                    gui->led.pwmG.val = gui->led.pwmG.pos;
                }
                Led.color.pwmG = gui->led.pwmG.val * 10;
                SetColor(Led.color.pwmR, Led.color.pwmG, Led.color.pwmB, Led.color.breath);
                break;
            case LEDSET_PWMB:
                if (gui->led.pwmB.pos <= 95)
                {
                    gui->led.pwmB.pos += 5;
                    gui->led.pwmB.val = gui->led.pwmB.pos;
                }
                else
                {
                    gui->led.pwmB.pos = 100;
                    gui->led.pwmB.val = gui->led.pwmB.pos;
                }
                Led.color.pwmB = gui->led.pwmB.val * 10;
                SetColor(Led.color.pwmR, Led.color.pwmG, Led.color.pwmB, Led.color.breath);
                break;         
            case LEDSET_PWMW:
                if (gui->led.pwmW.pos <= 95)
                {
                    gui->led.pwmW.pos += 5;
                    gui->led.pwmW.val = gui->led.pwmW.pos;
                }
                else
                {
                    gui->led.pwmW.pos = 100;
                    gui->led.pwmW.val = gui->led.pwmW.pos;
                }
                Led.light.pwmW = gui->led.pwmW.val * 10;
                SetLight(Led.light.pwmW, Led.light.delaySec);
                break;
            case LEDSET_DELA:
                if (gui->led.minDly.pos <= 55)
                {
                    gui->led.minDly.pos += 5;
                    gui->led.minDly.val = gui->led.minDly.pos << 1;
                }
                else
                {
                    gui->led.minDly.pos = 60;
                    gui->led.minDly.val = gui->led.minDly.pos << 1;
                }
                Led.light.delaySec = gui->led.minDly.val * 60;
                SetLight(Led.light.pwmW, Led.light.delaySec);
                break;  
            case LEDSET_SAVE:
                gui->led.save.focus = 0;
                DirectTo(gui, GUI_LED_MENU);
                break;
            case LEDSET_EXIT:
                gui->led.exit.focus = 0;
                memcpy((void*)&Led, (void*)&preLedSet, sizeof(Led));
                DirectTo(gui, GUI_LED_MENU);
                break;
        }
    }
    
}

/**
 * @brief  信息界面显示
 * @param  参数名 参数说明  
 * @param  [in] gui GUI结构体指针  
 * @retval 无
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 被 Display() 调用  \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 无 \n
 */
static void DispInf(STR_Gui *gui)
{
    static u8 prePage = 0;
    if (prePage != gui->info.pagePtr ||gui->upd)
    {
        gui->info.time.upd = 1;
        gui->info.temp.upd = 1;
        gui->info.bat.upd = 1;
        gui->info.batVol.upd = 1;
        gui->info.batChrg.upd = 1;
        gui->info.batFull.upd = 1;
        gui->info.bleStt.upd = 1;
        gui->info.bleConn.upd = 1;
        gui->info.ledDly.upd = 1;
        gui->info.secDly.upd = 1;
        gui->info.ver.upd = 1;
        gui->info.author.upd = 1;
        
        prePage = gui->info.pagePtr;
        gui->upd = 0;
    }
    
    switch (gui->info.pagePtr)
    {
        case 0:
            DispClock(&gui->info.time, 0);
            DispValue(&gui->info.temp, 0);
            DispSttPic(&gui->info.bat, 0);
            DispValue(&gui->info.batVol, 0);
            DispText(&gui->info.batChrg, 0);
            DispText(&gui->info.batFull, 0);
            DispText(&gui->info.bleStt, 0);
            DispText(&gui->info.bleConn, 0);
            DispText(&gui->info.ledDly, 0);
            if (gui->info.secDly.val)
            {
                DispValue(&gui->info.secDly, 0);
            }
            break;
        case 1:
            DispText(&gui->info.author, 0);
            DispText(&gui->info.ver, 0);
            break;
    }
}

/**
 * @brief  信息界面按键响应
 * @param  参数名 参数说明  
 * @param  [in] gui GUI结构体指针  
 * @param  [in] key 按键状态结构体指针
 * @retval 无
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 被 KeyAction() 调用  \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 无 \n
 */
static void KeyInf(STR_Gui *gui, STR_Key *key)
{
    if (key->keyPressed.bit.down)           // 下键: 退出
    {
        DirectTo(gui, GUI_INF_MENU);
    }
    else if (key->keyPressed.bit.left)      // 左键: 翻页
    {
        OledClear();
        gui->info.pagePtr ^= 1;
    }
    else if (key->keyPressed.bit.right)     // 右键: 翻页
    {
        OledClear();
        gui->info.pagePtr ^= 1;
    }
}

/**
 * @brief  设置界面显示
 * @param  参数名 参数说明  
 * @param  [in] gui GUI结构体指针  
 * @retval 无
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 被 Display() 调用  \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 无 \n
 */
static void DispSet(STR_Gui *gui)
{
    if (gui->upd)
    {
        gui->setting.ble.upd = 1;
        gui->setting.bleEn.upd = 1;
        gui->setting.time.upd = 1;
        gui->setting.timeSet.upd = 1;
        gui->setting.sleep.upd = 1;
        gui->setting.secSleep.upd = 1;
        gui->setting.save.upd = 1;
        gui->setting.exit.upd = 1;
        gui->upd = 0;
    }
    
    switch (gui->setting.editPtr)
    {
        case SET_BLE:
            gui->setting.exit.focus = 0;
            gui->setting.ble.focus = 1;
            break;
        case SET_SLP:
            gui->setting.ble.focus = 0;
            gui->setting.secSleep.focus = 1;
            break;
        case SET_YEA:
            gui->setting.secSleep.focus = 0;
        case SET_MON:
        case SET_DAT:
        case SET_HOU:
        case SET_MIN:
        case SET_SEC:            
            gui->setting.timeSet.focus = gui->setting.editPtr - 1;
            break;
        case SET_SAV:
            gui->setting.timeSet.focus = 0;
            gui->setting.save.focus = 1;
            break;
        case SET_EXT:
            gui->setting.save.focus = 0;
            gui->setting.exit.focus = 1;
            break;
    }
    
    DispText(&gui->setting.ble, 0);
    DispText(&gui->setting.sleep, 0);
    DispText(&gui->setting.time, 0);
    DispText(&gui->setting.save, 0);
    DispText(&gui->setting.exit, 0);
    DispSlid(&gui->setting.secSleep, 0);
    DispSttPic(&gui->setting.bleEn, 0);
    DispClock(&gui->setting.timeSet, 0);
}

/**
 * @brief  设置界面按键响应
 * @param  参数名 参数说明  
 * @param  [in] gui GUI结构体指针  
 * @param  [in] key 按键状态结构体指针
 * @retval 无
 * @par 使用全局变量 \n
 * @note ● 执行时间: \n
 *       ● 调用周期: 被 KeyAction() 调用  \n
 *       ● 可否打断: 可以 \n
 *
 * @par  注意:
 *       ● 无 \n
 */
static void KeySet(STR_Gui *gui, STR_Key *key)
{
    if (key->keyLongPressed.bit.down)       // 长按下键: 退出
    {
        gui->setting.ble.focus = 0;
        gui->setting.secSleep.focus = 0;
        gui->setting.timeSet.focus = 0;
        gui->setting.save.focus = 0;
        gui->setting.exit.focus = 0;
        DirectTo(gui, GUI_SET_MENU);
    }
    else if (key->keyPressed.bit.down)      // 下键: 下一个编辑项
    {
        gui->setting.editPtr++;
        if(gui->setting.editPtr == 10)
        {
            gui->setting.editPtr = 0;
        }
    }
    else if (key->keyPressed.bit.left)      // 左键: 减小
    {
        switch (gui->setting.editPtr)
        {
            case SET_BLE:
                gui->setting.bleEn.stt ^= 1;
                break;
            case SET_SLP:
                if (gui->setting.secSleep.pos >= 20)
                {
                    gui->setting.secSleep.pos -= 10;
                    gui->setting.secSleep.val = gui->setting.secSleep.pos;
                }
                else
                {
                    gui->setting.secSleep.pos = 10;
                    gui->setting.secSleep.val = gui->setting.secSleep.pos;
                }
                break;
            case SET_YEA:
                timeSet.year--;
                if (timeSet.year > 99)
                {
                    timeSet.year = 99;
                }
                break;
            case SET_MON:
                timeSet.month--;
                if (timeSet.month == 0)
                {
                    timeSet.month = 12;
                }
                break;
            case SET_DAT:
                timeSet.date--;
                if (timeSet.date == 0)
                {
                    timeSet.date = 31;
                }
                break;
            case SET_HOU:
                timeSet.hour--;
                if (timeSet.hour > 23)
                {
                    timeSet.hour = 23;
                }
                break;
            case SET_MIN:
                timeSet.min--;
                if (timeSet.min > 59)
                {
                    timeSet.min = 59;
                }
                break;
            case SET_SEC:            
                timeSet.sec--;
                if (timeSet.sec > 59)
                {
                    timeSet.sec = 59;
                }
                break;
            case SET_SAV:
                Sys.secSleep = gui->setting.secSleep.pos;
                Ble.en = gui->setting.bleEn.stt;
                BleCmd(Ble.en);
                TimeSet(&timeSet);
                gui->setting.save.focus = 0;
                DirectTo(gui, GUI_SET_MENU);
                break;
            case SET_EXT:
                gui->setting.exit.focus = 0;
                DirectTo(gui, GUI_SET_MENU);
                break;
        }
    }
    else if (key->keyPressed.bit.right)     // 右键: 增大
    {
        switch (gui->setting.editPtr)
        {
            case SET_BLE:
                gui->setting.bleEn.stt ^= 1;
                break;
            case SET_SLP:
                if (gui->setting.secSleep.pos <= 60)
                {
                    gui->setting.secSleep.pos += 10;
                    gui->setting.secSleep.val = gui->setting.secSleep.pos;
                }
                else
                {
                    gui->setting.secSleep.pos = 70;
                    gui->setting.secSleep.val = gui->setting.secSleep.pos;
                }
                break;
            case SET_YEA:
                timeSet.year++;
                if (timeSet.year > 99)
                {
                    timeSet.year = 0;
                }
                break;
            case SET_MON:
                timeSet.month++;
                if (timeSet.month > 12)
                {
                    timeSet.month = 1;
                }
                break;
            case SET_DAT:
                timeSet.date++;
                if (timeSet.date > 31)
                {
                    timeSet.date = 1;
                }
                break;
            case SET_HOU:
                timeSet.hour++;
                if (timeSet.hour > 23)
                {
                    timeSet.hour = 0;
                }
                break;
            case SET_MIN:
                timeSet.min++;
                if (timeSet.min > 59)
                {
                    timeSet.min = 0;
                }
                break;
            case SET_SEC:            
                timeSet.sec++;
                if (timeSet.sec > 59)
                {
                    timeSet.sec = 0;
                }
                break;
            case SET_SAV:
                Sys.secSleep = gui->setting.secSleep.pos;
                Ble.en = gui->setting.bleEn.stt;
                BleCmd(Ble.en);
                TimeSet(&timeSet);
                gui->setting.save.focus = 0;
                DirectTo(gui, GUI_SET_MENU);
                break;
            case SET_EXT:
                gui->setting.exit.focus = 0;
                DirectTo(gui, GUI_SET_MENU);
                break;
        }
    }
    
}


/**  @} */


