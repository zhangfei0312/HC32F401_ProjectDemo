#include "main.h"


void Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}


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

int main(void)
{ 
	/* USE for debug on line */
	//DBGMCU_Config(DBGMCU_SLEEP | DBGMCU_STOP | DBGMCU_STANDBY, ENABLE);	
	
	Delay(0xFFF);	
	//睡眠模式	
	//__WFI();				//6mA
	
	//停止模式	
	//PWR_EnterSTOPMode(PWR_MainRegulator_ON, PWR_STOPEntry_WFI);			 //4.5mA
	PWR_EnterSTOPMode(PWR_LowPowerRegulator_ON, PWR_STOPEntry_WFI);	 	 //2.5mA
	
	
	//待机模式	
	//进入待机模式必须开启PWR时钟	
	
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);			//1.5ua
//	PWR_ClearFlag(PWR_FLAG_WU | PWR_FLAG_SB);
//	PWR_WakeUpPinCmd (ENABLE);
//	PWR_EnterSTANDBYMode();
	
	
	while(1)
	{
		
	}
}


void RCC_Configuration(void)
{
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD, ENABLE);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
}


