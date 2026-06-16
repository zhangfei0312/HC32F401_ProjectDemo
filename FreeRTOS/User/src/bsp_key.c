#include "bsp.h"
/*
*********************************************************************************************************
*	函 数 名: bsp_InitKey
*	功能说明: 配置
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitKey(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;

	/* 打开GPIO时钟 */
	RCC_AHB1PeriphClockCmd(KEY1_GPIO_CLK, ENABLE);

	GPIO_StructInit(&GPIO_InitStructure);						//将每个 GPIO_InitStructure 成员的值填充为默认值
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		/* 设为输入模式 */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;		/*下拉电阻使能 */

	GPIO_InitStructure.GPIO_Pin =  KEY1_PIN;
	GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStructure);
	//	
}

#define KEY_PRESS   1   // 按下事件
#define KEY_RELEASE 2   // 释放事件
#define KEY_NONE    0   // 无事件

uint8_t Key_Scan(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    static uint8_t last_state = KEY_OFF;
    uint8_t current_state;

    // 读取当前按键状态
    current_state = (GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == KEY_ON) ? KEY_ON : KEY_OFF;

    // 检测到按下事件（上次是释放，这次是按下）
    if (last_state == KEY_OFF && current_state == KEY_ON) {
        last_state = current_state;
        return KEY_PRESS;
    }
    // 检测到释放事件（上次是按下，这次是释放）
    else if (last_state == KEY_ON && current_state == KEY_OFF) {
        last_state = current_state;
        return KEY_RELEASE;
    }

    // 状态未变化（一直按住或一直释放）或状态无效
    return KEY_NONE;
}

uint8_t event;

void Key_Hland(void)
{
		 event = Key_Scan(KEY1_GPIO_PORT, KEY1_PIN);
		if (event == KEY_PRESS) 
		{
			// 按键刚按下，执行按下时的动作（如点亮LED）
			GPIO_ResetBits(GPIO_PORT_LED1,GPIO_PIN_LED1);	
		}
		else if (event == KEY_RELEASE)
		{
				// 按键刚释放，执行释放时的动作（如关闭LED）
			GPIO_SetBits(GPIO_PORT_LED1,GPIO_PIN_LED1);
		}
		else
		{
				
		}
}
