/**
 *************************************************************************************************
 * @file    stm32_spi.h
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
 
#ifndef _STM32_SPI_H
#define _STM32_SPI_H

 
#include "stm32f10x.h"

/** @brief  SPI��ʼ���� */
void SpiInit(void);
/** @brief SPIͨ��DMA��ʽ�������� */
void SpiSend(u8 *buf, u16 len);
 
#endif
/**  @} */
 
