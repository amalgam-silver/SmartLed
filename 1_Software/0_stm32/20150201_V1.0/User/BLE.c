#include "BLE.h"


void bluetoothUsartInit(void);
void uartDecode(void);
u8 receiveBuffer[30];
u8 sendBuffer[200]="TTM:REN-HG";
u8 stateBLE;
u8 flagBLEConnected = 0;

void bluetoothCmd(u8 newState)
{
	if(newState == ENABLE)
	{
		GPIO_WriteBit(GPIOB,GPIO_Pin_0,Bit_RESET);
		stateBLE = ENABLE;
	}
	else
	{
		GPIO_WriteBit(GPIOB,GPIO_Pin_0,Bit_SET);
		stateBLE = DISABLE;
	}
}

void bluetoothInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	
	//PB10 -> BT_CON	PB11 -> BT_SLEEP
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//PB0 -> BT_EN	PB1 -> BT_BRTS
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_WriteBit(GPIOB,GPIO_Pin_1,Bit_SET);
	GPIO_WriteBit(GPIOB,GPIO_Pin_0,Bit_RESET);
	stateBLE = ENABLE;
	
	//�������������ж�
	EXTI_InitStructure.EXTI_Line = EXTI_Line10;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;  
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource10);
	
	bluetoothUsartInit();
	
}

void bluetoothUsartInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO, ENABLE);
	
	//PA10 -> USART1_RX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//PA9 -> USART1_TX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
//	USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
//	USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
//	USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
//	USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;
//	USART_ClockInit(USART1, &USART_ClockInitStructure);	
	//������9600��1λֹͣλ������żУ��λ
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1,&USART_InitStructure);
	
	USART_Cmd(USART1,ENABLE);
	
	//ʹ�ܽ����ж�
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	
	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
	
}

void USART1_IRQHandler(void)
{
	static u16 receiveCounter = 0;
	static u8 temp,flagStart = 0;
	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
		
		temp = USART_ReceiveData(USART1);
		if(flagStart == 1)
		{
			receiveBuffer[receiveCounter] = temp;
			receiveCounter ++;
            
			if(receiveCounter == receiveBuffer[3])
			{
				flagStart = 0;
				receiveCounter = 0;
				uartDecode();
			}
            /* ��ı��ĳ���Ϊ12 */
            else if (receiveCounter > 12)
            {
                receiveCounter = 0;
                flagStart = 0;
            }
		}
		else if(temp == 0x7f)
		{
			flagStart = 1;
			receiveBuffer[receiveCounter] = temp;
			receiveCounter ++;
		}
	}
}

void bleSend(u8 *Data, u16 LengthTX)
{
	u8 i,temp;
	
	GPIO_WriteBit(GPIOB,GPIO_Pin_1,Bit_RESET);
	delayUs(500);
	for(i=0; i<LengthTX; i++)
	{
		temp = Data[i];
		//����֡ ����
		USART_SendData(USART1,temp);
		//��״̬�Ĵ��� �ж�USART1������ɱ�־λ
		//USART1->DR = (temp & (u16)0x01FF); 
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET);
	}
	delayUs(200);
	GPIO_WriteBit(GPIOB,GPIO_Pin_1,Bit_SET);
}

