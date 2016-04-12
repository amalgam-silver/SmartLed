/**
 *************************************************************************************************
 * @file    stm32_uart.c
 * @author  Ge Yue
 * @version V1.0.0
 * @date    2016.03.18
 * @brief   stm32�Ĵ��ڶ�д
 *************************************************************************************************
 @verbatim 
 
  - �汾��       ����      ������        ���� 
    V1.0      2016.03.18   Ge Yue     stm32�Ĵ��ڶ�д; 
    
  - �汾��       ����      �޸���        ���� 
 @endverbatim 
 */

/**
 * @defgroup UART stm32_uart: CPU Group
 * @{
 */
 
#include "stm32_uart.h"

/** @brief ���ڽṹ�� */
STR_Uart Uart;

/**
 * @brief  ��ʼ��UART��ʹ��uart1+DMA,��RTU��ʽ�շ�����
 * @param  ������ ����˵��
 * @param  [in] baud usart1�Ĳ�����bps(2400~115200bps)
 * @retval ��
 * @par ʹ��ȫ�ֱ���
 *         �� \n
 * @note �� ִ��ʱ��: \n
 *       �� ��������: �ϵ��ʼ�� \n
 *       �� �ɷ���: ���� \n
 *
 * @par  ע��:
 *       ��  \n
 *
 */
void UsartInit(u16 baud)
{
    USART_InitTypeDef USART_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    DMA_InitTypeDef DMA_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

    /* ��ʱ�� */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;        // �����ַ���Լ�
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                 // �ڴ��ַ�Լ�
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; // ���䵥λ: �ֽ�
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;         // ���䵥λ: �ֽ�
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                            // ��ֹM->Mģʽ
    // DMA1 Channel4 (triggered by USART1 Tx event) Config
    DMA_DeInit(DMA1_Channel4);  
    DMA_InitStructure.DMA_PeripheralBaseAddr = USART1_DR_Base;              // DMA���ӵ�����Ĵ�����ַ
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)Uart.txBuf;	                // UART1���ͻ������׵�ַ
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;                      // ���ݷ��ͷ����ⲿ��ԴΪĿ�ĵ�ַ
    DMA_InitStructure.DMA_BufferSize = UART_BUF_MAX;                        // ���ͻ�������С
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                           // DMAģʽΪ����
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;      		        // ��ǰDMAͨ�����ȼ�Ϊ��
    DMA_Init(DMA1_Channel4, &DMA_InitStructure);                	        // ��ʼ��DMAͨ��  
   
    // DMA1 Channel5 (triggered by USART1 Rx event) Config
    DMA_DeInit(DMA1_Channel5);  
    DMA_InitStructure.DMA_PeripheralBaseAddr = USART1_DR_Base;
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)Uart.rxBuf;                 // UART1���ջ������׵�ַ
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                      // ���ݷ��ͷ���,�ⲿ��ԴΪԴ��ַ
    DMA_InitStructure.DMA_BufferSize = UART_BUF_MAX;                        // ���ջ�������С
  	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                         // DMAģʽΪѭ��
    DMA_Init(DMA1_Channel5, &DMA_InitStructure);                            // ��ʼ��DMAͨ�� 
    
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   	
    
    /* PA9 -> UART1_TX, PA10 -> UART1_RX */ 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;          		
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    		
    GPIO_Init(GPIOA, &GPIO_InitStructure);  
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;              
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   
    GPIO_Init(GPIOA, &GPIO_InitStructure);	
   
    /* ��ʼ������ */
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;                         //���ݳ���8bit
    USART_InitStructure.USART_StopBits = USART_StopBits_1;                              //ֹͣλ1bit
    USART_InitStructure.USART_Parity = USART_Parity_No;                                 //����żУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;     //��������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;                     //���ͽ���
    USART_InitStructure.USART_BaudRate = baud;                                          //������
    USART_Init(USART1, &USART_InitStructure);
    
    // Uart1 ͨѶ��ͨ DMA    
    USART_DMACmd(USART1, USART_DMAReq_Rx | USART_DMAReq_Tx, ENABLE);

    // Enable the USART1 
    USART_Cmd(USART1, ENABLE);		

    DMA_Cmd(DMA1_Channel4, ENABLE);
    DMA_Cmd(DMA1_Channel5, ENABLE);
    
}

