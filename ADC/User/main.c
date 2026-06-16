/*
*********************************************************************************************************
*
*	模块名称 : 主程序模块。
*	文件名称 : main.c
*	版    本 : V1.1
*

AHB 域的最大频率为 168 MHz(HSE PLL)
高速 APB2 域的最大允许频率为 84 MHz。
低速 APB1域的最大允许频率为 42 MHz。



*********************************************************************************************************
*/
#include "bsp.h"		/* 底层硬件驱动 */


/* 仅允许本文件内调用的函数声明 */
static void PrintfLogo(void);

/** @addtogroup Delay
  * @{
  */
void Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}

//void RCC_Configuration(void)
//{
//	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD, ENABLE);
//	
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
//	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
//}
/*
*********************************************************************************************************
*	函 数 名: SYS_Configuration
*	功能说明: 时钟
*	形    参：无
*	返 回 值: 错误代码(无需处理)
对于 SYSCLK、HCLK、PCLK2、PCLK1 这四个时钟的配置一般是：

HCLK = SYSCLK=PLLCLK = 168M，PCLK1=HCLK/2 = 84M，PCLK1=HCLK/4 = 42M


 
使用 HSE 时，设置系统时钟的步骤
 1、开启 HSE ，并等待 HSE 稳定
 2、设置 AHB、APB2、APB1 的预分频因子
 3、设置 PLL 的时钟来源
		设置 VCO 输入时钟 分频因子 m
		设置 VCO 输出时钟 倍频因子 n
		设置 PLLCLK 时钟分频因子 p
		设置 OTG FS,SDIO,RNG 时钟分频因子 q
 4、开启 PLL，并等待 PLL 稳定
 5、把 PLLCK 切换为系统时钟 SYSCLK
 6、读取时钟切换状态位，确保 PLLCLK 被选为系统时钟
*********************************************************************************************************
*/
#define PLL_M 25
#define PLL_N 336
#define PLL_P 2
#define PLL_Q 7

void SYS_Configuration(void)
{
	//使能HSE
	RCC_HSEConfig(RCC_HSE_ON);
	//等待HSE启动稳定
	while(RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET){}; 
		
	RCC_PLLCmd(DISABLE);    
	//RCC_HCLKConfig(RCC_SYSCLK_Div1);	//AHB 总线时钟 HCLK
	//RCC_PCLK1Config(RCC_HCLK_Div1);		//APB1 总线时钟 HCLK1  ---高速的总线时钟
	//RCC_PCLK2Config(RCC_HCLK_Div1);		//APB2 总线时钟 HCLK2	 ---低速的总线时钟
	RCC_HCLKConfig(RCC_SYSCLK_Div1);		//168MHZ
	RCC_PCLK1Config(RCC_HCLK_Div4);			//42MHZ
	RCC_PCLK2Config(RCC_HCLK_Div2);			//84MHZ
	//
	RCC_PLLConfig(RCC_PLLSource_HSE,8,336,2,8);    //PLL时钟配置168， M N P Q 
		
	RCC_PLLCmd(ENABLE);   
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET){};    
			
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);   
	while (RCC_GetSYSCLKSource() != 0x08){} 
}

/*
*********************************************************************************************************
*	函 数 名: main
*	功能说明: c程序入口
*	形    参：无
*	返 回 值: 错误代码(无需处理)
*********************************************************************************************************
*/
int main(void)
{ 
	SYS_Configuration();
	bsp_Init();					/* 硬件初始化 */
	Delay(0xFFF);	
	PrintfLogo();
	while(1)
	{
		//Key_Hland();
		GPIO_ResetBits(GPIO_PORT_LED1,GPIO_PIN_LED1);
		Delay_us(100);    	// 10000 * 10us = 1000ms
		GPIO_SetBits(GPIO_PORT_LED1,GPIO_PIN_LED1);
		Delay_us(100);    	// 10000 * 10us = 1000ms
		if(Flag)
		{
			Flag = 0;
			printf("\r\nADC1:ADC_DAT[0] = %u %u\n\r", ADC_DAT[0], ADC_Value[0]);
			ADC_DAT[0] = 0;
			ADC_Value[0] = 0;
			ADC_SoftwareStartConv(ADC1);
		}
	}
}

/**
  * @}
  */

/**
  * @}
  */
/*
*********************************************************************************************************
*	函 数 名: PrintfLogo
*	功能说明: 打印例程名称和例程发布日期, 接上串口线后，打开PC机的超级终端软件可以观察结果
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void PrintfLogo(void)
{
	
	//printf("HC32F401X");
	//printf("\n\r");
	/* 检测CPU ID */
	{
		/*用户手册：
			32.15.1 
			Unique device ID register (96 bits)
		*/
		uint32_t CPU_Sn0, CPU_Sn1, CPU_Sn2;

		CPU_Sn0 = *(__IO uint32_t*)(0x1FFF7A10);
		CPU_Sn1 = *(__IO uint32_t*)(0x1FFF7A10 + 4);
		CPU_Sn2 = *(__IO uint32_t*)(0x1FFF7A10 + 8);

		printf("\r\nCPU : HC32F401X, LQFP64, UID = %08X %08X %08X\n\r"
			, CPU_Sn2, CPU_Sn1, CPU_Sn0);
	}
	printf("\n\r");
	printf("*************************************************************\n\r");
}

