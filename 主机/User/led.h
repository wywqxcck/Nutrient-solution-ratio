/*��������*/
#ifndef __LED_H__
#define __LED_H__

#include "sys.h"
/*�궨�巽�������ֲ*/
#define LED_GPIO_PIN  GPIO_Pin_13 //����GPIO_Pin_x
#define LED_GPIO_PORT GPIOC       //��һ��GPIOx
#define LED_GPIO_CLK RCC_APB2Periph_GPIOC  //��һ��GPIOx��ʱ��

#define LED1 PCout(13)

/*��������*/
void LED_GPIO_Config(void);
void LED_OFF(void);
void LED_NO(void);
void LED_Change(void);

#endif
