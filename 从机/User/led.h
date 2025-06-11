/*条件编译*/
#ifndef __LED_H__
#define __LED_H__

#include "sys.h"
/*宏定义方便代码移植*/
#define LED_GPIO_PIN  GPIO_Pin_5 //引脚GPIO_Pin_x
#define LED_GPIO_PORT GPIOB      //那一个GPIOx
#define LED_GPIO_CLK RCC_APB2Periph_GPIOB  //哪一个GPIOx的时钟

#define LED1 PBout(5)

/*函数声明*/
void LED_GPIO_Config(void);
void LED_OFF(void);
void LED_NO(void);
void LED_Change(void);

#endif
