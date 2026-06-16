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


设置堆和栈的大小是通过修改启动文件start_hc32f4xx.s 文件
Stack_Size      EQU     0x00000400		--1024 字节
Heap_Size       EQU     0x00000200    --512


PA2 -- USART1_TX
PA3 -- USART1_RX

PA0 -- KEY_S1

PA1 -- EXI-


*********************************************************************************************************
*/
#include "bsp.h"		/* 底层硬件驱动 */

/*
**********************************************************************************************************
											函数声明
**********************************************************************************************************
*/

static void vTaskLED(void *pvParameters);
static void vTaskStart(void *pvParameters);
static void AppTaskCreate (void);

/*
**********************************************************************************************************
											变量声明
**********************************************************************************************************
*/
static TaskHandle_t xHandleTaskLED = NULL;
static TaskHandle_t xHandleTaskStart = NULL;

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
#define PLL_M 8
#define PLL_N 336
#define PLL_P 2
#define PLL_Q 8

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
	RCC_PCLK1Config(RCC_HCLK_Div4);			//42MHZ		APB1	
	RCC_PCLK2Config(RCC_HCLK_Div2);			//84MHZ   APB2
	//
	RCC_PLLConfig(RCC_PLLSource_HSE,PLL_M,PLL_N,PLL_P,PLL_Q);    //PLL时钟配置168， M N P Q 
		
	RCC_PLLCmd(ENABLE);   
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET){};    
			
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);   
	while (RCC_GetSYSCLKSource() != 0x08){} 
}
 /**
  * @brief  配置嵌套向量中断控制器NVIC
  * @param  无
  * @retval 无
  */



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
	__set_PRIMASK(1); 
	
	bsp_Init();					/* 硬件初始化 */
	
	/* 创建任务 */
	AppTaskCreate();
	
	 /* 启动调度，开始执行任务 */
    vTaskStartScheduler();
	while(1);
}

/*
*********************************************************************************************************
*	函 数 名: vTaskLED
*	功能说明: LED闪烁	
*	形    参: pvParameters 是在创建该任务时传递的形参
*	返 回 值: 无
*   优 先 级: 2  
*********************************************************************************************************
*/
static void vTaskLED(void *pvParameters)
{
    while(1)
    {
				bsp_LedToggle();	//
        vTaskDelay(200);
    }
}

/*
*********************************************************************************************************
*	函 数 名: vTaskStart
*	功能说明: 启动任务，也就是最高优先级任务，这里用作LED闪烁
*	形    参: pvParameters 是在创建该任务时传递的形参
*	返 回 值: 无
*   优 先 级: 4  
*********************************************************************************************************
*/
static void vTaskStart(void *pvParameters)
{
    while(1)
    {
		/* LED闪烁 */
				//bsp_LedToggle(4);
        vTaskDelay(400);
    }
}
/*
*********************************************************************************************************
*	函 数 名: AppTaskCreate
*	功能说明: 创建应用任务
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void AppTaskCreate (void)
{

	
	xTaskCreate( vTaskLED,    		/* 任务函数  */
                 "vTaskLED",  		/* 任务名    */
                 512,         		/* 任务栈大小，单位word，也就是4字节 */
                 NULL,        		/* 任务参数  */
                 2,           		/* 任务优先级*/
                 &xHandleTaskLED ); /* 任务句柄  */

	
	xTaskCreate( vTaskStart,     		/* 任务函数  */
                 "vTaskStart",   		/* 任务名    */
                 512,            		/* 任务栈大小，单位word，也就是4字节 */
                 NULL,           		/* 任务参数  */
                 4,              		/* 任务优先级*/
                 &xHandleTaskStart );   /* 任务句柄  */
}

