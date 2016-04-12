/**
 *************************************************************************************************
 * @file    stm32_spi.h
 * @author  Ge Yue
 * @version V1.0.0
 * @date    2016.03.25
 * @brief   stm32通过DMA SPI驱动OLED显示屏底层函数
 *************************************************************************************************
 @verbatim 
 
  - 版本号       日期      创建人        描述 
    V1.0      2016.03.25   Ge Yue     stm32通过DMA SPI驱动OLED显示屏底层函数; 
 
  - 版本号       日期      修改人        描述 
 @endverbatim 
 */

/**
 * @defgroup SPI stm32_spi: CPU Group
 * @{
 */
 
#ifndef _STM32_SPI_H
#define _STM32_SPI_H

 
#include "stm32f10x.h"

/** @brief  SPI初始函数 */
void SpiInit(void);
/** @brief SPI通过DMA方式发送数据 */
void SpiSend(u8 *buf, u16 len);
 
#endif
/**  @} */
 
