/*
*********************************************************************************************************
*	                                  
*	模块名称 : 串口驱动模块    
*	文件名称 : bsp_uart.c
*	版    本 : V1.0
*	说    明 : 实现printf和scanf函数重定向到串口1，即支持printf信息到USART1
*				实现重定向，只需要添加2个函数:
*				int fputc(int ch, FILE *f);
*				int fgetc(FILE *f);
*				对于KEIL MDK编译器，编译选项中需要在MicorLib前面打钩，否则不会有数据打印到USART1。
*********************************************************************************************************
*/
#include "HC32f4xx.h"

/*
*********************************************************************************************************
*	函 数 名: bsp_InitUart
*	功能说明: 初始化CPU的USART1串口硬件设备。未启用中断。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
#define BaudRate 115200
void bsp_InitUart(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	RCC_ClocksTypeDef RCC_ClocksStatus;
	//
	uint32_t apbclock = 0;          // 当前串口所在总线的时钟频率 (Hz)
	uint32_t baudrate = BaudRate;   // 你的目标波特率
	uint8_t  over8 = 0;             // 超采样模式：0 = 16倍采样, 1 = 8倍采样
	
	uint32_t integerdivider = 0;
	uint32_t fractionaldivider = 0;

	uint16_t brri_result = 0;              // 最终计算出的整数部分
	uint16_t brrf_result = 0;              // 最终计算出的小数部分
	
	/* 串口1 TX = PA2   RX = PA3 */
	/* 第1步： 配置GPIO */
	/* 打开 GPIO 时钟 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	/* 打开 UART 时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	/* 将 PA2 映射为 USART1_TX */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_6);

	/* 将 PA3 映射为 USART1_RX */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_6);

	/* 配置 USART Tx 为复用功能 */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	/* 输出类型为推挽 */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	/* 内部上拉电阻使能 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	/* 复用模式 */

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* 配置 USART Rx 为复用功能 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* 第2步： 配置串口硬件参数   00：16 倍采样，Baud=PCLK/(BRRI×16+BRRF)   */  
  /* Configure the USART Baud Rate */
  RCC_GetClocksFreq(&RCC_ClocksStatus);
	
	apbclock = RCC_ClocksStatus.PCLK2_Frequency;
	/* 1. 计算放大 100 倍的总体分频值 */
	if (over8 != 0)
	{
			// 8倍超采样
			integerdivider = ((25 * apbclock) / (2 * baudrate));    
	}
	else 
	{
			// 16倍超采样
			integerdivider = ((25 * apbclock) / (4 * baudrate));    
	}

	/* 2. 提取整数部分 (BRRI) */
	// integerdivider / 100 就是实际的整数分频值
	brri_result = (uint16_t)(integerdivider / 100); 

	/* 3. 提取并计算四舍五入后的小数部分 (BRRF) */
	// 用总数减去整数部分对应的放大值，得到纯小数部分（也是放大100倍的）
	fractionaldivider = integerdivider - (100 * brri_result);

	if (over8 != 0)
	{
			// 8倍采样下，小数位占 3 位 (最大值 7，所以 & 0x07)
			brrf_result = (uint16_t)((((fractionaldivider * 8) + 50) / 100) & 0x07);
	}
	else 
	{
			// 16倍采样下，小数位占 4 位 (最大值 15，所以 & 0x0F)
			brrf_result = (uint16_t)((((fractionaldivider * 16) + 50) / 100) & 0x0F);
	}
	USART_InitStructure.USART_OVER = over8 ? USART_OVER_8 : USART_OVER_16; 
	USART_InitStructure.USART_BRRI = brri_result; // 填入计算出的整数
	USART_InitStructure.USART_BRRF = brrf_result; // 填入计算出的小数
	
//	USART_InitStructure.USART_OVER = USART_OVER_16;	/* 波特率9600 */
//	USART_InitStructure.USART_BRRI = 45;//1093;				//波特率计数器整数部分寄存器			
//	USART_InitStructure.USART_BRRF = 9;//12;					//波特率计数器小数部分寄存器
	//
	USART_InitStructure.USART_MSBF = USART_LSB;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_Init(USART1, &USART_InitStructure);

	USART_Cmd(USART1, USART_Mode_Tx, ENABLE);		/* 使能串口 */

	/* CPU的小缺陷：串口配置好，如果直接Send，则第1个字节发送不出去
		如下语句解决第1个字节无法正确发送出去的问题 */
	USART_ClearFlag(USART1, USART_FLAG_TC);     /* 清发送完成标志，Transmission Complete flag */   
}
