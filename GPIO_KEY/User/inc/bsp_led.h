/*
*********************************************************************************************************
*
*	模块名称 : LED指示灯驱动模块
*	文件名称 : bsp_led.h
*	版    本 : V1.0
*	说    明 : 头文件
*
*	Copyright (C), 2013-2014, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#ifndef __BSP_LED_H
#define __BSP_LED_H

/*对应的RCC时钟 */
#define RCC_ALL_LED 	(RCC_AHB1Periph_GPIOC)

#define GPIO_PORT_LED1  GPIOC
#define GPIO_PIN_LED1		GPIO_Pin_14

/* 供外部调用的函数声明 */
void bsp_InitLed(void);
void bsp_LedToggle(void);

#endif

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
