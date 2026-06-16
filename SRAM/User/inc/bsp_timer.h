/*
*********************************************************************************************************
*
*	模块名称 : 定时器模块
*	文件名称 : bsp_timer.h
*
*********************************************************************************************************
*/

#ifndef __BSP_TIMER_H
#define __BSP_TIMER_H

#define BASIC_TIM           		TIM2
#define BASIC_TIM_CLK       		RCC_APB1Periph_TIM2

#define BASIC_TIM_IRQn					TIM2_IRQn
#define BASIC_TIM_IRQHandler    TIM2_IRQHandler

/* 提供给其他C文件调用的函数 */


void TIMx_Configuration(void);

#endif

/*****************************(END OF FILE) *********************************/