void uartDecode(void)
{
	u8 length = 0, command = 0, verify = 0, readOrWrite = 0;
	u8 i;
	timeStructure timeSetBLE;
    u8 data[20];
    extern u16 temperature;
	
	command = receiveBuffer[1];
	readOrWrite = receiveBuffer[2];
	length = receiveBuffer[3];
	
	for(i = 1; i < (length - 1); i++)
	{
		verify += receiveBuffer[i];
	}
	
	if(verify != receiveBuffer[length - 1])
		return;
	
	switch(command)
	{
		//ʱ��
		case 0x01:
			if(readOrWrite == 1)
			{
				timeSetBLE.year 	=	receiveBuffer[4];
				timeSetBLE.month 	= 	receiveBuffer[5];
				timeSetBLE.data 	=	receiveBuffer[6];
				timeSetBLE.hour 	=	receiveBuffer[7];
				timeSetBLE.minute 	= 	receiveBuffer[8];
				timeSetBLE.second 	= 	receiveBuffer[9];
				timeSetBLE.weekday =	receiveBuffer[10];
				timeSet(timeSetBLE);
			}
			break;
		//����ʱ��
		case 0x02:
			//д
			if(readOrWrite == 1)
			{
				timeScreenOff = receiveBuffer[4];
			}
			//��
			else if(readOrWrite == 0)
			{
				data[0] = 0x7f;
                data[1] = 0x02;
                data[2] = 0x00;
                data[3] = 0x06;
                data[4] = timeScreenOff;
                data[5] = data[1] + data[2] + data[3] + data[4];
                bleSend(data, 6);
			}
			break;
		//ϵͳ״̬
		case 0x03:
            if (readOrWrite == 0)
            {
                data[0] = 0x7f;
                data[1] = 0x03;
                data[2] = 0x00;
                data[3] = 0x0C;
                data[4] = stateCharging;            // �Ƿ�����
                data[5] = voltageBattery - 200;     // ��ص�ѹ
                data[6] = batteryState;             // ��ص����ȼ�
                data[7] = ledDelaySecond / 60;      // ������ʱʱ��
                data[8] = SW_VERSION;               // �̼��汾
                data[9] = temperature >> 8;         // �¶�
                data[10] = temperature;             // �¶�
                data[11] = 0;
                for (i = 1; i < 11; i++)
                {
                    data[11] += data[i];
                }
                bleSend(data, 12);
            }
			break;
		//LED
		case 0x04:
			//д
			if(readOrWrite == 1)
			{
				u8 mode = receiveBuffer[4];
                u8 delay = receiveBuffer[5];
                pwmWhite = receiveBuffer[6];                // ��������
                flagBreathLedEnable = receiveBuffer[7];     // ������ʹ��
                pwmRedSet = receiveBuffer[8];               // �������
                pwmGreenSet = receiveBuffer[9];             // �̵�����
                pwmBlueSet = receiveBuffer[10];             // ��������
               
                // ����ģʽ
                if (0 == mode)
                {
                    // �ı�����
                    flagColorLedEnable = 0;
                    pwmChange(pwmWhite, pwmWhite, pwmWhite);
                    
                    modeBreathLed = breathOff;
                }
                // �ʵ�ģʽ
                else
                {
                    // �ı���ɫ
                    flagColorLedEnable = 1;
                    pwmChange(pwmRedSet, pwmGreenSet, pwmBlueSet);
                    if (flagBreathLedEnable)
                    {
                        modeBreathLed = breathColor;
                    }
                    else
                    {
						modeBreathLed = breathOff;
                    }
                }
                
                if((delay > 0) && (pwmWhite > 0) && (flagColorLedEnable == DISABLE))
                {
                    //��ʼ��ʱ
                    flagDelayLedEnable = ENABLE;
                    //������ʱ����
                    ledDelaySecond = delay * 60;
                }			
                else
                {
                    flagDelayLedEnable = DISABLE;
                    ledDelaySecond = 0;
                }
			}
			//��
			else if(readOrWrite == 0)
			{
				data[0] = 0x7f;
                data[1] = 0x04;
                data[2] = 0x00;
                data[3] = 0x0c;
                data[4] = flagColorLedEnable;           // ģʽ
                data[5] = (ledDelaySecond + 59) / 60;   // ��ʱʱ��
                data[6] = pwmWhite;                     // ��������
                data[7] = flagBreathLedEnable;          // ������
                data[8] = pwmRedSet;                    // �������
                data[9] = pwmGreenSet;                  // �̵�����
                data[10] = pwmBlueSet;                  // ��������
                data[11] = 0;
                for (i = 1; i < 11; i++)
                {
                    data[11] += data[i];
                }
                bleSend(data, 12);
			}
			break;
		
		default:
			break;
	}
}
