#ifndef __BSP_KEY_H
#define __BSP_KEY_H


 //引脚定义
 /*******************************************************/
 #define KEY1_PIN GPIO_Pin_0
 #define KEY1_GPIO_PORT GPIOA
 
 #define KEY1_GPIO_CLK RCC_AHB1Periph_GPIOA
 
 /*******************************************************
 ** 按键按下标置宏
 * 按键按下为高电平，设置 KEY_ON=1， KEY_OFF=0
 */
 #define KEY_ON 	1
 #define KEY_OFF 	0
 
/* 供外部调用的函数声明 */
void bsp_InitKey(void);
void Key_Hland(void);

#endif
