/**
 *************************************************************************************************
 * @file    stm32_rcc.c
 * @author  Ge Yue
 * @version V1.0.0
 * @date    2016.03.18
 * @brief   stm32��ϵͳʱ������
 *************************************************************************************************
 @verbatim 
 
  - �汾��       ����      ������        ���� 
    V1.0      2016.03.18   Ge Yue     stm32��ϵͳʱ������; 
 
  - �汾��       ����      �޸���        ���� 
 @endverbatim 
 */

/**
 * @defgroup RCC stm32_rcc: CPU Group
 * @{
 */
 
#include "stm32_rcc.h"

/**
 * @brief  Rcc��ʼ����
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
void RccInit(void)
{
	//ʹ��оƬ�ڲ�RC����
	RCC_HSICmd(ENABLE);

	//����FLASH��ȡ�ȴ�ʱ��Ϊ0���ڣ�ϵͳƵ��Ϊ8MHZ��
	FLASH_SetLatency(FLASH_Latency_0);
	//����PLL����Ϊ�ڲ�����/2��8��Ƶ
	//PLLƵ�� 8MHZ / 2 * 8 = 8MHZ
	RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_2); 
	//ʹ��PLL 
	RCC_PLLCmd(ENABLE);
	//����ϵͳʱ����ԴΪPLL,8MHZ
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

	// AHB Ƶ�� = ϵͳƵ��    8MHZ
	RCC_HCLKConfig(RCC_SYSCLK_Div1);			
	// APB1 = AHB/4	         2MHZ 	
	RCC_PCLK1Config(RCC_HCLK_Div4);			
	// APB2 = AHB/4			 2MHZ		
	RCC_PCLK2Config(RCC_HCLK_Div1);	
}

/**
 * @brief  stm32����˯��״̬����
 * @param  ������ ����˵��  
 * @param  ��  
 * @retval ��
 * @par ʹ��ȫ�ֱ��� \n
 * @note �� ִ��ʱ��: \n
 *       �� ��������: �������� \n
 *       �� �ɷ���: ���� \n
 *
 * @par  ע��:
 *       �� �� \n
 */
void Sleep(void)
{
    PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
}

/**  @} */
