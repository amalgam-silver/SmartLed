# SmartLed
智能LED项目

硬件特性:
	Cotex-M3主控 STM32F103RBT; 
	BLE可与智能手机通讯; 
	实时时钟RTX8025; 
	温度传感器 LM75; 
	OLED显示屏; 
	3色高亮LED照明;

目录结构：

- 0_HardWare 		硬件相关设计文件
	- datasheet				相关芯片的datasheet
	- PCB					PCB工程，包括A~F共6块PCB
	- reference				参考资料，杜洋的PCB手表，主要参考了按键和结构的设计
- 1_Software		软件相关设计文档
	- 0_stm32	stm32端的代码
		- 20150201_V1.0		1.0版本, 基本功能实现
		- 20160325_V2.0		2.0版本, 性能优化, 软件架构重写
	- 1_Android		Android手机端代码
		- SmartLed 			Android Studio工程
		- reference			参考资料