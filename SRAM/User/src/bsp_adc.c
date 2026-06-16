/*
*********************************************************************************************************
*
*	模块名称 : ADC驱动模块
*	文件名称 : bsp_adc.c
*	版    本 : V1.0
*	说    明 : 实现ADC测量温度
*	修改记录 :
*********************************************************************************************************
*/
#include "bsp.h"
/* define ---------------------------------------------------------------------*/
// ADC GPIO 宏定义
//#define RHEOSTAT_ADC_GPIO_PORT 	GPIOB
//#define RHEOSTAT_ADC_GPIO_PIN 	GPIO_Pin_0
//#define RHEOSTAT_ADC_GPIO_CLK 	RCC_AHB1Periph_GPIOB

//// ADC 序号宏定义
//#define RHEOSTAT_ADC 					ADC1
//#define RHEOSTAT_ADC_CLK 			RCC_APB2Periph_ADC1
//#define RHEOSTAT_ADC_CHANNEL 	ADC_Channel_8

// // ADC 中断宏定义，ADC123 共用一个中断源
//#define Rheostat_ADC_IRQ ADC_IRQn
//#define Rheostat_ADC_INT_FUNCTION ADC_IRQHandler

/* 变量 ----------------------------------------------------------------------*/
//uint16_t uhADCConvertedValue;

/*
*********************************************************************************************************
*	函 数 名: bsp_InitADC
*	功能说明: ADC初始化
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitADC(void)
{
	GPIO_InitTypeDef	 GPIO_InitStructure;
  ADC_InitTypeDef 	 ADC_InitStruct;
	NVIC_InitTypeDef   NVIC_InitStructure;
	//
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA , ENABLE);
	//
	GPIO_StructInit(&GPIO_InitStructure);					//将每个 GPIO_InitStructure 成员的值填充为默认值
	//
	GPIO_InitStructure.GPIO_Pin	 		= GPIO_Pin_8 ;	//ADC1_AN0
	GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_AN;			//模拟输入
	//GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd 		= GPIO_PuPd_NOPULL;	//不上拉 不下拉
	GPIO_InitStructure.GPIO_Speed 	= GPIO_High_Speed;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//开启ADC时钟
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_ADC1, ENABLE);
	//ADC Init结构体 参数 初始化-----------------
	ADC_InitStruct.ADC_ClkDiv = ADC_Clk_Div12;
	ADC_InitStruct.ADC_Range  = ADC_Range_VDDA;
	//ADC_InitStruct.ADC_Range  = ADC_Range_2V4;		//内部参考2.4V
	ADC_InitStruct.ADC_OverSamplingMode 		= ADC_OverSampling_Disable;		//	
  ADC_InitStruct.ADC_OverSamplingTrigger 	= ADC_OverSamplingTrigger_Once;
	ADC_InitStruct.ADC_OverSamplingRate 		= ADC_2X_OverSamplingRate;
	ADC_InitStruct.ADC_OverSamplingOffset 	= ADC_OverSamplingOffset_0;
  ADC_Init(ADC1, &ADC_InitStruct);
	
	ADC_SQRInit(ADC1, ADC_SQR1, ADC_SQRxTRIG_Software, ADC_Ens0to0);
	// 配置 ADC 通道转换顺序和采样周期
	ADC_SQRSampleConfig(ADC1, ADC_SQR1, ADC_Channel0, ADC_InputChannel_AN0, ADC_SampTime390Clk);
	//ADC_SQRSampleConfig(ADC1, ADC_SQR1, ADC_Channel0, ADC_InputChannel_AN0, ADC_SampTime7Clk);
	//ADC_SQRSampleConfig(ADC1, ADC_SQR1, ADC_Channel1, ADC_InputChannel_AN1, ADC_SampTime9Clk);	
	//ADC_SQRSampleConfig(ADC1, ADC_SQR1, ADC_Channel2, ADC_InputChannel_AN2, ADC_SampTime12Clk);
	
	// ADC 转换结束产生中断，在中断服务程序中读取转换值
	ADC_ITConfig(ADC1, ADC_IT_EOS1, ENABLE);
	
  ADC_Cmd(ADC1,ENABLE);    			// 使能 ADC
	
	ADC_SoftwareStartConv(ADC1);	//开始 adc 转换，软件触发
	
  NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);	
	
}

uint32_t 	Flag = 0;
uint32_t 	ADC_DAT[4], ADC_Value[4];

void ADC_IRQHandler()
{
	ADC_DAT[0] = ADC_GetConversionValue(ADC1, ADC_SQR1, ADC_Channel0);
	//ADC_DAT[1] = ADC_GetConversionValue(ADC1, ADC_SQR1, ADC_Channel1);
	//ADC_DAT[2] = ADC_GetConversionValue(ADC1, ADC_SQR1, ADC_Channel2);
	
	//ADC_Value[0] = ADC_DAT[0]*330/2048; 
	//ADC_Value[0] = ADC_DAT[0]*240/2048; 
	ADC_Value[0] = (ADC_DAT[0]&0X7FF)*330/2048; 
	
	//ADC_Value[1] = ADC_DAT[1]*330/2048; 
	//ADC_Value[2] = ADC_DAT[2]*330/2048; 
	
	ADC_ClearFlag(ADC1, ADC_FLAG_EOS1);
	
	Flag = 1;
}

/*
*************************************************************************************************
  VSENSE：温度传感器的当前输出电压，与ADC_DR 寄存器中的结果ADC_ConvertedValue之间的转换关系为： 
            ADC_ConvertedValue * Vdd
  VSENSE = --------------------------
            Vdd_convert_value(0xFFF)  
    ADC转换结束以后，读取ADC_DR寄存器中的结果，转换温度值计算公式如下：
          V25 - VSENSE
  T(℃) = ------------  + 25
           Avg_Slope
  V25：  温度传感器在25℃时 的输出电压，典型值0.76 V。
  Avg_Slope：温度传感器输出电压和温度的关联参数，典型值2.5 mV/℃。
************************************************************************************************
*/

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
