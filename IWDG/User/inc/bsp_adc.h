/*
*********************************************************************************************************
*
*	模块名称 : ADC驱动模块
*	文件名称 : bsp_adc.c
*	版    本 : V1.0
*	说    明 : adc头文件
*	修改记录 :
*		版本号    日期        作者     说明
*		V1.0    2014-01-08   armfly   正式发布
*
*	Copyright (C), 2013-2014, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#ifndef __BSP_ADC_H
#define __BSP_ADC_H

#include "hc32f4xx.h"

void bsp_InitADC(void);
//float GetTemp(uint16_t advalue);
extern uint32_t 	ADC_DAT[4], ADC_Value[4];

extern uint32_t 	Flag ;
#endif

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
