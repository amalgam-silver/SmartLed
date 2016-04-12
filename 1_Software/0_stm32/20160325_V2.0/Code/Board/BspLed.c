/**
*************************************************************************************************
* @file    BspLed.c
* @author  Ge Yue
* @version V1.0.0
* @date    2016.03.21
* @brief   Led����
*************************************************************************************************
@verbatim 

- �汾��       ����      ������        ���� 
  V1.0      2016.03.21   Ge Yue       Led����; 

- �汾��       ����      �޸���        ���� 
@endverbatim 
*/

/**
* @defgroup LED BspLed: BSP Group
* @{
*/

#include "BspLed.h"
#include "Bsp.h"

/** @brief LED�ṹ�� */
STR_Led Led = {0,};

/**
 * @brief  Led��ʼ������
 * @param  ������ ����˵��  
 * @param  ��  
 * @retval ��
 * @par ʹ��ȫ�ֱ��� \n
 * @note �� ִ��ʱ��: \n
 *       �� ��������: ��������, ��ʼ��ʱ���� \n
 *       �� �ɷ���: ���� \n
 *
 * @par  ע��:
 *       �� �� \n
 */
void LedInit(void)
{
    PwmInit();
}

/**
 * @brief  ��������ģʽ����
 * @param  ������ ����˵��  
 * @param  [in] light ����
 * @param  [in] delay ��ʱ, ��λ: ��
 * @retval ��
 * @par ʹ��ȫ�ֱ��� \n
 * @note �� ִ��ʱ��: \n
 *       �� ��������: 500us \n
 *       �� �ɷ���: ���� \n
 *
 * @par  ע��:
 *       �� �� \n
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
            //��ʼ��ʱ
            Led.light.delayEn = 1;
        }			
        else
        {
            Led.light.delayEn = 0;
        }
    }
}

/**
 * @brief  ��������ģʽ����
 * @param  ������ ����˵��  
 * @param  [in] r �������
 * @param  [in] g �̹�����
 * @param  [in] b ��������
 * @param  [in] breath ������ʹ��
 * @retval ��
 * @par ʹ��ȫ�ֱ��� \n
 * @note �� ִ��ʱ��: \n
 *       �� ��������: 500us \n
 *       �� �ɷ���: ���� \n
 *
 * @par  ע��:
 *       �� �� \n
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
 * @brief  LED״̬���º���
 * @param  ������ ����˵��  
 * @param  [in] led LED�ṹ��ָ��
 * @retval ��
 * @par ʹ��ȫ�ֱ��� \n
 * @note �� ִ��ʱ��: \n
 *       �� ��������: 20ms \n
 *       �� �ɷ���: ���� \n
 *
 * @par  ע��:
 *       �� �� \n
 */
void LedUpdStt(STR_Led *led)
{
    /* ���������, �����Ƽ�����, �����Ƽ��������� */
    static u8 secCnt = 0, breathCnt = 0, breathDir = 0;
    
    /* ���ں����� */
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
    
    /* ���ڼ��� */
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
                /* ������ʱ�صƼ�ʱ */
                if (led->light.delayEn)
                {
                    if (secCnt == 25)
                    {
                        led->light.delaySec--;
                        /* ��ʱʱ�䵽, �ص� */
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
                /* ������ʹ�� */
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