/**
 * @brief  UART1���ݽ��ճ���ÿ20msִ��һ�Σ�������ݽ��ս��ȣ��������4msû���µ����ݱ����գ� \n
 *         ����Ϊ����ǰ���ݰ����ս������ȴ�������̴�������.
 *         ���շ�ʽΪDMA�����ݰ��ķֱ�ʹ�ü��ʱ��ֱ档
 * @param  ������ ����˵��
 * @param  [in] handle �������ݴ�����ָ��
 * @retval ��
 * @par ʹ��ȫ�ֱ���
 *         Uart: ����1�ṹ�� \n
 * @note �� ִ��ʱ��: \n
 *       �� ��������: 4ms \n
 *       �� �ɷ���: ���� \n
 *
 * @par  ע��:
 *       �� \n
 */
void RxUart(UartHandler handle)
{
    u32 tmpRp = 0;
    
    tmpRp = UART_BUF_MAX - DMA1_Channel5->CNDTR;     	//���㴮�ڽ���BUF�±�

	if(Uart.rPtr != tmpRp)				                //�������ݳ����б仯�����ڽ���������
	{
	    Uart.rPtr = tmpRp;
	}
	else
	{
		if(Uart.rPtr > 0)
		{
            Uart.rxFin = 1;				                //�������
            
            (*handle)(&Uart);                           //�û�������
            
            DMA_Cmd(DMA1_Channel5, DISABLE);			//�رյ�ǰͨ��������ͨ�����ã�������رգ����ܽ������ã���
	        DMA1_Channel5->CNDTR = UART_BUF_MAX;		//�趨�������ݳ��ȣ����ջ���ռ䣩
            Uart.rPtr=0;
	        DMA_Cmd(DMA1_Channel5, ENABLE);				//ʹ��DMA1ͨ��5����������
		}
	}
}

/**        
 * @brief  ����1�������ݡ�
 * @param  ������ ����˵��
 * @param  ��
 * @retval ��
 * @par ʹ��ȫ�ֱ���
 *         �� \n
 * @note �� ִ��ʱ��: \n
 *       �� ��������: �������� \n
 *       �� �ɷ���: ���� \n
 *
 * @par  ע��:
 *       �� ����ǰ�Ƚ�Ҫ���͵����ݷ��뻺���� Uart[].txBuf����ֵ���ͳ��� Uart[].tPtr�������ͱ�־λUart[].txFlg��1�� \n
 */
void TxUart(void)
{
    if(Uart.txFlg)
	{
		DMA_Cmd(DMA1_Channel4, DISABLE);	    // �رյ�ǰͨ��������ͨ�����ã�������رգ����ܽ������ã�		
		DMA1_Channel4->CNDTR = Uart.tPtr;	    // �趨�������ݳ���
     	DMA1->IFCR |= (u32)0x0000F000;	        // ���DMA_Cannel4�жϱ�־(usart1)
     	DMA_Cmd(DMA1_Channel4, ENABLE);		    // ʹ��DMAͨ��4����������
		Uart.txFlg = 0;
	}	
}

/**        
 * @brief  ��ȡ���ڷ���״̬
 * @param  ������ ����˵��
 * @param  ��
 * @retval 0 - ���ڷ��� ��0 - �������
 * @par ʹ��ȫ�ֱ���
 *         �� \n
 * @note �� ִ��ʱ��: \n
 *       �� ��������: �������� \n
 *       �� �ɷ���: ���� \n
 *
 * @par  ע��:
 *       ��  \n
 */
u8 GetSendStt(void)
{
    return USART_GetFlagStatus(USART1, USART_FLAG_TC);
}

/**  @} */

