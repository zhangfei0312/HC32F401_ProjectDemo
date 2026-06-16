/*
*********************************************************************************************************
*
*	模块名称 : BSP模块
*	文件名称 : bsp.h
*	说    明 : 这是底层驱动模块所有的h文件的汇总文件。 应用程序只需 #include bsp.h 即可，
*			  不需要#include 每个模块的 h 文件
*********************************************************************************************************
*/

#ifndef _BSP_H_
#define _BSP_H_

/* 定义 BSP 版本号 */
#define __STM32F1_BSP_VERSION		"0.1"

/* 开关全局中断的宏 */
#define ENABLE_INT()	__set_PRIMASK(0)	/* 使能全局中断 */
#define DISABLE_INT()	__set_PRIMASK(1)	/* 禁止全局中断 */

/* 这个宏仅用于调试阶段排错 */
#define BSP_Printf		printf
//#define BSP_Printf(...)

#include "hc32f4xx.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#ifndef TRUE
	#define TRUE  1
#endif

#ifndef FALSE
	#define FALSE 0
#endif

/* 通过取消注释或者添加注释的方式控制是否包含底层驱动模块 */
#include "bsp_led.h"
//#include "bsp_key.h"
#include "bsp_uart.h"
//#include "bsp_adc.h"
//#include "bsp_SysTick.h"
//#include "bsp_exti.h"
#include "bsp_timer.h"

/* 提供给其他C文件调用的函数 */
void bsp_Init(void);
void bsp_Idle(void);

#endif

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
