/**
 *************************************************************************************************
 * @file    stm32_spi.c
 * @author  Ge Yue
 * @version V1.0.0
 * @date    2016.03.25
 * @brief   stm32ͨ��DMA SPI����OLED��ʾ���ײ㺯��
 *************************************************************************************************
 @verbatim 
 
  - �汾��       ����      ������        ���� 
    V1.0      2016.03.25   Ge Yue     stm32ͨ��DMA SPI����OLED��ʾ���ײ㺯��; 
 
  - �汾��       ����      �޸���        ���� 
 @endverbatim 
 */

/**
 * @defgroup SPI stm32_spi: CPU Group
 * @{
 */
 
#include "stm32_spi.h"

/**@brief ��Ч�ֽ� */
u8 dummyByte = 0;

/**
 * @brief  SPI��ʼ����
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
void SpiInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;
	DMA_InitTypeDef  DMA_InitStructure;
    
    /* ʹ��GPIOA, AFIO, SPI, DMA1*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1 | RCC_APB2Periph_AFIO, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); 
    
    /* PA4 -> CS */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    /* PA5 -> CLK, PA7 -> MOSI */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    /* ����SPI */
	SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;	        // ���ݷ��򣺽����� 
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		                // STM32��SPI����ģʽ������ģʽ 
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;	                // ����λ���ȣ� 8λ 
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;			                // ���߿����Ǹߵ�ƽ */
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;		                // ʱ�ӵĵ�1�����ز������� */
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;			                // Ƭѡ���Ʒ�ʽ��������� */
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;  // ������Ԥ��Ƶϵ����2��Ƶ */
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	                // ����λ������򣺸�λ�ȴ� */
	SPI_InitStructure.SPI_CRCPolynomial = 7;			                // CRC����ʽ�Ĵ�������λ��Ϊ7�������̲��� */
	SPI_Init(SPI1, &SPI_InitStructure);  
    SPI_Cmd(SPI1, ENABLE);                                              // ʹ��SPI
	SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);                    // SPI����DMA���� 
    
    /* ����DMA, DMA1_CH3 -> SPI1_TX */
	DMA_DeInit(DMA1_Channel3);
	DMA_InitStructure.DMA_PeripheralBaseAddr =(u32)&SPI1->DR;               // �����ַ: SPI1��DR�Ĵ�����ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&dummyByte;                 // �ڴ��ַ: ����ʱָ��    
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;                      // ��������: �ڴ�->����
	DMA_InitStructure.DMA_BufferSize = 1;                                   // ����ʱָ��, �˴���Ч
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;        // �����ַ���Լ�
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                 // �ڴ��ַ�Զ�����ģʽ
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; // ���䵥λ: �ֽ�
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;         // ���䵥λ: �ֽ�
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                           // ��ѭ��ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;                     // ���ȼ�: ��
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                            // ���ڴ�->�ڴ淽ʽ 
	DMA_Init(DMA1_Channel3, &DMA_InitStructure);

	DMA_Cmd(DMA1_Channel3, ENABLE);
}

/**
 * @brief  SPIͨ��DMA��ʽ��������
 * @param  ������ ����˵��  
 * @param  [in] buf �������ݻ���ָ��  
 * @param  [in] len ���ͳ���
 * @par ʹ��ȫ�ֱ��� \n
 * @note �� ִ��ʱ��: \n
 *       �� ��������: �������� \n
 *       �� �ɷ���: ���� \n
 *
 * @par  ע��:
 *       �� �� \n
 */
void SpiSend(u8 *buf, u16 len)
{
    DMA_Cmd(DMA1_Channel3, DISABLE);	    // �رյ�ǰͨ��������ͨ�����ã�������رգ����ܽ������ã�		
    DMA1_Channel3->CNDTR = len;	            // �趨�������ݳ���
    DMA1_Channel3->CMAR = (u32)buf;         // �趨���ͻ����ַ
    DMA1->IFCR |= (u32)0x00000F00;	        // ���DMA_Cannel3�жϱ�־(SPI1)
    DMA_Cmd(DMA1_Channel3, ENABLE);		    // ʹ��DMAͨ��3����������
}

 /**  @} */

