/*��������*/
#ifndef __LED3_H__
#define __LED3_H__

#include "sys.h"
/*λ������*/
#define REDLED PBout(5)
#define GREENLED PBout(6)
#define YELLOWLED PBout(8)

/*��������*/
void LED3_GPIO_Config(void);
void RED_LED_ON(void);
void RED_LED_OFF(void);
void GREEN_LED_ON(void);
void GREEN_LED_OFF(void);
void YELLOW_LED_ON(void);
void YELLOW_LED_OFF(void);
void RED_LED_Change(void);
void GREEN_LED_Change(void);
void YELLOW_LED_Change(void);

#endif
