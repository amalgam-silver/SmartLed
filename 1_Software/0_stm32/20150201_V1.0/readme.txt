
V1.01
此版本增加了蓝牙通讯协议

V1.00
此版本主要在节能方面做了优化

硬件资源使用情况：

1.1.3寸OLED 	- 	接口：		模拟3线SPI
					外设模块：	GPIOA
					占用引脚：	PA3 -> RESET
								PA4 -> CS
								PA5 -> SCK
								PA7 -> SDA

2.RX8025T(RTC)	-	接口：		I2C
					外设模块：	GPIOB,I2C1,EXTI(Line9)
					占用引脚:	PB6 -> I2C_SCK
								PB7 -> I2C_SDA
								PB9	-> RTC_INT				

3.LM75(温度) 	- 	接口：		I2C
					外设模块：	GPIOB,I2C1
					占用引脚:	PB6 -> I2C_SCK
								PB7 -> I2C_SDA			

4.BLE模块		- 	接口：		USART
					外设模块：	GPIOA,GPIOB,USART1,EXTI(Line10)
					占用引脚:	PA9  -> TX
								PA10 -> RX
								PB0	 -> EN				
								PB1  -> TX_TRIG
								PB10 -> CONNECT
								PB11 -> SLEEP

6.LED			- 	接口：		PWM
					外设模块：	GPIOB,TIM1
					占用引脚:	PB13 -> TIM1N
								PB14 -> TIM2N	
								PB15 -> TIM3N

7.按键			- 	接口：		无
					外设模块：	GPIOA,GPIOB,EXTI(Line1,Line8,Line12)
					占用引脚:	PA1  -> 右
								PB8  -> 左	
								PB12 -> 下

界面：
1.主界面：显示时间、日期、温度、电池及蓝牙状态
2.设置界面：时间、日期设定，休眠时间设定，蓝牙开关设定
3.详细信息界面：时间、日期、温度、电池状态
4.LED界面：彩灯模式、呼吸灯模式、各灯PWM、延时夜灯


低功耗设计：
1.全速模式：		状态：系统时钟32MHz，HCLK 32MHz, PCLK1 2MHz, PCLK2 32MHz	
				场景：开屏状态

2.低速模式：		状态：系统时钟8MHz, HCLK 1MHz, PCLK1 1MHz, PCLK2 1MHz
				场景：关屏时，但出于呼吸灯模式、延时夜灯模式
				
3.睡眠模式：		状态：系统进入stop mode, 系统时钟停止， 需EXTI中断唤醒（按键中断、蓝牙连接中断）
				场景：关屏状态

系统状态改变：
1.	亮屏时，当无按键响应超过timeScreenOff时，	若不处于呼吸灯状态(flagBreathLedEnable == 0)、且不处于延时夜灯状态(flagDelayLedEnable == 0)、
											且蓝牙没有处于连接状态(flagBLEConnected == 0)
	打开按键中断，屏蔽秒中断，进入睡眠模式（唤醒条件有：按键中断、蓝牙连接中断）。
									
											若处于呼吸灯、延时夜灯或蓝牙连接状态
	关闭屏幕，低速模式。

2.	关屏且处于低速模式时， 若延时夜灯延时结束或蓝牙连接断开，打开按键中断，屏蔽秒中断，进入睡眠模式（唤醒条件有：按键中断、蓝牙连接中断）。

3.	当发生按键中断（屏幕即将打开），切换为全速模式，屏蔽按键中断，打开秒中断。

4.	当发生蓝牙连接中断，且蓝牙处于连接状态，切换为低速模式。

编译环境：MDK 4.72.1
固件库版本：3.5