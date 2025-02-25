

# 硬件调试说明



> 这里对我手里的普中准瑞Z100 STM32F103 这块板子进行测试，其中
>
> CH340 串口 从串口1  用杜邦线引到串口2 PD5 PD6 引脚上
>
> 杜邦线连接PA7--PA5，PA6--PA4，用于测试ADC采样和DAC模拟输出
>
> 杜邦线PA2（PWM生成）--PD13-PD14（双边缘PWM捕获）
>
> PE8 PE9(高级定时器 互补pwm带死区)



板子测试的资源接口大概如下，由cubmx生成

> Configuration	template
> STM32CubeMX 	6.12.0
> Date	08/20/2024
> MCU	STM32F103ZETx
>
> 
>
> PERIPHERALS	MODES	FUNCTIONS	PINS
> ADC1	IN6	ADC1_IN6	PA6
> ADC1	IN7	ADC1_IN7	PA7
> DAC	DAC_OUT1	DAC_OUT1	PA4
> DAC	DAC_OUT2	DAC_OUT2	PA5
> I2C1	I2C	I2C1_SCL	PB6
> I2C1	I2C	I2C1_SDA	PB7
> RCC	Crystal/Ceramic Resonator	RCC_OSC_IN	OSC_IN
> RCC	Crystal/Ceramic Resonator	RCC_OSC_OUT	OSC_OUT
> RCC	Crystal/Ceramic Resonator	RCC_OSC32_IN	PC14-OSC32_IN
> RCC	Crystal/Ceramic Resonator	RCC_OSC32_OUT	PC15-OSC32_OUT
> RTC	Activate Clock Source	RTC_VS_RTC_Activate	VP_RTC_VS_RTC_Activate
> RTC	Activate Calendar	RTC_VS_RTC_Calendar	VP_RTC_VS_RTC_Calendar
> RTC	No RTC Output	RTC_No_RTC_Output	VP_RTC_No_RTC_Output
> SPI2	Full-Duplex Master	SPI2_MISO	PB14
> SPI2	Full-Duplex Master	SPI2_MOSI	PB15
> SPI2	Full-Duplex Master	SPI2_SCK	PB13
> SYS	JTAG (4 pins)	SYS_JTCK-SWCLK	PA14
> SYS	JTAG (4 pins)	SYS_JTDI	PA15
> SYS	JTAG (4 pins)	SYS_JTDO-TRACESWO	PB3
> SYS	JTAG (4 pins)	SYS_JTMS-SWDIO	PA13
> SYS	SysTick	SYS_VS_Systick	VP_SYS_VS_Systick
> TIM1	Internal Clock	TIM1_VS_ClockSourceINT	VP_TIM1_VS_ClockSourceINT
> TIM1	PWM Generation CH1 CH1N	TIM1_CH1	PE9
> TIM1	PWM Generation CH1 CH1N	TIM1_CH1N	PE8
> TIM1	PWM Generation CH2	TIM1_CH2	PE11
> TIM2	Internal Clock	TIM2_VS_ClockSourceINT	VP_TIM2_VS_ClockSourceINT
> TIM3	Internal Clock	TIM3_VS_ClockSourceINT	VP_TIM3_VS_ClockSourceINT
> TIM4	Internal Clock	TIM4_VS_ClockSourceINT	VP_TIM4_VS_ClockSourceINT
> TIM4	Input Capture direct mode	TIM4_CH2	PD13
> TIM4	Input Capture direct mode	TIM4_CH3	PD14
> TIM5	Internal Clock	TIM5_VS_ClockSourceINT	VP_TIM5_VS_ClockSourceINT
> TIM5	PWM Generation CH3	TIM5_CH3	PA2
> USART2	Asynchronous	USART2_RX	PD6
> USART2	Asynchronous	USART2_TX	PD5
> USB	Device (FS)	USB_DM	PA11
> USB	Device (FS)	USB_DP	PA12
>
> 
>
> Pin Nb	PINs	FUNCTIONs	LABELs
> 2	PE3	GPIO_EXTI3	key3_exit
> 3	PE4	GPIO_EXTI4	key2_exit
> 4	PE5	GPIO_Output	led2
> 8	PC14-OSC32_IN	RCC_OSC32_IN	
> 9	PC15-OSC32_OUT	RCC_OSC32_OUT	
> 23	OSC_IN	RCC_OSC_IN	
> 24	OSC_OUT	RCC_OSC_OUT	
> 34	PA0-WKUP	GPIO_Input	key1
> 36	PA2	TIM5_CH3	
> 40	PA4	DAC_OUT1	
> 41	PA5	DAC_OUT2	
> 42	PA6	ADC1_IN6	
> 43	PA7	ADC1_IN7	
> 59	PE8	TIM1_CH1N	
> 60	PE9	TIM1_CH1	
> 64	PE11	TIM1_CH2	
> 73	PB12	GPIO_Output	FLASH_CS
> 74	PB13	SPI2_SCK	
> 75	PB14	SPI2_MISO	
> 76	PB15	SPI2_MOSI	
> 82	PD13	TIM4_CH2	
> 85	PD14	TIM4_CH3	
> 103	PA11	USB_DM	
> 104	PA12	USB_DP	
> 105	PA13	SYS_JTMS-SWDIO	
> 109	PA14	SYS_JTCK-SWCLK	
> 110	PA15	SYS_JTDI	
> 119	PD5	USART2_TX	
> 122	PD6	USART2_RX	
> 133	PB3	SYS_JTDO-TRACESWO	
> 135	PB5	GPIO_Output	led1
> 136	PB6	I2C1_SCL	
> 137	PB7	I2C1_SDA	
> PERIPHERALS	MODES	FUNCTIONS	PINS
> ADC1	IN6	ADC1_IN6	PA6
> ADC1	IN7	ADC1_IN7	PA7
> DAC	DAC_OUT1	DAC_OUT1	PA4
> DAC	DAC_OUT2	DAC_OUT2	PA5
> I2C1	I2C	I2C1_SCL	PB6
> I2C1	I2C	I2C1_SDA	PB7
> RCC	Crystal/Ceramic Resonator	RCC_OSC_IN	OSC_IN
> RCC	Crystal/Ceramic Resonator	RCC_OSC_OUT	OSC_OUT
> RCC	Crystal/Ceramic Resonator	RCC_OSC32_IN	PC14-OSC32_IN
> RCC	Crystal/Ceramic Resonator	RCC_OSC32_OUT	PC15-OSC32_OUT
> RTC	Activate Clock Source	RTC_VS_RTC_Activate	VP_RTC_VS_RTC_Activate
> RTC	Activate Calendar	RTC_VS_RTC_Calendar	VP_RTC_VS_RTC_Calendar
> RTC	No RTC Output	RTC_No_RTC_Output	VP_RTC_No_RTC_Output
> SPI2	Full-Duplex Master	SPI2_MISO	PB14
> SPI2	Full-Duplex Master	SPI2_MOSI	PB15
> SPI2	Full-Duplex Master	SPI2_SCK	PB13
> SYS	JTAG (4 pins)	SYS_JTCK-SWCLK	PA14
> SYS	JTAG (4 pins)	SYS_JTDI	PA15
> SYS	JTAG (4 pins)	SYS_JTDO-TRACESWO	PB3
> SYS	JTAG (4 pins)	SYS_JTMS-SWDIO	PA13
> SYS	SysTick	SYS_VS_Systick	VP_SYS_VS_Systick
> TIM1	Internal Clock	TIM1_VS_ClockSourceINT	VP_TIM1_VS_ClockSourceINT
> TIM1	PWM Generation CH1 CH1N	TIM1_CH1	PE9
> TIM1	PWM Generation CH1 CH1N	TIM1_CH1N	PE8
> TIM1	PWM Generation CH2	TIM1_CH2	PE11
> TIM2	Internal Clock	TIM2_VS_ClockSourceINT	VP_TIM2_VS_ClockSourceINT
> TIM3	Internal Clock	TIM3_VS_ClockSourceINT	VP_TIM3_VS_ClockSourceINT
> TIM4	Internal Clock	TIM4_VS_ClockSourceINT	VP_TIM4_VS_ClockSourceINT
> TIM4	Input Capture direct mode	TIM4_CH2	PD13
> TIM4	Input Capture direct mode	TIM4_CH3	PD14
> TIM5	Internal Clock	TIM5_VS_ClockSourceINT	VP_TIM5_VS_ClockSourceINT
> TIM5	PWM Generation CH3	TIM5_CH3	PA2
> USART2	Asynchronous	USART2_RX	PD6
> USART2	Asynchronous	USART2_TX	PD5
> USB	Device (FS)	USB_DM	PA11
> USB	Device (FS)	USB_DP	PA12
>
> 
>
> Pin Nb	PINs	FUNCTIONs	LABELs
> 2	PE3	GPIO_EXTI3	key3_exit
> 3	PE4	GPIO_EXTI4	key2_exit
> 4	PE5	GPIO_Output	led2
> 8	PC14-OSC32_IN	RCC_OSC32_IN	
> 9	PC15-OSC32_OUT	RCC_OSC32_OUT	
> 23	OSC_IN	RCC_OSC_IN	
> 24	OSC_OUT	RCC_OSC_OUT	
> 34	PA0-WKUP	GPIO_Input	key1
> 36	PA2	TIM5_CH3	
> 40	PA4	DAC_OUT1	
> 41	PA5	DAC_OUT2	
> 42	PA6	ADC1_IN6	
> 43	PA7	ADC1_IN7	
> 59	PE8	TIM1_CH1N	
> 60	PE9	TIM1_CH1	
> 64	PE11	TIM1_CH2	
> 73	PB12	GPIO_Output	FLASH_CS
> 74	PB13	SPI2_SCK	
> 75	PB14	SPI2_MISO	
> 76	PB15	SPI2_MOSI	
> 82	PD13	TIM4_CH2	
> 85	PD14	TIM4_CH3	
> 103	PA11	USB_DM	
> 104	PA12	USB_DP	
> 105	PA13	SYS_JTMS-SWDIO	
> 109	PA14	SYS_JTCK-SWCLK	
> 110	PA15	SYS_JTDI	
> 119	PD5	USART2_TX	
> 122	PD6	USART2_RX	
> 133	PB3	SYS_JTDO-TRACESWO	
> 135	PB5	GPIO_Output	led1
> 136	PB6	I2C1_SCL	
> 137	PB7	I2C1_SDA	
>
> 
>
> SOFTWARE PROJECT
>
> Project Settings : 
> Project Name : template
> Project Folder : C:\Users\79371\Desktop\WorkSpace\workspace_demo\STM32_Template
> Toolchain / IDE : CMake
> Firmware Package Name and Version : STM32Cube FW_F1 V1.8.5
>
>
> Code Generation Settings : 
> STM32Cube MCU packages and embedded software packs : Copy only the necessary library files
> Generate peripheral initialization as a pair of '.c/.h' files per peripheral : Yes
> Backup previously generated files when re-generating : No
> Delete previously generated files when not re-generated : Yes
> Set all free pins as analog (to optimize the power consumption) : No
>
> Toolchains Settings : 
> Compiler Optimizations :







# 参考文档



用于参考文章，项目，资料以及资源

* [STM32CubeMX生成文件](./template.txt)
* [硬件原理图](./Z100开发板原理图.pdf)








