/**
*************************************************************************************************
* @file    Manage.c
* @author  Ge Yue
* @version V1.0.0
* @date    2016.03.22
* @brief   Ӧ�ò���Ӳ����֮�䡢Ӧ�ò��ģ��֮������ݴ���
*************************************************************************************************
@verbatim 

- �汾��       ����      ������        ���� 
  V1.0      2016.03.22   Ge Yue    Ӧ�ò���Ӳ����֮�䡢Ӧ�ò��ģ��֮������ݴ��� 

- �汾��       ����      �޸���        ���� 
@endverbatim 
*/

/**
* @defgroup MANAGE Manage: APP Group
* @{
*/


#include "Manage.h"

/** @brief ϵͳ�����ṹ�� */
STR_Sys Sys;

/** @brief ϵͳ״̬����*/
void SysSttUpd(STR_Sys *sys)
{
    static u8 cnt = 0;
    
    cnt++;
    if (cnt == 10)
    {
        cnt = 0;
        sys->secSys++;
    }
    
    switch (sys->power->sysStt)
    {
        case NORMAL:
            if ((sys->secSleep <= 60) && (sys->secSys - sys->secLastKey > sys->secSleep))
            {
                if (sys->ble->conStt || sys->led->light.delayEn || sys->led->color.breath)
                {
                    sys->power->tarStt = OLEDOFF;
                }
                else 
                {
                    sys->power->tarStt = SLEEP;
                }
            }
            break;
        case OLEDOFF:
            if ((sys->secSleep >= 60) || (sys->secSys - sys->secLastKey < 2))
            {
                sys->power->tarStt = NORMAL;
            }
            else if (!sys->ble->conStt && !sys->led->light.delayEn && !sys->led->color.breath)
            {
                sys->power->tarStt = SLEEP;
            }
            break;
        case SLEEP:
            if ((sys->secSleep >= 60) || (sys->secSys - sys->secLastKey < 2))
            {
                sys->power->tarStt = NORMAL;
            }
            else if (sys->ble->conStt)
            {
                sys->power->tarStt = OLEDOFF;
            }
            break;
    }
}

void SoftwareInit(void)
{
    Sys.bat = &Bat;
    Sys.ble = &Ble;
    Sys.key = &KeyStt;
    Sys.led = &Led;
    Sys.temp = &Temp;
    Sys.rtc = &Rtc;
    Sys.power = &Power;
    Sys.gui = &Gui;
    Sys.secSleep = 70;
    
    GuiInit();
    
}

void GuiDataUpd(STR_Sys *sys)
{
    static u8 preBatChrg = 0, preBatFull = 0, preBleStt = 0, preBleConn = 0, preLedDlyEn = 0;
    /* ���� */
    sys->gui->main.dly.stt = sys->led->light.delayEn;
    sys->gui->main.ble.stt = sys->ble->en;
    sys->gui->main.bat.stt = sys->bat->lvl;
    sys->gui->main.temp.val = sys->temp->val;

    /* ��Ϣ���� */
    sys->gui->info.temp.val = sys->temp->val;
    sys->gui->info.batVol.val = sys->bat->vol;
    sys->gui->info.secDly.val = sys->led->light.delaySec;
    sys->gui->info.bat.stt = sys->bat->lvl;
    if (preBatChrg != sys->bat->stt.bit.chrg)
    {
        preBatChrg = sys->bat->stt.bit.chrg;
        sys->gui->info.batChrg.upd = 1;
        sys->gui->info.batChrg.str = (preBatChrg ? "Charging:Y" : "Charging:N");      
    }
    if (preBatFull != sys->bat->stt.bit.full)
    {
        preBatFull = sys->bat->stt.bit.full;
        sys->gui->info.batFull.upd = 1;
        sys->gui->info.batFull.str = (preBatFull ? "Full:Y" : "Full:N");        
    }
    if (preBleStt != sys->ble->en)
    {
        preBleStt = sys->ble->en;
        sys->gui->info.bleStt.upd = 1;
        sys->gui->info.bleStt.str = (preBleStt ? "BLE: On," : "BLE:Off,");      
    }
    if (preBleConn != sys->ble->conStt)
    {
        preBleConn = sys->ble->conStt;
        sys->gui->info.bleConn.upd = 1;
        sys->gui->info.bleConn.str = (preBleConn ? "Connected   " : "Disconnected");        
    }
    if (preLedDlyEn != sys->led->light.delayEn)
    {
        preLedDlyEn = sys->led->light.delayEn;
        sys->gui->info.ledDly.upd = 1;
        sys->gui->info.ledDly.str = (preLedDlyEn ? "Led Delay:    " : "Led Delay: Off");        
    }
}

/**  @} */
