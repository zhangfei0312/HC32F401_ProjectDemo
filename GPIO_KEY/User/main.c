#include "bsp.h"

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


void SYS_Configuration(void)
{
	RCC_HSEConfig(RCC_HSE_ON);
	
	while(RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET){}; 
		
	RCC_PLLCmd(DISABLE);    
	RCC_HCLKConfig(RCC_SYSCLK_Div1);
	RCC_PCLK1Config(RCC_HCLK_Div1);
	RCC_PCLK2Config(RCC_HCLK_Div1);
	
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
	bsp_Init();		/* 硬件初始化 */
	Delay(0xFFF);	

	while(1)
	{
		Key_Hland();
	}
}

/**
  * @}
  */

/**
  * @}
  */


