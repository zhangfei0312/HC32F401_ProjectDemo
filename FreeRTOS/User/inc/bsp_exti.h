#ifndef __EXTI_H
#define	__EXTI_H


//Òý½Å¶¨Òå
/*******************************************************/
#define KEY1_INT_GPIO_PORT                GPIOA
#define KEY1_INT_GPIO_CLK                 RCC_AHB1Periph_GPIOA
#define KEY1_INT_GPIO_PIN                 GPIO_Pin_1
#define KEY1_INT_EXTI_PORTSOURCE          EXTI_PortSourceGPIOA
#define KEY1_INT_EXTI_PINSOURCE           EXTI_PinSource1
#define KEY1_INT_EXTI_LINE                EXTI_Line1
#define KEY1_INT_EXTI_IRQ                 EXTI1_IRQn

#define KEY1_IRQHandler                   EXTI1_IRQHandler


/*******************************************************/

void EXTI_Key_Config(void);

#endif /* __EXTI_H */
