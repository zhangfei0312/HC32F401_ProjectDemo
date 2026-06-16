/*
*********************************************************************************************************
*
*	模块名称 : 定时器模块
*	文件名称 : bsp_timer.c
*********************************************************************************************************
*/
#include "bsp.h"

/*

定时器周期计算
定时事件生成时间主要由 TIMx_PSC 和 TIMx_ARR 两个寄存器值决定，这个也就是定时器的周期.

TIMx_ARR+1)×(TIMx_PSC+1)  /Fclk-PSC


1s 周期
TIMx_ARR = 9999 
TIMx_PSC = 8399+1

*/



 /**
  * @brief  基本定时器 TIM2 中断优先级配置
  * @param  无
  * @retval 无
  */
static void TIMx_NVIC_Configuration(void)
{
		NVIC_InitTypeDef NVIC_InitStructure;
		// 设置中断来源
		NVIC_InitStructure.NVIC_IRQChannel = BASIC_TIM_IRQn;
		//设置抢占优先级
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		//设置子优先级
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
		
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
}
/*
 * 注意：TIM_TimeBaseInitTypeDef结构体里面有5个成员，TIM6和TIM7的寄存器里面只有
 * TIM_Prescaler和TIM_Period，所以使用TIM6和TIM7的时候只需初始化这两个成员即可，
 * 另外三个成员是通用定时器和高级定时器才有.
 *-----------------------------------------------------------------------------
 * TIM_Prescaler         都有
 * TIM_CounterMode			 TIMx,x[6,7]没有，其他都有（基本定时器）
 * TIM_Period            都有
 * TIM_ClockDivision     TIMx,x[6,7]没有，其他都有(基本定时器)
 * TIM_RepetitionCounter TIMx,x[1,8]才有(高级定时器)


核心思路就是：PSC 控制计数精度，ARR 控制计数次数，两者共同决定定时周期。

当 APB1 分频系数 ≠ 1 时，定时器时钟自动 ×2

PLL → 168 MHz → HCLK ─┬─ /4 → PCLK1 = 42 MHz
                       │            │
                       │     APB1 prescaler = 4 ≠ 1
                       │     → 定时器时钟自动 ×2
                       │            │
                       │     TIM2 CK_PSC = 84 MHz 
                       │
                       └─ /2 → PCLK2 = 84 MHz
 *-----------------------------------------------------------------------------
 */
static void TIM_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	//恢复默认参数
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	
	// 开启TIMx_CLK,  TIMER2
  RCC_APB1PeriphClockCmd(BASIC_TIM_CLK, ENABLE); 

  /* 累计 TIM_Period个后产生一个更新或者中断*/		
  //当定时器从0计数到4999，即为5000次，为一个定时周期
  //TIM_TimeBaseStructure.TIM_Period = 1000-1;      								//ARR  
	TIM_TimeBaseStructure.TIM_Period = 10000-1; 
	TIM_TimeBaseStructure.TIM_Prescaler = 8400-1;									//84 MHz 定时器时钟/8400 = 10khz    0.1ms计算一次    0.1ms*10000 = 1s
	//定时器时钟源
  //TIM_TimeBaseStructure.TIM_Prescaler = 42000-1;							//PSC CK_CNT = fck / (psc[15:0] + 1)					
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;						//RCR
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 		//CR1.CKD	
	// 初始化定时器TIMx, x[2,3,4,5]
	TIM_TimeBaseInit(BASIC_TIM, &TIM_TimeBaseStructure);
	
		
	// 清除定时器更新中断标志位
	TIM_ClearFlag(BASIC_TIM, TIM_FLAG_Update);
	
	TIM_ARRPreloadConfig(BASIC_TIM, ENABLE);										//CR1_ARPE 自动重装预装载使能
	// 开启定时器更新中断
	TIM_ITConfig(BASIC_TIM,TIM_IT_Update,ENABLE);
	
	// 使能定时器
	TIM_Cmd(BASIC_TIM, ENABLE);	
}

/**
  * @brief  初始化基本定时器定时，1ms产生一次中断
  * @param  无
  * @retval 无
  */
void TIMx_Configuration(void)
{
	TIMx_NVIC_Configuration();	
  
  TIM_Mode_Config();
}

/**
  * @}
  */ 
	/**
  * @brief  This function handles TIM interrupt request.
  * @param  None
  * @retval None
  */	
void  BASIC_TIM_IRQHandler (void)
{
	if ( TIM_GetITStatus( BASIC_TIM, TIM_IT_Update) != RESET ) 
	{	
		LED1_TOGGLE;
		TIM_ClearITPendingBit(BASIC_TIM , TIM_IT_Update);  		 
	}		 	
}

